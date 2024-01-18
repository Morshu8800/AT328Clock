//Debug
void debug(void) {
  delay(1000);
  rtc.gettime();
  hour=rtc.Hours;
  minute=rtc.minutes;
  year=rtc.year;
  if (hour==0 && minute==0 && year==0) {
    oled.init();
    oled.clear();
    oled.setCursor(0, 0);
    oled.print("Ошибка! Код:<0x02>.");
    oled.setCursor(0, 1);
    oled.print("Подробнее об ошибке");
    oled.setCursor(0, 2);
    oled.print("cмотрите на сайте:");
    oled.setCursor(0, 3);
    oled.print("https://github.com/");
    oled.setCursor(0, 4);
    oled.print("Morshu8800/AT328Clock/");
    oled.setCursor(0, 5);
    oled.print("issues/Clock_Corrupt");
    while (1) {
      for (int i = 0; i < 2; i++) {
        digitalWrite(LED, 1);
        delay(200);
        digitalWrite(LED, 0);
        delay(200);
      }
      delay(2000);
    }
  }
  if(!AHT.begin()){
    oled.init();
    oled.clear();
    oled.setCursor(0, 0);
    oled.print("Ошибка! Код:<0x03>.");
    oled.setCursor(0, 1);
    oled.print("Подробнее об ошибке");
    oled.setCursor(0, 2);
    oled.print("cмотрите на сайте:");
    oled.setCursor(0, 3);
    oled.print("https://github.com/");
    oled.setCursor(0, 4);
    oled.print("Morshu8800/AT328Clock/");
    oled.setCursor(0, 5);
    oled.print("issues/AHT_Corrupt");
    while (1) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED, 1);
        delay(200);
        digitalWrite(LED, 0);
        delay(200);
      }
      delay(2000);
    }
  }
}