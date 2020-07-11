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

  char* str[24][3] = {{"you", "are my", "everything"}, {"i", "love", "you!"}, {"i", "would eat", "durian for you"},
                    {"i love", "planning for", "our future together"}, {"carrying", "you is my", "favorite workout"},
                    {"whenever", "you're with me", "i feel safe"}, {"whenever", "you need me", "i'll be there"},
                    {"you look so", "stunning in", "anything you wear"}, {"i think", "you have", "long legs!"},
                    {"you'll", "always be", "my best friend"}, {"you're so", "cute when", "you're passionate"},
                    {"being intimate", "with you makes my", "heart flutter A LOT"}, {"you make", "me laugh like", "no one else"},
                    {"saying goodnight", "is the hardest", "part of my day"}, {"you have", "the prettiest", "voice in the world"},
                    {"i", "wanna cuddle", "you forever"}, {"you", "are", "a-dory-able!"}, {"i love", "you more than", "you love noodles!"},
                    {"my worst", "day will never", "come if im with you"}, {"our 3 dogs, 1 (maybe)", "cat, and fish will", "have the bestest mom!"},
                    {"your", "smile is", "unbelievably pretty"}, {"nature is", "prettier when i'm", "hiking with you"},
                    {"i blush when", "you tell me to wait", "a sex- oop, SEC ;)"}, {"the only time i want", "to do the dishes is", "when you tell me to"}};
  for(int i = 0; i < 24; i ++){
    oled.Set_Color(VICTORIA_BLUE);
    for(int j = 0; j < 3; j ++){
      int start = 64 - strlen(str[i][j]) * 2.88;
      oled.print_String(start, (j + 3.2) * 15, str[i][j], FONT_5X8);
    }
    oled.Draw_Heart();
    delay(1000);
    oled.Clear_Screen();
  }
}


void loop() {

}
