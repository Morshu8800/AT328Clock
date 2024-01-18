/*
 Мой GitHub: https://github.com/Morshu8800/
*/

#include "Variables.h"
#include "Debug.h"

void setup(void) {
  if (EEPROM.read(0) != 'w') {
    EEPROM.put(1, alset);
    EEPROM.write(0, 'w');
  } else {
    EEPROM.get(1, alset);
  }
  pinMode(LED, OUTPUT);
  jx.calibrate();
  jy.calibrate();
  jx.exponent(GJ_CUBIC);
  jy.exponent(GJ_CUBIC);
  AHT.begin();
  rtc.begin();  //Инициализация модуля времени
  rtc.settime(second,minute,hour,day,month,year,weekday); //секунды минуты часы дни месяцы годы
  oled.init();           //Инициализация экрана
  oled.setContrast(63);  //Яркость экрана
  oled.clear();          //Очистка экрана
  oled.setScale(1);      //Размер шрифта
  runningText();         //Вызываем функцию приветствия
  //oled.drawBitmap(30, 0, secret_bitmap_66x66, 66, 66, BITMAP_NORMAL);
  //oled.drawBitmap(30, 0, secret_bitmap2_66x66, 66, 66, BITMAP_NORMAL);
}

void loop(void) {
  but.tick();
  jx.tick();
  jy.tick();
  dispout();
  control();
  savedat();
  Alarm();
  savebat();
}

