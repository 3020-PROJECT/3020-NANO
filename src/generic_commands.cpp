#include "3020.h"

#define MAX_COMMANDS 7

typedef enum COMMAND_ID{
  EXEC,
  READ,
  WRITE,
  AREAD,
  AWRITE,
  PINMODE,
  UNKNOWN
} e_command_id;

_THandler commands[MAX_COMMANDS];

extern char buffer[512];
extern size_t index;




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

void  parseAndExecute(String token, _THandler *arr){
  for (uint32_t i = 0; arr[i].handler != NULL; i++){
    if (token == arr[i].token){
        reply("OK", ("Found handler For " + token).c_str());
        return arr[i].handler();
    }
  }
  
  reply("KO", ("Couldn't find handler For " + token).c_str());
}

void  parseAndExecBuffer(char *buffer){
  char *command = strtok(buffer, " ");
  parseAndExecute(command, commands);
}

void initCommands(void){
  commands[EXEC].token = "exec";
  commands[EXEC].handler = exec;
  commands[READ].token = "read";
  commands[READ].handler = readPin;
  commands[WRITE].token = "write";
  commands[WRITE].handler = writePin;
  commands[AREAD].token = "aread";
  commands[AREAD].handler = areadPin;
  commands[AWRITE].token = "awrite";
  commands[AWRITE].handler = awritePin;
  commands[PINMODE].token = "pinmode";
  commands[PINMODE].handler = setPinMode;
  commands[UNKNOWN].token = "UNKNOWN";
  commands[UNKNOWN].handler = NULL;
}