void SENSOR(void) {

  BaroSensorRead(&pressione, &tempC, &quota);
  DisplayPresTemp(&pressione, &tempC, &quota);
}
void DisplayPresTemp(float* pressione, float* tempC, float* quota)
{
  u8g.firstPage();

  do
  {
    u8g.setFont(u8g_font_helvB08);
    u8g.setFontRefHeightExtendedText();
    u8g.setDefaultForegroundColor();
    u8g.setFontPosTop();
    u8g.drawStr(2, 0, "Pressione");
    u8g.setPrintPos(75, 0);
    u8g.print(*pressione);
    u8g.drawStr(4, 20, "Temp.  C");
    u8g.setPrintPos(75, 20);
    u8g.print(*tempC);
    u8g.print("\xb0");
    u8g.drawStr(4, 40, "Altitudine");
    u8g.setPrintPos(75, 40);
    u8g.print(*quota);
 //  _buffer = "Premi ESC per uscire";
  // MESSAGGIO_MENU (_buffer);
  }
  while (u8g.nextPage());
}
void BaroSensorRead(float* pressione, float* tempC, float* quota)
{
  *tempC = sensor.readTemp();

  *pressione = sensor.readPressure() / 100.0;


  // Calculate quota assuming 'standard' barometric
  // pressione of 1013.25 millibar = 101325 Pascal
  *quota =  sensor.readAltitude() / 131.2335958852;

}

