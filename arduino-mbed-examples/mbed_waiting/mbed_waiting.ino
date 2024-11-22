#include <mbed.h>
#include <mbed_wait_api.h>

using namespace mbed;

void setup()
{
  Serial.begin(9600);

  pinMode(5, OUTPUT);
}

void loop()
{
  digitalWrite(5, HIGH);
  wait_ms(100);
  digitalWrite(5, LOW);
  wait_ms(100);
}
