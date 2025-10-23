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

/* Commands
  exec  -> exec specific command
  read  -> read digital value from pin
  write -> write digital value to pin
  aread -> read analog value from pin
  awrite  -> write analog value to pin
*/


/* lcd related execs
  init lcd display
  set cursor
  print
*/


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
}


e_command_id  getCommandID(String command){
  uint8_t id;
  
  for (id = 0; id < MAX_COMMANDS; id++){
    if (command == commands[id])
      return (e_command_id)id;
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
  Serial.println(pin);
  
  if (pin < 0)
    return ;
  
  char  *value = strtok(NULL, " ");
  for (size_t i = 0; i < 2; i++)
  {
    if (!strcmp(value, values[i])){
      digitalWrite(pin, i);
      return ;
    }
  }
  return ;
}

void  readPin(){
  int8_t pin = getPinNumberDigital();
  if (pin < 0)
    return ;
  
  uint8_t  reading = digitalRead(pin);
  Serial.println(reading);
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
    return;
  
  char  *value = strtok(NULL, " ");
  for (size_t i = 0; i < 2; i++)
  {
    if (!strcmp(value, values[i])){
      pinMode(pin, i);
      return ;
    }
  }
}

void  execCommand(char *buffer){
  char *command = strtok(buffer, " ");
  e_command_id  id = getCommandID(command);

  Serial.println(id);
  switch (id)
  {
  case EXEC:
    exec();
    break;
  case PINMODE:
    setPinMode();
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
    break;
  }
}

void loop() {
  static String buffer;
  char          c;

  while (Serial.available()) {
    c = Serial.read();
    Serial.print(c);
    if (c == '\n' || c == '\r') {
      Serial.println("Received: " + buffer);
      execCommand((char *)buffer.c_str());
      buffer = "";
    }
    else
      buffer += c;
  }
}

