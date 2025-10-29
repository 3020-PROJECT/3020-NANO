#pragma once

#include <Arduino.h>

typedef void (*_token_handler)(void);

typedef struct tokenHandlerStruct{
  String token;
  _token_handler handler;
} _THandler;

void  reply(char *OK_KO, const char *optionalMessage);
void  exec(void);   // => exec lcd init
void  parseAndExecute(String token, _THandler *arr);

void    initCommands(void);
void    initExecDeviceCommands(void);
void    parseAndExecBuffer(char *buffer);