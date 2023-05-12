#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// WiFi credentials
//Laptop và wemos phải được kết nối với cùng 1 wifi 
const char* ssid = "Nokia 5.4";
const char* password = "pmt123456";
const char* serverUrl = "http://192.168.68.52:5000/api/lightSensor";
//http://192.168.68.52 là ip của máy , mở cmd chạy ipconfig lấy ip của máy. :5000 là port chạy nodejs.
const int lightSensorPin = A0;
const int led1Pin = D1;
const int led2Pin = D2;
const int led3Pin = D3;

String Query, Key, payload, value;

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
  doc["data"]["light"] = lightIntensity;
  String payload;
  serializeJson(doc, payload);
  Serial.println(payload);
  // Send HTTP POST request to server
  HTTPClient http;
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(payload);
  Serial.println(httpResponseCode);


  if (httpResponseCode == 200) {
    // Parse JSON response from server
    String response = http.getString();
    Serial.println(response);
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
  http.end();
  delay(5000);
}
