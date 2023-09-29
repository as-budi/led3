//#define BLYNK_TEMPLATE_ID "TMPL61nRdR0uv"
//#define BLYNK_TEMPLATE_NAME "3 LEDs"
//#define BLYNK_AUTH_TOKEN "4Z8Q3MTOh0arzelRyBwK4umHgibEZSWO"

#define BLYNK_TEMPLATE_ID "TMPL6MQP2qiyi"
#define BLYNK_TEMPLATE_NAME "jalan"
#define BLYNK_AUTH_TOKEN "0OPQLlrY1yQp_MVrdGO4DcdB49xQMTq1"

char ssid[] = "zenfone9";
char pass[] = "zxcvbnm9";

bool fetch_blynk_state = true;  //true or false

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <AceButton.h>
using namespace ace_button;

#define VPIN_1    V1 
#define VPIN_2    V2
#define VPIN_3    V3 

#define LED_R 13
#define LED_Y 12
#define LED_G 14
#define button1 25
#define button2 26
#define button3 27

#define wifiLed   2   //D2
int wifiFlag = 0;

bool rState = LOW;
bool yState = LOW;
bool gState = LOW;

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

ButtonConfig config1;
AceButton aBtn1(&config1);
ButtonConfig config2;
AceButton aBtn2(&config2);
ButtonConfig config3;
AceButton aBtn3(&config3);

void button1Handler(AceButton*, uint8_t, uint8_t);
void button2Handler(AceButton*, uint8_t, uint8_t);
void button3Handler(AceButton*, uint8_t, uint8_t);


BLYNK_WRITE(VPIN_1) {
  rState = param.asInt();
  digitalWrite(LED_R, rState);
}

BLYNK_WRITE(VPIN_2) {
  yState = param.asInt();
  digitalWrite(LED_Y, yState);
}

BLYNK_WRITE(VPIN_3) {
  gState = param.asInt();
  digitalWrite(LED_G, gState);
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_1, rState);
    Blynk.virtualWrite(VPIN_2, yState);
    Blynk.virtualWrite(VPIN_3, gState);
    }
    digitalWrite(wifiLed, HIGH);
    Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_1);
    Blynk.syncVirtual(VPIN_2);
    Blynk.syncVirtual(VPIN_3);
  }
}

void setup() {
     Serial.begin(115200);

     pinMode(wifiLed, OUTPUT);
     
     pinMode(LED_R, OUTPUT);
     pinMode(LED_Y, OUTPUT);
     pinMode(LED_G, OUTPUT);

     pinMode(button1, INPUT_PULLUP);
     pinMode(button2, INPUT_PULLUP);
     pinMode(button3, INPUT_PULLUP);

     config1.setEventHandler(button1Handler);
     config2.setEventHandler(button2Handler);
     config3.setEventHandler(button3Handler);

     aBtn1.init(button1);
     aBtn2.init(button2);
     aBtn3.init(button3);

     digitalWrite(LED_R, !rState);
     digitalWrite(LED_Y, !yState);
     digitalWrite(LED_G, !gState);

     digitalWrite(wifiLed, LOW);

     WiFi.begin(ssid, pass);
     timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
     Blynk.config(auth);
     delay(1000);

     if (!fetch_blynk_state){
          Blynk.virtualWrite(VPIN_1, rState);
          Blynk.virtualWrite(VPIN_2, yState);
          Blynk.virtualWrite(VPIN_3, gState);
     }
}

void loop() {
     Blynk.run();
     timer.run();

     aBtn1.check();
     aBtn2.check();
     aBtn3.check();
}

void button1Handler(AceButton*, uint8_t eventType, uint8_t) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      rState = !rState;
      digitalWrite(LED_R, rState);
      Blynk.virtualWrite(VPIN_1, rState);
      break;
  }
}

void button2Handler(AceButton*, uint8_t eventType, uint8_t) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      yState = !yState;
      digitalWrite(LED_Y, yState);
      Blynk.virtualWrite(VPIN_2, yState);
      break;
  }
}

void button3Handler(AceButton*, uint8_t eventType, uint8_t) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      gState = !gState;
      digitalWrite(LED_G, gState);
      Blynk.virtualWrite(VPIN_3, gState);
      break;
  }
}
