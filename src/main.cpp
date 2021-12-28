#include <Arduino.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "wifi_manager_param_helper.h"

WiFiManager wm;

WifiManagerParamHelper wm_helper(wm);

long long next_print = 0;
constexpr long long PRINT_PERIOD_MS = 2000; 

constexpr size_t CUSTOM_FIELD_LEN = 40;
constexpr std::array<ParamEntry, 2> PARAMS = {{
    {
      "devicenameid",
      "Device Name",
      CUSTOM_FIELD_LEN
    },
    {
      "mqttserverid",
      "MQTTServer",
      CUSTOM_FIELD_LEN
    }
}};

void setup_wifi() {

  WiFi.mode(WIFI_STA);

  wm_helper.Init(0xBEEF, PARAMS.data(), PARAMS.size());

  // automatically connect using saved credentials if they exist
  // If connection fails it starts an access point with the specified name
  if (wm.autoConnect("AutoConnectAP")) {
    Serial.println("connected...yeey :)");
    wm.startWebPortal();
  } else {
    Serial.println("Configportal running");
  }
}

void setup() {
  Serial.begin(9600);

  setup_wifi();
}

void loop() {
  wm.process();

  if (millis() > next_print) {
    Serial.println(String("Device name: ") + wm_helper.GetSetting(0));
    Serial.println(String("MQTT server: ") + wm_helper.GetSetting(1));
    next_print = millis() + PRINT_PERIOD_MS;
  }
}
