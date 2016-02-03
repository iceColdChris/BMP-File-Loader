#ifndef _ANALYZER_H_
#define _ANALYZER_H_

struct ImageInfo {
	// Path to the image file.
	// The memory for this string should be allocated dynamically.
	char * path;
	// Coordinates of the top-left corner of the largest rectangle of
	// uniform color.
	int top_left_x, top_left_y;
	// Coordinates of the bottom-right corner of the largest rectangle of
	// uniform color.
	int bottom_right_x, bottom_right_y;
};

// Returns a dynamically allocated array of ImageInfo structs, one per image.
// The number of valid elements in the array should be written to the address
// in parameter images_analyzed.
struct ImageInfo * analyze_images_in_directory(int thread_limit, const char * directory, int * images_analyzed);

#endif
