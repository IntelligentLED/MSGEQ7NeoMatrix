bool btnStateMode = 0;

bool stateM = 0;

byte EEmode     =   70;    //Var mode at EEPROM

void updateread() {
  static uint16_t bsm1;
  uint16_t tbsm1 = millis();
  if ((tbsm1 - bsm1) > 100) {
    btnStateMode = digitalRead(11);//pin tombol
    bsm1 = tbsm1;
  }

  if (btnStateMode == HIGH) {
    if (stateM == LOW) {
      modee++;
      EEPROM.write(EEmode, modee);
      Serial.println(modee);
      stateM = HIGH;
      matrix.clear();
      matrix.show();
      delay(100);
    }
  }
  if (btnStateMode == LOW)      stateM = LOW;

  if (EEPROM.read(EEmode) != 70 ) {
    modee     = EEPROM.read(EEmode);
  }
  if (modee > 32) modee = 1;

}

