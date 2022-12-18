#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//---- Sensor ----
#define LDR 3       // 조도센서
int old_light = 0;  // 이전 밝기

//---- Actuator ----
#define NEO_PIN 11
#define NUMPIXELS 50  // 사용할 NeoPixel 수
#define BRIGHTNESS 125 

Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);
int start_pixel = 0;

#define BUZZER 8                     // 수동부저
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C 연결 (2행 16열)

//---- Global ----
String user_id = "1";
String message = "";
String data = "";
String old_data = "";

//---- Slave ----
void setup() {
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();
  pixels.show();
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

  if (data != "" && !data.equals(old_data)) {
    parsingDone(data);
    Serial.print("msg : ");
    Serial.println(message);
    lcd.clear();
    lcd.setCursor(0, 0);
    if (message.length() > 16) {
      lcd.print(message.substring(0, 15));
      lcd.setCursor(0, 1);
      lcd.print(message.substring(15, 30));
    } else if (message.length() > 0 && message.length() < 16) {
      lcd.print(message);
    }
    else if (message.length() == 0) {
      lcd.print("No MESSAGE:)");
    }

    Serial.println("-----------");
    Serial.println(old_data);
    Serial.println(data);
    Serial.println("-----------");
    old_data = data;
  }

  delay(100);
}

// Master 요청 받은 경우 빛 감지
void requestEvent() {
  // 빛 감지
  if (old_light == 0) {
    old_light = analogRead(LDR);
  }
  int val = analogRead(LDR);
  Serial.print("LDR:");
  Serial.print(old_light);
  Serial.print(",");
  Serial.print(val);
  if (old_light - val > 300) {
    Wire.write('1');
  } else {
    Wire.write('0');
  }

  old_light = val;
  delay(1000);
}

String temp = "";
int count_sharp = 0;  // start-end flag

void receiveEvent(int howMany) {
  Serial.println(howMany);
  String sMessage = "";

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);

    if (count_sharp < 2) {
      if (c == '#')
        count_sharp += 1;
      else
        sMessage += c;
    } else
      Wire.read();
  }
  Serial.println();
  temp += sMessage;

  if (count_sharp == 2) {
    data = temp;
    Serial.print("data");
    Serial.println(data);
    count_sharp = 0;
    temp = "";
  }
  delay(100);
}

// 할일목록 조회 내용 파싱
void parsingDone(String res) {
  String color = "";
  int total_cnt = 0;
  int done_cnt = 0;
  int msg_positive = 0;
  Serial.print("hey:");
  Serial.println(res);

  if (res.indexOf('|') != -1) {
    // 1. 색상
    int idx = res.indexOf('|');
    if (idx > 0) {
      color = res.substring(0, idx);
    } else {
      color = "";
      offNeoPixels();
    }
    res = res.substring(idx + 1);

    // 2. todo 갯수
    if (res.indexOf('|') != -1 && res.indexOf(',') != -1) {
      idx = res.indexOf('|');            // 두번째 | Split
      int comma_idx = res.indexOf(',');  // , Split
      total_cnt = res.substring(0, comma_idx).toInt();
      done_cnt = res.substring(comma_idx + 1, idx).toInt();

      // if (idx + 1 > res.length())
      //   message = res.substring(idx + 1);
      // else
      //   message = "";

      // 3. message
      res = res.substring(idx + 1);
      if (res.indexOf(',') != -1) {
        comma_idx = res.indexOf(',');
        if (comma_idx == 0)
          message = "";
        else
          message = res.substring(0, comma_idx);
        msg_positive = res.substring(comma_idx + 1).toInt();
      }

      Serial.println("parsing done");
      if ((total_cnt > 0 && total_cnt == done_cnt) || msg_positive == 10) {
        // 모두완료
        onNeoPixels_rainbow();
        rudolph();
      } else {
        coloring(color, total_cnt, done_cnt);
      }
    }
  }

  delay(100);
}


// 할일 완료 여부에 따른 LED ON 처리
void coloring(String color, int total, int done) {
  if (color.length() == 0) {
    offNeoPixels();
    return;
  }
  // 시작 픽셀 초기화
  start_pixel = 0;
  int color_cnt = color.length() / 9;
  pixels.clear();

  int pixel_count = 0;
  if (done > 0)
    pixel_count = NUMPIXELS * (float(done) / float(total)) / done;
  else
    pixel_count = NUMPIXELS * (float(done) / float(total));

  for (int i = 0; i < color_cnt; i++) {
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

  delay(1000);
}
