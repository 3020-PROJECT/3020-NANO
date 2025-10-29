#include "3020.h"
#include "3020-lcd.h"
#include <Wire.h>

char buffer[512];
size_t index;

/* Commands
  exec  -> exec specific command
  read  -> read digital value from pin
  write -> write digital value to pin
  aread -> read analog value from pin
  awrite  -> write analog value to pin
*/


/* lcd related execs
  init lcd display => inits with the address, width and height
  set cursor
  print
*/


void  reply(char *OK_KO, const char *optionalMessage){
  Serial.write(OK_KO);
  if (optionalMessage){
    Serial.write(" [");
    Serial.write(optionalMessage);
    Serial.write("]");
  }
  Serial.write("\r\n");
}

void setup() {
  initCommands();
  initLCDCommands();
  initExecDeviceCommands();
  Wire.begin(); // to use Wire.endTransmission
  Serial.begin(9600);
  reply("OK", "Protocol initialized and ready !");
}

void loop() {
  char c;

  while (Serial.available()) {
    c = Serial.read();
    Serial.write((int)c);

    switch (c)
    {
      case '\r':
        break;
      case '\n':
        buffer[index] = '\0';
        parseAndExecBuffer(buffer);
        index = 0;
        break;
      case '\b':
        index--;
        buffer[index] = '\0';
        break;
      default:
        buffer[index] = c;
        buffer[index + 1] = '\0';
        index++;
        break;
    }
  }
}


