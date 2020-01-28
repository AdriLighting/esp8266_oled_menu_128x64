#include "oled_def.h"    
#ifdef ADAFRUIT_SD1306_LIB
    #include <Adafruit_SSD1306.h>
    extern Adafruit_SSD1306 display;
#endif
#ifdef SD1306WIRE_LIB
    #include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
    extern SSD1306Wire display;
#endif
    
#include "oled_menu.h"
#include "oled_display.h"
#include "tools.h"
#include "adriwifi.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

extern oled_menu_pos oled_menu_pos;


int 			oled_menu_selectedStrip 	= 0;

boolean	 		oled_menu_CC_loop			= false;
boolean 		oled_menu_CC_up 			= true;
uint8_t 		oled_menu_CC_hue;
unsigned long 	oled_menu_CC_lastChange;



// USER DECLAR OWN MENU
oled_menu_create * oled_menu_home;              	




PROGMEM oled_menu_item list_m1 [] = {           
    {"select effect",	&test_f1, 	&test_f2},
    {"color", 			&test_f1, 	&test_f2},
    {"off",     		&test_f1, 	&test_f2},
    {"statu serveur", 	&test_f1, 	&test_f2},
};
extern wifi_id wifi_id;



// CREATIONS MENU GENERALISER 
template <typename Callable>
void oled_menu_create_items(Callable f_1, Callable f_2, oled_menu_create * str) {
	str->contents[0].name 		= "center"; 
	str->contents[0].func_1 	= f_1; 
	str->contents[0].func_2 	= f_2; 
	str->contents[1].name 		= "out"; 
	str->contents[1].func_1 	= f_1; 
	str->contents[1].func_2 	= f_2; 
	str->size 					= 2;  
}

// USER DECLAR OWN OPTIONS
//                                  attribution du menu         nom du sous-titre
void opt1(){ oled_menu_setOption(   oled_menu_home ,            "s-titre"); }
PROGMEM oled_menu_options oled_menu_options_list [] = { 

};


// void lg1(boolean exec){
//     if (!exec) {oled_menu_set_lg(oled_menu_home); 			return;}
//     oled_display_mod = oled_display_time;
// }   

PROGMEM oled_menu_longClick_1 oled_menu_longClick_1_list [] = { 

};


PROGMEM oled_menu_longClick_2 oled_menu_longClick_2_list [] = { 

};


void oled_menu_setup(){
    // DEFINE LONG_CLICK LIST SIZE
    oled_menu_longClick_1_count = ARRAY_SIZE(oled_menu_longClick_1_list);
    oled_menu_longClick_2_count = ARRAY_SIZE(oled_menu_longClick_2_list);

    // DEFINE OPTIONLIST SIZE
    oled_menu_optCount = ARRAY_SIZE(oled_menu_options_list);

    // USER CREATE OWN MENU OJECT
    oled_menu_home          = new oled_menu_create("home");


    // USER CREATE OWN MENU ITEMS
    oled_menu_create_items_v2(list_m1, ARRAY_SIZE(list_m1), oled_menu_home); 	// oled_menu_home


    for( int i = 0; i < oled_menu_array_nbr; i++) {
    	// oled_menu_array[i]->print();
    	oled_menu_array[i]->clickmoveUp 	= true;
    	oled_menu_array[i]->clickmoveDown 	= true;
    	oled_menu_array[i]->clickmoveFunc 	= 1;
    }

	oled_menu_set_main(oled_menu_home);

	oled_display_mod = oled_display_time;
}

