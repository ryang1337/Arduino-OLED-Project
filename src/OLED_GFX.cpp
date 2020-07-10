#include "OLED_GFX.h"
#include "ASCII_Font.h"

#include "stdlib.h"

uint8_t Row,Column;
FONT_SIZE Font_Size;

OLED_GFX::OLED_GFX(void)  {
  Row = 0;
  Column = 0;
  Font_Size = FONT_5X8;
}

void OLED_GFX::Set_FontSize(FONT_SIZE size) {
  Font_Size = size;
}
  
void OLED_GFX::print_String(uint8_t x, uint8_t y, const uint8_t *text, FONT_SIZE size) {

  Font_Size = size;

  if(Font_Size == FONT_5X8) {
    Display_String_5x8(x, y, text);
  }
  else if(Font_Size == FONT_8X16) {
    Display_String_8x16(x, y, text);
  }
}

void OLED_GFX::Display_String_8x16(uint8_t x, uint8_t y, const uint8_t *text) {
  
  uint16_t i=0,j,k,n;
  if(x>120)  {
    x=1;
  }
  while(text[i]>0x00) {
    if((text[i]>=0x20)&&(text[i]<=0x7e))  {
      j=text[i]-0x20;
      for(n=0;n<2;n++)  {
        for(k=0;k<8;k++)  {
          Set_Address(x+k, n*8+y);
          Write_text(ascii_table_8x16[j][k+8*n]);
        }
      }
      i++;
      x+=8;
    }
    else
      i++;
  }
} 

void OLED_GFX::Display_String_5x8(uint8_t x, uint8_t y, const uint8_t *text)  {
  
  uint8_t i=0,j,k;
  
  while(text[i]>0x00) {  
    if((text[i]>=0x20)&&(text[i]<=0x7e))  {
      j=text[i]-0x20;
      for(k=0; k<5;k++)  {
        Set_Address(x+k, y);
        Write_text(ascii_table_5x8[j][k]);
      }
      Set_Address(x+5, y);   
      Write_text(0x00);
      i++;
      x+=6;
      if(x>=128)
        x=1;
    }
    else  
      i++;
  }
}

void OLED_GFX::Draw_Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  // Update in subclasses if desired!
  if(x0 == x1)  {
    if(y0 > y1) swap(y0, y1);
    Draw_FastVLine(x0, y0, y1 - y0 + 1);
  } 
  else if(y0 == y1) {
    if(x0 > x1)
      swap(x0, x1);
    Draw_FastHLine(x0, y0, x1 - x0 + 1);
  }
  else
    Write_Line(x0, y0, x1, y1);
}
 

// Draw a rectangle
void OLED_GFX::Draw_Rect(int16_t x, int16_t y, int16_t w, int16_t h) {
    
  Draw_FastHLine(x, y, w);
  Draw_FastHLine(x, y+h-1, w);
  Draw_FastVLine(x, y, h);
  Draw_FastVLine(x+w-1, y, h);
}


void OLED_GFX::Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

  if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT))
  return;

  // Y bounds check
  if (y+h > SSD1351_HEIGHT) {
    h = SSD1351_HEIGHT - y - 1;
  }

  // X bounds check
  if (x+w > SSD1351_WIDTH)  {
    w = SSD1351_WIDTH - x - 1;
  }

  // set location
  Write_Command(SSD1351_CMD_SETCOLUMN);
  Write_Data(x);
  Write_Data(x+w-1);
  Write_Command(SSD1351_CMD_SETROW);
  Write_Data(y);
  Write_Data(y+h-1);
  // fill!
  Write_Command(SSD1351_CMD_WRITERAM);  
  
  for (uint16_t i=0; i < w*h; i++) {
    Write_Data(color_fill_byte[0]);
    Write_Data(color_fill_byte[1]);
  }
}

void OLED_GFX::Fill_Circle(int16_t x0, int16_t y0, int16_t r) {
  
  Draw_FastVLine(x0, y0-r, 2*r+1);
  FillCircle_Helper(x0, y0, r, 3, 0);
}

// Used to do circles and roundrects
void OLED_GFX::FillCircle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t corner, int16_t delta)  {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while(x<y)  {
    if(f >= 0)  {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    if(corner & 0x1)  {
      Draw_FastVLine(x0+x, y0-y, 2*y+1+delta);
      Draw_FastVLine(x0+y, y0-x, 2*x+1+delta);
    }
    if(corner & 0x2)  {
      Draw_FastVLine(x0-x, y0-y, 2*y+1+delta);
      Draw_FastVLine(x0-y, y0-x, 2*x+1+delta);
    }
  }
}