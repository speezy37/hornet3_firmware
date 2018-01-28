#include <PololuLedStrip.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <stdlib.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 3
rgb_color colors[LED_COUNT];

#define RED    rgb_color(255,0,0)
#define GREEN  rgb_color(0,255,0)

// OLED I2C init
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

float current1, current2, voltage1, voltage2, track1, track2;
int count;

void draw(){
  u8x8.drawString( 0, 0, "POWER MONITORING");
  u8x8.drawString( 0, 2, "      V       V ");
  u8x8.drawString( 0, 4, "      A       A ");
  u8x8.drawString( 0, 6, "      %       % "); 
}

void updateOled(float volt1,float volt2, float curr1, float curr2, float in1, float in2){
  char test[5];
  
  dtostrf(volt1,5,2,test);
  u8x8.drawString( 0, 2, test); 
  
  dtostrf(volt2,5,2,test);
  u8x8.drawString( 8, 2, test); 

  dtostrf(curr1,5,2,test);
  u8x8.drawString( 0, 4, test); 

  dtostrf(curr2,5,2,test);
  u8x8.drawString( 8, 4, test);
  
  dtostrf(99.99f - in1,5,2,test);
  u8x8.drawString( 0, 6, test); 

  dtostrf(99.99f - in2,5,2,test);
  u8x8.drawString( 8, 6, test);
}

SIGNAL(TIMER0_COMPA_vect) {
  count++;
  
  if(count == 100){
    count = 0;
    
    current1 = (analogRead(3) * 33.0f) / 1023.0f;
    current2 = (analogRead(0) * 33.0f) / 1023.0f;

    track1 += (current1 / 36000.0f);
    track2 += (current2 / 36000.0f);

    voltage1 = (analogRead(2) * 3.3f * 24.7f) / (1023.0f * 4.7f);
    voltage2 = (analogRead(1) * 3.3f * 24.7f) / (1023.0f * 4.7f);
  }
}

void setup() {
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  draw();
  
  for(uint16_t i = 0; i < LED_COUNT; i++)
    colors[i] = GREEN;

  ledStrip.write(colors, LED_COUNT);
}

void loop() {
  if((voltage1 < 13.2) || (voltage2 < 13.2)){
    for(uint16_t i = 0; i < LED_COUNT; i++)
      colors[i] = RED;
  }
  else{
    for(uint16_t i = 0; i < LED_COUNT; i++)
      colors[i] = GREEN;
  }
  
  updateOled(voltage1, voltage2, current1, current2, track1, track2);
  
  ledStrip.write(colors, LED_COUNT);
  delay(250);
}
