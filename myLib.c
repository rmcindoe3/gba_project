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

//A color array you can iterate through with static counter variables
//  to produce a flashing effect.
const unsigned short bull_colors[6] = {
    COLOR(0, 0, 31),
    COLOR(0, 31, 31),
    COLOR(0, 31, 0),
    COLOR(31, 31, 0),
    COLOR(31, 0, 0),
    COLOR(31, 0, 31)
};

//Our ship's picture array
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

//Our picture array for the bosses
const unsigned short boss_picture[40*20] = {
    CYA, CYA, CYA, CYA, CYA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, CYA, CYA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, CYA, CYA, CYA, CYA, CYA,
    CYA, CYA, CYA, CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA, CYA, CYA, CYA,
    CYA, CYA, CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA, CYA, CYA,
    CYA, CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA, CYA,
    CYA, CYA, BLA, BLA, RED, RED, RED, RED, CYA, CYA, CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, CYA, CYA, RED, RED, RED, RED, BLA, BLA, CYA, CYA,
    CYA, BLA, BLA, RED, RED, RED, RED, CYA, RED, RED, RED, CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, RED, RED, RED, CYA, RED, RED, RED, RED, BLA, BLA, CYA,
    BLA, BLA, RED, RED, RED, RED, CYA, RED, RED, RED, RED, RED, CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, RED, RED, RED, RED, RED, CYA, RED, RED, RED, RED, BLA, BLA,
    BLA, BLA, RED, RED, RED, RED, CYA, RED, RED, RED, RED, RED, CYA, CYA, CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, CYA, CYA, RED, RED, RED, RED, RED, CYA, RED, RED, RED, RED, BLA, BLA,
    BLA, BLA, RED, RED, RED, RED, CYA, RED, RED, RED, RED, RED, CYA, CYA, CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, CYA, CYA, RED, RED, RED, RED, RED, CYA, RED, RED, RED, RED, BLA, BLA,
    BLA, BLA, RED, RED, RED, RED, CYA, RED, RED, RED, RED, RED, CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, RED, RED, RED, RED, RED, CYA, RED, RED, RED, RED, BLA, BLA,
    BLA, BLA, RED, RED, RED, RED, RED, CYA, RED, RED, RED, CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, RED, RED, RED, CYA, RED, RED, RED, RED, RED, BLA, BLA,
    BLA, BLA, RED, RED, RED, RED, RED, RED, CYA, CYA, CYA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, CYA, CYA, CYA, RED, RED, RED, RED, RED, RED, BLA, BLA,
    CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA,
    CYA, CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA, CYA,
    CYA, CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA, CYA,
    CYA, CYA, CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA, CYA, CYA,
    CYA, CYA, CYA, CYA, BLA, BLA, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLA, BLA, CYA, CYA, CYA, CYA,
    CYA, CYA, CYA, CYA, CYA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, CYA, CYA, CYA, CYA, CYA,
    CYA, CYA, CYA, CYA, CYA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, BLA, CYA, CYA, CYA, CYA, CYA,
};

//Our picture array for a full health normal enemy.
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

//Our picture array for a half health normal enemy.
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

//Our picture array for a full health big enemy.
const unsigned short big_enemy_full_health_picture[20*10] = {
    CYA, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, CYA, 
    CYA, BLU, BLU, BLU, BLU, BLU, BLA, BLU, BLU, BLA, BLA, BLU, BLU, BLA, BLU, BLU, BLU, BLU, BLU, CYA, 
    CYA, CYA, BLU, BLU, BLU, BLU, BLA, BLU, BLU, BLA, BLA, BLU, BLU, BLA, BLU, BLU, BLU, BLU, CYA, CYA, 
    CYA, CYA, CYA, BLU, BLU, BLU, BLU, BLU, BLU, BLA, BLA, BLU, BLU, BLU, BLU, BLU, BLU, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLU, BLU, BLU, BLU, BLU, BLA, BLA, BLU, BLU, BLU, BLU, BLU, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, CYA, BLU, BLU, BLU, BLU, BLU, BLU, BLU, BLU, CYA, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, BLU, BLU, BLU, BLU, BLU, BLU, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, CYA, BLU, BLU, BLU, BLU, CYA, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, BLU, BLU, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
};

