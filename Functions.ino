//Начальный экран
void startScreen()
{
  TFTscreen.stroke(0, 0, 255);
  TFTscreen.noFill();
TFTscreen.rect (108,48,19,32);//индикатор бататареи (потом добавить)
 TFTscreen.fill(0,0,255);
 TFTscreen.rect (112,52,11,24);
TFTscreen.noFill();
TFTscreen.stroke(0, 0, 255);
TFTscreen.rect(0,0,48,48);
TFTscreen.rect (48,0,79,48);

TFTscreen.rect (0,48,22,16);//активные спутники
TFTscreen.rect (0,64,22,16);//видимые спутники
TFTscreen.rect(0,48,127,112);
TFTscreen.rect(0,48,127,112);
TFTscreen.rect(0,112,127,159);
  }
void ModeFunc()//функция прерывания для переключения режимов
{
  
  if (millis() - debounce >= 200 && digitalRead(3)) {//эта вся фигня для устранения дребезга контактов(лень паять кондёры)(можно снизить до 100)
    debounce = millis();
Mode++;
flag++;

 if (Mode >3){
    Mode=0;
  }
  if (flag>7)
  {
    flag=0;
  }
 // Serial.println(Mode);для отладки
  //TFTscreen.background(0, 0, 0);
}
}
  
//функция выведения показаний SQM
void DisplSQM()
{
  TFTscreen.stroke(0, 0, 255);
  TFTscreen.setTextSize(2);//установим размер всего выводимого далее текста
  TFTscreen.text("SQM\n", 70,2);//выведем люксы 
String LuxString=String(mag);
  //преобразовать измерение в символьный массив
  LuxString.toCharArray(luxstring,6);
  TFTscreen.text(luxstring,55,20);
TFTscreen.setTextSize(1);
TFTscreen.text("Mag/ArcSec^2\n", 52, 38);
}

//функция выведения метеоданных
void DisplMeteo()
{
  TFTscreen.stroke(0,0,255);//установит цвет текста (красный)
  TFTscreen.setTextSize(1);//установим размер всего выводимого далее текста
  TFTscreen.text("Temperature:\n", 5,116);//выведем слово температура
  String tempString=String(Temp);
  //преобразовать измерение в символьный массив
  tempString.toCharArray(tempstring,3);
  TFTscreen.text(tempstring,80,116);//выведем температуру
  TFTscreen.text("*C",97,116);
TFTscreen.text("Pressure:\n", 5,131);//выведем слово давление
  String presString=String(Pres);
  //преобразовать измерение в символьный массив
  presString.toCharArray(presstring,5);
  TFTscreen.text(presstring,60,131);//выведем давление
  TFTscreen.text("Pa",85,131);
TFTscreen.text("Humidity:\n", 5,146);//выведем слово влажность
  String humString=String(Hum);
  //преобразовать измерение в символьный массив
  humString.toCharArray(humstring,3);
  TFTscreen.text(humstring,60,146);//выведем влажность (уточнить координату х)
  TFTscreen.text("%",75,146);
}

//функция фазы луны и времени восхода
void ReadMoon()
{
  //итог расчёта записывается в переменные phase и moon
  old = day+month+moonNum;//для 24года лунное число = 18, каждый год+11, если больше 30 то 30 вычитается
  if(old>30)
  {old=old-30;}
 /*
  время восхода = возраст луны *0,8 +6 время в часах, если больше 24, то 24 отнимаем
  moon=old*0.8+6;
  if(moon>24)
  {
    moon=moon-24;
  }
  */
  
}

