#include "myLib.h"

DMA *dma = (DMA *) 0x40000B0;

/** pause *****************************************************
 * Used to slow the game down between frames.
 * int time - number for relatively how long you want to pause.
 *************************************************************/
void pause(int time) {
    volatile int temp;
    int i;
    for (i = 0; i < 10000 * time; i++)
	temp = i;
}

/** setPixel *****************************************************
 * Sets a pixel at (row,col) in the given color
 * int row - the row at which you want to set a pixel.
 * int col - the column at which you want to set a pixel.
 * u16 color - the color that you want the pixel to be set to.
 *************************************************************/
void setPixel(int row, int col, u16 color) {
    VIDEO_BUFFER[OFFSET(row, col, SCREENWIDTH)] = color;
}


/** drawHorzLine *********************************************
 * Draws a horizontal line at (row,col) with the given length
 *  and color
 * int row - the row at which you want the line to start.
 * int col - the column at which you want the line to start.
 * int length - how long you want the line to be.
 * volatile unsigned short color - the color you want the line to be.
 *************************************************************/
void drawHorzLine(int row, int col, int length, volatile unsigned short color) {
    DMANow(3, &color, &VIDEO_BUFFER[OFFSET(row , col, SCREENWIDTH)], length |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_ON);
}

/** fillBackground ********************************************
 * Fills the background with the given color
 * volatile unsigned short color - the color you want the 
 *      background to be.
 *************************************************************/
void fillBackground(volatile unsigned short color) {
    DMANow(3, &color, VIDEO_BUFFER, 38400 |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_ON);

}

/** DMANow *****************************************************
 * Uses Direct Memory Access to set memory.
 * int channel - the DMA channel you want to use.
 * volatile void* source - pointer to the spot in memory you want copied
 * void* destination - pointer to the spot in memory where you want
 *                      the data to be copied to.
 * unsigned int control - sets the control bits of DMA
 *************************************************************/
void DMANow(int channel, volatile void* source, void* destination, unsigned int control) {
    dma[channel].src = source;
    dma[channel].dst = destination;
    dma[channel].cnt = control;
}

/** drawRect *****************************************************
 * Draws a rectangle at (row, col) with given height, width,
 *  and color.
 * int row - the row at which you want the rectangle to be
 * int col - the column at which you want the rectangle to be
 * int height - the height of the rectangle
 * int width - the width of the rectangle
 * volatile unsigned short color - the color you want the rectangle
 *                              to be.
 *************************************************************/
void drawRect(int row, int col, int height, int width, volatile unsigned short color) {
    int r;
    for(r = 0; r < height; r++)
	DMANow(3, &color, &VIDEO_BUFFER[OFFSET(row + r, col, SCREENWIDTH)], width |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_ON);
}

/** waitForVblank **********************************************
 * Waits for the gameboy display's vertical blank
 *************************************************************/
void waitForVblank() {
    while (SCANLINECOUNTER > 160);
    while (SCANLINECOUNTER < 160);
}
