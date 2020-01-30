/*
    24/01/2020 15:03:48 : mise en udf de mes fonction en vrac pour un ecran oled

    MICRO UDF POUR CREATION DE MENU POUR L'ECRAN OLED 128x64 MONOCHROME
    AUTHOR : ADRILIGHTING
    COMPATIBLE BOARD            : ESP8266 > 2.5
    EXEMPLE                     : oled_menu_128x64.ino
    LIBRAIRIE EXTERNE UTILISER  :   
                                        <SPI.h>                 : FROM ESP8266 2.6.3
                                        <Wire.h>                : FROM ESP8266 2.6.3
        ADAFRUIT LIB
        Adafruit-GFX-Library-1.5.6      <Adafruit_GFX.h>        : https://github.com/adafruit/Adafruit-GFX-Library
        Adafruit_SSD1306-master         <Adafruit_SSD1306.h>    : https://github.com/adafruit/Adafruit_SSD1306  

        ThingPulse OLED SSD1306 (ESP8266/ESP32/Mbed-OS) 
        esp8266-oled-ssd1306-master     "SSD1306Wire.h"         : https://github.com/ThingPulse/esp8266-oled-ssd1306/
    
*/

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#include <oled_display.h>




//DECLARE MENUS
oled_menu_create * oled_menu_home;              		// Main menu
oled_menu_create * oled_menu_home_lighting;     		// 
oled_menu_create * oled_menu_lighting_livingRoom;		// 
oled_menu_create * oled_menu_pool;						// 
oled_menu_create * oled_menu_livingRoom_lighting;		//					
oled_menu_create * oled_menu_pool_relay;				//		

//DECLARE MENU ITEMS LIST AND FUNCTIONS
void m2_i1_m(oled_menu_move move)	{oled_menu_init_menu (oled_menu_lighting_livingRoom);		}
void m1_i1_m(oled_menu_move move)	{oled_menu_init_menu (oled_menu_home_lighting);				}
void m1_i3_m(oled_menu_move move)	{oled_menu_init_menu (oled_menu_pool);						}
void m3_onOff(oled_menu_move move)	{oled_menu_init_menu (oled_menu_livingRoom_lighting);	}
void m4_onOff(oled_menu_move move)	{oled_menu_init_menu (oled_menu_pool_relay);				}

PROGMEM oled_menu_item list_m1 [] = {           // liste des fonction autribuer aux items
//  TITRE           subtitle    db click bp 1   db click bp 2 	db click bp 3 	db click bp 4
    {"lumierre",    "",         &m1_i1_m,		&om_tf,       	&om_tf,       	&om_tf},
    {"piscine",     "",			&m1_i3_m,		&om_tf, 		&om_tf,       	&om_tf},

};
PROGMEM oled_menu_item list_m2 [] = { 
    {"salon",       "",         &m2_i1_m, 		&om_tf, 		&om_tf,       	&om_tf},
    {"cuisine",     "",         &om_tf,       	&om_tf, 		&om_tf,       	&om_tf},
    {"terasse",     "",         &om_tf,       	&om_tf, 		&om_tf,       	&om_tf},

};
PROGMEM oled_menu_item list_m3 [] = { 
    {"commode",		"",         &m3_onOff, 		&om_tf, 		&om_tf,       	&om_tf},
    {"tv",     		"",         &m3_onOff,		&om_tf, 		&om_tf,       	&om_tf},
    {"canape",     	"",         &m3_onOff,		&om_tf, 		&om_tf,       	&om_tf},

};

void relay_on(oled_menu_move move){
	if (oled_menu_current == oled_menu_livingRoom_lighting) {}
	if (oled_menu_current == oled_menu_pool_relay) {}
}
void relay_off(oled_menu_move move){
	if (oled_menu_current == oled_menu_livingRoom_lighting) {}
	if (oled_menu_current == oled_menu_pool_relay) {}
}
PROGMEM oled_menu_item list_m4 [] = { 
    {"spot piscine",		"",         &m4_onOff, 		&om_tf, 		&om_tf,       	&om_tf},
    {"spot terrase",		"",         &m4_onOff,		&om_tf, 		&om_tf,       	&om_tf},
    {"dsijoncteur pompe",	"",         &m4_onOff,		&om_tf, 		&om_tf,       	&om_tf},
};

