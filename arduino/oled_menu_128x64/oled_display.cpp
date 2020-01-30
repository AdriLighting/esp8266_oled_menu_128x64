/*
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
*/

#include "oled_display.h"

#ifdef ADAFRUIT_SD1306_LIB
    #include <SPI.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
    Adafruit_SSD1306 display(128, 64, &Wire, -1);
#endif
#ifdef SD1306WIRE_LIB
    #include <Wire.h>               		// Only needed for Arduino 1.6.5 and earlier
    #include "SSD1306Wire.h"        		// legacy: #include "SSD1306.h"
    SSD1306Wire display(0x3c, SDA, SCL);
#endif

oled_display_mods   oled_display_mod;
oled_menu_position  oled_menu_getPos;

String oled_menu_starterItem;
int oled_menu_pos;
			
// #define DEBUG

String om_info_parm(String name, String value , String sep, int len, String last, String tdb1, String tdb2) {
   String s=name;
   while (s.length()<len) s+=" ";
   return tdb1+s+sep+tdb2+value+last;
}
String om_c_to_string(char * name){
	char buffer[80];
	sprintf(buffer, "%s", name);
	return String(buffer);
}

//
// CURSOR POSTION
// *****************************************************************************************************

int oled_menu_create::getPos(){
	int pos = cursor + startItem;
	return pos;
}
void oled_menu_create::setPos(boolean up, int size) {
	int max;
	String iName = getItemName();	

	#ifdef DEBUG
		Serial.printf("\n[oled_menu_setPos] menu: %s - old item: %s - old currPos: %d\n", name, iName.c_str(), cursor);
	#endif
	if (up) {
        cursor++;
        itemsCnt++;
        if (cursor > item_perPage) {
        	startItem 	+= 	(item_perPage)+1 ;
        	cursor 	= 	0;
        }
        max = size-item_perPage;
        #ifdef DEBUG
			Serial.printf("cursor: %d - itemsCnt: %d - startItem: %d - max: %d size: %d\n",cursor,itemsCnt,startItem,max,size );
        #endif
        if (itemsCnt > size-1) {
        	#ifdef DEBUG
        		Serial.printf("itemsCnt>size-1: new screen\n");
        	#endif        	
        	startItem 	= 0;
        	itemsCnt 	= 0;
        	cursor 	= 0;
        }
    } else {
        cursor--;
        itemsCnt--;
        if (cursor < 0) {
        	startItem 	-= 	(item_perPage+1);
        	cursor 	= 	item_perPage;
        }
        max = size-item_perPage;
        #ifdef DEBUG
			Serial.printf("cursor: %d - itemsCnt: %d - startItem: %d - max: %d size: %d\n",cursor,itemsCnt,startItem,max,size );
        #endif      
        if (itemsCnt < 0) {
        	#ifdef DEBUG
        		Serial.printf("itemsCnt<0: new screen\n");
        	#endif
			itemsCnt 	= size-1;
        	if ((max-1) < 0) 	startItem 	= 0;
        	else 				startItem 	= max-1;
        	if ((max-1) < 0) 	cursor 	= size-1;
        	else 				cursor 	= (size-max);
        } else {
	        if (startItem < 0) {
	        	#ifdef DEBUG
	        		Serial.printf("startItem<0: new screen\n");
	        	#endif	        	
	        	startItem 	= 0;
	        	cursor	= itemsCnt;  
	        }           	
        }       
    }
	#ifdef DEBUG
    	int pos = cursor + startItem;
		iName = contents[pos].name;	
		Serial.printf("new item: %s\n", iName.c_str());

		Serial.printf("[oled_menu_setPos] - end\n");
	#endif
   
}


//
// MENU AND ITEM NAME
// *****************************************************************************************************

String oled_menu_create::getItemName(){
	int pos = getPos();
	return contents[pos].name;
}
String oled_menu_create::getMenuName(){
	return om_c_to_string(name);
}

//
// MENU AND ITEM ARBORESCENCE
// *****************************************************************************************************

String arborescence_menu[12];
String arborescence_item[12];

