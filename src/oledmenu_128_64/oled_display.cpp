// ECRAN OLED
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#include "oled_display.h"
#include "tools.h"

// MEM OLED SCREEN
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DISPLAY MOD
oled_display_mods 	oled_display_mod;
oled_menu_position 	oled_menu_getPos;

// INT OLED SCREEN
void oled_init() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); 
    }
    display.clearDisplay();
    display.display();


    /*
    	oled_display_mod
		oled_display_off 	: le menu est detruit
		oled_display_menu	: le menu est actif
    */
    oled_display_mod 	= oled_display_off; 
    /*
    	    quand aucun menu n'est actif 
    	    	oled_menu_getPos = oled_menu_pos_none;
    */
    oled_menu_getPos 	= oled_menu_pos_none;
}

//
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
	String mName = oled_menu_current->name;
	String iName = oled_menu_current->contents[pos].name;	
	#ifdef DEBUG
		Serial.printf("\n[oled_menu_setPos] menu: %s - old item: %s - old currPos: %d\n", mName.c_str(), iName.c_str(), currseur);
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
}

int oled_menu_pos::getPos(){
	int pos = currseur + startItem;
	return pos;
}


//
// CREEATION DES MENU 
// *****************************************************************************************************

// MISE EN MEMOIRE DES POINTEURS 
oled_menu_create * oled_menu_current; 			// selected menu
oled_menu_create * oled_menu_array[OLED_MENU_ITEMS_MAX];
int oled_menu_array_nbr = 0;

oled_menu_create * oled_menu_home; 				// demo menu 1
oled_menu_create * oled_menu_home_lighting; 	// demo menu 2


oled_menu_create::oled_menu_create(String sName) {
	name = sName;
	oled_menu_array[oled_menu_array_nbr++] = this;
}

// POUR DEBUG
void oled_menu_create::print() {
	Serial.printf("\n[print menu] : %s - size:%d\n",name.c_str(), size);
	for( int i = 0; i < size; i++) {
        String s= "[" + String(i) + "]";
        while (s.length()<4) s+=" ";
		Serial.printf("%s name: %s\n", s.c_str(), contents[i].name.c_str());
	}
	Serial.printf("[print menu] end\n");

}

// CREATIONS DU TITRE + SOUSTITRE + CURSEUR 
void oled_menu_create::create_menu_header(String title, int currPos, int starMenu, int starHeader, int titleLeft, int subTitleLeft) {

	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(titleLeft, starHeader);
	display.println(title);
	if (subTitle != "") {
		display.setCursor(subTitleLeft, starHeader+10);
		display.println(subTitle);		
	}
	if (currPos == 0) currPos = starMenu;
	else currPos = (currPos*10) + starMenu; 

	display.setCursor(0, currPos);
	display.println(">"); 

}

// CREATIONS DES ITEMS
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

// CREATIONS DU MENU (SERVIRA AUSSI POUR LA NAVIGATION PAR CLICK)
void oled_menu_create::create_menu() {
	oled_menu_setOption();
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
	oled_menu_getPos 		= getPos;
	oled_menu_current 		= this;
	create_menu();
}


//
// GESTION DES CLICK BOUTON POUSSOIR
// *****************************************************************************************************

boolean oled_menu_getMenu(oled_menu_position search) {
	if (oled_menu_getPos == search) return true;
	return false;
}
void oled_menu_click_move(boolean up, oled_menu_create * m){
	oled_menu_pos.setPos(up, m->size);
	m->create_menu();	
}
void oled_menu_click_initMenu(oled_menu_create * m){
	if (oled_display_mod != oled_display_menu) oled_display_mod = oled_display_menu;
	m->init_menu();	
}
void oled_menu_setOption(){
	if (oled_menu_getMenu(oled_menu_pos_home_lighting)) { oled_menu_home_lighting->subTitle = "sous-titre"; }
}


// POUR LA NAVIGATION
/*
	void oled_menu_click_move(boolean up){ 		// sipml click bp 1 || bp 2
		if (oled_menu_getMenu(oled_menu_pos_none)) 			{ 	oled_menu_click_initMenu(oled_menu_home); return;	}

		if (oled_menu_getMenu(oled_menu_pos_home)) 				oled_menu_click_move(up, oled_menu_home);
		if (oled_menu_getMenu(oled_menu_pos_home_lighting)) 	oled_menu_click_move(up, oled_menu_home_lighting);
	}
*/	
	void oled_menu_click_move(boolean up){ 		// sipml click bp 1 || bp 2
		if (oled_menu_getMenu(oled_menu_pos_none)) 	{
			oled_menu_click_initMenu(oled_menu_home); 
		} else {
			oled_menu_click_move(up, oled_menu_current);
		}
	}


