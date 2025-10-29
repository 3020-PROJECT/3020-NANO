# pragma once

# include "3020.h"

#define MAX_EXEC_DEVS 3

typedef enum  exec_device_id{
  LCD,
  SERVO_180,
  UNKNOW
} e_exec_device_id;

_THandler exec_device_commands[MAX_EXEC_DEVS];

void  handleLCDCommands(void);