void print_move ( oled_menu_move move ) {
    if (oled_menu_current == oled_menu_main)   {
        oled_menu_pos = 0;
        arborescence_menu[oled_menu_pos] = om_c_to_string(oled_menu_current->name);
        arborescence_item[oled_menu_pos] = oled_menu_current->getItemName();        
    }
    else {
        
        if(move == oled_menu_move_back) {oled_menu_pos--;}
        if(move == oled_menu_move_next) {
            oled_menu_pos++;
            arborescence_item[oled_menu_pos-1] = oled_menu_current->lastSelectedItem;
            arborescence_menu[oled_menu_pos] = om_c_to_string(oled_menu_current->name);
        }
    }
}
int oled_menu_arborescence_getPos(){
    return oled_menu_pos;
}
String oled_menu_arborescence_print(int mod){
    String arbo;
    switch (mod) {
        case 0:
            for( int i = 0; i < oled_menu_pos; i++) { arbo += String(i) + " : " + arborescence_menu[i] + " - "; }
        break;
        case 1:
            for( int i = 0; i < oled_menu_pos; i++) { arbo += String(i) + " : " + arborescence_item[i] + " - "; }
        break;
    }
    return arbo;
}
void oled_menu_arborescence_add(oled_menu_move move) {
	if (!oled_menu_current->printMove) return;
	oled_menu_current->printMove = false;
	if ((move == oled_menu_move_next) || (move == oled_menu_move_back)) print_move ( move ) ;
}


//
// CREEATION DES MENU + ITEMS
// *****************************************************************************************************

// MISE EN MEMOIRE DES POINTEURS 
oled_menu_create * oled_menu_lg; 			
oled_menu_create * oled_menu_main; 			
oled_menu_create * oled_menu_current; 			
oled_menu_create * oled_menu_array[OLED_MENU_ITEMS_MAX];
int oled_menu_array_nbr = 0;

oled_menu_create::oled_menu_create(char * sName) {
	if (oled_menu_array_nbr < OLED_MENU_ITEMS_MAX) {
		name = sName;
		oled_menu_array[oled_menu_array_nbr++] = this;
	} else Serial.printf("[oled_menu_create] FUUUUUUUUUUULLLLLLLL\n");
}

// POUR DEBUG
String oled_menu_create::print() {
	String ret;
	for( int i = 0; i < size; i++) {
        String s= "[" + String(i) + "]";
        while (s.length()<4) s+=" ";
		ret += s + "name: " + contents[i].name + "\n";
	}
	return ret;
}
void oled_menu_create_print(oled_menu_create * m) {
	String s;
	s = m->print();
	Serial.printf("\n[print menu] : %s - size:%d\n%s",m->name, m->size, s.c_str());
}
String oled_menu_create::printCurrentItemPos(){
	String s;
	s += om_info_parm("itemName", 		getItemName(), 			":", 22, "\n", "", " ");
	s += om_info_parm("cursor", 		String(cursor), 		":", 22, "\n", "", " ");
	s += om_info_parm("itemsCnt",		String(itemsCnt), 		":", 22, "\n", "", " ");
	s += om_info_parm("startItem", 		String(startItem), 		":", 22, "\n", "", " ");
	s += om_info_parm("item_perPage",	String(item_perPage), 	":", 22, "\n", "", " ");
	s += "\n";
	s += om_info_parm("i_yMenu",		String(i_yMenu), 		":", 22, "\n", "", " ");
	s += om_info_parm("i_xSubTitle",	String(i_xSubTitle), 	":", 22, "\n", "", " ");
	s += om_info_parm("i_xTitle",		String(i_xTitle), 		":", 22, "\n", "", " ");
	s += om_info_parm("i_yTitle",		String(i_yTitle), 		":", 22, "\n", "", " ");
	return s;	
}
void oled_menu_create_printCurrentItemPos(oled_menu_create * m) {
	String s;
	s = m->printCurrentItemPos();
	Serial.printf("\n[printCurrentItemPos] menu: %s - size:%d\n%s", m->name, m->size, s.c_str());
}

