#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ESP8266httpUpdate.h>
#define TRIGGER_PIN 0


void setup() {
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT);
  // Подключаемся к wifi если подключения нет уходим в основной цикл.
  WIFIinit();
}

void loop() {
  // Проверяем нажатие на кнопку и если кнопка нажата перейдем в режим AP для настройки подключения
  OnDemandAP();
  // Проверяем подключины мы к роутеру и если да делаем перепрошивку модуля с сервера
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("ESP Update");
  webUpdate();
  }
}
void webUpdate() {
  // отключаем перезагрузку после обнавления FS
  ESPhttpUpdate.rebootOnUpdate(false);
  //Обнавляем FS
  t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs("http://backup.privet.lv/esp/sonoff/spiffs.0xBB000_flash_size_1Mb.256Kb_2017.04.21.bin");
  // включаем перезагрузку после прошивки
  ESPhttpUpdate.rebootOnUpdate(true);
  // Перепрошиваем модуль
  t_httpUpdate_return ret1 = ESPhttpUpdate.update("http://backup.privet.lv/esp/sonoff/build.0x00000_flash_size_1Mb.256Kb_2017.04.21.bin");
}

void OnDemandAP(){
   if ( digitalRead(TRIGGER_PIN) == LOW ) {
    WiFiManager wifiManager;
    if (!wifiManager.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
    Serial.println("connected...yeey :)");
  }
  }
  void WIFIinit() {
  // Попытка подключения к точке доступа
  WiFi.mode(WIFI_STA);
  byte tries = 11;
  WiFi.begin();
  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    // Если не удалось подключиться запускаем в режиме AP
    Serial.println("");
    Serial.println("WiFi up AP");
  }
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
