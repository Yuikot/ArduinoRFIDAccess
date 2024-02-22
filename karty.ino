#define USE_ALB_RFID
#define USE_ALB_LCD_I2C 
#define SS_PIN 10 // Chip Select Pin
#define RST_PIN 9 // Reset Pin
#include "ArduinoLearningBoard.h"
ALB_LCD_I2C lcd;
RFID rfid(SS_PIN, RST_PIN);
int serNum[5];


void setup()
{
  SPI.begin();
  rfid.init();
  lcd.init();
  lcd.clear();
  lcd.backlight(); // Włącz podświetlenie wyświetlacza
  startingScreen();
}

void dots(){
  for(int i = 0; i<3 ; i++){
    lcd.print(".");
    delay(1000);
  }
}


void startingScreen() 
{
  lcd.print("Please put your");
  while(!rfid.isCard())
  {
    lcd.setCursor(0, 1);
    lcd.print("card here");
    dots();
    lcd.setCursor(0, 1);
    lcd.print("card here   ");
  }
}

void loop()
{
	if(rfid.isCard())
	{
		if(rfid.readCardSerial())
		{
			lcd.clear(); 
      lcd.setCursor(0, 0); 
      bool accessGranted = true;
      byte expectedSerial[] = {0x83, 0xB8, 0x2B, 0x25, 0x35};
      for (int i = 0; i < 5; i++) {
        if (rfid.serNum[i] != expectedSerial[i]) {
          accessGranted = false;
          break;
        }
      }

      if(accessGranted)
      {
        lcd.print("Access granted");
        delay(5000);
        lcd.clear();
        rfid.halt();
        startingScreen();
      }
      else 
      {
        lcd.print("Access denied");
        delay(2000);
        lcd.clear();
        startingScreen();
      }
		}
	}
}