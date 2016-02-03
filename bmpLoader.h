#ifndef HW3_bmp_h
#define HW3_bmp_h

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

//Define the Windows Word, DWord, and Byte type
//For non windows users
#ifdef WIN32
/*Do Nothing*/
#else
typedef uint8_t  BYTE;    //8 Bits
typedef uint16_t WORD;    //2 Bytes
typedef uint32_t DWORD;   //4 Bytes
#endif

#pragma pack(1)
typedef struct {
    WORD Type;	//File type. Set to "BM".
    DWORD Size;	//Size in BYTES of the file.
    DWORD Reserved;	//Reserved. Set to zero.
    DWORD Offset;	//Offset to the data.
}bmpHeader;

typedef struct {
    DWORD headerSize;	//Size of rest of header. Set to 40.
    DWORD Width;	//Width of bitmap in pixels.
    DWORD Height;	//Height of bitmap in pixels.
    WORD Planes;	//Number of Planes. Set to 1.
    WORD BitsPerPixel;	//Number of bits per pixel.
    DWORD Compression;	//Compression. Usually set to 0.
    DWORD ImageSize;	//Size in bytes of the bitmap.
    DWORD XPixelsPerMeter;	//Horizontal pixels per meter.
    DWORD YPixelsPerMeter;	//Vertical pixels per meter.
    DWORD ColorsUsed;	//Number of colors used.
    DWORD ColorsImportant;	//Number of "important" colors.
    
}bmpInfoHeader;

typedef struct {
    BYTE Blue;
    BYTE Green;
    BYTE Red;
}Pixel;
#pragma pack(0)

WORD** LoadBitmapFile(char*, bmpInfoHeader*);

DWORD* FindArraySize(char *filename, bmpInfoHeader*);

#endif
