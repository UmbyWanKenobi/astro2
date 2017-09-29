/*
  /*printDate ();
  Serial.print(_buffer);
  Serial.print (" ");
 
  Serial.print(_buffer);
  Serial.println();
  Serial.print( F("Fix status: ") );
  if (fix.valid.status)
    Serial.print( (int) fix.status );
  Serial.println();

  if (fix.valid.location) {
    printDD_MMmmmm( fix.latitudeDMS, fix.latitudeDMS.NS() );
    Serial.print (_buffer);
    Serial.print(" -- ");
    printDD_MMmmmm( fix.longitudeDMS, fix.longitudeDMS.EW() );
    Serial.print (_buffer);
    Serial.println();
  }

  if (fix.valid.altitude) {
    Serial.print( F("Altitude: ") );
    Serial.println( fix.altitude() );
  }

  while ( i < 3 )
  {


    if ( scelta )
    {
     
      lcd.clear();  lcd.setCursor((20 - strlen("IMPOSTAZIONI")) / 2, 0);
      lcd.print("IMPOSTAZIONI");
      lcd.setCursor((20 - strlen(MENU[1][i])) / 2, 1);
      lcd.print( MENU[1][i] );
      scelta = false;
    }
    //  CICLO DI IMMISSIONE DEI PARAMETRI
    if ( PIU.isPressed () || MENO.isPressed () || SELECT.isPressed () )
    {
      if ( PIU.isPressed () )
      {
        if ( i == 0 ) {
          minuti += 1;
          sprintf (_buffer, "%s:", MENU[2][i]);
          lcd.setCursor((20 - strlen( _buffer )) / 2, 2);
          lcd.print (_buffer);
          row = 3; clear_row(row);
          sprintf(_buffer, "%10d", minuti);
          lcd.print( _buffer );
          char _buffer[10] = "";
        }
        if ( i == 1 ) {
          secondi += 0.01;
          if (secondi > 1) {
            secondi += 1.00;
            secondi = int(secondi);
          }
          sprintf (_buffer, "%s:", MENU[2][i]);
          lcd.setCursor((20 - strlen( _buffer )) / 2, 2);
          lcd.print (_buffer);
          row = 3; clear_row(row);
          dtostrf(secondi, 13, 3, _buffer);
          lcd.print( _buffer );
          char _buffer[10] = "";
        }
        if ( i == 2 ) {                                                                               //  In questa routine viene impostata la DECLINAZIONE MAGNETICA
          DECL_MAGN += 0.0003;                                                                             //  per la localitÃƒÂ  di osservazione. Essa si puÃƒÂ² ricavare visitando il sito
          Split_into_dms(DECL_MAGN, &_deg, &_min);                                                         //  http://www.magnetic-declination.com/ cercando la propria localitÃƒÂ 
          _D = _deg;                                                                                  //  anche se la stima di default (2Ã‚Â°45') copre abbastanza bene l'Italia Centrale.
          _M = _min;                                                                                  //  L'utente vedrÃƒÂ  visualizzato il valore in gradi sessadecimali 
          sprintf (_buffer, "%s:", MENU[2][i]);                                                       //  mentre l'azione di input avviene tutta in radianti, per le ragioni di
          lcd.setCursor((20 - strlen( _buffer )) / 2, 2);                                             //  calcolo di Arduino.
          lcd.print (_buffer);                                                                        //  infatti la routine prevede l'incremento - e il decremento, vedi piÃƒÂ¹ sotto -
          row = 3; clear_row(row);                                                                    //  di un valore pari a 0,0003, equivalente all'incirca ad un primo d'arco,
          sprintf(_buffer, "%3d\337%2d'", _D, _M);                                                    //  piÃƒÂ¹ che sufficiente per lo  scopo.
          lcd.setCursor((20 - strlen( _buffer )) / 2, row);                                           //  L'uso di due variabili temporanee (_D e _M) si ÃƒÂ¨ rivelato necesssario per
          lcd.print( _buffer );                                                                       //  motivi legati al'errata interpretazione dei puntatori (&_deg e &_min) da parte 
        }                                                                                             //  del processo di scrittura a video.
      }
      if ( MENO.isPressed () )
      {
        if ( i == 0 ) {
          minuti--;
          if (minuti <= 0) {
            minuti = 0;
          }
          sprintf (_buffer, "%s:", MENU[2][i]);
          lcd.setCursor((20 - strlen(_buffer )) / 2, 2);
          lcd.print (_buffer);
          row = 3; clear_row(row);
          sprintf(_buffer, "%10d", minuti);
          lcd.print( _buffer );

        }
        if ( i == 1 ) {
          if (secondi > 1) {
            secondi -= 1.00;
            secondi = int(secondi);
          }            else {
            secondi -= 0.01;
          }

          if (secondi <= 0) {
            secondi = 0.01;
          }
          sprintf (_buffer, "%s:", MENU[2][i]);
          lcd.setCursor((20 - strlen( _buffer )) / 2, 2);
          lcd.print (_buffer);
          row = 3; clear_row(row);
          dtostrf(secondi, 13, 3, _buffer);
          lcd.print( _buffer );

        }
        if ( i == 2 ) {
          DECL_MAGN -= 0.0003;
          Split_into_dms(DECL_MAGN, &_deg, &_min);
          _D = _deg;
          _M = _min;
          sprintf (_buffer, "%s:", MENU[2][i]);
          lcd.setCursor((20 - strlen( _buffer )) / 2, 2);
          lcd.print (_buffer);
          row = 3; clear_row(row);
          sprintf(_buffer, "%3d\337%2d'", _D, _M);
          lcd.setCursor((20 - strlen( _buffer )) / 2, row);
          lcd.print( _buffer );
        }
      }
      if ( SELECT.isPressed () )
      {
        lcd.clear();
        i++;
        scelta = true;
      }
    }
    delay( 200 );
  }
}
//------------------------------------------------------------------------------------------------------------------//


//----------------------------(CICLO MINORE DI PULIZIA DELLA RIGA DI STAMPA   )-------------------------------------//
void clear_row(int row) {
  lcd.setCursor(0, row);
  lcd.print("                   ");
  lcd.setCursor(0, row);
}


//---------------------------------(   VISUALIZZA I PARAMETRI IN MEMORIA   )-----------------------------//
void _F_CONTAltezzaO_()
{
  for (int i = 0; i < 3; i++)
  {
    lcd.clear();  lcd.setCursor((20 - strlen("CONTROLLO")) / 2, 0);
    lcd.print("CONTROLLO");
    lcd.setCursor((20 - strlen(MENU[1][i])) / 2, 1);
    lcd.print( MENU[1][i] );


    if ( i == 0 ) {

      sprintf (_buffer, "%s: %s", MENU[2][i], String(minuti).c_str());
      lcd.setCursor((20 - strlen( _buffer )) / 2, 2);
      lcd.print (_buffer);

    }
    if ( i == 1 ) {
      sprintf (_buffer, "%s: %s", MENU[2][i], String(secondi).c_str());
      lcd.setCursor((20 - strlen( _buffer )) / 2, 2);
      lcd.print (_buffer);

    }
    if ( i == 2 ) {
      sprintf (_buffer, "%s: %s", MENU[2][i], String(DECL_MAGN).c_str());
      lcd.setCursor((20 - strlen( _buffer )) / 2, 2);
      lcd.print (_buffer);

    }

    delay( 3000 );
  }
}
//--------------------------------------------------------------------------------------
*/