// CREATIONS DE l'INTERFACE : TITRE + SOUSTITRE + CURSEUR 
#ifdef ADAFRUIT_SD1306_LIB
void oled_menu_create::create_menu_header(char * title, int currPos, int starMenu, int starHeader, int titleLeft, int subTitleLeft) {
	int pos = getPos();
	String subTitle = contents[pos].subTitle;	
	oled_clear();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(titleLeft, starHeader);
	display.println(title);
	if (starMenu>=20) {
		if (subTitle != "") {
			display.setCursor(subTitleLeft, starHeader+10);
			display.println(subTitle);		
		}
	}
	if (currPos == 0) currPos = starMenu;
	else currPos = (currPos*10) + starMenu; 

	display.setCursor(0, currPos);
	display.println(">"); 

}
#endif
#ifdef SD1306WIRE_LIB
void oled_menu_create::create_menu_header(char * title, int currPos, int starMenu, int starHeader, int titleLeft, int subTitleLeft) {
	int pos = getPos();
	String subTitle = contents[pos].subTitle;	
	oled_clear();
	display.setFont(Dialog_plain_11);
	display.drawString(titleLeft, starHeader, title);
	if (starMenu>=20) {
		if (subTitle != "") {
			display.drawString(subTitleLeft, starHeader+10, subTitle);
		}
	}
	if (currPos == 0) currPos = starMenu;
	else currPos = (currPos*10) + starMenu; 
	display.setFont(Dialog_plain_10);
	display.drawString(0, currPos, ">");
}
#endif

// CREATIONS DE l'INTERFACE : ITEMS
#ifdef ADAFRUIT_SD1306_LIB
void oled_menu_create::create_menu_items() {
	int start 	= i_yMenu;
	int cnt 	= 0;
	int maxCnt 	= item_perPage;
	for(int i=startItem; i < size ; i++) {
		display.setCursor(10, start);
		display.println(contents[i].name);
		start += 10;
		cnt++;
		if (cnt > (maxCnt)) break;
	}

	display.display();
}
#endif
#ifdef SD1306WIRE_LIB
void oled_menu_create::create_menu_items() {
	int start 	= i_yMenu;
	int cnt 	= 0;
	int maxCnt 	= item_perPage;
	display.setFont(Dialog_plain_11);
	for(int i=startItem; i < size ; i++) {
		display.drawString(10, start, contents[i].name);
		start += 10;
		cnt++;
		if (cnt > (maxCnt)) break;
	}

	display.display();
}
#endif

// CREATIONS DU MENU
void oled_menu_create::init_menu(){
	printMove = true;
	oled_menu_current 		= this;
	create_menu();
}
void oled_menu_init_menu (oled_menu_create * m) {
	m->lastSelectedItem = oled_menu_current->getItemName();
	m->init_menu();
}
void oled_menu_create::create_menu() {
	create_menu_header(
		name,
		cursor,
		i_yMenu,
		i_yTitle, 
		i_xTitle, 
		i_xSubTitle
	);
	create_menu_items();
}

// ITEM SUBTITLE
void oled_menu_create::set_subtitle (String str) {
	int pos = getPos();
	contents[pos].subTitle = str;
}


//
// ADDING ITEMS WITH oled_menu_item class
// *****************************************************************************************************
void oled_menu_create_items_v2(oled_menu_item list[], int size, oled_menu_create * m){
	for(int i=0; i < size ; i++) {
		m->contents[i].name 	= om_c_to_string(list[i].name); 
		m->contents[i].func_1 	= list[i].func_1; 
		m->contents[i].func_2 	= list[i].func_2; 
		#ifdef FUNC_ITEM_3
		m->contents[i].func_3 	= list[i].func_3; 
		#endif	
		#ifdef FUNC_ITEM_4
		m->contents[i].func_4 	= list[i].func_4; 
		#endif		
	}	
	m->size 	= size;  	
}


