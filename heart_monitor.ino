#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
/* The header file containing the wifi username and password
 * Looks like this:
 * const char * networkName = "your wifi network's name";
 * const char * networkPswd = "your wifi network's password"; 
 */
#include <Secrets.h>

using pin = const int;


// Internet domain to request from:
const char * hostDomain = "http://192.168.1.78:3000/"; //"example.com";
const int hostPort = 80;

//the input for the ECG data
#define SENSOR A2
pin BUTTON_PIN = 0;
pin LED_PIN = 13;
pin TX_PIN = 17;
pin RX_PIN = 16;

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

  digitalWrite(LED_PIN, LOW); // LED off
  Serial.print("Press button 0 to connect to ");
  Serial.println(hostDomain);
}

void loop()
{
  LEDTest();
  //requestURL(hostDomain, hostPort);
  auto ecg = readECG(TX_PIN, RX_PIN, SENSOR);
  //Serial.println(ecg);
  delay(10);
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
  Serial.println("Beginning http");
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
    requestURL(hostDomain, hostPort); // Connect to server
    digitalWrite(LED_PIN, LOW); // Turn off LED

  }
}
