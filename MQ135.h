#include <ESP8266WiFi.h>

void setupMQ135() {
  Serial.begin(115200);
  Serial.println("Connecting to ");
}

int timeSinceLastReadMQ135 = 0;

void loopMQ135() {
  float j = analogRead(A0);

  if (isnan(j))
  {
    Serial.println("Failed to read from MQ-5 sensor!");
    timeSinceLastReadMQ135 = 0;
    return;
  }
  else {
    Serial.print("Gas Level: ");
    Serial.println(j / 1023 * 100);
    timeSinceLastReadMQ135 = 0;
  }
  
  
  timeSinceLastReadMQ135 += 1000;
}
