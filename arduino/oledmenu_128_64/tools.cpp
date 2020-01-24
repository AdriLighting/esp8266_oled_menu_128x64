#include <arduino.h>
#ifdef ESP8266
   #include <ESP8266WiFi.h>
#else
   #include <WiFi.h>
#endif 
#include <FS.h>
#ifdef ESP32
   #include "SPIFFS.h"
#endif
#include "tools.h"


char printf_buf[512];

fs(tdblspace,     "\t");
fs(tparm,         " :\t");

void add_string(char * result, String s) {

   String t=s;
   s.toCharArray(result+strlen(result),s.length()+1);
}

String info_parm(String name, String value) {
   String s=name;
   while (s.length()<25) s+=" ";
   return fsget(tdblspace)+s+fsget(tparm)+value+"\n";
}
String info_parm(String name, String value, int space) {
   String s=name;
   while (s.length()<space) s+=" ";
   return fsget(tdblspace)+s+fsget(tparm)+value+"\n";
}
String info_parm(String name, int value) {
   String s=name;
   while (s.length()<25) s+=" ";
   return fsget(tdblspace)+s+fsget(tparm)+String(value)+"\n";
}
String info_parm(String name, String value , String sep) {
   String s=name;
   while (s.length()<25) s+=" ";
   return fsget(tdblspace)+s+sep+fsget(tdblspace)+value+"\n";
}
String info_parm(String name, String value , String sep, int len, String last, String tdb1, String tdb2) {
   String s=name;
   while (s.length()<len) s+=" ";
   return tdb1+s+sep+tdb2+value+last;
}
int explode(String s, char sep, String * list) {
	String t=s+sep;
	int str_index=0;
	int list_index=0;
	int j;
	String sub;
	while (str_index < t.length() ) {
		j=t.indexOf(sep,str_index);
		if (j!=-1) {
			sub=t.substring(str_index, j);
			//Serial.printf("explode index:%d %s\n", str_index,sub.c_str() );
			list[list_index++]=sub;
			str_index=j+1;
		}
	}
	list[list_index]="";
	return list_index;
}

String ip2string(IPAddress a) {
  char buf[18];
  fssprintf(buf,"%d.%d.%d.%d",a[0],a[1],a[2],a[3]);
  return String(buf);
}

IPAddress string2ip(String ip) {
	String a[10];
	explode(ip, '.', a);
	IPAddress adr={a[0].toInt(),a[1].toInt(),a[2].toInt(),a[3].toInt()};
	return adr;
}

void seconds2time(unsigned long s, char * time) {
   int milli      = (s                    ) % 1000;
   int seconds    = (s /   (1000)         ) % 60   ;
   int minutes    = (s /   (1000*60)      ) % 60   ;
   int hours      = (s /   (1000*3600)    ) % 24   ;
   int days    = (s /  (1000*3600*24)  )     ;
   sprintf(time,"%d-%02d:%02d:%02d", days, hours , minutes, seconds);
}

String on_time() {
   char t[100];
   unsigned long offset = 0; //1000 * 60 * 60 * 24 * 3 ;
   unsigned long ms=millis()+offset;
   seconds2time(ms, t);
   return String(t)+" ";
}

String literal_value(String name, String xml){
   String open,close;
   String value = "";
   int start, end;
   open="<"+name+">";
   close="</"+name+">";
   start=xml.indexOf(open);
   if (start!=-1) {
      start+=name.length()+2;
      end=xml.indexOf(close);
      value=xml.substring(start,end);
   }
   return value;
}

String literal_item(String name, String value) {
   String x="<"+name+">"+value+"</"+name+">";
   return x;
}


String ini_item_section(String name) {
   String x="[" + name + "]\n";
   return x;
}
String ini_item_key(String name, String value) {
   String x= name + "=" + value + "\n";
   return x;
}

void toggleBool(String header, boolean *val ){
   *val ? *val = false : *val = true;
   Serial.printf("%s : %d\n", header.c_str(), *val);
}
// void _toggleBool(String header, boolean *val ){
//    *val ? *val = false : *val = true;
//    Serial.printf("%s : %d\n", header.c_str(), *val);
// }


