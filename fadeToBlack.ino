//static uint16_t x[12];
int x;
void wipeOff(int SpeedDelay) {
  Tmr = millis();
  if ((Tmr - lastTmr) >= SpeedDelay) {
    clr += 7;
    lastTmr = Tmr;
    for (x = 0; x < jumFrek; x++) {
      if (v[x] < value[x]) value[x] -= 1;
      //      matrix.drawPixel(x, value[x], 0); //  Set pixel's color (in RAM)
    }
  }
  for (x = 0; x <jumFrek; x++) {
    if (v[x] > value[x]) {
      value[x] = v[x] ;
    }
    for (int dob = 0; dob < mw; dob += jumFrek) {
      matrix.drawFastVLine( dob+x, value[x], mh, 0); //  Set pixel's color (in RAM)
    }
  }
}

void addOn(uint16_t color, int SpeedDelay) {
  TmrMax = millis();
  if ((TmrMax - lastTmrMax) >= SpeedDelay) {
    lastTmrMax = TmrMax;
    for (int maxx = 0; maxx < jumFrek; maxx++) {
      if (v[maxx] > valueMax[maxx]) {
        valueMax[maxx] = v[maxx] ;
      }
      if (v[maxx] < valueMax[maxx]) valueMax[maxx] -= 1;
      //      matrix.drawPixel(x, value[x], 0); //  Set pixel's color (in RAM)
    }
  }
  for (int maxx = 0; maxx < jumFrek; maxx++) {
    if (v[maxx] > valueMax[maxx]) {
      valueMax[maxx] = v[maxx] ;
    }
    for (int dob = 0; dob < mw; dob += jumFrek) {
      matrix.drawPixel(dob+maxx, valueMax[maxx], color); //  Set pixel's color (in RAM)
    }
  }
}
