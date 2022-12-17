int color[7][3] = {{255, 0, 0},   // 빨
                  {255, 50, 0},   // 주
                  {255, 255, 0},  // 노
                  {0, 255, 0},    // 초
                  {0, 0, 255},    // 파
                  {0, 5, 70},     // 남
                  {100, 0, 255}   // 보
                  };
void onNeoPixels(int r, int g, int b, int count){
  // pixels.clear(); // Set all pixel colors to 'off'
  Serial.println("Hey");
  
  for(int i=0; i<count; i++) { // For each pixel...
    pixels.setPixelColor(start_pixel + i, pixels.Color(r, g, b));
    pixels.show();  
    delay(100); // Pause before next pass through loop
  }
  start_pixel += count;
}


void onNeoPixels_rainbow(){
  pixels.clear(); // Set all pixel colors to 'off'
  int c = 0;

  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(color[c][0], color[c][1], color[c][2]));
    pixels.show();  
    if (c == 6)
      c = 0;
    else
      c += 1;
    delay(100); // Pause before next pass through loop
  }
  pixels.rainbow(0);
}