// COMMON LIST
PROGMEM oled_menu_item list_m5 [] = { 
    {"on",				"",         &relay_on,		&om_tf, 		&om_tf,       	&om_tf},
    {"off",				"",         &relay_off,		&om_tf, 		&om_tf,       	&om_tf},
};

/*
    ACTION QUAND UN LONG CLICK EST EFFECTUER (atrribuer au menu afficher) :

    en attendant de trouver comment passer le pointeur d'une classe dans une structure
        (voir.cpp)
        dans un premier temp j'assigne le menu sélectionner avec le pointeur : oled_menu_lg
        puis on exécute les actions voulues
            C'EST PAS GOOD
*/
void lg1(boolean exec, oled_menu_move move){
    if (!exec) {
        oled_menu_set_lg(oled_menu_home); 
        return;
    }
    oled_display_mod        = oled_display_off;
    oled_clear();
    // oled_draw_bmp (0, 0, 128, 64, logo_bmp);
    display.display();   

}   
void lg2(boolean exec, oled_menu_move move){
    if (!exec) {oled_menu_set_lg(oled_menu_home_lighting); return;}
    oled_menu_home->init_menu();   
} 
void lg3(boolean exec, oled_menu_move move){
    if (!exec) {oled_menu_set_lg(oled_menu_lighting_livingRoom); return;}
    oled_menu_home_lighting->init_menu();   
} 
void lg4(boolean exec, oled_menu_move move){
    if (!exec) {oled_menu_set_lg(oled_menu_pool); return;}
    oled_menu_home->init_menu();   
}  
void lg5(boolean exec, oled_menu_move move){
    if (!exec) {oled_menu_set_lg(oled_menu_livingRoom_lighting); return;}
    oled_menu_lighting_livingRoom->init_menu();   
} 
void lg6(boolean exec, oled_menu_move move){
    if (!exec) {oled_menu_set_lg(oled_menu_pool_relay); return;}
    oled_menu_pool->init_menu();   
}    
PROGMEM oled_menu_longClick_1 oled_menu_longClick_1_list [] = { 
    { &lg1},
    { &lg2},
    { &lg3},
    { &lg4},
    { &lg5},
    { &lg6},
};
PROGMEM oled_menu_longClick_2 oled_menu_longClick_2_list [] = {};
#ifdef FUNC_ITEM_3
PROGMEM oled_menu_longClick_3 oled_menu_longClick_3_list [] = {};
#endif  
#ifdef FUNC_ITEM_3
PROGMEM oled_menu_longClick_4 oled_menu_longClick_4_list [] = {};
#endif  

