#include <pgmspace.h>
#define CREDENTIALS

#define THINGNAME "ESP8266_Air_Quality" //change this
const char WIFI_SSID[] = "huawei phone";               //change this
const char WIFI_PASS[] = "hotspot pass";           //change this
const char AWS_IOT_ENDPOINT[] = "aws endpoint"; //change this
 
int8_t TIME_ZONE = 0;

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
  //
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
 //
-----END CERTIFICATE-----
 
 
)KEY";
 
// Device Private Key                                               
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
//
-----END RSA PRIVATE KEY-----
 
 
)KEY";
