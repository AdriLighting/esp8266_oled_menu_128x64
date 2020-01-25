#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// MEM OLED SCREEN
Adafruit_SSD1306 display(128, 64, &Wire, -1);


/*
    24/01/2020 15:03:48 : mise en udf de mes fonction en vrac pour un ecran oled

    MICRO UDF POUR CREATION DE MENU POUR L'ECRAN OLED 128x64 MONOCHROME
    AUTHOR : ADRILIGHTING
    COMPATIBLE BOARD            : ESP8266 > 2.5
    EXEMPLE                     : oled_menu_128x64.ino
    LIBRAIRIE EXTERNE UTILISER  :   
                                    <SPI.h>                 : FROM ESP8266 2.6.3
                                    <Wire.h>                : FROM ESP8266 2.6.3
        Adafruit-GFX-Library-1.5.6  <Adafruit_GFX.h>        : https://github.com/adafruit/Adafruit-GFX-Library
        Adafruit_SSD1306-master     <Adafruit_SSD1306.h>    : https://github.com/adafruit/Adafruit_SSD1306  
    
    AFAIRE :
        FONCTION POUR MEMORISER LE DERNIER MENU + CURSEUR
            Permettra d'effacer le menu pour afficher d'autre donnée puis de réafficher le menu sans devoir tout réinitialiser
        FONCTION POUR AFFICHER DES DONNEES EXTERN (bitmap, statu serveur, etc...)
*/

#include "oled_display.h"
#include "bmp.h"

extern oled_menu_pos oled_menu_pos;



// USER DECLAR OWN MENU
oled_menu_create * oled_menu_home;              // demo menu 1
oled_menu_create * oled_menu_home_lighting;     // demo menu 2

//USER DECLAR MENU ITEM
void m1_i1_m(){oled_menu_init_menu (oled_menu_home_lighting);}
PROGMEM oled_menu_item list_m1 [] = {           // liste des fonction autribuer aux items
//  TITRE           Fonction db click bp 1  Fonction db click bp 2
    {"lumierre",    &m1_i1_m,               &test_f2},
    {"temperature", &test_f1,               &test_f2},
    {"piscine",     &test_f1,               &test_f2},

};
PROGMEM oled_menu_item list_m2 [] = { 
//  TITRE           Fonction db click bp 1  Fonction db click bp 2    
    {"cuisine",     &test_f1,               &test_f2},
    {"salon",       &test_f1,               &test_f2},
    {"terasse",     &test_f1,               &test_f2},
    {"chambre_1",   &test_f1,               &test_f2},
    {"chambre_2",   &test_f1,               &test_f2},
    {"chambre_3",   &test_f1,               &test_f2},
    {"chambre_4",   &test_f1,               &test_f2},
    {"chambre_5",   &test_f1,               &test_f2},

};

/*
    EXEMPLES DE DIFFERENTES FONCTION POUR CREER DES MENUS  (WS2812B_2_RINGS.ino)

    void oled_menu_create_items_v3(){
        char buff[80];
        for (int i = 0; i < effect_listCount; ++i) {
            sprintf(buff, "%s", effect_list[i].name);
            oled_menu_effect_list->contents[i].name = String(buff); 
            oled_menu_effect_list->contents[i].func_1 = &oled_menu_click_effectList; 
            oled_menu_effect_list->contents[i].func_2 = &test_f2; 
        } 
        oled_menu_effect_list->size             = effect_listCount;
    }

    // CREATIONS MENU GENERALISER 
    template <typename Callable>
    void oled_menu_create_items(Callable f_1, Callable f_2, oled_menu_create * str) {
        str->contents[0].name       = "center"; 
        str->contents[0].func_1     = f_1; 
        str->contents[0].func_2     = f_2; 
        str->contents[1].name       = "out"; 
        str->contents[1].func_1     = f_1; 
        str->contents[1].func_2     = f_2; 
        str->size                   = 2;  
    }
    void oled_menu_create_items_v4() {
        oled_menu_create_items(&oled_menu_click_home_effectList,    &test_f2,   oled_menu_home_effect);
        oled_menu_create_items(&oled_menu_click_home_color,         &test_f2,   oled_menu_home_color);
        oled_menu_create_items(&oled_menu_click_home_off,           &test_f2,   oled_menu_home_off);
    }
*/

// USER DECLAR OWN OPTIONS
//                                  attribution du menu         nom du sous-titre
void opt1(){ oled_menu_setOption(   oled_menu_home ,            "s-titre"); }
void opt2(){ oled_menu_setOption(   oled_menu_home_lighting ,   "sous-titre"); }
PROGMEM oled_menu_options oled_menu_options_list [] = { 
    { &opt1},
    { &opt2},
};


