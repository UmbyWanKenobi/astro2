void Split_into_dms( float angle, unsigned int* _deg, unsigned int* _min ) {
  float t;
  unsigned int d, m;
  float s;

  Serial.println(angle);
  if (angle < 0.0) {
    angle = - angle;
  }
  angle = GRADI_DA_RADIANTI(angle);
  //Serial.println(angle);
  d = (unsigned int)angle;
  // Serial.println(d);
  t = (angle - (float )d) * 60.0;
  //  Serial.println(t);
  m = (unsigned int)(t);


  if (m == 60) {
    m = 0;
    d++;
  }

  *_deg = d;
  *_min = m;

  //  Serial.println(_deg);
  Serial.println(angle);


}

float GRADI_DA_RADIANTI(float angolo) {
  float gradi;
  gradi = angolo * 180 / pi;
  return gradi;
}

void MESSAGGIO_MENU () {
  uint8_t h;
  u8g_uint_t w, d, e;
  w = u8g.getWidth();
  e = u8g.getHeight();

  u8g.setFont(u8g_font_6x12);

  h = u8g.getFontAscent() - u8g.getFontDescent();
  //char *_buffer = "Premi ESC per uscire";
  // if (ug8.getStrWidth(_buffer)> e) {
//  d = (w - u8g.getStrWidth(_buffer)) / 2;
  u8g.setDefaultForegroundColor();

  u8g.drawBox(0, e - h + 1 , w, h);
  u8g.setDefaultBackgroundColor();

//  u8g.drawStr(d, e - 1, _buffer);

}


