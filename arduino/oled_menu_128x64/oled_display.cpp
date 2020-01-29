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

		
					boolean exec 			: servira uniquement pour une fonction declencher pour un LONG CLICK sur un bouton poussoir.
												(a modifier par lavenir si je trouve le moyen de passer le pointeur d'un objet dans la data d'une structure)	
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
			
	#define DEBUG

//
// CURSOR POSTION
// *****************************************************************************************************

int oled_menu_create::getPos(){
	int pos = cursor + startItem;
	return pos;
}
String oled_menu_create::getItemName(){
	int pos = getPos();
	String iName = contents[pos].name;
	return iName;
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
// CREEATION DES MENU 
// *****************************************************************************************************

// MISE EN MEMOIRE DES POINTEURS 
oled_menu_create * oled_menu_lg; 			
oled_menu_create * oled_menu_main; 			
oled_menu_create * oled_menu_old; 			
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
void oled_menu_create::print() {
	Serial.printf("\n[print menu] : %s - size:%d\n",name, size);
	for( int i = 0; i < size; i++) {
        String s= "[" + String(i) + "]";
        while (s.length()<4) s+=" ";
		Serial.printf("%s name: %s\n", s.c_str(), contents[i].name.c_str());
	}
	Serial.printf("[print menu] end\n");
}


// CREATIONS DU TITRE + SOUSTITRE + CURSEUR 
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

// CREATIONS DES ITEMS
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

// CREATIONS DU MENU (SERVIRA AUSSI POUR LA NAVIGATION PAR CLICK)
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

// INITIALISATION DU MENU QUAND IL SERA CHARGER
void oled_menu_create::init_menu(){
	oled_menu_old 			= oled_menu_current;
	lastSelectedItem 		= getItemName();
	oled_menu_current 		= this;
	create_menu();
}
void oled_menu_init_menu (oled_menu_create * m) {
	m->init_menu();
}
void oled_menu_create::set_subtitle (String str) {
	int pos = getPos();
	contents[pos].subTitle = str;
}

//
// ADDING ITEMS WITH oled_menu_item class
// *****************************************************************************************************
void oled_menu_create_items_v2(oled_menu_item list[], int size, oled_menu_create * m){
	char buffer[80];
	for(int i=0; i < size ; i++) {
		sprintf(buffer, "%s", list[i].name );
		m->contents[i].name 	= String(buffer); 
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
void print_move ( oled_menu_move move ) ;
void oled_menu_arborescence_add(oled_menu_move move) {
	if ((move == oled_menu_move_next) || (move == oled_menu_move_back)) print_move ( move ) ;
}


	void oled_menu_set_main(oled_menu_create * m) { oled_menu_main = m; oled_menu_current = m;}
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
		int pos = oled_menu_current->getPos();
		oled_menu_current->contents[pos].func_1(move);
		oled_menu_arborescence_add(move);
	}
	void oled_menu_click_2_set(oled_menu_move move){ 				// db click bp 2
		#ifdef DEBUG
		Serial.printf("\n[oled_menu_click_2_set]\n");
		#endif		
		if (oled_display_mod != oled_display_menu) return;
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

	for(int i=0; i < oled_menu_longClick_1_count ; i++) {
		oled_menu_longClick_1_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) {oled_menu_longClick_1_list[i].func_1(true, move);oled_menu_arborescence_add(move);}
	}	
}

int oled_menu_longClick_2_count = 0;
void oled_menu_long_click_2(oled_menu_move move) { 			// longClick bp 2
	if (oled_menu_longClick_2_count <= 0) return;

	for(int i=0; i < oled_menu_longClick_2_count ; i++) {
		oled_menu_longClick_2_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) {oled_menu_longClick_2_list[i].func_1(true, move);oled_menu_arborescence_add(move);}
	}	
}
#ifdef FUNC_ITEM_3
int oled_menu_longClick_3_count = 0;
void oled_menu_long_click_3(oled_menu_move move) { 			// longClick bp 3
	if (oled_menu_longClick_3_count <= 0) return;

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

	for(int i=0; i < oled_menu_longClick_4_count ; i++) {
		oled_menu_longClick_4_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) {oled_menu_longClick_4_list[i].func_1(true, move);oled_menu_arborescence_add(move);}
	}	
}
#endif


//
// CREATION DES ITEMS + FONCTION ATTRIBUER AUX ITEMS 
// *****************************************************************************************************

void om_tf(oled_menu_move move){
	int pos = oled_menu_current->getPos();
	String mName = oled_menu_current->name;
	String iName = oled_menu_current->contents[pos].name;
	Serial.printf("[func] menu: %s - item: %s - item pos: %d\n", mName.c_str(), iName.c_str(), pos );
}

		

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



 