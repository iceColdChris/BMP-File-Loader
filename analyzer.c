#include "analyzer.h"
#include "bmpLoader.h"
#include "LinkedList.h"
#include "semaphore.h"
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

semaphore s;


typedef struct ThreadHelper{
    WORD** image;
    struct ImageInfo* Imageinfo;
    
    DWORD Width;
    DWORD Height;
    
    int* images_analyzed;
    
    int index;
    
}thread_helper;

int getdir(const char *path, listNode* root)
{
    struct dirent *entry;
    DIR *dirpath;
    
    int imagesFound = 0;
    char *bmp = ".bmp\0";
    
    dirpath = opendir(path);
    
    if (dirpath == NULL)
    {
        perror("opendir");
        return EXIT_FAILURE;
    }
    
    while((entry = readdir(dirpath)) != NULL)
    {
        if(entry->d_type == DT_REG)
        {
            char *word = entry->d_name;
            char *end = strchr(word, '.');
            
            //If it is a bmp file print it
            if(strcmp(end, bmp) == 0)
            {
                
                char *fpath = malloc(sizeof(char*)*1024);
                strcat(fpath, path);
                if(path[strlen(path) - 1] != '/') strcat(fpath,"/");
                strcat(fpath, word);
                strcat(fpath, "\0");
                imagesFound += 1;
                push(root, fpath);
                
            }
        }
    }
    
    closedir(dirpath);
    
    return imagesFound;
    
}

//Currently doesnt find a rectangle at all
void findRectangle(struct ImageInfo* image_info, DWORD width, DWORD height, WORD** imageData, int * images_analyzed, int index)
{
    
    int i,j,k,l, area = 0, max_area = 0, bot_x = 0, bot_y = 0, top_x = 0, top_y = 0;
    
    
    for(i = 0 ; i < height; i++)//iterate through rows
    {
        bot_y = i;
        for(j=0; j < width; j++) //iterate through cols
        {
            
            for(k = i; k < height; k++) //find adjacent pixels height wise
            {
                top_y = k;
                top_x = j;
                
                for(l = j; l < width; l++) //finding adjacent pixels width wise
                {
                    
                   
                    
                    if((k > 0) && (l == j) && imageData[k][l] != imageData[k - 1][l])
                    {
                        area = ((top_x - bot_x))*((top_y - bot_y) );
                        
                        if(area < 0) area = abs(area);
                        
                        if(area >= max_area)
                        {
                            max_area = area;
                            image_info[index].bottom_right_x = bot_x;
                            image_info[index].bottom_right_y = height - bot_y;
                            image_info[index].top_left_x = top_x;
                            image_info[index].top_left_y = height - top_y;
                        }
                        
                        break;
                    }
                    else if(imageData[k][l] != imageData[k][l+1])
                    {
                        break;
                    }
                    bot_x = l;
                }
            }
        }
    }
    
    * images_analyzed +=  1;
    semaphore_down(&s);
    
    
}


void * analyze_images(void* arg)
{
    thread_helper* helper = (thread_helper*)arg;
    
    findRectangle(helper->Imageinfo, helper->Width, helper->Height, helper->image, helper->images_analyzed, helper->index);
    
    return NULL;
}


// Returns a dynamically allocated array of ImageInfo structs, one per image.
// The number of valid elements in the array should be written to the address
// in parameter images_analyzed.
struct ImageInfo * analyze_images_in_directory(int thread_limit, const char * directory, int * images_analyzed) {
   
    
    *images_analyzed = 0;
    
    semaphore_init(&s, 0);
    
    int i = 0;
    
    //That linked list
    listNode *root = malloc(sizeof(listNode));
    
    //Find all the paths to the images
    int totalImages = getdir(directory, root);
    printf("%d Image(s) loaded\n", totalImages);
    
    struct ImageInfo (* images) = malloc(totalImages * sizeof(struct ImageInfo));
    listNode *current = root, *temp;
    
    
    //Adds the path to the ImageInfo array
    //for each image
    while(current != NULL)
    {
        images[i].path = current->val;
        i++;
        temp = current;
        current = current->next;
        free(temp);
    }
    
    
    //Array of thread helpers for each image
    thread_helper* storage = malloc(sizeof(thread_helper) * totalImages);
    
    //loads the required information into the thread helper
    for(i=0; i < totalImages; i++)
    {
        bmpInfoHeader* headder;
        storage[i].image = LoadBitmapFile(images[i].path,  headder);
        storage[i].Imageinfo = images;
        storage[i].Width = FindArraySize(images[i].path, headder)[0];
        storage[i].Height = FindArraySize(images[i].path, headder)[1];
        storage[i].images_analyzed = images_analyzed;
        storage[i].index = i;
        
        
    }
    
    
    pthread_mutex_t mymutex;
    

    
    pthread_t threads[totalImages];
    i=0;
    while(i < totalImages)
    {
        
        if(semaphore_getCount(&s) < thread_limit)
        {
            if (pthread_create(&threads[i], NULL, analyze_images, &storage[i]) != 0) {
                printf("ERROR: Unable to create new thread.\n");
                exit(EXIT_FAILURE);
            }
            i++;
            semaphore_up(&s);
        }
        
    }
    int k;
    
    for (i = 0; i < totalImages; i++) {
        pthread_mutex_lock(&mymutex);
        pthread_join(threads[i], NULL);
        pthread_mutex_unlock(&mymutex);
        
        for(k = 0; k < storage[i].Height; k++)
            free(storage[i].image[k]);
        
        free(storage[i].image);
    }
    
    
    
    mySemaphore_destroy(&s);
    free(current);
    free(storage);
    return images;
}
