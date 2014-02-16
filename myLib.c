#include "myLib.h"

DMA *dma = (DMA *) 0x40000B0;

/** Stalls the program scaling with the integer input */
void pause(int time) {
    volatile int temp;
    int i;
    for (i = 0; i < 10000 * time; i++)
	temp = i;
}

/** Sets a pixel at (row,col) in the given color */
void setPixel(int row, int col, u16 color) {
    VIDEO_BUFFER[OFFSET(row, col, SCREENWIDTH)] = color;
}


/** Draws a horizontal line at (row, col) with given length and color */
void drawHorzLine(int row, int col, int length, volatile unsigned short color) {
    DMANow(3, &color, &VIDEO_BUFFER[OFFSET(row , col, SCREENWIDTH)], length |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_ON);
}

/** Fills the background with the provided color */
void fillBackground(volatile unsigned short color) {
    DMANow(3, &color, VIDEO_BUFFER, 38400 |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_ON);

}

void DMANow(int channel, volatile void* source, void* destination, unsigned int control) {
    dma[channel].src = source;
    dma[channel].dst = destination;
    dma[channel].cnt = control;
}

/** Draws a rectangle at (row, col) with given height, width, and color */
void drawRect(int row, int col, int height, int width, volatile unsigned short color) {
    int r;
    for(r = 0; r < height; r++)
	DMANow(3, &color, &VIDEO_BUFFER[OFFSET(row + r, col, SCREENWIDTH)], width |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_ON);
}

void waitForVblank() {
    while (SCANLINECOUNTER > 160);
    while (SCANLINECOUNTER < 160);
}

const unsigned short bull_colors[6] = {
    COLOR(0, 0, 31),
    COLOR(0, 31, 31),
    COLOR(0, 31, 0),
    COLOR(31, 31, 0),
    COLOR(31, 0, 0),
    COLOR(31, 0, 31)
};

const unsigned short ship_picture[20*20] = {
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, RED, RED, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, RED, RED, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, RED, RED, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, RED, RED, CYA, CYA, CYA, GRE, RED, RED, GRE, CYA, CYA, CYA, RED, RED, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, RED, RED, CYA, CYA, GRE, RED, RED, GRE, CYA, CYA, RED, RED, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, RED, RED, CYA, GRE, GRE, RED, RED, GRE, GRE, CYA, RED, RED, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, RED, RED, CYA, GRE, GRE, RED, RED, GRE, GRE, CYA, RED, RED, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, RED, RED, GRE, GRE, GRE, RED, RED, GRE, GRE, GRE, RED, RED, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, RED, GRE, GRE, GRE, RED, RED, GRE, GRE, GRE, RED, CYA, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, RED, GRE, GRE, GRE, RED, RED, GRE, GRE, GRE, RED, CYA, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, GRE, RED, GRE, GRE, GRE, RED, RED, GRE, GRE, GRE, RED, GRE, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, GRE, GRE, RED, GRE, GRE, GRE, RED, RED, GRE, GRE, GRE, RED, GRE, GRE, CYA, CYA, CYA, 
    RED, CYA, CYA, GRE, GRE, RED, GRE, GRE, GRE, BLU, BLU, GRE, GRE, GRE, RED, GRE, GRE, CYA, CYA, RED, 
    RED, CYA, GRE, GRE, GRE, RED, GRE, GRE, GRE, BLU, BLU, GRE, GRE, GRE, RED, GRE, GRE, GRE, CYA, RED, 
    RED, CYA, GRE, GRE, GRE, RED, GRE, GRE, BLU, BLU, BLU, BLU, GRE, GRE, RED, GRE, GRE, GRE, CYA, RED, 
    RED, CYA, GRE, GRE, GRE, RED, GRE, GRE, BLU, BLU, BLU, BLU, GRE, GRE, RED, GRE, GRE, GRE, CYA, RED, 
    RED, RED, GRE, GRE, GRE, RED, GRE, BLU, BLU, BLU, BLU, BLU, BLU, GRE, RED, GRE, GRE, GRE, RED, RED, 
    RED, RED, GRE, GRE, GRE, RED, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, RED, GRE, GRE, GRE, RED, RED, 
    RED, RED, GRE, GRE, GRE, RED, GRE, GRE, GRE, BLU, BLU, GRE, GRE, GRE, RED, GRE, GRE, GRE, RED, RED, 
    RED, RED, GRE, GRE, GRE, RED, GRE, GRE, GRE, RED, RED, GRE, GRE, GRE, RED, GRE, GRE, GRE, RED, RED, 
};


const unsigned short enemy_full_health_picture[20*10] = {
    CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, 
    CYA, RED, RED, RED, RED, RED, BLA, RED, RED, BLA, BLA, RED, RED, BLA, RED, RED, RED, RED, RED, CYA, 
    CYA, CYA, RED, RED, RED, RED, BLA, RED, RED, BLA, BLA, RED, RED, BLA, RED, RED, RED, RED, CYA, CYA, 
    CYA, CYA, CYA, RED, RED, RED, RED, RED, RED, BLA, BLA, RED, RED, RED, RED, RED, RED, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, RED, RED, RED, RED, RED, BLA, BLA, RED, RED, RED, RED, RED, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, CYA, RED, RED, RED, RED, RED, RED, RED, RED, CYA, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, RED, RED, RED, RED, RED, RED, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, CYA, RED, RED, RED, RED, CYA, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, RED, RED, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
};

const unsigned short enemy_half_health_picture[20*10] = {
    CYA, RED, BLA, RED, BLA, RED, BLA, RED, BLA, RED, BLA, RED, BLA, RED, BLA, RED, BLA, RED, BLA, CYA, 
    CYA, BLA, RED, BLA, RED, BLA, BLA, BLA, RED, BLA, BLA, BLA, RED, BLA, RED, BLA, RED, BLA, RED, CYA, 
    CYA, CYA, BLA, RED, BLA, RED, BLA, RED, BLA, BLA, BLA, RED, BLA, BLA, BLA, RED, BLA, RED, CYA, CYA, 
    CYA, CYA, CYA, BLA, RED, BLA, RED, BLA, RED, BLA, BLA, BLA, RED, BLA, RED, BLA, RED, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, RED, BLA, RED, BLA, BLA, BLA, RED, BLA, RED, BLA, RED, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, BLA, RED, BLA, RED, BLA, RED, BLA, RED, BLA, RED, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, CYA, BLA, RED, BLA, RED, BLA, RED, BLA, RED, CYA, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, BLA, RED, BLA, RED, BLA, RED, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, CYA, BLA, RED, BLA, RED, CYA, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, BLA, RED, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
};









