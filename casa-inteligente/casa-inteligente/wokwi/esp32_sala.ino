#include <WiFi.h>
#include <PubSubClient.h>

// Datos WiFi
const char* ssid = "WOKWI-GUEST";
const char* password = "";

// Broker MQTT público
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 2; // LED interno (luz)
int temperatura = 25; // temperatura simulada

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  if (String(topic) == "casa/sala/luz") {
    if (msg == "ON") {
      digitalWrite(ledPin, HIGH);
    } else if (msg == "OFF") {
      digitalWrite(ledPin, LOW);
    }
  }
  else if (String(topic) == "casa/sala/temperatura_set") {
    temperatura = msg.toInt();
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Sala")) {
      client.subscribe("casa/sala/luz");
      client.subscribe("casa/sala/temperatura_set");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

unsigned long lastTempPublish = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastTempPublish > 5000) { // cada 5 segundos
    client.publish("casa/sala/temperatura", String(temperatura).c_str());
    lastTempPublish = now;
  }
}
