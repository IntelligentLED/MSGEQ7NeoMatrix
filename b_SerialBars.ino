void serialBars(uint16_t FPS) {
  brightVal = map(analogRead(A2), 0, 1023, 5, 255);
  fade = brightVal;
  matrix.setBrightness(fade);

  // Visualize the average bass of both channels

  //URUTAN NILAI FREQUANSI BISA DI TUKAR-TUKAR DISINI,
  jumFrek = 7; //KALAU ADA YANG DI HAPUS, UBAH NILAI disini

  uint8_t input = MSGEQ7.get(MSGEQ7_BASS);
  // Reduce noise
  input = mapNoise(input);
  int freq[9] = {
    MSGEQ7.get(MSGEQ7_1),
    MSGEQ7.get(MSGEQ7_2),
    MSGEQ7.get(MSGEQ7_3),//MID
    MSGEQ7.get(MSGEQ7_4),
    MSGEQ7.get(MSGEQ7_5),//HIGH
    MSGEQ7.getVolume(),
    MSGEQ7.get(MSGEQ7_6),
    MSGEQ7.get(MSGEQ7_0),//LOW & BASS
    //    MSGEQ7.get(MSGEQ7_MID),
    //    MSGEQ7.get(MSGEQ7_LOW),
    //    MSGEQ7.get(MSGEQ7_BASS),
    //    MSGEQ7.get(MSGEQ7_HIGH),
    input
  };


  // Save the difference between the last beat
  static uint8_t lastInput = 0;
  int delta = input - lastInput;
  lastInput = input;

  if (input == 0) {
    countIn++;
  }
  else {
    countIn = 0;
  }


  for (int con = 0; con < 8; con++) {
    //        freqMax[con] = max(freq[con], freqMax[co/n]);
    //    if (freq[con] >= frekMax[con]) frekMax[con] = freq[con];
    //    else frekMax[con] = frekMax[con];
    //    if (freq[con] <= frekMin[con]) frekMin[con] = freq[con];
    //    else frekMin[con] = frekMin[con];
    frekMin[con] = min(freq[con], frekMin[con]);
    //    if(freqMax[con]>=frekMin[con])deltaFrek[con] = freqMax[con] - frekMin[con];
    //    Serial.print(deltaFrek[con]);
    //    Serial.print(F("\t"));
  }
  for (int con = 0; con < 8; con++) {
    Serial.print(freq[con]);
    Serial.print(F("\t"));
  }
  //    Serial.print(analogRead(A0));
  //    Serial.print(F("\t"));

  // Highlight high pitches
  valMax = map(analogRead(A1), 0, 1023, 10, 500);
  ledMin = map(valMax, 10, 500, mh / 2, -mh / 2);
  if (countIn == 1000) {
    for (int con = 0; con < jumFrek; con++) {
      valMin[con] = freq[con];

      EEPROM.write(con, freq[con]);
      Serial.print(valMin[con]);
      Serial.print(F(" \t"));
    }

    matrix.setTextColor(WheelMatrix(257));
    matrix.setCursor(0, 0);
    //    matrix.print("OK");

    matrix.show();
    Serial.println(" EEPROM success!!");
    delay(1000);
    matrix.clear();
  }

  for (int con = 0; con < jumFrek; con++) {

    if (freq[con] < valMin[con])freq[con] = valMin[con];
    if (freq[con] >= valMax) freq[con] = valMax;
    v[con] = map(freq[con], valMin[con], valMax, ledMin, mh);
    //rainbow height
    for (int rnbw = 0; rnbw < mh; rnbw++) {
      for (int dob = 0; dob < mw; dob += jumFrek) {
        switch (modee) {
          case 1:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(rnbw * 255 / mh));
            addOnSwitch = 0;
            break;
          case 2:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix((rnbw * 255 / mh) + clr & 255));
            addOnSwitch = 0;
            break;
          case 3:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(rnbw * 255 / mh));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 4:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix((rnbw * 255 / mh) + clr & 255));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 5:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(rnbw * 255 / mh));
            addOnSwitch = 1;
            clrAddOn = 0;
            break;
          case 6:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix((rnbw * 255 / mh) + clr & 255));
            addOnSwitch = 1;
            clrAddOn = 0;
            break;
          case 7:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(rnbw * 255 / mh));
            addOnSwitch = 1;
            clrAddOn = 85;
            break;
          case 8:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix((rnbw * 255 / mh) + clr & 255));
            addOnSwitch = 1;
            clrAddOn = 85;
            break;
          case 9:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(rnbw * 255 / mh));
            addOnSwitch = 1;
            clrAddOn = 170;
            break;
          case 10:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix((rnbw * 255 / mh) + clr & 255));
            addOnSwitch = 1;
            clrAddOn = 170;
            break;

          case 11:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix((dob + con) * 255 / mw));
            addOnSwitch = 0;
            break;
          case 12:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(((dob + con) * 255 / mw) + clr & 255));
            addOnSwitch = 0;
            break;
          case 13:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix((dob + con) * 255 / mw));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 14:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(((dob + con) * 255 / mw) + clr & 255));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 15:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix((dob + con) * 255 / mw));
            addOnSwitch = 1;
            clrAddOn = 0;
            break;
          case 16:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(((dob + con) * 255 / mw) + clr & 255));
            addOnSwitch = 1;
            clrAddOn = 0;
            break;

          case 17:
            matrix.drawPixel(dob + con, rnbw, FireMatrix((mh - rnbw) * 255 / mh));
            addOnSwitch = 0;
            break;
          case 18:
            matrix.drawPixel(dob + con, rnbw, FireMatrix((mh - rnbw) * 255 / mh));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 19:
            matrix.drawPixel(dob + con, rnbw, FireMatrix((mh - rnbw) * 255 / mh));
            addOnSwitch = 1;
            clrAddOn = 0;
            break;
          case 20:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(clr & 255));
            addOnSwitch = 0;
            clrAddOn = 0;
            break;
          case 21:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(clr & 255));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 22:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(clr & 255));
            addOnSwitch = 1;
            clrAddOn = 0;
            break;
          case 23:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(clr & 255));
            addOnSwitch = 1;
            clrAddOn = 85;
            break;
          case 24:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(clr & 255));
            addOnSwitch = 1;
            clrAddOn = 170;
            break;
          case 25:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(257));
            addOnSwitch = 1;
            clrAddOn = 0;
            break;
          case 26:
            matrix.drawPixel(dob + con, rnbw, WheelMatrix(0));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 27:
            matrix.drawPixel(dob + con, rnbw, RedBlue((mh - rnbw) * 255 / mh));
            addOnSwitch = 0;
            break;
          case 28:
            matrix.drawPixel(dob + con, rnbw, RedBlue((mh - rnbw) * 255 / mh));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 29:
            matrix.drawPixel(dob + con, rnbw, RedBlue((mh - rnbw) * 255 / mh));
            addOnSwitch = 1;
            clrAddOn = 212;
            break;
          case 30:
            matrix.drawPixel(dob + con, rnbw, RedBlue((rnbw * 255 / mh) + clr & 255));
            addOnSwitch = 0;
            break;
          case 31:
            matrix.drawPixel(dob + con, rnbw, RedBlue((rnbw * 255 / mh) + clr & 255));
            addOnSwitch = 1;
            clrAddOn = 257;
            break;
          case 32:
            matrix.drawPixel(dob + con, rnbw, RedBlue((rnbw * 255 / mh) + clr & 255));
            addOnSwitch = 1;
            clrAddOn = 212;
            break;

          default : modee = 1; break;
        }
      }
    }
  }

  updateread();
  wipeOff(speedFade);
  if (addOnSwitch == 1)addOn(WheelMatrix(clrAddOn), speedFadeMax);
  matrix.show();

  char c = '=';
  if (delta >= 20)
    c = '#';

  // Visualize with characters as bars
  for (uint8_t i = 0; i < (input / 4); i++)
  {
    if (i == 192 / 4 - 1)
      Serial.print('+');
    else if (i == 128 / 4 - 1)
      Serial.print('*');
    else if (i == 64 / 4 - 1)
      Serial.print('X');
    else
      Serial.print(c);
  }
  Serial.println();
}