//функция отображения фазы луны и времени восхода (да, можно было вывести битовый массив как изображение, но я не ищу лёгких путей)
void DisplMoon()
{
if (old >= 29&& old<=2){//новолуние
  TFTscreen.noFill();
TFTscreen.stroke(0, 0, 255);
TFTscreen.circle(24,20,12);
}
if (old >=3 && old <= 6){//молодая
TFTscreen.fill(0,0,255);
TFTscreen.stroke(255, 0, 0);
TFTscreen.circle(24,20,12);
TFTscreen.fill(0,0,0);
TFTscreen.stroke(0, 0, 0);
TFTscreen.circle(20,20,12);
}
if(old >=7 && old <=10){//первая четверть
TFTscreen.fill(0,0,255);
TFTscreen.stroke(0, 0, 255);
TFTscreen.circle(24,20,12);
TFTscreen.fill(0,0,0);
TFTscreen.stroke(0, 0, 0);
TFTscreen.rect(5,5,20,30);
}
if (old >=11 && old < 14)//прибывающая
{
TFTscreen.noFill();
TFTscreen.stroke(0, 0, 255);
TFTscreen.circle(24,20,12);
TFTscreen.fill(255,0,0);
TFTscreen.stroke(0, 0, 0);
TFTscreen.circle(27,20,12);
TFTscreen.noFill();
TFTscreen.stroke(0, 0, 255);
TFTscreen.circle(24,20,12);
}
if (old >= 14 && old <= 17)//полнолуние
{
TFTscreen.fill(0,0,255);
TFTscreen.stroke(0,0,255);
TFTscreen.circle(24,20,12);
}
if (old >=18 && old <=20)//убывающая
{
TFTscreen.noFill();
TFTscreen.stroke(0, 0, 255);
TFTscreen.circle(24,20,12);
TFTscreen.fill(0,0,255);
TFTscreen.stroke(0, 0, 0);
TFTscreen.circle(20,20,12);
TFTscreen.noFill();
TFTscreen.stroke(0, 0, 255);
TFTscreen.circle(24,20,12);
}
if (old >=21 && old <=24)//последняя четверть
{
TFTscreen.fill(0,0,255);
TFTscreen.stroke(0, 0, 255);
TFTscreen.circle(24,20,12);
TFTscreen.fill(0,0,0);
TFTscreen.stroke(0, 0, 0);
TFTscreen.rect(24,5,20,30);
}
if (old >=25 && old <=28)//Старая
{
TFTscreen.fill(0,0,255);
TFTscreen.stroke(0, 0, 255);
TFTscreen.circle(24,20,12);
TFTscreen.fill(0,0,0);
TFTscreen.stroke(0, 0, 0);
TFTscreen.circle(28,20,12);
}
/*
    String moonString=String(moon);//код вывода восхода Луны
    moonString.toCharArray(moonstring,3);
  TFTscreen.text(moonstring,10,38);
  */
}

//функция выведения показаний GPS
void DisplGPS()
{
  TFTscreen.stroke(0,0,255);
TFTscreen.setTextSize(3);
  if (hour < 10)//костыль из-за хреновой библиотеки
  {
    TFTscreen.setTextSize(3);
     String hourString=String(hour);
hourString.toCharArray(hourstring,3);
  TFTscreen.text(hourstring,43,52);
    TFTscreen.text("0", 25,52);
  }
   if (hour >=10)
  {
    TFTscreen.setTextSize(3);
   String hourString=String(hour);
hourString.toCharArray(hourstring,3);
  TFTscreen.text(hourstring,25,52);
  }
TFTscreen.text(" : \n", 40,52);
if (minute < 10)//тоже костыль из-за хреновой библиотеки
  {
    TFTscreen.setTextSize(3);
    String minuteString=String(minute);
minuteString.toCharArray(minutestring,3);
  TFTscreen.text(minutestring,91,52);
  TFTscreen.text("0", 73,52);
  }
  if (minute >=10)
  {
    TFTscreen.setTextSize(3);
    String minuteString=String(minute);
minuteString.toCharArray(minutestring,3);
  TFTscreen.text(minutestring,73,52);
  }
if (day<10){ //иииии....ещё один костыль!
  TFTscreen.setTextSize(1);
  String dayString=String(day);
dayString.toCharArray(daystring,3);
TFTscreen.text("0", 38,75);
  TFTscreen.text(daystring,44,75);
}
if (day>=10)
{
  TFTscreen.setTextSize(1);
  String dayString=String(day);
dayString.toCharArray(daystring,3);
  TFTscreen.text(daystring,38,75);
}
TFTscreen.text(".\n", 48,75);
if (month<10){ //да, код прямо скажу инвалид 1 группы...
  TFTscreen.setTextSize(1);
 String monthString=String(month);
monthString.toCharArray(monthstring,3);
  TFTscreen.text(monthstring,59,75);
TFTscreen.text("0", 53,75);
}
if (month>=10)
{
  TFTscreen.setTextSize(1);
String monthString=String(month);
monthString.toCharArray(monthstring,3);
  TFTscreen.text(monthstring,53,75);
}
String monthString=String(month);
monthString.toCharArray(monthstring,3);
  TFTscreen.text(monthstring,53,75);
TFTscreen.text(".\n", 63,75);
String yearString=String(year);
yearString.toCharArray(yearstring,5);
  TFTscreen.text(yearstring,68,75);

TFTscreen.text("Latitude:\n", 5,85);// выведем широту
  String latString=String(Lat);
    latString.toCharArray(latstring,10);
  TFTscreen.text(latstring,60,85); 
 TFTscreen.text("Longitude:\n", 5,95);
  String longString=String(Long);
    longString.toCharArray(longstring,10);
  TFTscreen.text(longstring,65,95);//выведем долготу
 
 TFTscreen.text("Altitude:\n", 5,105);
  String altString=String(Alt);//выведем высоту
    altString.toCharArray(altstring,4);
  TFTscreen.text(altstring,65,105);
   
     String satactString=String(SatAct);
    satactString.toCharArray(satactstring,3);
  TFTscreen.text(satactstring,5,53);//выведем активные спутники
  String satvisString=String(SatVis);//выведем количество видимых спутников
    satvisString.toCharArray(satvisstring,3);
  TFTscreen.text(satvisstring,5,69);

}


