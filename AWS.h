#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // handle inc messages
#include <time.h>
#include "credentials.h"

// current time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//milliseconds passed since the program started
unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;

#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub" // put this on subscribe
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub" // put this on topic when you want to publish
 
// MQTT certs
WiFiClientSecure net = WiFiClientSecure();
BearSSL::X509List cert(AWS_CERT_CA);
BearSSL::X509List client_crt(AWS_CERT_CRT);
BearSSL::PrivateKey key(AWS_CERT_PRIVATE);
PubSubClient MQTTclient(net); // MQTT client

time_t now;
time_t nowish = 1510592825; // sets current time

// get date + time
void NTPConnect(void)
{
  Serial.println("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);

  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  //Serial.print("Current time: ");
  //Serial.print(asctime(&timeinfo));
}

String getDate(unsigned long epochTime)
{
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;
  String currentDate = String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear);
  return currentDate;
}

//read messae from AWS 
void messageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message [");
  Serial.print(topic);
  Serial.print("]: ");

  //print message to serial 
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
 
 
void connectAWS()
{
  delay(3000);

  //connecting to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected -");
 
  NTPConnect();

  net.setBufferSizes(512, 512);
  timeClient.begin();
 
  // configure WiFiClientSecure to use AWS credentials
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
 
 // connect to MQTT on AWS endpoint
  MQTTclient.setServer(AWS_IOT_ENDPOINT, 8883); //Port - 8883 for secure MQTT communications

  // create message handler
  MQTTclient.setCallback(messageReceived);
 
  Serial.println("Connecting to AWS IOT...");
 
  //connect to esp
  while (!MQTTclient.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  if (!MQTTclient.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  MQTTclient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}

 // send message to AWS 
void publishMessageTO_AWS(float h, float t, float gas, float conPM1, float conPM25)
{
  StaticJsonDocument<200> doc; // simple message

  //doc["Time_since"] = millis()/1000; // publish no of time (ms) since program ran on AWS, //1000 for s

  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  String formattedDate = getDate(epochTime);

  //doc["Date"] = formattedDate;
  doc["Time"] = formattedTime;
  Serial.println(formattedDate);
  
  doc["Temperature"] = t; // publish temp
  doc["Humidity"] = h; // publish humid
  doc["GasLevel"] = gas; 
  doc["PM10"] = conPM1; 
  doc["PM2.5"] = conPM25; 

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  MQTTclient.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void setupAWS()
{
  Serial.begin(115200);
  connectAWS();
}

void loopAWS(float h, float t, float gas, float conPM1, float conPM25)
{
  now = time(nullptr);
  

  if (!MQTTclient.connected())
  {
    connectAWS();
  }
  else
  {
    MQTTclient.loop(); //  called regularly to allow the client to process inc msgs, to send publish data + refresh connection
    if (millis() - lastMillis > 5000) //millis = now
    {
      lastMillis = millis(); // set last message to now
      publishMessageTO_AWS(h, t, gas, conPM1, conPM25);
    }
  }

}