/*
    ACTION QUAND UN LONG CLICK EST EFFECTUER (atrribuer au menu afficher) :

    en attendant de trouver comment passer le pointeur d'une classe dans une structure
        (voir.cpp)
        dans un premier temp j'assigne le menu sélectionner avec le pointeur : oled_menu_lg
        puis on exécute les actions voulues
            C'EST PAS GOOD
*/
void lg1(boolean exec){
    if (!exec) {
        oled_menu_set_lg(oled_menu_home); 
        return;
    }
    oled_display_mod = oled_display_off;
    display.clearDisplay();
    display.drawBitmap  (0, 0, logo_bmp, 128, 64, 1);
    display.display();   
}   
void lg2(boolean exec){
    if (!exec) {
        oled_menu_set_lg(oled_menu_home_lighting); 
        return;
    }
    oled_menu_home->init_menu();   
}  
PROGMEM oled_menu_longClick_1 oled_menu_longClick_1_list [] = { 
    { &lg1},
    { &lg2},
    // { &lg2, oled_menu_home}, POUET Pouet
};
PROGMEM oled_menu_longClick_2 oled_menu_longClick_2_list [] = { 

};


void oled_init() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); 
    }
    display.clearDisplay();
    display.display();
}
void setup() {
    Serial.begin(115200);

    oled_init();

    // DEFINE LONG_CLICK LIST SIZE
    oled_menu_longClick_1_count = ARRAY_SIZE(oled_menu_longClick_1_list);
    oled_menu_longClick_2_count = ARRAY_SIZE(oled_menu_longClick_2_list);

    // DEFINE OPTIONLIST SIZE
    oled_menu_optCount = ARRAY_SIZE(oled_menu_options_list);

    // USER CREATE OWN MENU OJECT
    oled_menu_home              = new oled_menu_create("home");
    oled_menu_home_lighting     = new oled_menu_create("home_lighting");

    // USER CREATE OWN MENU ITEMS
    oled_menu_create_items_v2(list_m1, ARRAY_SIZE(list_m1), oled_menu_home);
    oled_menu_create_items_v2(list_m2, ARRAY_SIZE(list_m2), oled_menu_home_lighting);
        // oled_menu_create_items_v3() 
        // oled_menu_create_items_v4() 

    for( int i = 0; i < oled_menu_array_nbr; i++) {
        oled_menu_array[i]->print();                // SEE SI TOUT EST BIEN A SA PLACE
        oled_menu_array[i]->clickmoveUp     = true; // REMPLACER L'ACTION SIMPL CLICK PAR L'ACTION DB CLICK
        oled_menu_array[i]->clickmoveDown   = true; // REMPLACER L'ACTION SIMPL CLICK PAR L'ACTION DB CLICK
        oled_menu_array[i]->clickmoveFunc   = 1;    // CHOIX DE LA FONCTION DB CLICK (0 = FUNC1 - 1 = FUNC2)
    }


    // DEFINE MAIN MENU
    oled_menu_set_main(oled_menu_home);


    // ativation du menu (click bp, requette, etc....)
    // oled_menu_click_initMenu(oled_menu_home);
    //                  ||
    /*
        oled_display_mod
                        oled_display_off    : le menu est effacer
                        oled_display_menu   : le menu est actif
    */
    oled_display_mod    = oled_display_off;  

    // INITIALISATION DES MENU DONE !


    // DISPLAY BMP NOOBPI FOR FUN
    display.clearDisplay();
    display.drawBitmap  (0, 0, logo_bmp, 128, 64, 1); 
    display.display();

}


// MAIN LOOP
void keyboard_loop();
void loop() {
    keyboard_loop(); // SIMULATION DES BOUTON POUSSOIR POUR LES TEST
}


String user_menu_info(){
    String s;
    s += "r = ESP reset\n";
    s += "1 = next item\n";     // bp1
    s += "3 = back item\n";     // bp2
    s += "4 = db click f1\n";   // bp1
    s += "5 = db click f2\n";   // bp2
    s += "2 = long click f1\n"; // bp1
    s += "5 = long click f2\n"; // bp2
    s = "\n[MENU]\n" + s + "\n";
    Serial.printf("%s\n", s.c_str());
    return s;
}
String user_menu(char c) {
    String s = "";
    switch (c) {
        case 'm' : s= "menu";           user_menu_info();               break;
        case 'r' : s= "ESP reset";      ESP.reset();                    break;
        case '1' : s= "1";              oled_menu_click_move(true);     break;
        case '3' : s= "3";              oled_menu_click_move(false);    break;
        case '4' : s= "4";              oled_menu_click_1_set();        break;
        case '6' : s= "6";              oled_menu_click_2_set();        break;
        case '2' : s= "2";              oled_menu_long_click_1();       break;
        case '5' : s= "5";              oled_menu_long_click_2();       break;
     
    }
    return s;
}
void keyboard_loop() {
    if (Serial.available()) {
        char c=Serial.read();
        user_menu(c);
    }
}
