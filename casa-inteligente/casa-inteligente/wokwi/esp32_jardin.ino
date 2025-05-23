#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "WOKWI-GUEST";
const char* password = "";

const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

const int bombaPin = 2; // LED para bomba
int humedad = 60; // humedad simulada %

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
  if (String(topic) == "casa/jardin/bomba") {
    if (msg == "ON") {
      digitalWrite(bombaPin, HIGH);
    } else if (msg == "OFF") {
      digitalWrite(bombaPin, LOW);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Jardin")) {
      client.subscribe("casa/jardin/bomba");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  pinMode(bombaPin, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

unsigned long lastHumedadPublish = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastHumedadPublish > 5000) { // cada 5 segundos
    humedad += (random(-2,3)); // cambia humedad +/- 2
    if (humedad < 30) humedad = 30;
    if (humedad > 90) humedad = 90;
    client.publish("casa/jardin/humedad", String(humedad).c_str());
    lastHumedadPublish = now;
  }
}
