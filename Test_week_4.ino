#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;
BlynkTimer timer;
WidgetLED led1(V6);

char auth[] = "wyW4-1FXnh9SnKquTqQ4YrZT9OiyrFa-";
char ssid[] = "telenet-0BA24";
char pass[] = "aWdYfkF68ane";

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

#define led 18

int gewensteWaarde;
float Val = 0;


void sendSensor()
{
  float t = bmp.readTemperature();
  Blynk.virtualWrite(V5, t);
}

#define gewensteWaarde V7

BLYNK_WRITE(gewensteWaarde)
{
  int gt = param.asInt();
  Val = gt;
  Serial.print("Gewenste waarde ingesteld op: ");
  Serial.println(gt);
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("BMP280 test"));

  pinMode(led, OUTPUT);

  if (!bmp.begin(0x76))
  {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
    while (1);
  }

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);

  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();

  float t = bmp.readTemperature();
  
  if (t <= Val)
  {
    digitalWrite(led, HIGH);
    led1.on();
  }

  else
  {
    digitalWrite(led, LOW);
    led1.off();
  }


  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  Serial.println();
  
  delay(1000);
}
