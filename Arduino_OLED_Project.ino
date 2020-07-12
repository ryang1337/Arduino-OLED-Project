#include "src/OLED_Driver.h"
#include "src/OLED_GFX.h"
#include "src/DS3231.h"

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

using namespace std;

OLED_GFX oled = OLED_GFX();
DS3231 rtc(SDA, SCL);

unsigned long prevMillis = 0;
const long interval = 2000;

unsigned int currMessage = EEPROM.read(0);
unsigned int prevDay = EEPROM.read(1);

char* messages[23][3] = {{"you", "are my", "everything"}, {"i", "love", "you!"}, {"i", "would eat", "durian for you"},
                        {"i love", "planning for", "our future together"}, {"carrying", "you is my", "favorite workout"},
                        {"whenever", "you're with me", "i feel safe"}, {"the only time i want", "to do the dishes is", "when you tell me to"},
                        {"you look so", "stunning in", "anything you wear"}, {"i think", "you have", "long legs!"},
                        {"you'll", "always be", "my best friend"}, {"you're so", "cute when", "you're passionate"},
                        {"being intimate", "with you makes my", "heart flutter A LOT"}, {"you make", "me laugh like", "no one else"},
                        {"saying goodnight", "is the hardest", "part of my day"}, {"you have", "the prettiest", "voice in the world"},
                        {"i", "wanna cuddle", "you forever"}, {"you", "are", "a-dory-able!"}, {"i love", "you more than", "you love noodles!"},
                        {"my worst", "day will never", "come if im with you"}, {"our 3 dogs, 1 (maybe)", "cat, and fish will", "have the bestest mom!"},
                        {"your", "smile is", "unbelievably pretty"}, {"nature is", "prettier when i'm", "hiking with you"},
                        {"i blush when", "you tell me to wait", "a sex- oop, SEC ;)"}};

void setup()  {

  Serial.begin(115200);

  rtc.begin();

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

  // my code literally breaks if i dont have 
  // this here i have no idea why but im just
  // gonna leave it.  ¯\_(ツ)_/¯
  Serial.println(currMessage);

  printMessage(currMessage);
}

void printMessage(int num){
  oled.Clear_Screen();

  oled.Set_Color(VICTORIA_BLUE);

  for(int i = 0; i < 3; i ++){
    int start = 64 - strlen(messages[num][i]) * 2.88; // centers text horizontally based on length of string
    oled.print_String(start, (i + 3.2) * 15, messages[num][i], FONT_5X8);
  }

  oled.Draw_Heart();
}



void loop() {

  unsigned long currMillis = millis();

  if(currMillis - prevMillis >= interval){
    // get current day and store into int
    char* currDate = rtc.getDateStr();
    char dayStr[2];
    strncpy(dayStr, currDate, 2);
    int currDay = atoi(dayStr);

    if(currDay != prevDay){

      // print next message
      currMessage = (currMessage + 1) % 23;
      printMessage(currMessage);
      
      prevDay = currDay;

      // update permanent memory to recall last printed
      // message and minute so messages play even after
      // restart or power off
      EEPROM.update(0, currMessage);
      EEPROM.update(1, currDay);
    }
    prevMillis = currMillis;
  }
}