void dispout(void) {
  while (dispflag == 0) {
    oled.clear();
    dispflag = !dispflag;
    break;
  }
  if (timedisp == 1 and settup == 0 and setings_alarm == 0 and setings_time == 0) {
    oled.setScale(2);
    oled.setCursor(0, 0);
    oled.print(rtc.gettime("H:i:s"));
    oled.setScale(1);
    oled.setCursor(0, 2);
    oled.print(rtc.gettime("d/M/Y,D"));
    oled.setCursor(10, 6);
    oled.print("МЕНЮ");
    oled.setCursor(70, 6);
    oled.print("НАСТР.БУД");
    if (menusel == 0) {
      oled.setCursor(60, 6);
      oled.print(" ");
      oled.setCursor(0, 6);
      oled.print(curs);
    } else {
      oled.setCursor(0, 6);
      oled.print(" ");
      oled.setCursor(60, 6);
      oled.print(curs);
    }

    if (millis() - timetemp > 500) {
      timetemp = millis();
      Humidity = AHT.readHumidity(AHT10_USE_READ_DATA);
      Temperature = AHT.readTemperature(AHT10_FORCE_READ_DATA);
      oled.setCursor(0, 4);
      oled.print("Т: ");
      oled.setCursor(15, 4);
      oled.print(Temperature);
      oled.setCursor(30, 4);
      oled.print("C");
      oled.setCursor(50, 4);
      oled.print("В: ");
      oled.setCursor(65, 4);
      oled.print(Humidity);
      oled.setCursor(80, 4);
      oled.print("%");
      if (Temperature < 10) {
        oled.setCursor(20, 4);
        oled.print(" ");
      }
      if (Humidity < 10) {
        oled.setCursor(70, 4);
        oled.print(" ");
      }
    }
    
  } else if (timedisp == 0 and settup == 1 and setings_alarm == 0 and setings_time == 0) {
    while (dispflag == 0) {
      oled.clear();
      dispflag = !dispflag;
      break;
    }
    oled.setCursor(53, 0);
    oled.print("МЕНЮ");
    if (sel == 0) {
      oled.setCursor(0, 1);
      oled.print(curs);
      oled.setCursor(0, 3);
      oled.print(" ");
      oled.setCursor(0, 5);
      oled.print(" ");
      oled.setCursor(0, 7);
      oled.print(" ");
    } else if (sel == 1) {
      oled.setCursor(0, 1);
      oled.print(" ");
      oled.setCursor(0, 3);
      oled.print(curs);
      oled.setCursor(0, 5);
      oled.print(" ");
      oled.setCursor(0, 7);
      oled.print(" ");
    } else if (sel == 2) {
      oled.setCursor(0, 1);
      oled.print(" ");
      oled.setCursor(0, 3);
      oled.print(" ");
      oled.setCursor(0, 5);
      oled.print(curs);
      oled.setCursor(0, 7);
      oled.print(" ");
    } else if (sel == 3) {
      oled.setCursor(0, 1);
      oled.print(" ");
      oled.setCursor(0, 3);
      oled.print(" ");
      oled.setCursor(0, 5);
      oled.print(" ");
      oled.setCursor(0, 7);
      oled.print(curs);
    }
    if (sel > 3) sel = 0;
    else if (sel < 0) sel = 3;
    oled.setCursor(6, 1);
    oled.print("НАСТРОЙКИ БУДИЛЬНИКА");
    oled.setCursor(6, 3);
    oled.print("НАСТРОЙКИ ЧАСОВ");
    oled.setCursor(6, 5);
    oled.print("О РАЗРАБОТЧИКАХ");
    oled.setCursor(6, 7);
    oled.print("ВЫХОД");
  } else if (timedisp == 0 and setings_alarm == 1 and settup == 2 and setings_time == 0) {
    oled.setCursor(5, 0);
    oled.print("НАСТРОЙКИ БУДИЛЬНИКА");
    if (alset.alarm_hour1 < 10 or alset.alarm_hour1 > 23) {
      oled.setCursor(10, 2);
      oled.print(fh);
      oled.setCursor(16, 2);
      oled.print(alset.alarm_hour1);
    } else if (alset.alarm_hour1 > 10 or alset.alarm_hour1 < 23) {
      oled.setCursor(10, 2);
      oled.print(alset.alarm_hour1);
    }
    oled.setCursor(24, 2);
    oled.print(":");
    if (alset.alarm_minute1 < 10 or alset.alarm_minute1 > 59) {
      oled.setCursor(30, 2);
      oled.print(fm);
      oled.setCursor(36, 2);
      oled.print(alset.alarm_minute1);
    } else if (alset.alarm_minute1 > 10 or alset.alarm_minute1 < 59) {
      oled.setCursor(30, 2);
      oled.print(alset.alarm_minute1);
    }
    oled.setCursor(10, 6);
    oled.print("ВЫХОД");
    if (alset.alarm_flag1 == 0) {
      oled.setCursor(10, 4);
      oled.print("БУД1:ВЫКЛ");
    } else if (alset.alarm_flag1 == 1) {
      oled.setCursor(10, 4);
      oled.print("БУД1:ВКЛ ");
    }
    if (alset.alarm_hour2 < 10 or alset.alarm_hour2 > 23) {
      oled.setCursor(75, 2);
      oled.print(fh);
      oled.setCursor(81, 2);
      oled.print(alset.alarm_hour2);
    } else if (alset.alarm_hour2 > 10 or alset.alarm_hour2 < 23) {
      oled.setCursor(75, 2);
      oled.print(alset.alarm_hour2);
    }
    oled.setCursor(89, 2);
    oled.print(":");
    if (alset.alarm_minute2 < 10 or alset.alarm_minute2 > 59) {
      oled.setCursor(95, 2);
      oled.print(fm);
      oled.setCursor(101, 2);
      oled.print(alset.alarm_minute2);
    } else if (alset.alarm_minute2 > 10 or alset.alarm_minute2 < 59) {
      oled.setCursor(95, 2);
      oled.print(alset.alarm_minute2);
    }
    if (alset.alarm_flag2 == 0) {
      oled.setCursor(75, 4);
      oled.print("БУД2:ВЫКЛ");
    } else if (alset.alarm_flag2 == 1) {
      oled.setCursor(75, 4);
      oled.print("БУД2:ВКЛ ");
    }

    if (almset == 0 and setings_alarm_hour1 == 0 and setings_alarm_minute1 == 0 and setings_alarm_hour2 == 0 and setings_alarm_minute2 == 0) {
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(curs);
      oled.setCursor(0, 6);
      oled.print(" ");
      oled.setCursor(65, 4);
      oled.print(" ");
      oled.setCursor(112, 2);
      oled.print(" ");
      oled.setCursor(65, 2);
      oled.print(" ");
      oled.setCursor(65, 6);
      oled.print(" ");
    } else if (almset == 1 and setings_alarm_hour1 == 0 and setings_alarm_minute1 == 0 and setings_alarm_hour2 == 0 and setings_alarm_minute2 == 0) {
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(curs);
      oled.setCursor(47, 2);
      oled.print(sruc);
      oled.setCursor(0, 6);
      oled.print(" ");
      oled.setCursor(65, 4);
      oled.print(" ");
      oled.setCursor(112, 2);
      oled.print(" ");
      oled.setCursor(65, 2);
      oled.print(" ");
      oled.setCursor(65, 6);
      oled.print(" ");
    } else if (almset == 2) {
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(" ");
      oled.setCursor(0, 4);
      oled.print(curs);
      oled.setCursor(0, 6);
      oled.print(" ");
      oled.setCursor(65, 4);
      oled.print(" ");
      oled.setCursor(112, 2);
      oled.print(" ");
      oled.setCursor(65, 2);
      oled.print(" ");
      oled.setCursor(65, 6);
      oled.print(" ");
    } else if (almset == 3 and setings_alarm_hour1 == 0 and setings_alarm_minute1 == 0 and setings_alarm_hour2 == 0 and setings_alarm_minute2 == 0) {
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(" ");
      oled.setCursor(0, 6);
      oled.print(" ");
      oled.setCursor(65, 4);
      oled.print(" ");
      oled.setCursor(112, 2);
      oled.print(" ");
      oled.setCursor(65, 2);
      oled.print(curs);
      oled.setCursor(65, 6);
      oled.print(" ");
    } else if (almset == 4 and setings_alarm_hour1 == 0 and setings_alarm_minute1 == 0 and setings_alarm_hour2 == 0 and setings_alarm_minute2 == 0) {
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(" ");
      oled.setCursor(0, 6);
      oled.print(" ");
      oled.setCursor(65, 4);
      oled.print(" ");
      oled.setCursor(65, 2);
      oled.print(curs);
      oled.setCursor(112, 2);
      oled.print(sruc);
      oled.setCursor(65, 6);
      oled.print(" ");
    } else if (almset == 5) {
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(" ");
      oled.setCursor(0, 6);
      oled.print(" ");
      oled.setCursor(112, 2);
      oled.print(" ");
      oled.setCursor(65, 2);
      oled.print(" ");
      oled.setCursor(65, 4);
      oled.print(curs);
      oled.setCursor(0, 6);
      oled.print(" ");
    } else if (almset == 6) {
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(" ");
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(0, 6);
      oled.print(curs);
      oled.setCursor(65, 4);
      oled.print(" ");
      oled.setCursor(112, 2);
      oled.print(" ");
      oled.setCursor(65, 2);
      oled.print(" ");
      oled.setCursor(65, 6);
      oled.print(" ");
    }
    if (almset > 6) almset = 0;
    else if (almset < 0) almset = 6;
    if (almset == 0 and setings_alarm_hour1 == 1) {
      oled.setCursor(0, 2);
      oled.print("?");
    } else if (almset == 1 and setings_alarm_minute1 == 1) {
      oled.setCursor(47, 2);
      oled.print("?");
    } else if (almset == 3 and setings_alarm_hour2 == 1) {
      oled.setCursor(65, 2);
      oled.print("?");
    } else if (almset == 4 and setings_alarm_minute2 == 1) {
      oled.setCursor(112, 2);
      oled.print("?");
    }
  } else if (timedisp == 0 and setings_alarm == 0 and settup == 3 and setings_time == 1) {
    oled.setCursor(12, 0);
    oled.print("НАСТРОЙКА ЧАСОВ");
    if (s_hour < 10 or s_hour > 23) {
      oled.setCursor(10, 2);
      oled.print(fh);
      oled.setCursor(16, 2);
      oled.print(s_hour);
    } else if (s_hour > 10 or s_hour < 23) {
      oled.setCursor(10, 2);
      oled.print(s_hour);
    }
    oled.setCursor(24, 2);
    oled.print(":");
    if (s_minute < 10 or s_minute > 59) {
      oled.setCursor(30, 2);
      oled.print(fm);
      oled.setCursor(36, 2);
      oled.print(s_minute);
    } else if (s_minute > 10 or s_minute < 59) {
      oled.setCursor(30, 2);
      oled.print(s_minute);
    }
    oled.setCursor(10, 4);
    oled.print("УСТАНОВИТЬ ВРЕМЯ");
    oled.setCursor(10, 6);
    oled.print("ВЫХОД");

    if (timset == 0 and setings_hour == 0) {
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(curs);
      oled.setCursor(0, 6);
      oled.print(" ");
    } else if (timset == 1 and setings_minute == 0) {
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(curs);
      oled.setCursor(47, 2);
      oled.print(sruc);
      oled.setCursor(0, 6);
      oled.print(" ");
    } else if (timset == 2) {
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(" ");
      oled.setCursor(0, 4);
      oled.print(curs);
      oled.setCursor(0, 6);
      oled.print(" ");
    } else if (timset == 3) {
      oled.setCursor(47, 2);
      oled.print(" ");
      oled.setCursor(0, 2);
      oled.print(" ");
      oled.setCursor(0, 4);
      oled.print(" ");
      oled.setCursor(0, 6);
      oled.print(curs);
    }
    if (timset > 3) timset = 0;
    else if (timset < 0) timset = 3;
    if (timset == 0 and setings_hour == 1) {
      oled.setCursor(0, 2);
      oled.print("?");
    } else if (timset == 1 and setings_minute == 1) {
      oled.setCursor(47, 2);
      oled.print("?");
    }
  } else if (timedisp == 0 and setings_alarm == 0 and settup == 4 and setings_time == 0) {
    oled.setCursor(13, 0);
    oled.print("О РАЗРАБОТЧИКАХ");
    oled.setCursor(0, 1);
    oled.print("ЭТИ ЧАСЫ СОЗДАНЫ");
    oled.setCursor(0, 3);
    oled.print("ГЛАВ.ПРОГ:FlY DeN");
    oled.setCursor(0, 5);
    oled.print("ПРОГ.БИ-ТЕК: ALEX.G");
    oled.setCursor(0, 7);
    oled.print(curs);
    oled.setCursor(45, 7);
    oled.print(" ");
    oled.setCursor(10, 7);
    oled.print("ВЫХОД");
    oled.setCursor(60, 7);
    oled.print("ВЕРСИЯ:1.1");
  }
}