//функция чтения показаний люксометра и вычисление MAG
void ReadSQM() { 
  String gainString = "Max gain";
  delay(50);
  tsl.setGain(TSL2591_GAIN_HIGH);// максимальное усиление люксометра
  luminosity = tsl.getFullLuminosity();//проводим первое измерение
  delay(50);
  luminosity = tsl.getFullLuminosity(); //проводим второе для стабилизации показаний.
  ir = luminosity >> 16;//вычисляем ИК составляющую
  full = luminosity & 0xFFFF;//полный спектр
  visible = full - ir;//вычисляем освещённость видимого спектра
  if (visible== 0xFFFF||ir==0xFFFF) {//далее проверяем освещённость и снижаем усиление при высокой
    gainString = "High gain";
    tsl.setGain(TSL2591_GAIN_HIGH);
    gainscale = HIGHSCALE;
    luminosity = tsl.getFullLuminosity();
    delay(50);
    luminosity = tsl.getFullLuminosity();
    ir = luminosity >> 16;
    full = luminosity & 0xFFFF;
    visible = full - ir;
    if (visible == 0xFFFF || ir == 0xFFFF) {
      gainString = "Med gain";
      tsl.setGain(TSL2591_GAIN_MED);
      gainscale = MEDSCALE;
      luminosity = tsl.getFullLuminosity();
      delay(50);
      luminosity = tsl.getFullLuminosity();
      ir = luminosity >> 16;
      full = luminosity & 0xFFFF;
      visible = full - ir;
      if (visible == 0xFFFF || ir == 0xFFFF) {
        gainString = "Low gain";
        tsl.setGain(TSL2591_GAIN_LOW);
        gainscale = LOWSCALE;
        luminosity = tsl.getFullLuminosity();
        delay(50);
        luminosity = tsl.getFullLuminosity();
        ir = luminosity >> 16;
        full = luminosity & 0xFFFF;
        visible = full - ir;

      }
    }
  }
  adjustedIR = (float)ir / gainscale;
  adjustedVisible = (float)visible / gainscale;
  mag = -1.085736205*log(.925925925 * pow(10,-5.)*adjustedVisible);//вычисляем значение темноты неба
  if (isinf(mag)) {
    mag = 25.0;
  }
  mag = mag + ReadEESqmCalOffset();//калибровка значения из EEPROM
  mag = float (int ( mag * 100 + 0.5) ) /100;
       for (int a = 119 ; a >= 0 ; --a )
{
sqmArray[a] = sqmArray[a - 1];
}
sqmArray[0] = mag;
}


//Функция чтения метеоданных
void ReadMeteo()
{
  Pres = bme.readPressure();
    Temp = bme.readTemperature();
    Hum =  bme.readHumidity();
     Temp = Temp + TempCalOffset;  //с калибровкой
     for (int a = 119 ; a >= 0 ; --a )
{
tempArray[a] = tempArray[a - 1];
}
tempArray[0] = Temp;
    for (int a = 119 ; a >= 0 ; --a )
{
presArray[a] = presArray[a - 1];
}
presArray[0] = Pres;
}

