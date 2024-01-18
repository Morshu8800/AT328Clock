#include <GyverOLED.h>    //Библиотека дисплея
#include <iarduino_RTC.h> //Библиотека модуля часов
#include <EncButton.h>    //Библиотека кнопки
#include <AHT10.h>        //Библиотека датчика температуры и влажности
#include <EEPROM.h>       //Библиотека Энергонезависимой памяти
#include <GyverJoy.h>     //Библиотека Джойстика

#define EEPROM_ADDR 90    //Адрес ячейки памяти
#define buzzer_freq 2000  //Частота зуммера
#define PIEZO 9           //Зуммер
#define LED 13             //Светодиод будильника


iarduino_RTC rtc(RTC_DS3231);                     //Часы
GyverJoy jy(A0);                                  //Ось OY
GyverJoy jx(A1);                                  //Ось OX
Button but(A2);                                    //Кнопка
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;   //Экран
AHT10 AHT(0x38);                                  //Датчик

int16_t fh = 0, fm = 0, Humidity, Temperature, hour=20, minute=59, second=0, day=18, month=1;
int16_t year = 24, weekday=4, s_minute = 0, s_hour = 0, menusel = 1, almset = 0, timset = 0, settup = 0, sel=0, sel2=0;
bool timedisp = 1, dispflag = 0, setings_alarm = 0, setings_alarm_hour1 = 0, setings_alarm_minute1 = 0, setings_alarm_hour2 = 0, setings_alarm_minute2 = 0;
bool setings_time = 0, sp = 1, setings_hour = 0, setings_minute = 0;
char curs='>',sruc='<';
uint32_t TimerBuzzer, timetemp, savebattime, inputwait;

struct{                          //Структура для сохранения данных
  bool alarm_flag1 = 0, alarm_flag2 = 0;
  uint16_t alarm_hour1 = 0, alarm_minute1 = 0, alarm_hour2 = 0, alarm_minute2 = 0;
} alset;                         //Её наименование (ярлык)
