#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "VNPT - TRI";
const char* password = "87654321@@@";
const char* serverUrl = "http://localhost:5000/api/lightSensor";

const int lightSensorPin = A0;
const int led1Pin = D1;
const int led2Pin = D2;
const int led3Pin = D3;

String Query,Key,payload,value;

WiFiClient client;

void setup() {
  pinMode(lightSensorPin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}

void loop() {
  int lightIntensity = analogRead(lightSensorPin);

  // Construct JSON payload
  StaticJsonDocument<200> doc;
  Key = "data";
  value = lightIntensity;
  Query = Key+":{"+"light"+":"+value+"}";
  Serial.println(Query);
  doc["data"]["light"] = lightIntensity;
  String payload;
  serializeJson(doc, payload);
  Serial.println(payload);
  // Send HTTP POST request to server
  HTTPClient http;
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(Query);
  Serial.println(httpResponseCode);
  http.end();

  if (httpResponseCode == 200) {
    // Parse JSON response from server
    String response = http.getString();
    DynamicJsonDocument json(200);
    deserializeJson(json, response);

    int numLights = json["data"]["numLights"];

    // Turn on/off the appropriate LEDs based on the number of lights
    digitalWrite(led1Pin, numLights >= 1 ? HIGH : LOW);
    digitalWrite(led2Pin, numLights >= 2 ? HIGH : LOW); 
    digitalWrite(led3Pin, numLights >= 3 ? HIGH : LOW);

    Serial.print("Light intensity: ");
    Serial.print(lightIntensity);
    Serial.print(", Number of lights: ");
    Serial.println(numLights);
  } else {
    Serial.print("HTTP POST request failed, error code: ");
    Serial.println(httpResponseCode);
  }

  delay(5000);
}
