//SQM - Монитор качества неба
/*
 * код написан для ардуино мега про, если надо переделать под нано, можно убрать функцию расчёта и отображения фазы Луны, ну и изменить номера пинов (та ещё задача)
 */
//подключаем библиотеки
#include <Wire.h>//Подключение I2c
#include <Adafruit_Sensor.h>//упрощает работу с сенсорами
#include <math.h>//библиотека для вычислений
#include <Adafruit_BME280.h>//датчик погоды(температура/давление/влажность)
#include "Adafruit_TSL2591.h"//люксометр
#include<TFT.h>//графический ЖКИ
#include <EEPROM.h>//подключаем встроенное ПЗУ для записи калибовочных данных люксометра
#include <SoftwareSerial.h>                       //  Подключаем библиотеку для работы с программным UART, до подключения библиотеки iarduino_GPS_NMEA.
#include <iarduino_GPS_NMEA.h>                    //  Подключаем библиотеку для расшифровки строк протокола NMEA получаемых по UART.

#define TFTcontrast 44//настройка вывода яркости экрана
//настройка люксометра
#define SQMBUTTON 7 //пин для кнопки включения люксометра
#define LOWSCALE 1.0//коэффициенты усиления (взяты из библиотеки датчика)
#define MEDSCALE 25.0
#define HIGHSCALE 428.0
#define MAXSCALE 9876.0
#define SQM_CAL_OFFSET  -1.0  // обычная калибровка офсетами SQM из ЕЕПРОМ
#define TEMP_CAL_OFFSET 0.0   // температурная калибровка
                          

//#define pinRX 6                          // для нано GPS Определяем вывод RX (программного UART) на плате Arduino к которому подключён вывод TX модуля. Номер вывода можно изменить.
//#define pinTX 5  

//время обновления показаний
#define UpdateMeteo 300
#define UpdateGPS  20

TFT TFTscreen = TFT(10,9,8);//настраиваем выводы ЖКЭ
//SoftwareSerial    SerialGPS(pinRX, pinTX);        //  для нано Объявляем объект SerialGPS для работы с функциями и методами библиотеки SoftwareSerial, указав выводы RX и TX Arduino.
iarduino_GPS_NMEA gps;                            //  Объявляем объект gps для работы с функциями и методами библиотеки iarduino_GPS_NMEA.

char tempstring[3];//для отображения числового значения переменной, её нужно преобразовать в символьный массив
char presstring[5];
char humstring[3];
char luxstring[6];
int tempArray[120];//массив для записи показаний датчика температуры
double sqmArray[120];//массив для записи показаний датчика sqm
float presArray[120];
char currentString[3];
volatile int Mode = 0;
volatile int flag = 0;
volatile uint32_t debounce;//это всё для функции прерывания и переключения режимов если чо
//присваиваем ключевое слово для вызова датчиков
Adafruit_BME280 bme;
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
//переменные для хранения показаний датчиков
double mag = 0;//SQM
int Temp = 0;//метеоданные
int Hum = 0;
float Pres = 0;
int hour;//gps
char hourstring[3];
int minute;
char minutestring[3];
int day;
char daystring[3];
int month;
char monthstring[3];
int year;
char yearstring[5];
double Lat;
char latstring[8];
double Long;
char longstring[8];
int Alt;
char altstring[4];
int SatAct;
char satactstring[3];
int SatVis;
char satvisstring[3];
//Луна

int old;//возраст
int moonNum=18;//лунное число
//char moonstring[3];

//Вспомогательные переменные для настройки датчиков
double gainscale = MAXSCALE;
uint32_t luminosity;
uint16_t ir, full, visible;
double adjustedVisible, adjustedIR;
float SqmCalOffset =  SQM_CAL_OFFSET ;   // SQM калибровка из EEPROM
float TempCalOffset = TEMP_CAL_OFFSET;   // температурная калибровка из EEPROM





