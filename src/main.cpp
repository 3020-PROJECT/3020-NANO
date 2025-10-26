#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

typedef struct lcdDevice
{
  uint8_t address;
  uint8_t height;
  uint8_t width;
} _lcdDevice;


#define MAX_COMMANDS 6

typedef enum COMMAND_ID{
  EXEC,
  READ,
  WRITE,
  AREAD,
  AWRITE,
  PINMODE,
  UNKNOWN
} e_command_id;

String commands[MAX_COMMANDS];

char buffer[256];
static size_t index;

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

void initCommands(){
  commands[EXEC] = "exec";
  commands[READ] = "read";
  commands[WRITE] = "write";
  commands[AREAD] = "aread";
  commands[PINMODE] = "pinmode";
  commands[AWRITE] = "awrite";
}

void setup() {
  lcd = {0x3F, 16, 2};
  lcd.init();
  lcd.backlight();
  lcd.print("wow");
  initCommands();
  Serial.begin(9600);
  reply("OK", "Protocol initialized and ready !");
}


e_command_id  getCommandID(String command){
  uint8_t id;
  
  for (id = 0; id < MAX_COMMANDS; id++){
    if (command == commands[id]){
      reply("OK", "Command Found");
      return (e_command_id)id;
    }
  }
  return UNKNOWN;
}

void  exec(){

}

int8_t  getPinNumber(){
  char *pin = strtok(NULL, " ");

  int8_t nbr = atoi(pin);
  return nbr;
}

int8_t  getPinNumberAnalog(){
  int8_t pin = getPinNumber();

  if (pin >= 14 && pin <= 21)
    return pin;
  return -1;
}

int8_t  getPinNumberDigital(){
  int8_t pin = getPinNumber();

  if (pin >= 0 && pin <= 13)
    return pin;
  return -1;
}

void  writePin(){
  static const char *values[] = {"low", "high"};

  int8_t pin = getPinNumberDigital();
  
  if (pin < 0)
    return reply("KO", "digital pin must be between 0 and 13"); 
  
  char  *value = strtok(NULL, " ");
  for (size_t i = 0; i < 2; i++)
  {
    if (!strcmp(value, values[i])){
      digitalWrite(pin, i);
      return ;
    }
  }
  reply("KO", "wating low or high");
  return ;
}

void  readPin(){
  int8_t pin = getPinNumberDigital();
  if (pin < 0)
    return ;
  
  uint8_t  reading = digitalRead(pin);
  String rep("Value = " + reading);
  reply("OK", rep.c_str());
}

void  awritePin(){
  int8_t pin = getPinNumberAnalog();
  if (pin < 0)
    return ;
  //  to finish later
}

void  areadPin(){
  int8_t pin = getPinNumberAnalog();
  if (pin < 0)
    return ;
  
  //  to finish later
}

void  setPinMode(){
  static const char *values[] = {"input", "output"};
  int8_t pin = getPinNumber();

  if ((pin < 0 || pin > 21))
    return reply("KO", "pin number must be between 0 and 21");
  
  char  *value = strtok(NULL, " ");
  for (size_t i = 0; i < 2; i++)
  {
    if (!strcmp(value, values[i])){
      pinMode(pin, i);
      return reply("OK", "pinmode set");
    }
  }
}

void  execCommand(char *buffer){
  char *command = strtok(buffer, " ");
  e_command_id  id = getCommandID(command);

  switch (id)
  {
  case EXEC:
    exec();
    break;
  case PINMODE:
    setPinMode();
    break;
  case  READ:
    readPin();
    break;
  case  WRITE:
    writePin();
    break;
  case  AREAD:
    areadPin();
    break;
  case  AWRITE:
    awritePin();
    break;
  default:
    reply("KO", "Unknown Command");
    break;
  }
}

void loop() {
  char c;

  while (Serial.available()) {
    c = Serial.read();
    Serial.write(c);
    if (c == '\n') {
      buffer[index] = '\0';
      execCommand(buffer);
      index = 0;
    } 
    else if (index < 255) {
      buffer[index++] = c;
    }
  }
}