//Our picture array for a half health big enemy.
const unsigned short big_enemy_half_health_picture[20*10] = {
    CYA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, BLA, CYA, 
    CYA, BLA, BLU, BLA, BLU, BLA, BLA, BLA, BLU, BLA, BLA, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, CYA, 
    CYA, CYA, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLA, BLA, BLU, BLA, BLA, BLA, BLU, BLA, BLU, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLA, BLA, BLU, BLA, BLU, BLA, BLU, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, BLU, BLA, BLU, BLA, BLA, BLA, BLU, BLA, BLU, BLA, BLU, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, CYA, BLA, BLU, BLA, BLU, BLA, BLU, BLA, BLU, CYA, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, BLA, BLU, BLA, BLU, BLA, BLU, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, CYA, BLA, BLU, BLA, BLU, CYA, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, BLA, BLU, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
};

//Our picture array for a full health tri enemy.
const unsigned short tri_enemy_full_health_picture[20*10] = {
    CYA, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, CYA, 
    CYA, GRE, GRE, GRE, GRE, GRE, BLA, GRE, GRE, BLA, BLA, GRE, GRE, BLA, GRE, GRE, GRE, GRE, GRE, CYA, 
    CYA, CYA, GRE, GRE, GRE, GRE, BLA, GRE, GRE, BLA, BLA, GRE, GRE, BLA, GRE, GRE, GRE, GRE, CYA, CYA, 
    CYA, CYA, CYA, GRE, GRE, GRE, GRE, GRE, GRE, BLA, BLA, GRE, GRE, GRE, GRE, GRE, GRE, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, GRE, GRE, GRE, GRE, GRE, BLA, BLA, GRE, GRE, GRE, GRE, GRE, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, CYA, GRE, GRE, GRE, GRE, GRE, GRE, GRE, GRE, CYA, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, GRE, GRE, GRE, GRE, GRE, GRE, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, CYA, GRE, GRE, GRE, GRE, CYA, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, GRE, GRE, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
};

//Our picture array for a half health tri enemy.
const unsigned short tri_enemy_half_health_picture[20*10] = {
    CYA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, BLA, CYA, 
    CYA, BLA, GRE, BLA, GRE, BLA, BLA, BLA, GRE, BLA, BLA, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, CYA, 
    CYA, CYA, BLA, GRE, BLA, GRE, BLA, GRE, BLA, BLA, BLA, GRE, BLA, BLA, BLA, GRE, BLA, GRE, CYA, CYA, 
    CYA, CYA, CYA, BLA, GRE, BLA, GRE, BLA, GRE, BLA, BLA, BLA, GRE, BLA, GRE, BLA, GRE, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, GRE, BLA, GRE, BLA, BLA, BLA, GRE, BLA, GRE, BLA, GRE, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, BLA, CYA, BLA, GRE, BLA, GRE, BLA, GRE, BLA, GRE, CYA, BLA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, BLA, GRE, BLA, GRE, BLA, GRE, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, BLA, CYA, CYA, CYA, CYA, BLA, GRE, BLA, GRE, CYA, CYA, CYA, CYA, BLA, CYA, CYA, CYA, 
    CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, BLA, GRE, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, CYA, 
};

//Our picture array for the health bars at bottom of screen.
const unsigned short health_bar[8*8] = {
    CYA, CYA, CYA, RED, RED, CYA, CYA, CYA, 
    CYA, CYA, RED, RED, RED, RED, CYA, CYA, 
    CYA, RED, RED, GRE, GRE, RED, RED, CYA, 
    RED, RED, GRE, GRE, GRE, GRE, RED, RED, 
    RED, RED, GRE, GRE, GRE, GRE, RED, RED, 
    CYA, RED, RED, GRE, GRE, RED, RED, CYA, 
    CYA, CYA, RED, RED, RED, RED, CYA, CYA, 
    CYA, CYA, CYA, RED, RED, CYA, CYA, CYA, 
};