//
// GESTION DES CLICK BOUTON POUSSOIR
// *****************************************************************************************************
boolean oled_menu_getMenu(oled_menu_position search) {
	if (oled_menu_getPos == search) return true;
	return false;
}
boolean oled_menu_getMenu(oled_menu_create * current, oled_menu_create * search) {
	if (current == search) return true;
	return false;
}
void oled_menu_click_move(boolean up, oled_menu_create * m){
	oled_menu_current->setPos(up, m->size);
	m->create_menu();	
}
void oled_menu_click_initMenu(oled_menu_create * m){
	oled_display_mod = oled_display_menu;
	m->init_menu();	
}


// POUR LA NAVIGATION
	void oled_menu_click_move(boolean up){ 		// sipml click bp 1 || bp 2
		int pos = oled_menu_current->getPos();
		if (oled_display_mod!= oled_display_menu) 	{
			if (oled_display_mod == oled_display_disp) {
				oled_display_mod = oled_display_menu;
				oled_menu_current->create_menu();
			} else oled_menu_click_initMenu(oled_menu_main); 
		} else {
			if (up) {
				if (oled_menu_current->clickmoveUp)	{oled_menu_click_move(up, oled_menu_current);}
				else {
					if (oled_menu_current->clickmoveFunc == 0) oled_menu_current->contents[pos].func_1(oled_menu_move_up);
					if (oled_menu_current->clickmoveFunc == 1) oled_menu_current->contents[pos].func_2(oled_menu_move_up);
			#ifdef FUNC_ITEM_3
					if (oled_menu_current->clickmoveFunc == 2) oled_menu_current->contents[pos].func_3(oled_menu_move_up);
			#endif	
			#ifdef FUNC_ITEM_4
					if (oled_menu_current->clickmoveFunc == 3) oled_menu_current->contents[pos].func_4(oled_menu_move_up);
			#endif
				}				
			} 
			if (!up) {
				if (oled_menu_current->clickmoveDown)	{oled_menu_click_move(up, oled_menu_current);}
				else {
					if (oled_menu_current->clickmoveFunc == 0) oled_menu_current->contents[pos].func_1(oled_menu_move_down);
					if (oled_menu_current->clickmoveFunc == 1) oled_menu_current->contents[pos].func_2(oled_menu_move_down);
			#ifdef FUNC_ITEM_3
					if (oled_menu_current->clickmoveFunc == 2) oled_menu_current->contents[pos].func_3(oled_menu_move_down);
			#endif	
			#ifdef FUNC_ITEM_4
					if (oled_menu_current->clickmoveFunc == 3) oled_menu_current->contents[pos].func_4(oled_menu_move_down);
			#endif					
				}
			} 
			
			
		}
	}


// POUR EFFECTUER L'ACTION ATTRIBUER A L'ITEM SELECTIONNER
	void oled_menu_click_1_set(oled_menu_move move){ 				// db click bp 1
		#ifdef DEBUG
		Serial.printf("\n[oled_menu_click_1_set]\n");
		#endif
		if (oled_display_mod != oled_display_menu) return;
		oled_menu_current->printMove = false;
		int pos = oled_menu_current->getPos();
		oled_menu_current->contents[pos].func_1(move);
		oled_menu_arborescence_add(move);
	}
	void oled_menu_click_2_set(oled_menu_move move){ 				// db click bp 2
		#ifdef DEBUG
		Serial.printf("\n[oled_menu_click_2_set]\n");
		#endif		
		if (oled_display_mod != oled_display_menu) return;
		oled_menu_current->printMove = false;
		int pos = oled_menu_current->getPos();
		oled_menu_current->contents[pos].func_2(move);
		oled_menu_arborescence_add(move);
	}
#ifdef FUNC_ITEM_3
	void oled_menu_click_3_set(oled_menu_move move){ 				// db click bp 3
		#ifdef DEBUG
		Serial.printf("\n[oled_menu_click_3_set]\n");
		#endif
		if (oled_display_mod != oled_display_menu) return;
		oled_menu_current->printMove = false;
		int pos = oled_menu_current->getPos();
		oled_menu_current->contents[pos].func_3(move);
		oled_menu_arborescence_add(move);
	}	
