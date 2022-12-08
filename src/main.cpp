#include <Arduino.h>
#include <wire.h>
#include <Adafruit_ST7735.h>
#include <menu.h>
#include <ST7735_Renderer.h>

#define TFT_CS     15
#define TFT_RST    2
#define TFT_DC     0

Adafruit_ST7735 Tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Menu AppMenu;
ST7735_Renderer DisplayRenderer = {&Tft};

/// @brief Method to draw data to the screen for the first menu item
void displayFirstScreen()
{
  Tft.setCursor(0, 0);
  Tft.write("First Screen!");
  Tft.setCursor(0, 20);
  Tft.write("All ur stuff goes here!");
}

void setup() {
  Tft.initR(INITR_BLACKTAB);
  Tft.fillScreen(ST7735_BLACK);

  Serial.begin(115200);

  AppMenu.UseRenderer(&DisplayRenderer);
  AppMenu.Add("Shiny shit", &displayFirstScreen);
}

void loop() {
  // put your main code here, to run repeatedly:
}