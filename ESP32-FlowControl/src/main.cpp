#include <Arduino.h>
#include "my_functions.hpp"
#include "my_variables.hpp"
#include "EasyButton.h"
#include "LEDDisplay74HC595.h"

// Podłączone piny
#define SCLK_PIN 15
#define RCLK_PIN 2
#define DIO_PIN 0

const int BUTTON_1_PIN = 4;
#define BUTTON_2_PIN 16
#define BUTTON_3_PIN 17

#define FLOW_PIN 21

#define BUZZER_PIN 5

#define MOSFET_PIN 23

#define LED_PIN 22

// Inicjalizacja ekranu
LEDDisplay74HC595 ledDisplay(SCLK_PIN, RCLK_PIN, DIO_PIN);
volatile float displayed_value = 0;
volatile int displayed_decimal_places = 0;

// Inicjalizacja przycisków
EasyButton command_button_1(BUTTON_1_PIN);
EasyButton command_button_2(BUTTON_2_PIN);
EasyButton command_button_3(BUTTON_3_PIN);

// Inicjalizacja liczenia przepływu
volatile int pulses = 0;

// Zmienna włączenia buzzera
bool isBuzzerOn = false;

// Deklaracje funkcji
void onFlowInterrupt();
void TimerRun();
void onTimer();
void buzzerControl();
void Button_1_Action();
void Button_2_Action();
void Button_3_Action();

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Inicjalizacja połączenia z pamięcią i wczytywanie zmiennych
  InitializeMemory();

  // Inicjalizacja połączenia wifi i email
  InitializeEmailAndWiFi();

  // Inicjalizacja przycisków
  command_button_1.begin();
  command_button_1.onPressed(Button_1_Action);
  //command_button_2.begin();
  command_button_2.onPressed(Button_2_Action);
  //command_button_3.begin();
  command_button_3.onPressed(Button_3_Action);

  // Inicjalizacja ekranika
  ledDisplay.refresh(0, 0);

  // Podpinanie przerwania przepływu
  attachInterrupt(FLOW_PIN, onFlowInterrupt, RISING);

  // Inicjalizacja połączenia z komputerem
  Serial.begin(115200);
  Serial.setTimeout(1000);
}

void loop() {
  command_button_1.read();
  command_button_2.read();
  command_button_3.read();
  SerialResponse();
  TimerRun();
  buzzerControl();

  if (armored)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);
}

void onFlowInterrupt() {
  pulses++;
}

// Funkcja przerwania czasowego powodująca wyświetlenie informacji
// Wywoływane co 1ms
void onTimer(){
  static int ms_loop_counter = 0;
  static int s_loop_counter = 0;

  ms_loop_counter++;
  // Zerowanie licznika milisekund co 10K -> 10 sekund
  if (ms_loop_counter == 10000) {
    ms_loop_counter = 0;
  }
  // Dodawanie do licznika sekund
  if (ms_loop_counter % 1000 == 0) {
    s_loop_counter++;
  }
  // Zerowanie licznika sekund co 18K -> 5 godzin
  if (s_loop_counter == 18000) {
    s_loop_counter = 0;
  }

  if (ms_loop_counter % 1000 == 0 && s_loop_counter % 60 == 0) {
    // Co 1 minutę
    // Obliczanie flow
    flow = pulses / flow_factor;
    pulses = 0;
    displayed_value = flow;

    if (flow < 10)
      displayed_decimal_places = 3;
    else if (flow < 100)
      displayed_decimal_places = 2;
    else if (flow < 1000)
      displayed_decimal_places = 1;
    else
      displayed_decimal_places = 0;

    // Sprawdzenie poprawności przepływu
    if (armored && !below_target_dedected && flow < flow_target) {
      TriggerBelowTargetDedected();
    }
  }

  ledDisplay.refresh(displayed_value, displayed_decimal_places);
}

void TimerRun() {
  static const unsigned long REFRESH_INTERVAL = 1; // ms
  static unsigned long lastRefreshTime = 0;
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
	{
		lastRefreshTime += REFRESH_INTERVAL;
    onTimer();
	}
}

void buzzerControl() {
  if (below_target_dedected && (!isBuzzerOn)) {
    tone(BUZZER_PIN, 300);
    isBuzzerOn = true;
  }
  if ((!below_target_dedected) && (isBuzzerOn)) {
    noTone(BUZZER_PIN);
    isBuzzerOn = false;
  }
}

void Button_1_Action() {
  if (armored)
    ArmFlowSensor(false);
  else
    ArmFlowSensor(true);
}

void Button_2_Action() {
  Print_AllPreferences();
}

void Button_3_Action() {
  SendEmail("ESP32-Flow TestEmail", "ESP32-Flow\nTest email send from button");
}
