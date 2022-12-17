#include <Adafruit_NeoPixel.h>
#include<LiquidCrystal_I2C.h>
#include<Wire.h>
//---- Sensor ----


//---- Actuator ----
#define NEO_PIN 11
#define NUMPIXELS 50 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);
int start_pixel = 0;

#define BUZZER 8
LiquidCrystal_I2C lcd(0x27, 16, 2);   // LCD I2C 연결 (2행 16열)

//---- Global ----
String user_id = "1";
String message = "";
String data = "";
String old_data = "";

//---- Slave ----
void setup() {
  Wire.begin(1);   
  Wire.onReceive(receiveEvent);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  lcd.begin();
  lcd.backlight();
  // lcd.print("Hello");
  
  Serial.begin(115200);
  
  user_id = "1";
  message = "hello";
  data = "";
  old_data = "";
}
void loop() {
  // onNeoPixels_rainbow();
  if(data != "" && !data.equals(old_data)){
    parsingDone(data);
    Serial.print("msg : ");
    Serial.println(message);
    lcd.clear();
    lcd.setCursor(0,0);
    if(message.length()>16){
      lcd.print(message.substring(0, 15));
      lcd.setCursor(0,1);
      lcd.print(message.substring(15, 30));
    }else{
      lcd.print(message);
    }
    old_data = data;
  }
  
  delay(100);
}

String temp = "";
int count_sharp = 0;  // start-end flag
void receiveEvent(int howMany){
  String sMessage = "";

  while (Wire.available()) { 
    char c = Wire.read();
    Serial.print(c);
    
    
    // sMessage = Wire.readStringUntil('\n');  
    // Serial.println(sMessage); 
    if (c == '#')
      count_sharp += 1;
    else
      sMessage += c;

    if (count_sharp == 2)
      break;
  }
  Serial.println();
  temp += sMessage;
  
  Serial.println(sMessage);
  Serial.println(temp);
    // Serial.println(); 
  // if(sMessage.indexOf('#') != -1)
  // {
  //   // Serial.println("###"); 
  //   // temp += sMessage;
  //   // Serial.println(temp);
  //   if (count_sharp == 2)
  //   {
  //     data = temp;
  //     Serial.print('data : ');
  //     Serial.println(data);
  //     count_sharp = 0;
  //   }
  // }

  if (count_sharp == 2)
  {
    data = temp;
    Serial.print('data : ');
    Serial.println(data);
    count_sharp = 0;
    temp = "";
  }
  delay(1000);
}

// 할일목록 조회 내용 파싱
void parsingDone(String res){
  String color = "";
  int total_cnt = 0;
  int done_cnt = 0;

   if (res.indexOf('|') != -1){
     int idx = res.indexOf('|');
     color = res.substring(0, idx);

     // 갯수
     res = res.substring(idx + 1);
     if(res.indexOf('|') != -1)
     {
      idx = res.indexOf('|');            // 두번째 | Split
      if (res.indexOf(',') != -1)
      {
        int cnt_idx = res.indexOf(',');    // , Split
        total_cnt = res.substring(0, cnt_idx).toInt();
        done_cnt = res.substring(cnt_idx+1, idx).toInt();
        if(idx + 1 < res.length())
          message = res.substring(idx + 1);
        else
          message = "";
          
        Serial.print(total_cnt);
        Serial.print("/");
        Serial.print(done_cnt);
        Serial.print("/");
        Serial.println(message);
        if(total_cnt == done_cnt)
        {
          onNeoPixels_rainbow();
        }else{
          coloring(color, total_cnt, done_cnt);
        }
      }
     }
   
    delay(100);
   }
}

// 할일 완료 여부에 따른 LED ON 처리
void coloring(String color, int total, int done){
  // 시작 픽셀 초기화
  start_pixel = 0;
  int color_cnt = color.length() / 9;
  pixels.clear();

  int pixel_count = NUMPIXELS * (float(done)/float(total)) / done;
  for(int i = 0; i < color_cnt; i++)
  {
    int r = color.substring((i * 9), (i * 9) + 3).toInt();
    int g = color.substring((i * 9) + 3, (i * 9) + 6).toInt();
    int b = color.substring((i * 9) + 6, (i * 9) + 9).toInt();

    Serial.print("r:");
    Serial.print(r);
    Serial.print("g:");
    Serial.print(g);
    Serial.print("b:");
    Serial.println(b);

    // 네오픽셀 ON
    onNeoPixels(r, g, b, pixel_count);
  }

  delay(10000);
}
