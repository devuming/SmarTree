#include <Adafruit_NeoPixel.h>
#include<LiquidCrystal_I2C_Hangul.h>
#include<Wire.h>
#include <ArduinoJson.h>
//---- Sensor ----
//---- Wifi ----
#include "WiFiEsp.h"
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3);  //RX, TX
char ssid[] = "umin";            // your network SSID (name)
char pass[] = "umin1234";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char server[] = "15.164.111.44";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 30000L; // 60초 마다

WiFiEspClient client;

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
// LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2);   // LCD I2C 연결 (2행 16열)

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  // lcd.init();
  // lcd.backlight();
  
  Serial.begin(115200);     // initialize serial for debugging
  Serial1.begin(9600);      // initialize serial for ESP module
  // initialize ESP module
  WiFi.init(&Serial1);      // initialize ESP module

  // 와이파이 연결
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  
  printWifiStatus();
}
void loop() {
  // onNeoPixels_rainbow();
  // todo List 조회
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    String line = client.readStringUntil('\r');
    
    if(line.indexOf(':') != -1){
      String header = line.substring(line.indexOf('\n') + 1, line.indexOf(':'));
      if(header.equals("MSG"))
      {
        message = line.substring(line.indexOf(':') + 1);
        Serial.println(message);
      }
    }
    else if(line.indexOf('#') != -1)
    {
      todo = line.substring(line.indexOf('#') + 1, line.length());
      Serial.print(todo);
    }
  }

  // 변경된 경우에만 조명새로하기
  if(todo != "" && !todo.equals(old_todo))
  {
    parsingDone(todo);
    old_todo = todo;
  }
  todo = "";    //  버퍼 비우기
  
  delay(100);

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    // httpRequest("/service/done/" + user_id);
    httpRequest("/service/message/" + user_id);
  }
}
