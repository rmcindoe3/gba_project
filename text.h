#ifndef TEXT_H
#define TEXT_H

void drawChar(int row, int col, char ch, unsigned short color);
void drawString(int row, int col, char *str, unsigned short color);

#define OFFSET2(r, c, rowlen) ((r)*(rowlen)+(c))

extern const unsigned char fontdata_6x8[12288];

#endif