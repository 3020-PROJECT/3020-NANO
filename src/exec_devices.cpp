# include "exec_devices.h"

extern char buffer[512];
extern size_t index;

void  handleServo180Commands(void){
  
}

void initExecDeviceCommands(void){
  exec_device_commands[LCD].token = "lcd";
  exec_device_commands[LCD].handler = handleLCDCommands;
  exec_device_commands[SERVO_180].token = "servo180";
  exec_device_commands[SERVO_180].handler = handleServo180Commands;
  exec_device_commands[UNKNOW].handler = NULL;
}

void  exec(void){
  char *command = strtok(NULL, " ");
  
  parseAndExecute(command, exec_device_commands);
}