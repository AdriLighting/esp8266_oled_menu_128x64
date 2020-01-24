#ifndef TOOLS_H
   #define TOOLS_H
   #ifdef ESP8266
      #include <ESP8266WiFi.h>
   #else
      #include <WiFi.h>
   #endif 

   // extern char printf_buf[100];

   // #define fs(parm_a,parm_b) static const char parm_a[] PROGMEM = parm_b
   // #define fsprintln(parm_a) Serial.println(FPSTR(parm_a))
   // #define fsprint(parm_a) Serial.print(FPSTR(parm_a))
   // #define fsget(parm_a) String(FPSTR(parm_a))
   // #define fsprintf(parm_a,...) {sprintf_P(printf_buf, PSTR(parm_a), __VA_ARGS__); Serial.print(printf_buf);}
   // #define fssprintf(parm_b, parm_a,...) sprintf_P(parm_b, PSTR(parm_a), __VA_ARGS__)

   
#include <FS.h>
#ifdef ESP32
   #include "SPIFFS.h"
#endif

   extern char printf_buf[512];

   #define fs(parm_a,parm_b) static const char parm_a[] PROGMEM = parm_b
   #define fsprintln(parm_a) Serial.println(FPSTR(parm_a))
   #define fsprint(parm_a) Serial.print(FPSTR(parm_a))
   #define fsget(parm_a) String(FPSTR(parm_a))
   #define fsprintf(parm_a,...) {sprintf_P(printf_buf, PSTR(parm_a), __VA_ARGS__); Serial.print(printf_buf);}
   #define fssprintf(parm_b, parm_a,...) sprintf_P(parm_b, PSTR(parm_a), __VA_ARGS__)

   void add_string(char * result, String s);




   String      info_parm(String name, String value);
   String      info_parm(String name, int value);
   String      info_parm(String name, String value , String sep);
   String      info_parm(String name, String value , String sep, int len, String last, String tdb1, String tdb2);   
   String      info_parm(String name, String value, int space)  ;  

   String      ini_item_section(String name);
   String      ini_item_key(String name, String value);

   String      literal_item(String name, String value);
   String      literal_value(String name, String xml);

   void        seconds2time(unsigned long s, char * time);
   String      on_time();

   String      ip2string(IPAddress a);
   IPAddress   string2ip(String ip);

   int         explode(String s, char sep, String * list);


#endif

