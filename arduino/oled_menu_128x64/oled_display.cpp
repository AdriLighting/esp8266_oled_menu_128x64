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
	
*/


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#include "oled_def.h"    
#ifdef ADAFRUIT_SD1306_LIB
	#include <Adafruit_SSD1306.h>
	extern Adafruit_SSD1306 display;
#endif
#ifdef SD1306WIRE_LIB
    #include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
    extern SSD1306Wire display;
#endif

#include "oled_display.h"
#include "bmp.h"

oled_display_mods   oled_display_mod;
oled_menu_position  oled_menu_getPos;

#define DEBUG

//
// NAVIGATION
// *****************************************************************************************************

oled_menu_pos oled_menu_pos;

oled_menu_pos::oled_menu_pos() {
	currseur 		= 0;
	itemsCnt 		= 0;
	startItem		= 0;
	starMenu		= 20;
	starHeader		= 0;
	titleLeft 		= 10;
	subTitleLeft 	= 12;

	item_perPage 	= 3;	
}

String info_parm(String name, String value , String sep, int len, String last, String tdb1, String tdb2) {
   String s=name;
   while (s.length()<len) s+=" ";
   return tdb1+s+sep+tdb2+value+last;
}
void oled_menu_pos::print(){
	Serial.printf("\n[print oled_menu_pos]\n");
	String s;
	s += info_parm("currseur", 		String(currseur), 		":", 22, "\n", "", " ");
	s += info_parm("itemsCnt", 		String(itemsCnt), 		":", 22, "\n", "", " ");
	s += info_parm("startItem", 	String(startItem), 		":", 22, "\n", "", " ");
	s += info_parm("starMenu", 		String(starMenu), 		":", 22, "\n", "", " ");
	s += info_parm("starHeader",	String(starHeader), 	":", 22, "\n", "", " ");
	s += info_parm("item_perPage", 	String(item_perPage), 	":", 22, "\n", "", " ");
		
	Serial.printf("%s[print oled_menu_pos] : end\n", s.c_str());
}

void oled_menu_pos::setPos(boolean up, int size) {
	int max;
	int pos = getPos();
	char * mName = oled_menu_current->name;
	String iName = oled_menu_current->contents[pos].name;	
	#ifdef DEBUG
		Serial.printf("\n[oled_menu_setPos] menu: %s - old item: %s - old currPos: %d\n", mName, iName.c_str(), currseur);
	#endif
	if (up) {
        currseur++;
        itemsCnt++;
        if (currseur > item_perPage) {
        	startItem 	+= 	(item_perPage)+1 ;
        	currseur 	= 	0;
        }
        max = size-item_perPage;
        #ifdef DEBUG
			Serial.printf("currseur: %d - itemsCnt: %d - startItem: %d - max: %d size: %d\n",currseur,itemsCnt,startItem,max,size );
        #endif
        if (itemsCnt > size-1) {
        	#ifdef DEBUG
        		Serial.printf("itemsCnt>size-1: new screen\n");
        	#endif        	
        	startItem 	= 0;
        	itemsCnt 	= 0;
        	currseur 	= 0;
        }
    } else {
        currseur--;
        itemsCnt--;
        if (currseur < 0) {
        	startItem 	-= 	(item_perPage+1);
        	currseur 	= 	item_perPage;
        }
        max = size-item_perPage;
        #ifdef DEBUG
			Serial.printf("currseur: %d - itemsCnt: %d - startItem: %d - max: %d size: %d\n",currseur,itemsCnt,startItem,max,size );
        #endif      
        if (itemsCnt < 0) {
        	#ifdef DEBUG
        		Serial.printf("itemsCnt<0: new screen\n");
        	#endif
			itemsCnt 	= size-1;
        	if ((max-1) < 0) 	startItem 	= 0;
        	else 				startItem 	= max-1;
        	if ((max-1) < 0) 	currseur 	= size-1;
        	else 				currseur 	= (size-max);
        } else {
	        if (startItem < 0) {
	        	#ifdef DEBUG
	        		Serial.printf("startItem<0: new screen\n");
	        	#endif	        	
	        	startItem 	= 0;
	        	currseur	= itemsCnt;  
	        }           	
        }       
    }
	#ifdef DEBUG
    	pos = currseur + startItem;
		iName = oled_menu_current->contents[pos].name;	
		Serial.printf("new item: %s\n", iName.c_str());
		Serial.printf("[oled_menu_setPos] - end\n");
	#endif
	oled_menu_current->currseur 	= currseur;
	oled_menu_current->startItem 	= startItem;
	oled_menu_current->itemsCnt 	= itemsCnt;	    
}

