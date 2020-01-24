
#include "oled_display.h"

extern oled_menu_pos oled_menu_pos;

void keyboard_loop();

void setup() {
	Serial.begin(115200);

	oled_init();

    oled_menu_pos.print();
    
    oled_menu_home          	= new oled_menu_create("home");
    oled_menu_home_lighting   	= new oled_menu_create("home_lighting");

    oled_menu_create_items_home();
    oled_menu_create_items_home_lighting();

    oled_menu_home->print();
    oled_menu_home_lighting->print();

    // ativation du menu (click bp, requette, etc....)
    oled_menu_click_initMenu(oled_menu_home);
}

void loop() {
	keyboard_loop(); // for replace bp
}



String user_menu_info(){
    String s;
    s += "r = ESP reset\n";
    s += "1 = next item\n";
    s += "3 = back item\n";
    s += "4 = db click f1\n";
    s += "5 = db click f2\n";
    s += "2 = long click f1\n";
    s += "5 = long click f2\n";
    s = "\n[MENU]\n" + s + "\n";
    Serial.printf("%s\n", s.c_str());
    return s;
}

String user_menu(char c) {
    String s = "";
    int pin;
    switch (c) {
        case 'm' : s= "menu"; 			user_menu_info();   			break;
        case 'r' : s= "ESP reset";   	ESP.reset();        			break;
        case '1' : s= "1"; 				oled_menu_click_move(true); 	break;
        case '3' : s= "3"; 				oled_menu_click_move(false); 	break;
        case '4' : s= "4"; 				oled_menu_click_1_set(); 		break;
        case '6' : s= "6"; 				oled_menu_click_2_set(); 		break;
        case '2' : s= "2"; 				oled_menu_long_click_1(); 		break;
        case '5' : s= "5"; 				oled_menu_long_click_2(); 		break;
     
    }
    return s;
}

void keyboard_loop() {
    if (Serial.available()) {
        char c=Serial.read();
        user_menu(c);
    }
}