void oled_init() {
    #ifdef ADAFRUIT_SD1306_LIB
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
            Serial.println(F("SSD1306 allocation failed"));
            for(;;); 
        }
        oled_clear();
        display.display();
    #endif 
    #ifdef SD1306WIRE_LIB
        display.init();
        display.flipScreenVertically();
        display.setFont(ArialMT_Plain_10);  
        oled_clear();
        display.display();        
    #endif   
}
void setup() {
    Serial.begin(115200);

    oled_init();

    // DECLARE LONG_CLICK LIST SIZE
    oled_menu_longClick_1_count = ARRAY_SIZE(oled_menu_longClick_1_list);
    oled_menu_longClick_2_count = ARRAY_SIZE(oled_menu_longClick_2_list);
#ifdef FUNC_ITEM_3
    oled_menu_longClick_3_count = ARRAY_SIZE(oled_menu_longClick_3_list);
#endif  
#ifdef FUNC_ITEM_3
    oled_menu_longClick_4_count = ARRAY_SIZE(oled_menu_longClick_4_list);
#endif  

    // CREATE MENU OJECT
    oled_menu_home              		= new oled_menu_create("home");
    oled_menu_home_lighting     		= new oled_menu_create("home_lighting");
    oled_menu_lighting_livingRoom 		= new oled_menu_create("livingRoom");
    oled_menu_livingRoom_lighting 		= new oled_menu_create("livingRoom_lighting");
    oled_menu_pool						= new oled_menu_create("piscine");
    oled_menu_pool_relay				= new oled_menu_create("piscine_relay");

    // CREATE MENU ITEMS
    oled_menu_create_items_v2(list_m1, ARRAY_SIZE(list_m1), oled_menu_home);
    oled_menu_create_items_v2(list_m2, ARRAY_SIZE(list_m2), oled_menu_home_lighting);
    oled_menu_create_items_v2(list_m3, ARRAY_SIZE(list_m3), oled_menu_lighting_livingRoom);
    oled_menu_create_items_v2(list_m4, ARRAY_SIZE(list_m4), oled_menu_pool);
    oled_menu_create_items_v2(list_m5, ARRAY_SIZE(list_m5), oled_menu_livingRoom_lighting);
    oled_menu_create_items_v2(list_m5, ARRAY_SIZE(list_m5), oled_menu_pool_relay);


    for( int i = 0; i < oled_menu_array_nbr; i++) {
        oled_menu_create_print(oled_menu_array[i]); // SEE SI TOUT EST BIEN A SA PLACE

        oled_menu_array[i]->clickmoveUp     = true; // REMPLACER L'ACTION SIMPL CLICK PAR L'ACTION DB CLICK
        oled_menu_array[i]->clickmoveDown   = true; // REMPLACER L'ACTION SIMPL CLICK PAR L'ACTION DB CLICK
        oled_menu_array[i]->clickmoveFunc   = 1;    // CHOIX DE LA FONCTION DB CLICK (0 = FUNC1 - 1 = FUNC2)

        oled_menu_array[i]->cursor          = 0; 	// DEFAULT VALUE
        oled_menu_array[i]->startItem       = 0;	// DEFAULT VALUE
        oled_menu_array[i]->itemsCnt        = 0; 	// DEFAULT VALUE

        oled_menu_array[i]->item_perPage    = 3;	// items per menu

        oled_menu_array[i]->i_yMenu         = 20;	// Strating top item
        oled_menu_array[i]->i_xTitle        = 10;	// starting left menu title
        oled_menu_array[i]->i_yTitle        = 0;    // strating top title        
        oled_menu_array[i]->i_xSubTitle     = 12;	// strating left subtitle
    }

    // DEFINE MAIN MENU
    oled_menu_set_main(oled_menu_home);

    // INITIALISATION DES MENU DONE !

    // Display MAIN MENU
    /*
        oled_display_mod
                        oled_display_off    : le menu est effacer
                        oled_display_menu   : le menu est actif
    */
	oled_display_mod    = 	oled_display_menu; 
	oled_menu_current 	-> 	init_menu();

}


// MAIN LOOP
void keyboard_loop();
void loop() {
    keyboard_loop(); // SIMULATION DES BOUTON POUSSOIR POUR LES TEST
}


String user_menu_info(){
    String s;
    s += "r = ESP reset\n";
    s += "1 = next item\n";     		// bp1
    s += "3 = back item\n";     		// bp2
    s += "4 = db click f1\n";   		// bp1
    s += "5 = db click f2\n";   		// bp2
    s += "2 = long click f1\n"; 		// bp1
    s += "5 = long click f2\n"; 		// bp2
    s += "a = arborescence_menu\n"; 	//
    s += "z = arborescence_item\n"; 	//
    s += "e = printCurrentItemPos\n"; 	//
    s = "\n[MENU]\n" + s + "\n";
    Serial.printf("%s", s.c_str());
    return s;
}
String user_menu(char c) {
    String s = "";
    String p = "";
    switch (c) {
        case 'm' : s= "menu";           user_menu_info();               break;
        case 'r' : s= "ESP reset";      ESP.reset();                    break;
        case '1' : s= "1";              oled_menu_click_move(true);     break;
        case '3' : s= "3";              oled_menu_click_move(false);    break;
        case '4' : s= "4";              oled_menu_click_1_set(oled_menu_move_next);        break;
        case '6' : s= "6";              oled_menu_click_2_set(oled_menu_move_none);        break;
        case '2' : s= "2";              oled_menu_long_click_1(oled_menu_move_back);       break;
        case '5' : s= "5";              oled_menu_long_click_2(oled_menu_move_none);       break;
        case 'a' : s= "a";              p = oled_menu_arborescence_print(0); Serial.printf("\n[arborescence menu] : %s\n", p.c_str());		break;
        case 'z' : s= "z";              p = oled_menu_arborescence_print(1); Serial.printf("\n[arborescence items] : %s\n", p.c_str());		break;
        case 'e' : s= "e";              p = oled_menu_current->printCurrentItemPos(); Serial.printf("\n[item pos/style]\n%s\n", p.c_str());	break;
     
    }
    return s;
}
void keyboard_loop() {
    if (Serial.available()) {
        char c=Serial.read();
        user_menu(c);
    }
}