void control(void) {
  if (timedisp == 1 and settup == 0 and setings_alarm == 0 and setings_time == 0) {
    if (millis() - inputwait > 200 and jx.value() >= 100 and menusel == 0) {
      menusel++;
      inputwait=millis();
    } else if (millis() - inputwait > 200 and jx.value() <= -100 and menusel == 1) {
      menusel--;
      inputwait=millis();
    }
    if (but.click() and menusel == 1) {
      setings_alarm = 1;
      settup = 2;
      dispflag = 0;
      timedisp=0;
      but.reset();
    }
    if (but.click() and timedisp == 1 and menusel == 0) {
      settup = 1;
      timedisp = 0;
      dispflag = 0;
      but.reset();
    }
  }
  if (timedisp == 0 and settup == 1 and setings_alarm == 0 and setings_time == 0) {
    if (millis() - inputwait > 200 and jy.value() >= 100) {
      sel++;
      inputwait=millis();
    } else if (millis() - inputwait > 200 and jy.value() <= -100) {
      sel--;
      inputwait=millis();
    }
    if (sel == 0 and but.click()) {
      setings_alarm = 1;
      settup = 2;
      dispflag = 0;
      but.reset();
    } else if (sel == 1 and but.click()) {
      settup = 3;
      setings_time = 1;
      dispflag = 0;
      but.reset();
    } else if (sel == 2 and but.click()) {
      settup = 4;
      dispflag = 0;
      but.reset();
    } else if (sel == 3 and but.click()) {
      settup = 0;
      dispflag = 0;
      timedisp = 1;
      sel = 0;
      but.reset();
    }
  }
  if (timedisp == 0 and settup == 2 and setings_alarm == 1 and setings_time == 0) {
    if (millis() - inputwait > 200 and jy.value() >= 100 and setings_alarm_minute1 == 0 and setings_alarm_hour1 == 0 and setings_alarm_minute2 == 0 and setings_alarm_hour2 == 0) {
      almset++;
      inputwait=millis();
    } else if (millis() - inputwait > 200 and jy.value() <= -100 and setings_alarm_minute1 == 0 and setings_alarm_hour1 == 0 and setings_alarm_minute2 == 0 and setings_alarm_hour2 == 0) {
      almset--;
      inputwait=millis();
    }
    if (setings_alarm_minute1 == 0 and setings_alarm_minute2==0 and almset == 0 and but.holding()) {
      setings_alarm_hour1 = !setings_alarm_hour1;
      but.reset();
    } else if (setings_alarm_hour1 == 0 and setings_alarm_hour2==0 and almset == 1 and but.holding()) {
      setings_alarm_minute1 = !setings_alarm_minute1;
      but.reset();
    } else if (setings_alarm_minute1 == 0 and setings_alarm_minute2==0 and almset == 3 and but.holding()) {
      setings_alarm_hour2 = !setings_alarm_hour2;
      but.reset();
    } else if (setings_alarm_hour1 == 0 and setings_alarm_hour2==0 and almset == 4 and but.holding()) {
      setings_alarm_minute2 = !setings_alarm_minute2;
      but.reset();
    }
    if (millis() - inputwait > 200 && jy.value() >= 100 && setings_alarm_hour1 == 1 && setings_alarm_minute1 == 0 && setings_time == 0 && setings_alarm == 1 && almset == 0 && setings_alarm_hour2 == 0 && setings_alarm_minute2 == 0 && setings_time == 0) {
      alset.alarm_hour1++;
      inputwait=millis();
      if (alset.alarm_hour1 == 24) {
        alset.alarm_hour1 = 0;
      }
    }
    if (millis() - inputwait > 200 && jy.value() <= -100 && setings_alarm_hour1 == 1 && setings_alarm_minute1 == 0 && setings_time == 0 && setings_alarm == 1 && almset == 0 && setings_alarm_hour2 == 0 && setings_alarm_minute2 == 0 && setings_time == 0) {
      alset.alarm_hour1--;
      inputwait=millis();
      if (alset.alarm_hour1 == -1) {
        alset.alarm_hour1 = 23;
      }
    }
    if (millis() - inputwait > 200 && jy.value() >= 100 && setings_alarm_hour1 == 0 && setings_alarm_minute1 == 1 && setings_time == 0 && setings_alarm == 1 && almset == 1 && setings_alarm_hour2 == 0 && setings_alarm_minute2 == 0 && setings_time == 0) {
      alset.alarm_minute1++;
      inputwait=millis();
      if (alset.alarm_minute1 == 60) {
        alset.alarm_minute1 = 0;
      }
    }
    if (millis() - inputwait > 200 && jy.value() <= -100 && setings_alarm_hour1 == 0 && setings_alarm_minute1 == 1 && setings_time == 0 && setings_alarm == 1 && almset == 1 && setings_alarm_hour2 == 0 && setings_alarm_minute2 == 0 && setings_time == 0) {
      alset.alarm_minute1--;
      inputwait=millis();
      if (alset.alarm_minute1 == -1) {
        alset.alarm_minute1 = 59;
      }
    }
    if (millis() - inputwait > 200 && jy.value() >= 100 && setings_alarm_hour1 == 0 && setings_alarm_minute1 == 0 && setings_time == 0 && setings_alarm == 1 && almset == 3 && setings_alarm_hour2 == 1 && setings_alarm_minute2 == 0 && setings_time == 0) {
      alset.alarm_hour2++;
      inputwait=millis();
      if (alset.alarm_hour2 == 24) {
        alset.alarm_hour2 = 0;
      }
    }
    if (millis() - inputwait > 200 && jy.value() <= -100 && setings_alarm_hour1 == 0 && setings_alarm_minute1 == 0 && setings_time == 0 && setings_alarm == 1 && almset == 3 && setings_alarm_hour2 == 1 && setings_alarm_minute2 == 0 && setings_time == 0) {
      alset.alarm_hour2--;
      inputwait=millis();
      if (alset.alarm_hour2 == -1) {
        alset.alarm_hour2 = 23;
      }
    }
    if (millis() - inputwait > 200 && jy.value() >= 100 && setings_alarm_hour1 == 0 && setings_alarm_minute1 == 0 && setings_time == 0 && setings_alarm == 1 && almset == 4 && setings_alarm_hour2 == 0 && setings_alarm_minute2 == 1 && setings_time == 0) {
      alset.alarm_minute2++;
      inputwait=millis();
      if (alset.alarm_minute2 == 60) {
        alset.alarm_minute2 = 0;
      }
    }
    if (millis() - inputwait > 200 && jy.value() <= -100 && setings_alarm_hour1 == 0 && setings_alarm_minute1 == 0 && setings_time == 0 && setings_alarm == 1 && almset == 4 && setings_alarm_hour2 == 0 && setings_alarm_minute2 == 1 && setings_time == 0) {
      alset.alarm_minute2--;
      inputwait=millis();
      if (alset.alarm_minute2 == -1) {
        alset.alarm_minute2 = 59;
      }
    } else if (almset == 5 and but.holding()) {
      alset.alarm_flag2 = !alset.alarm_flag2;
      but.reset();
    }else if (almset == 2 and but.holding()) {
      alset.alarm_flag1 = !alset.alarm_flag1;
      but.reset();
    } else if (almset == 6 and but.click()) {
      settup = 1;
      setings_alarm = 0;
      dispflag = 0;
      almset = 0;
      but.reset();
    }
  }
  if (timedisp == 0 and settup == 3 and setings_alarm == 0 and setings_time == 1) {
    if (millis() - inputwait > 200 && jy.value() >= 100 and setings_minute == 0 and setings_hour == 0) {
      timset++;
      inputwait=millis();
    } else if (millis() - inputwait > 200 and jy.value() <= -100 and setings_minute == 0 and setings_hour == 0) {
      timset--;
      inputwait=millis();
    }
    if (setings_minute == 0 and timset == 0 and but.holding()) {
      setings_hour = !setings_hour;
      but.reset();
    } else if (setings_hour == 0 and timset == 1 and but.holding()) {
      setings_minute = !setings_minute;
      but.reset();
    }
    if (millis() - inputwait > 200 && jy.value() >= 100 && setings_hour == 1 && setings_minute == 0 && setings_time == 1 && setings_alarm == 0 && timset == 0) {
      s_hour++;
      inputwait=millis();
      if (s_hour == 24) {
        s_hour = 0;
      }
    }

    if (millis() - inputwait > 200 and jy.value() <= -100 && setings_hour == 1 && setings_minute == 0 && setings_time == 1 && setings_alarm == 0 && timset == 0) {
      s_hour--;
      inputwait=millis();
      if (s_hour == -1) {
        s_hour = 23;
      }
    }

    if (millis() - inputwait > 200 && jy.value() >= 100 && setings_hour == 0 && setings_minute == 1 && setings_time == 1 && setings_alarm == 0 && timset == 1) {
      s_minute++;
      inputwait=millis();
      if (s_minute == 60) {
        s_minute = 0;
      }
    }

    if (millis() - inputwait > 200 and jy.value() <= -100 && setings_hour == 0 && setings_minute == 1 && setings_time == 1 && setings_alarm == 0 && timset == 1) {  //Настраиваем время кнопками вверх вниз
      s_minute--;
      inputwait=millis();
      if (s_minute == -1) {
        s_minute = 59;
      }
    } else if (timset == 2 and but.click()) {
      dispflag = 0;
      timset = 0;
      but.reset();
      rtc.settime(second, s_minute, s_hour);
    } else if (timset == 3 and but.click()) {
      settup = 1;
      setings_time = 0;
      dispflag = 0;
      timset = 0;
      but.reset();
    }
  } else if (timedisp == 0 and setings_alarm == 0 and settup == 4 and setings_time == 0) {
    if (millis() - inputwait > 200 && jy.value() >= 100) {
      inputwait=millis();
      sel2 = 1;
    } else if (millis() - inputwait > 200 and jy.value() <= -100) {
      inputwait=millis();
      sel2 = 0;
    }
    if (but.click() and sel2 == 0) {
      settup = 1;
      dispflag = 0;
      but.reset();
    }

    if (alset.alarm_flag1 == 1 and sp == 1) {
      digitalWrite(LED, LOW);
    } else if (alset.alarm_flag1 == 1 and sp == 0) {
      digitalWrite(LED, HIGH);
    } else if (alset.alarm_flag1 == 0 and sp == 1 or sp == 0) {
      digitalWrite(LED, LOW);
    }
  }
}

  //Звук для зуммера
  void Sound() {
    if (millis() - TimerBuzzer > 500) {  //Делаем кратковременный писк зумера
      TimerBuzzer = millis();
      tone(PIEZO, buzzer_freq, 250);
    }
  }

  //Будильник
  void Alarm() {
    rtc.gettime();         //Получаем время
    hour = rtc.Hours;      //Получаем часы в цифрах
    minute = rtc.minutes;  //Получаем минуты в цифрах

    if (hour == alset.alarm_hour1 && minute == alset.alarm_minute1 && alset.alarm_flag1 == 1 || hour==alset.alarm_hour2 && minute==alset.alarm_minute2 && alset.alarm_flag2==1) {  //Если время выставленное в настройках совпало то пищим зумером
      Sound();
    } else {
      noTone(PIEZO);
    }
  }


  void runningText(void) {
    oled.setCursor(0, 0);
    oled.print("ТЕХНОЛОГИЯ:");
    oled.setCursor(0, 2);
    oled.print("FLYDEN И ALEXGYVER");
    oled.setCursor(0, 4);
    oled.print("ВЕРСИЯ 1.1,(С)2024");
    oled.setCursor(0, 6);
    oled.print("ИНИЦИАЛИЗАЦИЯ...");
    debug();
    oled.clear();
  }

  void savebat(void) {
    if (millis() - savebattime > 150000 and timedisp==1 and settup==0 and setings_alarm==0 and setings_time==0) {
      savebattime = millis();
      oled.setPower(false);
      sp = 0;
      }
      if (but.hold()) {
        oled.setPower(true);
        savebattime = millis();
        sp = 1;
        but.reset();
      }
  }

  //Сохранение данных
  void savedat() {
    if (timedisp == 1 and settup == 0) {  //Если всё спокойно
      EEPROM.put(1, alset);               //то записываем в ячейку 1 отличающиеся значения
    }
  }