int oled_menu_pos::getPos(){
	int pos = currseur + startItem;
	return pos;
}
String oled_menu_pos::getItemName(){
	int pos = currseur + startItem;
	// String mName = oled_menu_current->name;
	String iName = oled_menu_current->contents[pos].name;
	return iName;
}

//
// CREEATION DES MENU 
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
	int pos = oled_menu_pos.getPos();
	String subTitle = oled_menu_current->contents[pos].subTitle;	
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
	int pos = oled_menu_pos.getPos();
	String subTitle = oled_menu_current->contents[pos].subTitle;	
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
void oled_menu_create::create_menu_items(int startItem, int starMenu, int item_perPage) {
	int start 	= starMenu;
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
void oled_menu_create::create_menu_items(int startItem, int starMenu, int item_perPage) {
	int start 	= starMenu;
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
		oled_menu_pos.currseur,
		oled_menu_pos.starMenu,
		oled_menu_pos.starHeader, 
		oled_menu_pos.titleLeft, 
		oled_menu_pos.subTitleLeft
	);
	create_menu_items(oled_menu_pos.startItem, oled_menu_pos.starMenu, oled_menu_pos.item_perPage);
}


// INITIALISATION DU MENU QUAND IL SERA CHARGER
void oled_menu_create::init_menu(){
	oled_menu_pos.currseur 	= 0;
	oled_menu_pos.itemsCnt	= 0;
	oled_menu_pos.startItem	= 0;
	oled_menu_current 		= this;
	create_menu();
}
void oled_menu_init_menu (oled_menu_create * m) {
	m->init_menu();
}
void oled_menu_create::set_subtitle (String str) {
	int pos = oled_menu_pos.getPos();
	contents[pos].subTitle = str;
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
	oled_menu_pos.setPos(up, m->size);
	m->create_menu();	
}
void oled_menu_click_initMenu(oled_menu_create * m){
	oled_display_mod = oled_display_menu;
	m->init_menu();	
}