#endif	
#ifdef FUNC_ITEM_4
	void oled_menu_click_4_set(oled_menu_move move){ 				// db click bp 4
		#ifdef DEBUG
		Serial.printf("\n[oled_menu_click_4_set]\n");
		#endif		
		if (oled_display_mod != oled_display_menu) return;
		oled_menu_current->printMove = false;
		int pos = oled_menu_current->getPos();
		oled_menu_current->contents[pos].func_4(move);
		oled_menu_arborescence_add(move);
	}
#endif


// POUR EFFECTUER L'ACTION ATTRIBUER AU MENU SLECTIONNER
bool isSelectedMenu(oled_menu_create * m){
	if (oled_menu_getMenu(oled_menu_current, m)) return true;
	return false;
}	
void oled_menu_set_lg(oled_menu_create * m) { oled_menu_lg = m; }

int oled_menu_longClick_1_count = 0;
void oled_menu_long_click_1(oled_menu_move move) { 			// longClick bp 1
	if (oled_menu_longClick_1_count <= 0) return;
	oled_menu_current->printMove = false;
	for(int i=0; i < oled_menu_longClick_1_count ; i++) {
		oled_menu_longClick_1_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) {oled_menu_longClick_1_list[i].func_1(true, move);oled_menu_arborescence_add(move);}
	}	
}

int oled_menu_longClick_2_count = 0;
void oled_menu_long_click_2(oled_menu_move move) { 			// longClick bp 2
	if (oled_menu_longClick_2_count <= 0) return;
	oled_menu_current->printMove = false;
	for(int i=0; i < oled_menu_longClick_2_count ; i++) {
		oled_menu_longClick_2_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) {oled_menu_longClick_2_list[i].func_1(true, move);oled_menu_arborescence_add(move);}
	}	
}
#ifdef FUNC_ITEM_3
int oled_menu_longClick_3_count = 0;
void oled_menu_long_click_3(oled_menu_move move) { 			// longClick bp 3
	if (oled_menu_longClick_3_count <= 0) return;
	oled_menu_current->printMove = false;
	for(int i=0; i < oled_menu_longClick_3_count ; i++) {
		oled_menu_longClick_3_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) {oled_menu_longClick_3_list[i].func_1(true, move);oled_menu_arborescence_add(move);}
	}	
}
#endif	
#ifdef FUNC_ITEM_4
int oled_menu_longClick_4_count = 0;
void oled_menu_long_click_4(oled_menu_move move) { 			// longClick bp 4
	if (oled_menu_longClick_4_count <= 0) return;
	oled_menu_current->printMove = false;
	for(int i=0; i < oled_menu_longClick_4_count ; i++) {
		oled_menu_longClick_4_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) {oled_menu_longClick_4_list[i].func_1(true, move);oled_menu_arborescence_add(move);}
	}	
}
#endif


//
// DIVERS
// *****************************************************************************************************

// SET MAIN MENU
void oled_menu_set_main(oled_menu_create * m) {
	oled_menu_main 			= m;
	oled_menu_current 		= m;
    arborescence_menu[0] 	= om_c_to_string(m->name);
    arborescence_item[0] 	= m->getItemName();  		
}


//
void om_tf(oled_menu_move move){
	int pos = oled_menu_current->getPos();
	String mName = oled_menu_current->name;
	String iName = oled_menu_current->contents[pos].name;
	Serial.printf("\n[func] menu: %s - item: %s - item pos: %d\n", mName.c_str(), iName.c_str(), pos );
}

		
//
void oled_clear() {
	#ifdef ADAFRUIT_SD1306_LIB
	    display.clearDisplay();
	#endif
	#ifdef SD1306WIRE_LIB
	    display.clear();
	#endif    
}
#ifdef SD1306WIRE_LIB
	void oled_draw_bmp(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const uint8_t bitmap[]){
		 // display.drawXbm(xMove, yMove, width, height, xbm);   
		 display.drawBitmap  (xMove, yMove, bitmap, width, height, 1);  
	}
#endif  
#ifdef ADAFRUIT_SD1306_LIB
	void oled_draw_bmp(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t bitmap[]){
		 display.drawBitmap  (x, y, bitmap, w, h, 1);  
	}
#endif



 