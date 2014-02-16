#ifndef MYLIB_H
#define	MYLIB_H

#define REG_DISPCTL *(unsigned short *)0x4000000
#define VIDEO_BUFFER ((unsigned short *)0x6000000)
#define SCANLINECOUNTER *(volatile u16 *)0x4000006

/* Create a color with the specified RGB values */
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)

/* Calculate the position of a pixel based on it's row and column */
#define SCREENWIDTH 240
#define OFFSET(r,c,rowlen) ((r)*(rowlen)+(c))

//Predefined colors
#define RED COLOR(31, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0, 0, 31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0
#define WHITE COLOR(31, 31, 31)

// Buttons
#define BUTTON_A (1<<0)
#define BUTTON_B (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START (1<<3)
#define BUTTON_RIGHT (1<<4)
#define BUTTON_LEFT (1<<5)
#define BUTTON_UP (1<<6)
#define BUTTON_DOWN (1<<7)
#define BUTTON_R (1<<8)
#define BUTTON_L (1<<9)

#define BUTTONS *(volatile unsigned int *)0x4000130
#define BUTTON_HELD(key)  (~(BUTTONS) & (key))
#define BUTTON_PRESSED(key) (!(~(oldButtons)&(key)) && (~buttons & (key)))

typedef unsigned short u16;

// *** DMA =========================================================

void DMANow(int channel, volatile void* source, void* destination, unsigned int control);

// DMA channel 3 register definitions
#define REG_DMA3SAD *(volatile u32*)0x40000D4  // source address
#define REG_DMA3DAD *(volatile u32*)0x40000D8  // destination address
#define REG_DMA3CNT *(volatile u32*)0x40000DC  // control register

typedef volatile struct
{
    volatile const void *src;
    volatile void *dst;
    volatile unsigned int cnt;
} DMA;

extern DMA *dma;

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

void setPixel(int, int, unsigned short);
void waitForVblank();
void drawHorzLine(int row, int col, int length, volatile unsigned short color);
void drawVertLine(int row, int col, int length, u16 color);
void fillBackground(volatile unsigned short color);
void DMANow(int channel, volatile void* source, void* destination, unsigned int control);
void drawRect(int row, int col, int height, int width, volatile unsigned short color);
void pause(int);

//defined colors that help when assigning ship_picture
#define RED COLOR(31, 0, 0)
#define GRE COLOR(0, 31, 0)
#define BLU COLOR(0, 0, 31)
#define MAG COLOR(31, 0, 31)
#define CYA COLOR(0, 31, 31)
#define YEL COLOR(31, 31, 0)
#define BLA 0
#define WHI COLOR(31, 31, 31)

extern const unsigned short ship_picture[20*20];
extern const unsigned short enemy_full_health_picture[20*10];
extern const unsigned short enemy_half_health_picture[20*10];
extern const unsigned short health_bar[8*8];
extern const unsigned short bull_colors[6];

#endif	/* MYLIB_H */

