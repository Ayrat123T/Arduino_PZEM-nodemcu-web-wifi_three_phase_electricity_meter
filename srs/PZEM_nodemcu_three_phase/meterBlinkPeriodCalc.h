#pragma once
#include <queue>
#include "values.h"

int KYimpNumSumm = 0;                          // текущее кол-во импульсов
int winHi = 0, winLo = 1024;                   // пределы гистерезиса
int dataCur;                                   // временное хранение текущих данных фоторезистора
unsigned long microTimer;                      // Стоп-таймер в микросекундах
double meterBlinkPeriod;                       // Период моргания счётчика
std::queue<double> meterBlinkPeriods;          // Очередь из последних периодов моргания счётчика                         
boolean ledState, ledStateOld;                 // текущее логическое состояние фоторезистора
float meterWattage = 0;                        // текущая мощность счётчика

void checkLedState();
void findAnalogWindow(int analogData);
void checkLogic(int analogData);
void closeAnalogWindow();
void initWindow();

void checkLedState() {
  yield();
  dataCur = analogRead(ANALOG_PIN);                   // запоминаем значение на сенсоре
  findAnalogWindow(dataCur);                          // расширяем окно, если значение выходит за его пределы
  ledStateOld = ledState;                             // сохраняем в буфер старое значение уровня сенсора
  checkLogic(dataCur);                                // оцениваем состояние сенсора и сохраняем его значение в ledState

  if (ledStateOld && !ledState) {                     // ИНДикатор только что загорелся
    Serial.print("led state has been changed, new blink period = ");
    // вычисление длины последнего импульса
    meterBlinkPeriod = double(micros() - microTimer) / 1000000;               // длина последнего импульса = текущее время - время прошлого перехода
    Serial.println(meterBlinkPeriod);
    microTimer = micros();                            // запоминаем время этого перехода в таймер
    // вычисление длины последнего импульса   
    KYimpNumSumm++;
  }
  if (!ledStateOld && ledState) { // ИНДикатор только что погас
    closeAnalogWindow();          // ужимаем пороги окна сенсора, чтобы они хронически не росли.
  }
  yield();
}

void initWindow() {
  unsigned long startTimer = millis() + 5000;
  Serial.print("initialization light window");
  while (millis() < startTimer) {
    dataCur = analogRead(ANALOG_PIN);
    findAnalogWindow(dataCur);
    delay(1000);
    Serial.print(" . ");
  }
  Serial.println("initialization light window complited");
}

void findAnalogWindow(int analogData) {
  if (analogData > winHi) winHi = analogData; // запомнить значение как верхнее, если оно выше него
  if (analogData < winLo) winLo = analogData; // запомнить значение как нижнее, если оно ниже него
}

void checkLogic(int analogData) {
  if (winLo < winHi - 5) {
    int percCur = map(analogData, winLo, winHi, 0, 99); // переводим значение в проценты от шкалы
    if (percCur <= 33) ledState = 0;
    if (percCur >= 66) ledState = 1;
  }
}

void closeAnalogWindow() {
  if (winLo < winHi - 30) {
    int winDif = (winHi - winLo);                // вычисляем ширину окна
    winHi = winHi - (winDif / CLOSE_WIN_FACTOR); // вычитаем 1/10 ширины из верхнего порога
    winLo = winLo + (winDif / CLOSE_WIN_FACTOR); // прибавляем 1/10 ширины к нижнему порогу
  }
}