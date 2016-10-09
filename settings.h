#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

const int SELECTED_IP = -3;
const int SELECTED_CYCLE = -2;
const int SELECTED_NEXT = -1;

//const byte MAX_LAUNCH_COUNT = 3;

/*
struct launch_type {
  int32_t seconds_left;
  char name[9];
};
*/

struct launch_type {
  char launch_status;
  char time_status;
  char launch_time[10 + 1];
  char agency[20 + 1];
  char rocket[20 + 1];
  char payload[20 + 1];
  char destination[10 + 1];
};

class Settings {
  
  public:
    void loadFromEEPROM();
    void saveToEEPROM();

    //Things to save
    char website[24] = "nextrocket.space";
    char url_setting_part[24] = "/";
    char url_user_part[10*3+1] = "1a";
    uint8_t intensity = 6;
    int8_t selected_menu = SELECTED_CYCLE;

    //Things not to save
    uint8_t launch_count = 1;
    
    launch_type launch;
    void loadLaunch(int index);
    void processApiResponse(int index, uint8_t data);
    
    uint8_t selected_launch = 0; //selected launch index
    
    char* url_const_part = "api.php?v=2&q=";

    uint32_t time_downloaded;


  private:
    void setLaunchByte(int index, uint8_t data);
    
    /*
    //Thins not to save
    launch_type launches[MAX_LAUNCH_COUNT] = {
      //{1000, "DEMO    "}, // Can be used for demo
      {0, "LOADING "},
      {0, "LOADING "},
      {0, "LOADING "},
    }; // Launches that can be displayed
    */
    
};

extern Settings settings;

#endif

