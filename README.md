# oled_menu_128x64
 
little libraires for esp8226.

create multiple menu and sebMenu for the oled screen 128x64 monochrome.

Push button management for navigation and running functions.
/*
	24/01/2020 15:03:48 : mise en forme de mes fonction en vrac pour un ecran oled

	MICRO LIBRAIRIE POUR CREATION DE MENU POUR L'ECRAN OLED 128x64 MONOCHROME
	AUTEUR 						: ADRILIGHTING
	COMPATIBLE BOARD 			: ESP8266 > 2.5
	EXEMPLE 					: oled_menu_128x64.ino
	LIBRAIRIE EXTERNE UTILISER 	: 	
										<SPI.h>					: FROM ESP8266 2.6.3
										<Wire.h>				: FROM ESP8266 2.6.3
		ADAFRUIT LIB
		Adafruit-GFX-Library-1.5.6 		<Adafruit_GFX.h>		: https://github.com/adafruit/Adafruit-GFX-Library
		Adafruit_SSD1306-master 		<Adafruit_SSD1306.h>	: https://github.com/adafruit/Adafruit_SSD1306	

		ThingPulse OLED SSD1306 (ESP8266/ESP32/Mbed-OS) 
		esp8266-oled-ssd1306-master 	"SSD1306Wire.h" 		: https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/SSD1306Wire.h
	
	DOC :
	MENU :
		chaque menu est a jouter par un objet de la class oled_menu_create.
			les menu sont tous independant et ne comporte pas de sous menu.
			chaque menu contient ses propre items ainsi que leur parametres de poisition et de style.
			pour naviguer entre diffrents menu il faudra passer par une fonction attribuer a un item qui permettera d'afficher le menu voullu.
	ITEMS :
		pours attribhuer des items a un menu il faudra passer par la structure oled_menu_def faisant partie de la class oled_menu_create:
			la structure oled_menu_def comporte :
				la limite est des 25 items par menu mais peut etre modifier suivant la memoire flash dont vous disposer.
				String name; 							= le nom sous lequell sera afficher l'item.
				String subTitle; 						= le sous titre attribuer a l'item quand celui-ci sera selectionner.
				void (*func_1)(oled_menu_move move); 	= fonction atribuer a l'item selectionner
				void (*func_2)(oled_menu_move move);	 	
				void (*func_3)(oled_menu_move move); 	
				void (*func_4)(oled_menu_move move); 	
					param 
						oled_menu_move move : servira a indiquer si la fonction est un mouvement d'item ou de menu, ou autre
												oled_menu_move_up,
											    oled_menu_move_down,
											    oled_menu_move_next,
											    oled_menu_move_back,
											    oled_menu_move_none
					fonction :
						les fonctions de la structure oled_menu_def sont atribuer par default a un SIMPLE CLIQUE d'un bouton poussoir
							Exemples:
								if ((digitalread(bp_1, pull)) oled_menu_click_1_set(oled_menu_move move);
								if ((digitalread(bp_2, pull)) oled_menu_click_2_set(oled_menu_move move);
								if ((digitalread(bp_3, pull)) oled_menu_click_3_set(oled_menu_move move);
								if ((digitalread(bp_4, pull)) oled_menu_click_4_set(oled_menu_move move);
				Exemples:
					obj_menu->struct_items[nbr].name 			= myItem.
					obj_menu->struct_items[nbr].subTitle 		= myItemSubtitle.
					obj_menu->struct_items[nbr].func_1|2|3|4 	= myItemfunc_1|2|3|4 
		
		la structure oled_menu_item permet de creer un liste d'item et de leur fonction, pour ensuite les attribuer au menu voullu avec la fonction oled_menu_create_items_v2 :
			Exemples :
				PROGMEM oled_menu_item list_m1 [] = {           
				    {"i1",  "", &func1, &func2, &func3, &func4},
				    {"i2",  "", &func1, &func2, &func3, &func4},
				};
				oled_menu_create * oled_menu_home;  
				oled_menu_home    = new oled_menu_create("oled_menu_home")
				oled_menu_create_items_v2(list_m1, ARRAY_SIZE(list_m1), oled_menu_home);  

		

*/
