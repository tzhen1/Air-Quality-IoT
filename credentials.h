#include <pgmspace.h>
#define CREDENTIALS

#define THINGNAME "ESP8266_Air_Quality" //change this
const char WIFI_SSID[] = "HUAWEI P30 Pro";               //change this
const char WIFI_PASS[] = "7c11c1ff179d";           //change this
const char AWS_IOT_ENDPOINT[] = "a303gogysdnh0c-ats.iot.eu-west-2.amazonaws.com"; //change this
 
int8_t TIME_ZONE = 0;

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUZPD+S/wtRdaBB3ikcJ3I6xARLDIwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMTEwOTAwMjQw
MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALHlfrieI9cQw6WJXNcX
c++LlsLsiJszD9N9+Co0J9VHOKzlA1L3qqw9fS5zetHt5h0wIgABqsxpyWeIfWGE
agRcDkMwbWSp4K1zaeEP8GPAhEziWsIx0OxXIscIPRokvIrj/AaqHWKbkbIzraLd
XxBvSGT6/n7AclJTwhDUMsO34yXmyXVgs6UK2+NzYfDCkhg5UJ5GjP1VzPsPxHML
W5x1MmNlIFyqQJH5zyvkjusXyZzzySeAEBNtmFlsMIpVPuve9n/HX4Uhhc874Qny
LKWZ6/e9E+2gYlHxzdfMOLPq1GGgw5zZuouNDp7ijgRvmkPW70hs4012WHjDn0MO
+n0CAwEAAaNgMF4wHwYDVR0jBBgwFoAU/ttTrPNNE9OMlkU7BlGRlG24qqYwHQYD
VR0OBBYEFK2VGE9fHvTXgKlbvAvS14HgwWh0MAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCelmPWjcOZG2TvhUovixm9g6Ym
PLxAIgLYW4K+7fQrDhC6QBePUlWHc0uHu+oEBbbFS4u7cDEqKYCPi5FbZ78+aVd5
8/B15FZZX8f7F3RdLePcEpN9AjybOjO0ZgDx4axUwJ+0M1gK2m6oO44UVpVONaXh
R8Ag6jXZXNMRzJRP1pjthh4E/wiSntJws3vrU7nt8kCeN7yY8mx/MSrHAgmD5TtC
7PXl3RsMOS/Ps9LV811aXgOJ3+TJUsuBtR04Ahdqk3iVV7FvJzpS+h+R09rr/Hx2
CIDsUVBz2hqGS9uU37cez4PVAcCzPpyWMDVMEidaaWrL+XsMepM4p3DCRte8
-----END CERTIFICATE-----
 
 
)KEY";
 
// Device Private Key                                               
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAseV+uJ4j1xDDpYlc1xdz74uWwuyImzMP0334KjQn1Uc4rOUD
UveqrD19LnN60e3mHTAiAAGqzGnJZ4h9YYRqBFwOQzBtZKngrXNp4Q/wY8CETOJa
wjHQ7Fcixwg9GiS8iuP8BqodYpuRsjOtot1fEG9IZPr+fsByUlPCENQyw7fjJebJ
dWCzpQrb43Nh8MKSGDlQnkaM/VXM+w/EcwtbnHUyY2UgXKpAkfnPK+SO6xfJnPPJ
J4AQE22YWWwwilU+6972f8dfhSGFzzvhCfIspZnr970T7aBiUfHN18w4s+rUYaDD
nNm6i40OnuKOBG+aQ9bvSGzjTXZYeMOfQw76fQIDAQABAoIBAFyDfs0/4F+d5kEW
GGP7XF18+GBSZhBgE6Dm58H68aQmr8CqMHpbNjTwnUMk5wB5eaxANc5PD6XSdd0T
zDCEjiXKQdiLPLiW57MMTEmw4lDd82+QX6aSBZaDEiDzleMcq7iHK6/DlBiN0s0b
tvb1rNR9Kubo6KS+LDXIPSpHUEcbYlnvEVBoWrwRl6ApRAvO48bFzlej0uB4ckKh
kI/tfagEc/+FccPH+EwkpiRwyVPlEiZADZrft2fquxs9Ov2wmY8OfwuEXmCoEf6L
j/6vGOpU9Gh9uwVKzaONR/AimrHIMvGG0bCIlWemcUJulGjOUJE8xFuMsZ+hgGKc
AU+lvwECgYEA17L1RIfd5q95WpoWk1jB1w+Njp7AIRLUG3purTpWkuguy/ZKlN/F
knLMtP4jUsSEd88bFvpt2NJRJLvr/YqDopTXEOgAGWuWgo6iNqYVQJ/0kYwNow+0
gVO1u94ROMzXBjQ2FHCJUcdw3boLB5pkqqokYIHG+AEZngRv2XLevSECgYEA0yJp
Y+HLugrpvCl2fnw4E7MiqxPsrSwGdSIy1Lhl1+knWSlTcNf2VMQ8wO+0XMJ2QCCq
HK3lHaxyUCWpvCYN6sKurUnBZpnY/XbKQnZB6CaIBgcjWAbf0uJcX2QtLhv3F31e
ZtRDvuOJ3TYlNzCPkzrVtrBJXwPPRoYzgTkCFd0CgYEApEh2C/MbbEa7u+jeRu1z
/ZOn8jcxCD+p8x/FygRDyfIzKj90wHzKoubyyOtDBhgM+J9x94dXQKu3B65laB8m
cR7ByuuYn8PUx61RGSRXzXfGs1AjVs1wYovaP7M8FLHXF6+OfPTVccbgTNAU9H6A
K4gY2rQdUeislTC1671UrmECgYEAo3yi9OeztCSdJslIatrPJVnxv10XSGStv2ey
yorauZxdjqn0ZY2kvbCShqdWFvmxTSi98KUqo5aTkcIIM5uX7S/AyIgzDLqWjnW8
Wkht8lBbxacjVBdGm2oSMv9ECM84vuSiA6fbqWVRrIwvtxOQKWZGdEb/GWa+ypJC
MY6uMbUCgYEAjjBVSvws2Gp8jLKnwUV9uPfsnWJvm0bRblVnxH64YmhAcfTZzmqc
v0NM1ElKLKB50lFpYOXUUZ8rQUPArL73f8QYNDAAr1VItVhxEMsOaTdSuGGyYAH2
md0Ld88qYxqKv3M3UzSMuO3DMhEajmzX+XGMVo2ltyA6q3rKaVyA++Q=
-----END RSA PRIVATE KEY-----
 
 
)KEY";