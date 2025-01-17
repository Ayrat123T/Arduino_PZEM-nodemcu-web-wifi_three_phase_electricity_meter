#pragma once
#include <queue>
#include <PZEM004Tv30.h>

#define APSSID "SmartGridComMeterESPap" // Имя точки доступа, которую создаст ESP
#define STASSID "Redmi_DF75"            // Точка доступа (логин и пароль от wifi), к которой подключится ESP
#define STAPSK "51194303" 
#define STASSID2 "Admin"
#define STAPSK2 "123456789" 
#define ANALOG_PIN A0
#define CLOSE_WIN_FACTOR 10             // 1/CLOSE_WIN_FACTOR для сужения окна с каждой стороны

PZEM004Tv30 pzem1(D1, D2); // (RX,TX) подключиться к TX,RX PZEM1
PZEM004Tv30 pzem2(D5, D6); // (RX,TX) подключиться к TX,RX PZEM2
PZEM004Tv30 pzem3(D7, D0); // (RX,TX) подключиться к TX,RX PZEM3

  float current = 0; // суммарный ток
  float power = 0;   // суммарная мощность
  float energy = 0;  // суммарная энергия

  float voltage1 = 0;
  float current1= 0;
  float power1= 0;
  float energy1= 0;
  float frequency1= 0;
  float pf1= 0;

  float voltage2 = 0;
  float current2= 0;
  float power2= 0;
  float energy2= 0;
  float frequency2= 0;
  float pf2= 0;

  float voltage3 =0;
  float current3= 0;
  float power3= 0;
  float energy3= 0;
  float frequency3= 0;
  float pf3= 0;

int constMeterImpsNum = 1000;                  // постояннная счётчика
float SMDAccuraty = 100;                       // Погрешность счётчика
int WtTokWtScale = 1000;                       // коэффициент перевода Вт в кВт
int сurrentTransformerTransformationRatio = 1; // коэффициент трансформации трансформтора тока

int KYimpNumSumm = 0;                          // текущее кол-во импульсов
int winHi = 0, winLo = 1024;                   // пределы гистерезиса
int dataCur;                                   // временное хранение текущих данных фоторезистора
unsigned long microTimer;                      // Стоп-таймер в микросекундах
double meterBlinkPeriod;                       // Период моргания счётчика
std::queue<double> meterBlinkPeriods;          // Очередь из последних периодов моргания счётчика    
size_t queueSize = 1;                     
double queueSum = 0; 
boolean ledState, ledStateOld;                 // текущее логическое состояние фоторезистора
float meterWattage = 0;                        // текущая мощность счётчика