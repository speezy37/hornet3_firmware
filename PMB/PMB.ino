#include <PololuLedStrip.h>
#include "U8glib.h"

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 3
rgb_color colors[3];

// OLED I2C init
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);

float current1, current2, voltage1, voltage2;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  
  u8g.drawStr( 0, 15, "Power Monitoring");
  u8g.drawStr( 0, 30, "     V       V  ");
  u8g.setPrintPos(0, 30);
  u8g.print(voltage1);
  u8g.setPrintPos(64, 30);
  u8g.print(voltage2);
  u8g.drawStr( 0, 45, "     A       A  ");
  u8g.setPrintPos(0, 45);
  u8g.print(current1);
  u8g.setPrintPos(64, 45);
  u8g.print(current2);
  u8g.drawStr( 0, 60, "Capacity: ");
}

void setup() {
  Serial.begin(9600);
  
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
      colors[i] = rgb_color(0,255,0);
  }

  ledStrip.write(colors, LED_COUNT);
}

void loop() {
  u8g.firstPage();
  do {
    draw();
  } while( u8g.nextPage() );
  
  current1 = (analogRead(3) * 33.0) / 1023.0;
  current2 = (analogRead(0) * 33.0) / 1023.0;

  voltage1 = (analogRead(2) * 3.3 * 24.7) / (1023.0 * 4.7);
  voltage2 = (analogRead(1) * 3.3 * 24.7) / (1023.0 * 4.7);
  
  Serial.print(current1);
  Serial.print(" A  ");
  Serial.print(voltage1);
  Serial.print(" V ");
  Serial.print(current2);
  Serial.print(" A  ");
  Serial.print(voltage2);
  Serial.println(" V");

  if((voltage1 < 13.2) || (voltage2 < 13.2)){
    for(uint16_t i = 0; i < LED_COUNT; i++)
    {
        colors[i] = rgb_color(255,0,0);
    }    
  }
  else{
    for(uint16_t i = 0; i < LED_COUNT; i++)
    {
        colors[i] = rgb_color(0,255,0);
    } 
  }
  
  ledStrip.write(colors, LED_COUNT);
  delay(100);
}
