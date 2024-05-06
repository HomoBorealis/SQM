// позиции в EEPROM 
#define EEPROM_SQM_CAL_INDEX_C       1
#define EEPROM_SQM_CAL_INDEX_F       3
#define EEPROM_TEMP_CAL_INDEX_C      7
#define EEPROM_TEMP_CAL_INDEX_F      8 
#define EEPROM_AUTO_TEMP_INDEX_C     14
#define EEPROM_AUTO_CONTRAS_INDEX_C  15
#define EEPROM_CONTRAS_INDEX_C       17
#define EEPROM_CONTRAS_INDEX_B       20


// читаем SQMCalOffset из EEPROM
float ReadEESqmCalOffset(){
  float f;
  if ( EEPROM.read(EEPROM_SQM_CAL_INDEX_C) == 'x') {
    f = EEPROM_readFloat(EEPROM_SQM_CAL_INDEX_F);
  }
  else {
   f = SQM_CAL_OFFSET;  
  } 
  return f;
}

// записываем SQMCalOffset в EEPROM
void WriteEESqmCalOffset( float f){
   if ( ( f > 25 )|| ( f < -25) ) return;  // значение вне диапазона 
   EEPROM.write(EEPROM_SQM_CAL_INDEX_C,'x');
   EEPROM_writeFloat(EEPROM_SQM_CAL_INDEX_F, f);
}


// читаем температурную поправку из EEPROM
float ReadEETempCalOffset(){
  float f;
    if ( EEPROM.read(EEPROM_TEMP_CAL_INDEX_C) == 'T' ) {
    f = EEPROM_readFloat(EEPROM_TEMP_CAL_INDEX_F);
  }
  else {
    f = TEMP_CAL_OFFSET;   
  }
  return f;
}

// записываем температурную поправку EEPROM
void WriteEETempCalOffset( float f) {
  if ( ( f > 50 )|| ( f < -50) ) return;  // значение вне диапазона 
   EEPROM.write(EEPROM_TEMP_CAL_INDEX_C,'T'); 
   EEPROM_writeFloat(EEPROM_TEMP_CAL_INDEX_F, f);
}

/* Автоконтраст, сделаю по-другому
boolean ReadEEAutoContras() {
  if ( EEPROM.read(EEPROM_AUTO_CONTRAS_INDEX_C) == 'N' )  
     return false;
  else 
     return true;
}

// Write AutoContras
void WriteEEAutoContras( boolean _b ) {
  if ( _b )
    EEPROM.write(EEPROM_AUTO_CONTRAS_INDEX_C,'Y');
  else 
    EEPROM.write(EEPROM_AUTO_CONTRAS_INDEX_C,'N');
}
*/
// Чтение автоматической температурной калибровки
boolean ReadEEAutoTempCal() {
  if ( EEPROM.read(EEPROM_AUTO_TEMP_INDEX_C) == 'N' )  
     return false;
  else 
     return true;
}

// Запись
void WriteEEAutoTempCal( boolean _b ) {
  if ( _b )
    EEPROM.write(EEPROM_AUTO_TEMP_INDEX_C,'Y');
  else 
    EEPROM.write(EEPROM_AUTO_TEMP_INDEX_C,'N');
}

/* Read Display contras from EEPROM
uint8_t ReadEEcontras(){
   uint8_t _f;
  if ( EEPROM.read(EEPROM_CONTRAS_INDEX_C) == 'C') {
    _f = EEPROM.read(EEPROM_CONTRAS_INDEX_B);
  }
  else {
   _f = DEFALUT_CONTRAS;  
  } 
  return _f;
}

// Write Display Contras to EEPROM
void WriteEEScontras( uint8_t _f){
   EEPROM.write(EEPROM_CONTRAS_INDEX_C,'C');
   EEPROM.write(EEPROM_CONTRAS_INDEX_B, _f);
}

*/
// запись 4бита в EEPROM на позицию i (4бита)
void EEPROM_writeQuad(byte i,byte *v) {
  EEPROM.write(i+0,*v); v++;
  EEPROM.write(i+1,*v); v++;
  EEPROM.write(i+2,*v); v++;
  EEPROM.write(i+3,*v);
}

// чтение 4битов из EEPROM на позиции i (4бита)
void EEPROM_readQuad(int i,byte *v) {
  *v=EEPROM.read(i+0); v++;
  *v=EEPROM.read(i+1); v++;
  *v=EEPROM.read(i+2); v++;
  *v=EEPROM.read(i+3);  
}

// запись 4битов констант с плавающей точкой в EEPROM на позиции i
void EEPROM_writeFloat(byte i,float f) {
  EEPROM_writeQuad(i,(byte*)&f);
}

// чтение 4битов констант с плавающей точкой из EEPROM на позиции i
float EEPROM_readFloat(byte i) {
  float f;
  EEPROM_readQuad(i,(byte*)&f);
  return f;
}
