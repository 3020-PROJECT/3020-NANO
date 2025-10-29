#include "3020.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

# define MAX_LCD_COMMANDS 5

typedef enum  LCD_COMMANDS_id{
  INIT_LCD,
  WRITE,
  CLEAR,
  SET_CURSOR,
  UNKNOW
} e_lcd_command_ids;

LiquidCrystal_I2C   lcd(0x27, 16, 2);
_THandler   static  commands[MAX_LCD_COMMANDS];


bool    lcdIsConnected(uint8_t addr){
    Wire.beginTransmission(addr);
    return (Wire.endTransmission() == 0);
}

bool    checkLCDParams(char *addr, char *height, char *width){
    
    if (!addr || !height || !width)
        return false;

    uint8_t addr_val = (strncmp(addr, "0x", 2) == 0) ? strtol(addr, NULL, 16) : atoi(addr);
    if (!lcdIsConnected(addr_val)){
        reply("KO", "LCD not found at the specified addr");
        return false;
    }
    uint8_t height_val = atoi(height);  // checks to do later; I'm tired now. 
    uint8_t width_val = atoi(width);    // checks to do later; I'm tired now.

    lcd = {addr_val, width_val, height_val};
    return true;
}

void    initLCD(void){
    char    *addr = strtok(NULL, " "), 
            *width = strtok(NULL, " "),
            *height = strtok(NULL, " ");

    if (!checkLCDParams(addr, height, width))
        return reply("KO", "LCD PARAMS !!");
    
    lcd.init();
    lcd.backlight();
    reply("OK", "init screen");
}

void    writeLCD(void){
    char *s = strtok(NULL, "\0");
    
    if (!s || !(*s))
        return ;
    lcd.print(s);
}

void    clearLCD(void){
    lcd.clear();
}

bool    validateCursor(char *x, char *y){
    
    if (!x || !y)
        return false;

    uint8_t x_val = atoi(x); // check xy later
    uint8_t y_val = atoi(y); // check xy later
    
    lcd.setCursor(x_val, y_val);
    return true;
}

void    cursroSetLCD(void){
    char    *x = strtok(NULL, " "), 
            *y = strtok(NULL, " ");

    if (!validateCursor(x, y))
        return reply("KO", "Cursor x, y incorrect");
    
}

void    initLCDCommands(void){
    commands[INIT_LCD].token = "init";
    commands[INIT_LCD].handler = initLCD;

    commands[WRITE].token = "write";
    commands[WRITE].handler = writeLCD;

    commands[CLEAR].token = "clear";
    commands[CLEAR].handler = clearLCD;

    commands[SET_CURSOR].token = "cset";
    commands[SET_CURSOR].handler = cursroSetLCD;

    commands[UNKNOW].handler = NULL;
}

void  handleLCDCommands(void){
    char *token = strtok(NULL, " ");
    parseAndExecute(token, commands);
}