//функция чтения показаний gps
void ReadGPS()
{
gps.read();//  Читаем данные (чтение может занимать больше 1 секунды). Функции можно указать массив для получения данных о спутниках.

     /* Время: */  hour=gps.Hours; minute=gps.minutes;
     /* Дата:*/  day=gps.day; month=gps.month; year=gps.year;
     /* Координаты (широта, долгота, высота): */  Lat=gps.latitude; Long=gps.longitude; Alt=gps.altitude;
     /* Спутники (активные/наблюдаемые):          */ SatAct=gps.satellites[GPS_ACTIVE]; SatVis=gps.satellites[GPS_VISIBLE];
    
     if (year==24)//вычислять лунное число буду пока так
     {
      moonNum=18;
     }
     else
     {
      moonNum=((year-24)*11)+18;
      if (moonNum>=30)
      {
        moonNum=moonNum-30;
     }
     }
}
void GraphTemp()//2 режим - вывод графика температуры
{
  int q;
// Вывести текущую температуру
TFTscreen.background(0, 0, 0); // Очистить экран, установив черный цвет фона
delay(100);//чтоб не мерцал
TFTscreen.stroke(0, 0, 255); 
TFTscreen.setTextSize(1);
TFTscreen.text("Temperature:", 10, 5);
String tempString=String(Temp);
  //преобразовать измерение в символьный массив
  tempString.toCharArray(tempstring,3);
  TFTscreen.text("*C", 105, 5);
TFTscreen.text(tempstring, 85, 5);
// Нарисовать оси графика
TFTscreen.setTextSize(1);
TFTscreen.text("+35", 5, 20);
TFTscreen.text("+30", 5, 30);
TFTscreen.text("+25", 5, 40);
TFTscreen.text("+20", 5, 50);
TFTscreen.text("+15", 5, 60);
TFTscreen.text("+10", 5, 70);
TFTscreen.text("+5", 5, 80);
TFTscreen.text("0", 5, 90);
TFTscreen.text("-5", 5, 100);
TFTscreen.text("-10", 5, 110);
TFTscreen.text("-15", 5, 120);
TFTscreen.text("-20", 5, 130);
TFTscreen.text("-25", 5, 140);
TFTscreen.text("-30", 5, 150);
TFTscreen.line(25, 25, 25, 159);
// Нарисовать график изменения температуры
for (int a = 30 ; a < 127 ; a++)
{
// Преобразовать температуру в координату Y на экране ЖКИ
q = (99 - (tempArray[a - 30] * 2));
TFTscreen.point(a,q);
}
}

void GraphSQM()
{
   int q;
// Вывести текущую освещённость
TFTscreen.background(0, 0, 0); // Очистить экран, установив черный цвет фона
delay(100);
TFTscreen.stroke(0, 0, 255); 
TFTscreen.setTextSize(1);
TFTscreen.text("SQM:", 3, 5);
String LuxString=String(mag);
  //преобразовать измерение в символьный массив
  LuxString.toCharArray(luxstring,5);
  TFTscreen.text("Mag/ArcSec^2\n", 57, 5);
TFTscreen.text(luxstring, 27, 5);
// Нарисовать оси графика
TFTscreen.setTextSize(1);
TFTscreen.text("24", 5, 20);
TFTscreen.text("23.5", 5, 30);
TFTscreen.text("23", 5, 40);
TFTscreen.text("22.5", 5, 50);
TFTscreen.text("22", 5, 60);
TFTscreen.text("21.5", 5, 70);
TFTscreen.text("21", 5, 80);
TFTscreen.text("20.5", 5, 90);
TFTscreen.text("20", 5, 100);
TFTscreen.text("19.5", 5, 110);
TFTscreen.text("19", 5, 120);
TFTscreen.text("18.5", 5, 130);
TFTscreen.text("18", 5, 140);
TFTscreen.text("17.5", 5, 150);
TFTscreen.line(30, 25, 30, 159);
// Нарисовать график изменения освещённости
for (int a = 35 ; a < 127 ; a++)
{
// Преобразовать освещённость в координату Y на экране ЖКИ
q = map(sqmArray[a - 35],17.5,24,159,30);
TFTscreen.point(a,q);
}
}
void GraphPres()//график давления
{
  int q;
// Вывести текущее давление
TFTscreen.background(0, 0, 0); // Очистить экран, установив черный цвет фона
delay(100);
TFTscreen.stroke(0, 0, 255); 
TFTscreen.setTextSize(1);
TFTscreen.text("Pressure:", 3, 5);
String presString=String(Pres);
  //преобразовать измерение в символьный массив
  presString.toCharArray(presstring,5);
  TFTscreen.text("Pa", 100, 5);
TFTscreen.text(presstring, 72, 5);
// Нарисовать оси графика
TFTscreen.setTextSize(1);
TFTscreen.text("1085", 5, 20);
TFTscreen.text("1067", 5, 30);
TFTscreen.text("1049", 5, 40);
TFTscreen.text("1031", 5, 50);
TFTscreen.text("1013", 5, 60);
TFTscreen.text("995", 5, 70);
TFTscreen.text("977", 5, 80);
TFTscreen.text("960", 5, 90);
TFTscreen.text("942", 5, 100);
TFTscreen.text("924", 5, 110);
TFTscreen.text("906", 5, 120);
TFTscreen.text("888", 5, 130);
TFTscreen.text("870", 5, 140);
TFTscreen.text("852", 5, 150);
TFTscreen.line(35, 25, 35, 159);
// Нарисовать график изменения освещённости
for (int a = 40 ; a < 127 ; a++)
{
// Преобразовать освещённость в координату Y на экране ЖКИ
q = map(presArray[a - 40],8520,10850,159,30);
TFTscreen.point(a,q);
}
}
