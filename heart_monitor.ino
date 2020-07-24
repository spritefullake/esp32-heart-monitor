#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include "Secrets.h"

using pin = const int;


// Internet domain to request from:
const char * hostDomain = ("http://" + (String)siteAddress + ":3000/").c_str();
const int hostPort = 80;

//the input for the ECG data
#define SENSOR A2
pin BUTTON_PIN = 0;
pin LED_PIN = 13;
pin TX_PIN = 17;
pin RX_PIN = 16;

WiFiClient wifi;
const char * wsDomain = siteAddress;
WebSocketClient ws_client = WebSocketClient(wifi,wsDomain,3000);

void setup()
{
  // Initilize hardware:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(TX_PIN, INPUT);
  pinMode(RX_PIN, INPUT);
 

  // Connect to the WiFi network (see function below loop)
  connectToWiFi(networkName, networkPswd);

  //setup websockets

  digitalWrite(LED_PIN, LOW); // LED off
  Serial.print("Press button 0 to connect to ");
  Serial.println(hostDomain);
}

const int socket_delay = 200;
const int ecg_delay = 100;
const int buffer_size = (int)(socket_delay / ecg_delay);
String ecg_buffer = "";
int count = 0; 
void loop()
{

  ws_client.begin();
  while (ws_client.connected()){
    auto ecg = readECG(TX_PIN, RX_PIN, SENSOR);
    Serial.println("WS sending");
    ws_client.beginMessage(TYPE_TEXT);
    ws_client.print(ecg);
    ws_client.endMessage();
    Serial.println(ecg);

    // check if a message is available to be received
    int messageSize = ws_client.parseMessage();

    if (messageSize > 0) {
      Serial.println("Received a message:");
      auto message = ws_client.readString();
      
    }

    delay(socket_delay);
  }
  Serial.println("Disconnected :(");

  auto ecg = readECG(TX_PIN, RX_PIN, SENSOR);
  Serial.println(ecg);
  
  
  delay(100);
}

// Read ECG from the AD8232 heart monitor
String readECG(pin L0_minus, pin L0_plus, pin sensor){
  if((digitalRead(L0_minus) == 1) || (digitalRead(L0_plus) == 1)){
    return "!";
  }
  else{
    int ecg_value = analogRead(sensor);
    return String(ecg_value);
  }
}

void connectToWiFi(const char * ssid, const char * pwd)
{
  int ledState = 0;

  printLine();
  Serial.println("Connecting to WiFi network: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) 
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_PIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void requestURL(const char * host, uint8_t port)
{
  printLine();
  Serial.println("Connecting to domain: " + String(host));

  HTTPClient http;
  Serial.println("[HTTP] begin...");
  http.begin(String(host));
  int httpCode = http.GET();

  if(httpCode > 0){
    Serial.printf("HTTP Get Code: %d\n", httpCode);
    Serial.println(http.getString());
  }
  else{
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

}

void postURL(const char * host, uint8_t port, String payload)
{
  printLine();
  Serial.println("Connecting to domain: " + String(host));

  HTTPClient http;
  Serial.println("[HTTP] begin...");
  http.begin(String(host));
  
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  int httpCode = http.POST("value=22123");//"{\"value\":\"23.234\"}");

  if(httpCode > 0){
    Serial.printf("HTTP POST Code: %d\n", httpCode);
    Serial.println(http.getString());
  }
  else{
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

}

void printLine()
{
  Serial.println();
  for (int i=0; i<30; i++)
    Serial.print("-");
  Serial.println();
}
void LEDTest(){
  if (digitalRead(BUTTON_PIN) == LOW)
  { // Check if button has been pressed
    while (digitalRead(BUTTON_PIN) == LOW)
      ; // Wait for button to be released

    digitalWrite(LED_PIN, HIGH); // Turn on LED
    postURL(hostDomain, hostPort, "Hello World here"); // Connect to server
    digitalWrite(LED_PIN, LOW); // Turn off LED

  }
}
