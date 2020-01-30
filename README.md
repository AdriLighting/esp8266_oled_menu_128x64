# oled_menu_128x64
 
little libraires for esp8226.

create multiple menu and sebMenu for the oled screen 128x64 monochrome.

Push button management for navigation and running functions.



	24/01/2020 15:03:48 : création d'une librairie avec mes fonction en vrac pour un ecran oled

	MICRO LIBRAIRIE POUR LA CREATION DE MENU POUR L'ECRAN OLED 128x64 MONOCHROME
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
			pour naviguer entre diffrents menu il faudra passer par une fonction attribuer a un item, qui permettera d'afficher le menu voullu.

			class oled_menu_create {
				public:
					oled_menu_def contents[OLED_MENU_ITEMS_MAX]; 	: Struct qui contiendra les items du menu

				public:
					int 				size; 						: nbr d'items du menu
					char * 				name;						: nom du menu
					boolean 			clickmoveUp; 				: par default si un bp et attribuer a la fonction oled_menu_click_move(direction) alor le simple click servira a monter ou descendre
																		si vous avez attribuer un bp a cette fonction il est possible de passer clickmoveUp ou clickmoveDown en false pour que le simple click
																		lance la fonction de la struct oled_menu_def
					boolean 			clickmoveDown;
					int 				clickmoveFunc;				: si clickmoveUp ou clickmoveUp est false clickmoveFunc definira quelle fonction de la strcut oled_menu_def, sera executer

					String lastSelectedItem;						: memorise le nom du dernier item selectionner
					boolean printMove = false; 						: sert a identifier si la fonction utiliser sert a ceer un menu

					int cursor 			= 0;						: position du curseur
					int itemsCnt 		= 0;						: incrementation pour definir la postion du curseur et des items
					int startItem		= 0;						: incrementation pour definir la position de depart des items
					int item_perPage	= 0;						: nombre maximum d'item par ecran

					int i_yMenu			= 0;						: postion top du menu
					int i_xSubTitle 	= 0;						: postion guauche du sous-titre 
					int i_xTitle 		= 0;						: postion guauche du titre 
					int i_yTitle 		= 0;						: postion top du titre 
				void
					int 	getPos();								: return position de litem dans sa struct
					String 	getItemName();							: return le nom de litem
					String 	getMenuName();							: return le nom du menu
					void 	set_subtitle (String str);				: definit le nom du sous-titre de litem
					String 	print();								: return les item du menu
					String 	printCurrentItemPos();					: return postion dur curseur
					void 	create_menu();							: affiche le menu
					void 	init_menu();							: initialisation du menu
								
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
		
		la structure oled_menu_item permet de creer une liste d'item et de leur fonction, pour ensuite les attribuer au menu voullu avec la fonction oled_menu_create_items_v2 :
			Exemples :
				PROGMEM oled_menu_item list_m1 [] = {           
				    {"i1",  "", &func1, &func2, &func3, &func4},
				    {"i2",  "", &func1, &func2, &func3, &func4},
				};
				oled_menu_create * oled_menu_home;  
				oled_menu_home    = new oled_menu_create("oled_menu_home")
				oled_menu_create_items_v2(list_m1, ARRAY_SIZE(list_m1), oled_menu_home);  

	FoNCTION :
		void 	oled_menu_create_print(oled_menu_create * m);					: seial.print du menu
		void 	oled_menu_create_printCurrentItemPos(oled_menu_create * m);		: Serial.print position curseur e
		int 	oled_menu_arborescence_getPos();								
		String 	oled_menu_arborescence_print(int mod);							: 0 print larborescence menu - 1 print arbo items

		void 	oled_menu_init_menu (oled_menu_create * m); 					: affiche le menu

		void 	oled_menu_set_main(oled_menu_create * m); 						: definit le menu de base
		void 	oled_menu_set_lg(oled_menu_create * m);							: definit a quelle menu sera attribuer la fonction(long click)

		void oled_menu_click_1|2|3|4_set(oled_menu_move move); 					: fonction dependant de la struct oled_menu_def (voir plus haut)

		void oled_menu_long_click_1|2|3|4(oled_menu_move move); 				:

	struct oled_menu_longClick_1|2|3|4 : (a modifier par lavenir si je trouve le moyen de passer le pointeur d'un objet dans la data d'une structure)	
		la struct oled_menu_longClick_ servira a ajoute des fonction qui sera attribuer a un menu
			Exemple :
				declaration de l'array :
					void lg1(boolean exec, oled_menu_move move){
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

				application		
					if ((digitalread(bp_1, pull){while > 3000}) oled_menu_long_click_1(oled_menu_move move);

					void oled_menu_long_click_1(oled_menu_move move) { 			// longClick bp 1

						if (oled_menu_longClick_1_count <= 0) return;

						oled_menu_current->printMove = false;

						for(int i=0; i < oled_menu_longClick_1_count ; i++) {

							-> DANS UN PERIMER TEMPS ON LANCE LA FONCTION AVEC LE PARAM EXEC FALSE POUR QUE LA FONCTION INITIALISE LE MENU 
								oled_menu_longClick_1_list[i].func_1(false, move);	

							-> ENSUITE SI LE MENU INITIALISER CORRESSPON AU MENU AFFICHER ON LANCE LA FONCTION AVEC LE PARAM EXEC TRUE POUR EXECUTER LE CORE DE LA FONCTION
								if (isSelectedMenu(oled_menu_lg)) {oled_menu_longClick_1_list[i].func_1(true, move);oled_menu_arborescence_add(move);}
						}	

					}