void setup() {
  attachInterrupt(1, ModeFunc, CHANGE);
  Serial.begin(9600);//ну так на всякий случай
  Serial1.begin(9600);                       //  Инициируем работу с программной шиной UART для получения данных от GPS модуля на скорости 9600 бит/сек.
     gps.begin(Serial1);                        //  Инициируем расшифровку строк NMEA указав объект используемой шины UART (вместо программной шины, можно указывать аппаратные: Serial, Serial1, Serial2, Serial3).
     gps.timeZone(3);                             //  Указываем часовой пояс (±12 часов), или GPS_AutoDetectZone для автоматического определения часового пояса по долготе.
   TFTscreen.begin();//активируем экран
   TFTscreen.initR(INITR_GREENTAB);//Настройка экрана, сменить если есть смещение. На что? придётся погуглить
  TFTscreen.background(0,0,0);//Очищаем(устанавливаем цвета rgb в 0, т.е. чёрный цвет)
 TFTscreen.stroke(0,0,255);//установит цвет текста (красный)
//настраиваем люксометр и датчик погоды
pinMode(SQMBUTTON, INPUT);
bool status;
status = bme.begin();
tsl.begin();
  tsl.setGain(TSL2591_GAIN_MAX);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);
   SqmCalOffset = ReadEESqmCalOffset();     // Данные для калибровки люксометра из EEPROM
   TempCalOffset = ReadEETempCalOffset();   // Калибровка температуры из EEPROM
   startScreen();
   ReadMeteo();
   DisplMeteo();
   ReadGPS();
   ReadMoon();
   DisplMoon();
   DisplGPS();
   ReadSQM();
   DisplSQM();
   if(mag>=20){//яркость экрана устанавливается сразу после включения и измерения освещённости (Может потом сделать по-другому?)
   analogWrite(TFTcontrast,25);
   }
//   sqm.setCalibrationOffset(SqmCalOffset);  // хз пока, надо ли ваще
}

void loop() {

switch (Mode){//функция переключения режимов работы
  case 0:{//первый режим стартовый экран
    if (flag==0)
    {
      TFTscreen.background(0, 0, 0);
      flag++;
      DisplMeteo();
       DisplMoon();
       DisplGPS();
       DisplSQM();
    }
    startScreen();
if (round(millis()/1000) % UpdateMeteo == 0)
{
  TFTscreen.stroke(0, 0, 0);//очистка предыдущих показаний
  TFTscreen.text(tempstring,80,116);
  TFTscreen.text(presstring,60,131);
  TFTscreen.text(humstring,60,146);
    ReadMeteo();
    DisplMeteo(); //обновление погоды
}
if (digitalRead (SQMBUTTON) == HIGH)//показания люксометра снимаются по нажатии кнопки
{
  TFTscreen.stroke(0, 0, 0);
   delay(100);//для устранения дребезга контактов
  TFTscreen.fill(0,0,0);
  TFTscreen.rect(55,20,70,20);
   ReadSQM();
   DisplSQM();
}
if (round(millis()/1000) % UpdateGPS == 0)//обновление gps
{
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.fill(0,0,0);
  TFTscreen.rect(25,51,82,33);
  /* пока сделаю обновление времени проще
  TFTscreen.setTextSize(3);
  TFTscreen.text(hourstring,25,52);
  TFTscreen.text(minutestring,73,52);
  TFTscreen.setTextSize(1);
  TFTscreen.text(daystring,38,75);
  TFTscreen.text(monthstring,53,75);
  TFTscreen.text(yearstring,68,75);
  */
  TFTscreen.text(satvisstring,5,69);
  TFTscreen.text(satactstring,5,53);
  TFTscreen.text(latstring,60,85);
  TFTscreen.text(longstring,65,95);
TFTscreen.text(altstring,65,105);
    ReadGPS();
    DisplGPS();
  ReadMoon();
  DisplMoon();
}
}
break;
case 1:{
  if (flag==2)//очистим экран один раз при переключении режима, далее он будет обновляться по времени
    {
      TFTscreen.background(0, 0, 0);
      GraphTemp();
      flag++;//т.е.3
    }
while (round(millis()/1000) % UpdateMeteo == 0)
{
  ReadMeteo();
  GraphTemp();
}
}
  break;
  case 2:
{
    if (flag==4)//очистим экран один раз при переключении режима, далее он будет обновляться по времени
    {
      TFTscreen.background(0, 0, 0);
      GraphPres();
      flag++;//т.е.5
    }
if (round(millis()/1000) % UpdateMeteo == 0)
  {
  ReadMeteo();
  GraphPres();
  }
  }
  break;
case 3:{
   if (flag==6)//очистим экран один раз при переключении режима, далее он будет обновляться по времени
    {
      TFTscreen.background(0, 0, 0);
      GraphSQM();
      flag++;//т.е.7
    }
  if (digitalRead (SQMBUTTON) == HIGH)
  {
    ReadSQM();
   delay(200);//подождём пока он там всё досчитает
   GraphSQM();
  }
  }
  }
}

   
