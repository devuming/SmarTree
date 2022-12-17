#include <Adafruit_NeoPixel.h>
#include<LiquidCrystal_I2C.h>
#include<Wire.h>
#include <ArduinoJson.h>
//---- Sensor ----

String todo = "";
String old_todo = "none";
String user_id = "1";
String message = "";
String old_message = "";

//---- Actuator ----
#define NEO_PIN 11
#define NUMPIXELS 50 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);
int start_pixel = 0;

#define BUZZER 8
LiquidCrystal_I2C lcd(0x27, 16, 2);   // LCD I2C 연결 (2행 16열)

//---- Slave ----
void setup() {
  Wire.begin(8);   
  Wire.onReceive(receiveEvent);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  lcd.begin();
  lcd.backlight();
  
  Serial.begin(115200);
}
void loop() {
  // onNeoPixels_rainbow();
  // todo List 조회
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  delay(1000);
}
void receiveEvent(int howMany){
  Serial.println("receive");
  while (Wire.available()) {
    char c = Wire.read();
    message += c;
  }
  Serial.print("수신:");
  Serial.println(message);
  lcd.print(message);
  
  // 변경된 경우에만 LCD새로하기
  if(message != "" && !message.equals(old_message))
  {
    old_message = message;
  }

  // if(todo != "" && !todo.equals(old_todo))
  // {
    // parsingDone(todo);
  //   old_todo = todo;
  // }
  // todo = "";    //  버퍼 비우기
  
  delay(1000);
}
