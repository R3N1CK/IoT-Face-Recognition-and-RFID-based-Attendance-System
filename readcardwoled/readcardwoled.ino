#include <SPI.h>
#include <MFRC522.h>


#include <Wire.h>
#include"U8glib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>
#include<Fonts/Cousine_Regular_7.h>
#include<Fonts/font.h>
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SS_PIN 53
#define RST_PIN 5

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte nuidPICC[4];
const unsigned char myBitmap []  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x2f, 0xf8, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0xff, 0xf9, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xe0, 0x0b, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x00, 0x01, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x8e, 0x00, 0x00, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3c, 0x00, 0x00, 0x1c, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x70, 0x01, 0x00, 0x0e, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x60, 0x07, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc3, 0xff, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xc3, 0xff, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x83, 0xff, 0x00, 0x01, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x83, 0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x03, 0xff, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x03, 0x0f, 0x71, 0x80, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x03, 0x73, 0x71, 0x80, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x03, 0x7b, 0x73, 0x80, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x03, 0x7b, 0x33, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x03, 0x07, 0x3b, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x07, 0x7f, 0x3f, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x03, 0x77, 0x1e, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x03, 0x7f, 0x1e, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x03, 0x7b, 0x0e, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x03, 0x7b, 0x0e, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x01, 0xff, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0xff, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0xff, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xc0, 0x7f, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x80, 0x7f, 0x00, 0x03, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x20, 0x3f, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x20, 0x1f, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x60, 0x07, 0x00, 0x0a, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x4c, 0x03, 0x00, 0x24, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x00, 0x02, 0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x28, 0x19, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x6d, 0x09, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x49, 0x08, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x83, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



void setup() 
{
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  u8g2.begin();
  u8g2.enableUTF8Print();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }/*
  display.clearDisplay();
  display.setCursor(30, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" RV College");
  display.display();
  //delay(3000);

  display.setCursor(0, 22);
  display.setFont(&Roboto_Black_12);
  display.setTextColor(WHITE);
  display.println("   MCA Department");
  display.display();
  //delay(3000);
  */
  u8g2.firstPage();
  do {
      //u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
      //u8g2.drawGlyph(65,60,118); 
      //u8g2.drawGlyph(45,60,117); 
      u8g2.drawBitmap(0,0,16,64,myBitmap); 
      u8g2.setFont(u8g2_font_ncenB12_tr);
      u8g2.setCursor(6, 15);
      u8g2.print("RV COLLEGE");         
  } while ( u8g2.nextPage() );
  delay(3000);
  
   
  
  
  delay(3000);
  
  u8g2.firstPage();
  do {
     u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
      u8g2.drawGlyph(65,60,118); 
        u8g2.drawGlyph(45,60,117); 
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(45, 15);
    u8g2.print("MCA");  
    u8g2.setCursor(4,30);
    u8g2.print("Department");   
  } while ( u8g2.nextPage() );

  delay(3000);
  
  u8g2.firstPage();
  do {
     
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(37, 15);
    u8g2.print("Smart"); 
    u8g2.setCursor(10, 35);
    u8g2.print("Attendance");
    u8g2.setCursor(30, 55);
    u8g2.print("System");      
  } while ( u8g2.nextPage() );
 

  
}

void(* resetFunc) (void) = 0;

void loop() {
  
  
  
 while (!Serial.available()) {   
  }

  int status=Serial.parseInt();


    
  //Serial.print(status);
  if (status==1)
  {
  
     // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  while(!rfid.PICC_IsNewCardPresent()){}
  //rfid.PICC_IsNewCardPresent();
    

  // Verify if the NUID has been readed
  //while (rfid.PICC_ReadCardSerial()) {}
  rfid.PICC_ReadCardSerial();
    
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    //    Serial.println(F("Error: Unsupported card error"));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3] ) {
    //    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    printHex(rfid.uid.uidByte, rfid.uid.size);    
    
  }  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  }

  
else if(status==2)
{
  u8g2.firstPage();
  do {
     u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
      u8g2.drawGlyph(70,55,84); 
        u8g2.drawGlyph(40,55,102); 
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(12, 15);
    u8g2.print("Place Card");     
  } while ( u8g2.nextPage() );       
}
           
else if(status==3)
{
  u8g2.firstPage();
  do {
     u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
      u8g2.drawGlyph(70,55,229); 
        u8g2.drawGlyph(40,55,64); 
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(12, 15);
    u8g2.print("Show Face");     
  } while ( u8g2.nextPage() );
}

else if(status==4)
{
    u8g2.firstPage();
  do {
     u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
      u8g2.drawGlyph(70,55,72); 
        u8g2.drawGlyph(40,55,264); 
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(12, 15);
    u8g2.print("  Marked!");     
  } while ( u8g2.nextPage() );
}
else if(status==5)
{
   u8g2.firstPage();
  do {
     u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
      u8g2.drawGlyph(70,55,210); 
        u8g2.drawGlyph(40,55,121); 
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.setCursor(10, 12);
    u8g2.print("Already Marked");     
  } while ( u8g2.nextPage() );
}
else if(status==6)
{
    u8g2.firstPage();
  do {
     u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
      u8g2.drawGlyph(70,55,87); 
        u8g2.drawGlyph(40,55,139); 
    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.setCursor(10, 15);
    u8g2.print("Invalid Card!");     
  } while ( u8g2.nextPage() );
}
else if (status==7)
{
  resetFunc();
}
}        
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    //    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

  
