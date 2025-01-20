#pragma once
#include "values.h"
#include "meterBlinkPeriodCalc.h"

void SetPzem1Values() {
  voltage1 = 0;checkLedState();
  current1= 0;checkLedState();
  power1= 0;checkLedState();
  energy1= 0;checkLedState();
  frequency1= 0;checkLedState();
  pf1= 0;checkLedState();
  if (!isnan(voltage1 = pzem1.voltage())) {
    current1 = pzem1.current() * currentTransformerTransformationRatio;checkLedState();
    current += current1;checkLedState();
    frequency1 = pzem1.frequency();checkLedState();
    pf1 = pzem1.pf();checkLedState();
    power1 = pzem1.power() / WtTokWtScale * currentTransformerTransformationRatio;checkLedState();
    power += power1;checkLedState();
    energy1 = pzem1.energy() * currentTransformerTransformationRatio;checkLedState();
    energy += energy1;checkLedState();
  }
}

void SetPzem2Values() {
  voltage2 = 0;checkLedState();
  current2= 0;checkLedState();
  power2= 0;checkLedState();
  energy2= 0;checkLedState();
  frequency2= 0;checkLedState();
  pf2= 0;checkLedState();
  if (!isnan(voltage2 = pzem2.voltage())) {
    current2 = pzem2.current() * currentTransformerTransformationRatio;checkLedState();
    current += current2;checkLedState();
    frequency2 = pzem2.frequency();checkLedState();
    pf2 = pzem2.pf();checkLedState();
    power2 = pzem2.power() / WtTokWtScale * currentTransformerTransformationRatio;checkLedState();
    power += power2;checkLedState();
    energy2 = pzem2.energy() * currentTransformerTransformationRatio;checkLedState();
    energy += energy2;checkLedState();
  }
}

void SetPzem3Values() {
  voltage3 =0;checkLedState();
  current3= 0;checkLedState();
  power3= 0;checkLedState();
  energy3= 0;checkLedState();
  frequency3= 0;checkLedState();
  pf3= 0;checkLedState();
  if (!isnan(voltage3 = pzem3.voltage())) {
    current3 = pzem3.current() * currentTransformerTransformationRatio;checkLedState();
    current += current3;checkLedState();
    frequency3 = pzem3.frequency();checkLedState();
    pf3 = pzem3.pf();checkLedState();
    power3 = pzem3.power() / WtTokWtScale * currentTransformerTransformationRatio;checkLedState();
    power += power3;checkLedState();
    energy3 = pzem3.energy() * currentTransformerTransformationRatio;checkLedState();
    energy += energy3;checkLedState();
  }
}