#pragma once
#include <execution>
#include <algorithm>
#include <cmath>
#include "values.h"

void checkLedState();
void initWindow();
void findAnalogWindow(int analogData);
void checkLogic(int analogData);
void closeAnalogWindow();

// возвращает среднее арифметическое
template <typename ExecutionPolicy, typename IteratorRange>
double GetArithmeticMean(ExecutionPolicy&& Policy, IteratorRange begin, IteratorRange end) {
    double res = std::reduce(Policy, begin, end);
    int sum = (end - begin);
    return res / sum;
}

// возвращает стандартное отклонение
template <typename ExecutionPolicy, typename IteratorRange>
double GetStandardDeviation(ExecutionPolicy&& Policy, IteratorRange begin, IteratorRange end) {
    double arithmetic_mean = GetArithmeticMean(Policy, begin, end);
    return std::sqrt(std::transform_reduce(Policy, begin, end, 0.0, std::plus{}, [&arithmetic_mean](const auto delt) { // среднеквадратичное отклонение
        return pow(arithmetic_mean - delt, 2);
        }) / (end - begin - 1));
}

void checkLedState() {
  yield();
  dataCur = analogRead(ANALOG_PIN);// запоминаем значение на сенсоре
  findAnalogWindow(dataCur);       // расширяем окно, если значение выходит за его пределы
  ledStateOld = ledState;          // сохраняем в буфер старое значение уровня сенсора
  checkLogic(dataCur);             // оцениваем состояние сенсора и сохраняем его значение в ledState

  if (ledStateOld && !ledState) {  // ИНДикатор только что загорелся
    Serial.print("led state has been changed, new blink period = ");
    // вычисление длины последнего импульса
    meterBlinkPeriod = double(micros() - microTimer) / 1000000;               // длина последнего импульса = текущее время - время прошлого перехода
    if (queueSize > 1) { 
        meterBlinkPeriods.push(meterBlinkPeriod); // добавляем период моргания в очередь, если пользователь задал её длину > 1
        if (meterBlinkPeriods.size() > queueSize) { 
            queueSum -= meterBlinkPeriods.front(); // корректрируем сумму очереди
            meterBlinkPeriods.pop(); // удаляем первый элемент, если очередь переполнена
        }
        queueSum += meterBlinkPeriod;
        meterBlinkPeriod = queueSum / meterBlinkPeriods.size();
    }
    meterWattage = 3600 / meterBlinkPeriod  / constMeterImpsNum; // нагрузка (кВт) = кол-во таких импульсов в часе разделить на имп за 1кВт*ч
    if (power) SMDAccuraty = (power - meterWattage) / power * 100;
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