// POUR EFFECTUER L'ACTION ATTRIBUER A L'ITEM SELECTIONNER
/*
	void oled_menu_click_1_set(){ 				// db click bp 1
		int pos = oled_menu_pos.getPos();
		
		if (oled_menu_getMenu(oled_menu_pos_home) ) 			{ 	oled_menu_home 				->contents[pos].func_1(); 	return;	}
		if (oled_menu_getMenu(oled_menu_pos_home_lighting) ) 	{ 	oled_menu_home_lighting 	->contents[pos].func_1(); 	return;	}

	}
*/
	void oled_menu_click_1_set(){ 				// db click bp 1
		int pos = oled_menu_pos.getPos();
		oled_menu_current->contents[pos].func_1();
	}
	void oled_menu_click_2_set(){ 				// db click bp 1
		int pos = oled_menu_pos.getPos();
		oled_menu_current->contents[pos].func_2();
	}


// POUR EFFECTUER L'ACTION ATTRIBUER AU MENU SLECTIONNER
void oled_menu_long_click_1() { 			// longClick bp 1
	switch (oled_menu_getPos) {
		case oled_menu_pos_none:
		break;

		case oled_menu_pos_home:
			// for leaving menu
			oled_display_mod = oled_display_off;
			oled_menu_getPos = oled_menu_pos_none;
				// clear display if necessaire
				display.clearDisplay();
				display.display();
		break;
		case oled_menu_pos_home_lighting: oled_menu_home->init_menu(); break;
	}
}
void oled_menu_long_click_2() { 			// longClick bp 2
	switch (oled_menu_getPos) {
		case oled_menu_pos_none:
		break;

		case oled_menu_pos_home:
		break;
		case oled_menu_pos_home_lighting:
		break;		
	}
}


//
// CREATION DES ITEMS + FONCTION ATTRIBUER AUX ITEMS 
// *****************************************************************************************************

void test_f1(){
	int pos = oled_menu_pos.getPos();
	String mName = oled_menu_current->name;
	String iName = oled_menu_current->contents[pos].name;
	Serial.printf("\n[demo func_1] menu: %s - item: %s\n", mName.c_str(), iName.c_str() );
}
void test_f2(){
	int pos = oled_menu_pos.getPos();
	String mName = oled_menu_current->name;
	String iName = oled_menu_current->contents[pos].name;
	Serial.printf("\n[demo test_f2] menu: %s - item: %s\n", mName.c_str(), iName.c_str() );
}

		
// MENU PRICIPAL
void oled_menu_click_home_lighting() 	{ oled_menu_home_lighting->init_menu(); }
void oled_menu_create_items_home(){
	oled_menu_home->contents[0].name 	= "lumierre"; 
	oled_menu_home->contents[0].func_1 	= &oled_menu_click_home_lighting; 
	oled_menu_home->contents[0].func_2 	= &test_f2; 

	oled_menu_home->contents[1].name 	= "temperature"; 
	oled_menu_home->contents[1].func_1 	= &test_f1; 
	oled_menu_home->contents[1].func_2 	= &test_f2; 

	oled_menu_home->contents[2].name 	= "piscine"; 
	oled_menu_home->contents[2].func_1 	= &test_f1; 
	oled_menu_home->contents[2].func_2 	= &test_f2; 

	oled_menu_home->getPos 				= oled_menu_pos_home;
	oled_menu_home->size 				= 3;
}
	// lumierre
	void oled_menu_create_items_home_lighting(){
		oled_menu_home_lighting->contents[0].name 		= "cuisine"; 
		oled_menu_home_lighting->contents[0].func_1 	= &test_f1; 
		oled_menu_home_lighting->contents[0].func_2 	= &test_f2; 

		oled_menu_home_lighting->contents[1].name 		= "salon"; 
		oled_menu_home_lighting->contents[1].func_1 	= &test_f1; 
		oled_menu_home_lighting->contents[1].func_2 	= &test_f2; 

		oled_menu_home_lighting->contents[2].name 		= "terrase"; 
		oled_menu_home_lighting->contents[2].func_1 	= &test_f1; 
		oled_menu_home_lighting->contents[2].func_2 	= &test_f2; 

		oled_menu_home_lighting->getPos 				= oled_menu_pos_home_lighting;
		oled_menu_home_lighting->size 					= 3;
	}

// CREATIONS MENU GENERALISER 
template <typename Callable>
void oled_menu_create_items(Callable f_1, Callable f_2, oled_menu_create * str, oled_menu_position pos) {
	str->contents[0].name 		= "g1"; 
	str->contents[0].func_1 	= f_1; 
	str->contents[0].func_2 	= f_2; 
	str->contents[1].name 		= "g2"; 
	str->contents[1].func_1 	= f_1; 
	str->contents[1].func_2 	= f_2; 
	
	str->getPos 				= pos;
	str->size 					= 2;  
}





