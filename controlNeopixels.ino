int color[7][3] = {
  { 255, 0, 0 },    // 빨
  { 255, 50, 0 },   // 주
  { 255, 255, 0 },  // 노
  { 0, 255, 0 },    // 초
  { 0, 0, 255 },    // 파
  { 0, 5, 70 },     // 남
  { 100, 0, 255 }   // 보
};

void onNeoPixels(int r, int g, int b, int count) {
  // pixels.clear(); // Set all pixel colors to 'off'
  pixels.setBrightness(BRIGHTNESS);

  for (int i = 0; i < count; i++) {  // For each pixel...
    pixels.setPixelColor(start_pixel + i, pixels.Color(r, g, b));
    pixels.show();

    delay(10);
  }
  start_pixel += count;
}


void onNeoPixels_rainbow() {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < NUMPIXELS; i++) {
      int pixelHue = firstPixelHue + (i * 65536L / NUMPIXELS);
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show();
    delay(10);
  }
}

void offNeoPixels() {
  for (int i = BRIGHTNESS; i >= 0; i--) {
    pixels.setBrightness(i);
    pixels.show();
  }
  pixels.clear();
  pixels.show();
  delay(100);
}


