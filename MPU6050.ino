
void LIVELLA()
{
  bool reset = 0;
  float a, b, c, d, e, f;
  byte x, y;
  FunctionsMPU(); // Acquisisco assi AcX, AcY, AcZ.

  Altezza = INCLINAZIONE_ALTEZZA(AcX, AcY, AcZ) ;   //Calcolo angolo Altezza
  Inclinazione = INCLINAZIONE_ALTEZZA(AcY, AcX, AcZ) ;  //Calcolo angolo Inclinazione
  if (Inclinazione < -1) {
    digitalWrite (SINISTRA, HIGH);
    digitalWrite (DESTRA, LOW);
    digitalWrite  (BOLLA, LOW);
  } else if (Inclinazione > 1) {
    digitalWrite (SINISTRA, LOW);
    digitalWrite (DESTRA, HIGH);
    digitalWrite  (BOLLA, LOW);
  } else {
    digitalWrite (SINISTRA, LOW);
    digitalWrite (DESTRA, LOW);
    digitalWrite  (BOLLA, HIGH);
  }
  if (ENTER.isPressed () ) {

  }
  a = (Altezza + 180) * pi / 180;
  b = (Altezza + 270) * pi / 180;
  c = (Altezza - 270) * pi / 180;
  d = (Inclinazione + 180) * pi / 180;
  e = (Inclinazione + 270) * pi / 180;
  f = (Inclinazione - 270) * pi / 180;


  {
    u8g.firstPage();

    do
    {

      u8g.setFont(u8g_font_chikita);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      printFix();
      u8g.drawStr(75, 15, "Inclinazione");
      u8g.setPrintPos(90, 55);
      u8g.print(Inclinazione);
      u8g.drawStr(2, 15, "Altezza");
      u8g.setPrintPos(16, 55);
      u8g.print(Altezza);
      u8g.drawTriangle(22 * cos (a) + 28, 22 * sin (a) + 50,
                       4 * cos (b) + 28, 4 * sin (b) + 50,
                       4 * cos (c) + 28, 4 * sin (c) + 50);

      u8g.drawCircle(28, 50, 22);
      u8g.drawCircle(28, 50, 23);
      x = (25 * cos ((fix.latitude() + 180) * pi / 180) + 28);
      y = (25 * sin ((fix.latitude() + 180) * pi / 180) + 50);

      u8g.drawLine( x, y, 28, 50 );



      u8g.drawTriangle(22 * sin (d) + 100, 22 * cos (d) + 50,
                       4 * sin (e) + 100, 4 * cos (e) + 50,
                       4 * sin (f) + 100, 4 * cos (f) + 50);
      u8g.drawCircle(100, 50, 22);
      u8g.drawCircle(100, 50, 23);
      x = (25 * sin (pi) + 100);
      y = (25 * cos (pi) + 50);
      u8g.drawLine( x, y, 100, 50 );

      //  _buffer = "Premi ESC per uscire Premi ENTER per zero";
      // MESSAGGIO_MENU(_buffer);
    }
    while (u8g.nextPage());
  }
  // delay (500);
}

void init_MPU() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //delay(1000);
}

//Funzione per il calcolo degli angoli Inclinazione e Altezza
double INCLINAZIONE_ALTEZZA(double A, double B, double C) {
  double val_A, val_B, Value;

  val_A = A;
  val_B = (B * B) + (C * C);
  val_B = sqrt(val_B);

  Value = atan2(val_A, val_B);
  Value = Value * 180 / pi;

  return (float)Value;
}

//Funzione per l'acquisizione degli assi X,Y,Z del MPU6050
void FunctionsMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_YOUT_H) & 0x40 (ACCEL_YOUT_L)
  /*
    Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  */


}

