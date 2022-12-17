// this method makes a HTTP connection to the server
void httpRequest(String url)
{
  Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    
    // send the HTTP PUT request
    client.println("GET " + url + " HTTP/1.1");
    client.println(F("Host: 15.164.111.44"));
    client.println("Connection: keep-Alive");
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}

// 와이파이 상태 출력
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  // Serial.print("Signal strength (RSSI):");
  // Serial.print(rssi);
  // Serial.println(" dBm");
}

// 할일목록 조회 내용 파싱
void parsingDone(String res){
  String color = "";
  int total_cnt = 0;
  int done_cnt = 0;

   if (res.indexOf('|') != -1){
     int idx = res.indexOf('|');
     color = res.substring(0, idx);
     String count = res.substring(idx + 1, res.length());

     // 갯수
     int cnt_idx = count.indexOf(',');
     total_cnt = count.substring(0, cnt_idx).toInt();
     done_cnt = count.substring(cnt_idx+1, count.length()).toInt();
   }
   
   Serial.print(" t:");
   Serial.print(total_cnt);
   Serial.print(" done:");
   Serial.println(done_cnt);
   coloring(color, total_cnt, done_cnt);
   delay(100);
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
    Serial.print("pixel_count:");
    Serial.println(pixel_count);

    // 네오픽셀 ON
    onNeoPixels(r, g, b, pixel_count);
  }
  delay(10000);
}
