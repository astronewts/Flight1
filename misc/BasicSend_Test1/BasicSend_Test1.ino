#include <IridiumSBD.h>

//#include <SoftwareSerial.h>
//SoftwareSerial nss(18, 19);

IridiumSBD isbd(Serial3, 50);

static const int ledPin = 13;

void setup()
{
  int signalQuality = -1;

  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  Serial3.begin(19200);

  isbd.attachConsole(Serial);
  
  isbd.attachDiags(Serial);
  
  isbd.setPowerProfile(1);
  isbd.begin();
  
  isbd.useMSSTMWorkaround(false);

  int err = isbd.getSignalQuality(signalQuality);
  if (err != 0)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(err);
    return;
  }

  Serial.print("Signal quality is ");
  Serial.println(signalQuality);

  err = isbd.sendSBDText("Hello, world!");
  if (err != 0)
  {
    Serial.print("sendSBDText failed: error ");
    Serial.println(err);
    return;
  }

  Serial.println("Hey, it worked!");
  Serial.print("Messages left: ");
  Serial.println(isbd.getWaitingMessageCount());
}

void loop()
{
   digitalWrite(ledPin, HIGH);
}

bool ISBDCallback()
{
   digitalWrite(ledPin, (millis() / 1000) % 2 == 1 ? HIGH : LOW);
   return true;
}
