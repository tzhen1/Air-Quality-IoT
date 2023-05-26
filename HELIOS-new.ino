#include "DHT22.h"
#include "OLED.h"
#include "DSM501A.h"
#include "MQ135.h"
#include "AWS.h"

#define FAN_PIN 13 //GPIO13 at pin D7   
unsigned long currentMillis_FAN;
unsigned long startTime; // global set to 0
unsigned long lastMillis_FAN; // global set to 0
unsigned long interval_fan = 10000; //60000 1 min, 5000

#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
///////////////////////

const char *ssid = "HUAWEI P30 Pro";     // replace with your wifi ssid and wpa2 key
const char *password = "7c11c1ff179d";   // Wifi-password

WiFiClient client;
unsigned long myChannelNumber = 1933210;
const char * myWriteAPIKey = "T88EDUNV84VPKXTQ";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;
////////////////////////////////////

void setup() 
{

  setupOLED();
  setupDHT();
  setupMQ135();
  setupDSM501A();

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW); // set fan low
  
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  setupAWS(); // Initialize AWS

}

void loop() 
{
  currentMillis_FAN = millis(); //get current time
  loopDHT();
  loopMQ135();
  loopDSM501A();

  if(currentMillis_FAN - lastMillis_FAN >= interval_fan) //millis calls current time
  {
    lastMillis_FAN = millis(); //set to current, only if pass through
    digitalWrite(FAN_PIN, HIGH);
    delay(5000);  
    digitalWrite(FAN_PIN, LOW);
  }

////////////////////////////////////////////////////////////////////////////////
float h = dht.readHumidity();
float t = dht.readTemperature();
float gas = analogRead(A0);
float conPM1 = calculateConcentration(lowpulseoccupancyPM1,30);
float conPM25 = calculateConcentrationSM(lowpulseoccupancyPM25,30);
 // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
    // set the fields with the values
    ThingSpeak.setField(1, t);
    //ThingSpeak.setField(1, temperatureF);
    ThingSpeak.setField(2, h);
    ThingSpeak.setField(3, gas);
    ThingSpeak.setField(4, conPM1);
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    // data for gas sensor
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

////////////////////////////////////////////////////////////////////////////////////
    // publish to AWS
    loopAWS(h, t, gas, conPM1, conPM25);
///////////////////////////////////////////////////////////////////////////////////
    if(x == 200){
      Serial.println("Channel update successful.");
      delay(10);
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();

  // DH22 sensor for oled //
  display.clearDisplay(); 
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print("Humidity: ");
  display.println(dht.readHumidity());
  display.display();

  display.setCursor(0,10);
  display.print("Temperature: ");
  display.println(dht.readTemperature());
  display.display();
  // DH22 sensor for oled //

  // MQ135 sensor for oled //
  display.setCursor(0,20);
  display.print("Gas Level: ");
  float j = analogRead(A0);
  display.println(j / 1023 * 100);
  display.display();

  // DSM501A sensor for oled //
  display.setCursor(0,30);
  display.print("PM10:");
  display.print(conPM1);
  display.setCursor(0,40);
  display.print("PM2.5:");
  display.println(conPM25);
  display.display();
  
  delay(2000);

}
