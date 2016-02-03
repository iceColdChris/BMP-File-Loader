#include <stdio.h>
#include "bmpLoader.h"

WORD** LoadBitmapFile(char *filename, bmpInfoHeader *bitmapInfoHeader)
{
    FILE *filePtr; //our file pointer
    bmpHeader bitmapFileHeader; //our bitmap file header

    
    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;
    
    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(bmpHeader),1,filePtr);
    
    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.Type !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }
    
    bitmapInfoHeader = malloc(sizeof(bmpInfoHeader));
    
    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(bmpInfoHeader),1,filePtr);
    
    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.Offset, SEEK_SET);
    
    WORD**pixel_arrayp;
    int i, j;
    
    
    pixel_arrayp = (WORD**)calloc(bitmapInfoHeader->Height,sizeof(WORD*));
    
    for(i=0;i<bitmapInfoHeader->Height; i++) {
        pixel_arrayp[i] = (WORD*)calloc(bitmapInfoHeader->Width,sizeof(WORD));
    }
    
    const int padding = bitmapInfoHeader->Width % 4;
    
    for(i=0; i < bitmapInfoHeader->Height; i++)
    {
        
        for(j=0; j < bitmapInfoHeader->Width; j++)
        {
            
            fread(&pixel_arrayp[i][j], 3,1,  filePtr);
        }
        
        fseek(filePtr, padding, SEEK_CUR);

    }
    return pixel_arrayp;
}

DWORD* FindArraySize(char* filename , bmpInfoHeader *bitmapInfoHeader)
{
    FILE *filePtr; //our file pointer
    bmpHeader bitmapFileHeader; //our bitmap file header
    DWORD* sizeArray = malloc(2 * sizeof(DWORD));
    
    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;
    
    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(bmpHeader),1,filePtr);
    
    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.Type !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }
    
    bitmapInfoHeader = malloc(sizeof(bmpInfoHeader));
    
    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(bmpInfoHeader),1,filePtr);
    
    
    sizeArray[0] = bitmapInfoHeader->Width;
    sizeArray[1] = bitmapInfoHeader->Height;
    
    return sizeArray;
}
