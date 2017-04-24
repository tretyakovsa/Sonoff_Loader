#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ESP8266httpUpdate.h>
#define TRIGGER_PIN 0
#define LED_PIN 13    // Светодиод

void setup() {
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT);
  // Подключаемся к wifi если подключения нет уходим в основной цикл.
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
 WiFiManager wifiManager;

  wifiManager.autoConnect("sonoff-loader");

  digitalWrite(LED_PIN, LOW);

}

void loop() {
  // Проверяем подключины мы к роутеру и если да делаем перепрошивку модуля с сервера
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("ESP Update");
    //Обновляем оба файла с сервера
    webUpdate();
  }
}
void webUpdate() {
  // отключаем перезагрузку после обнавления FS
  digitalWrite(LED_PIN, LOW);
  ESPhttpUpdate.rebootOnUpdate(false);
  //Обнавляем FS
  t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs("http://backup.privet.lv/esp/sonoff/spiffs.0xBB000_flash_size_1Mb.256Kb_2017.04.23.bin");
  // включаем перезагрузку после прошивки
  digitalWrite(LED_PIN, HIGH);
  ESPhttpUpdate.rebootOnUpdate(true);
  // Перепрошиваем модуль
  t_httpUpdate_return ret1 = ESPhttpUpdate.update("http://backup.privet.lv/esp/sonoff/build.0x00000_flash_size_1Mb.256Kb_2017.04.23.bin");
}




