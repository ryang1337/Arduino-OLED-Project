#include "src/OLED_Driver.h"
#include "src/OLED_GFX.h"

#include <SPI.h>
#include <Wire.h>

using namespace std;

OLED_GFX oled = OLED_GFX();

void setup()  {

  //Init GPIO
  pinMode(oled_cs, OUTPUT);
  pinMode(oled_rst, OUTPUT);
  pinMode(oled_dc, OUTPUT);

  //Init UART
  Serial.begin(115200);

#if INTERFACE_4WIRE_SPI
  //Init SPI
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();

#elif INTERFACE_3WIRE_SPI

  pinMode(oled_sck, OUTPUT);
  pinMode(oled_din, OUTPUT);

#endif

  oled.Device_Init();

  oled.Set_Color(VICTORIA_BLUE);

  char* str[3] = {"i love", "you more", "than anything"};
  for(int i = 0; i < 3; i ++){
    int start = 64 - strlen(str[i]) * 2.5;
    oled.print_String(start, (i + 3) * 15, str[i], FONT_5X8);
  }

  oled.Draw_Heart();
}


void loop() {

}