// POUR LA NAVIGATION
	void oled_menu_set_main(oled_menu_create * m) { oled_menu_main = m; }
	void oled_menu_click_move(boolean up){ 		// sipml click bp 1 || bp 2
		int pos = oled_menu_pos.getPos();
		boolean exec = true;
		if (oled_display_mod!= oled_display_menu) 	{
			if (oled_display_mod == oled_display_disp) {
				oled_display_mod = oled_display_menu;
				oled_menu_current->create_menu();
			} else oled_menu_click_initMenu(oled_menu_main); 
		} else {
			if (up) {
				if (oled_menu_current->clickmoveUp)	oled_menu_click_move(up, oled_menu_current);
				else {
					if (oled_menu_current->clickmoveFunc == 0) oled_menu_current->contents[pos].func_1(exec, oled_menu_move_up);
					if (oled_menu_current->clickmoveFunc == 1) oled_menu_current->contents[pos].func_2(exec, oled_menu_move_up);
			#ifdef FUNC_ITEM_3
					if (oled_menu_current->clickmoveFunc == 2) oled_menu_current->contents[pos].func_3(exec, oled_menu_move_up);
			#endif	
			#ifdef FUNC_ITEM_4
					if (oled_menu_current->clickmoveFunc == 3) oled_menu_current->contents[pos].func_4(exec, oled_menu_move_up);
			#endif
				}				
			} 
			if (!up) {
				if (oled_menu_current->clickmoveDown)	oled_menu_click_move(up, oled_menu_current);
				else {
					if (oled_menu_current->clickmoveFunc == 0) oled_menu_current->contents[pos].func_1(exec, oled_menu_move_down);
					if (oled_menu_current->clickmoveFunc == 1) oled_menu_current->contents[pos].func_2(exec, oled_menu_move_down);
			#ifdef FUNC_ITEM_3
					if (oled_menu_current->clickmoveFunc == 2) oled_menu_current->contents[pos].func_3(exec, oled_menu_move_down);
			#endif	
			#ifdef FUNC_ITEM_4
					if (oled_menu_current->clickmoveFunc == 3) oled_menu_current->contents[pos].func_4(exec, oled_menu_move_down);
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
		int pos = oled_menu_pos.getPos();
		oled_menu_current->contents[pos].func_1(true, move);
	}
	void oled_menu_click_2_set(oled_menu_move move){ 				// db click bp 2
		#ifdef DEBUG
		Serial.printf("\n[oled_menu_click_2_set]\n");
		#endif		
		if (oled_display_mod != oled_display_menu) return;
		int pos = oled_menu_pos.getPos();
		oled_menu_current->contents[pos].func_2(true, move);
	}
#ifdef FUNC_ITEM_3
	void oled_menu_click_3_set(oled_menu_move move){ 				// db click bp 3
		#ifdef DEBUG
		Serial.printf("\n[oled_menu_click_3_set]\n");
		#endif
		if (oled_display_mod != oled_display_menu) return;
		int pos = oled_menu_pos.getPos();
		oled_menu_current->contents[pos].func_3(true, move);
	}
#endif	
#ifdef FUNC_ITEM_4
	void oled_menu_click_4_set(oled_menu_move move){ 				// db click bp 4
		#ifdef DEBUG
		Serial.printf("\n[oled_menu_click_4_set]\n");
		#endif		
		if (oled_display_mod != oled_display_menu) return;
		int pos = oled_menu_pos.getPos();
		oled_menu_current->contents[pos].func_4(true, move);
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
		if (isSelectedMenu(oled_menu_lg)) oled_menu_longClick_1_list[i].func_1(true, move);
	}	
}

int oled_menu_longClick_2_count = 0;
void oled_menu_long_click_2(oled_menu_move move) { 			// longClick bp 2
	if (oled_menu_longClick_2_count <= 0) return;

	for(int i=0; i < oled_menu_longClick_2_count ; i++) {
		oled_menu_longClick_2_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) oled_menu_longClick_2_list[i].func_1(true, move);
	}	
}
#ifdef FUNC_ITEM_3
int oled_menu_longClick_3_count = 0;
void oled_menu_long_click_3(oled_menu_move move) { 			// longClick bp 3
	if (oled_menu_longClick_3_count <= 0) return;

	for(int i=0; i < oled_menu_longClick_3_count ; i++) {
		oled_menu_longClick_3_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) oled_menu_longClick_3_list[i].func_1(true, move);
	}	
}
#endif	
#ifdef FUNC_ITEM_4
int oled_menu_longClick_4_count = 0;
void oled_menu_long_click_4(oled_menu_move move) { 			// longClick bp 4
	if (oled_menu_longClick_4_count <= 0) return;

	for(int i=0; i < oled_menu_longClick_4_count ; i++) {
		oled_menu_longClick_4_list[i].func_1(false, move);
		if (isSelectedMenu(oled_menu_lg)) oled_menu_longClick_4_list[i].func_1(true, move);
	}	
}
#endif


//
// CREATION DES ITEMS + FONCTION ATTRIBUER AUX ITEMS 
// *****************************************************************************************************

void om_tf(boolean exec, oled_menu_move move){
	int pos = oled_menu_pos.getPos();
	String mName = oled_menu_current->name;
	String iName = oled_menu_current->contents[pos].name;
	Serial.printf("[func] menu: %s - item: %s - item pos: %d\n", mName.c_str(), iName.c_str(), pos );
	// char buffer[200];
	// sprintf(buffer, "[func] menu: %s - item: %s - item pos: %d\n", mName.c_str(), iName.c_str(), pos);
	// debug(buffer);

}

		

// // CREATIONS MENU GENERALISER 
// template <typename Callable>
// void oled_menu_create_items(Callable f_1, Callable f_2, oled_menu_create * str) {
// 	str->contents[0].name 		= "g1"; 
// 	str->contents[0].func_1 	= f_1; 
// 	str->contents[0].func_2 	= f_2; 
// 	str->contents[1].name 		= "g2"; 
// 	str->contents[1].func_1 	= f_1; 
// 	str->contents[1].func_2 	= f_2; 
// 	str->size 					= 2;  
// }

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


 