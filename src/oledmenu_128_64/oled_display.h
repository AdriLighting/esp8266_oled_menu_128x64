#ifndef OLED_DISPLAY_H
   	#define OLED_DISPLAY_H
	#include <Arduino.h>
	#include <FastLED.h>
	#include <SPI.h>
	#include <Wire.h>
	#include <Adafruit_GFX.h>
	#include <Adafruit_SSD1306.h>

	extern Adafruit_SSD1306 display;

	enum oled_display_mods { 
	    oled_display_menu,
	    oled_display_off,
	    oled_display_time
	};
	extern oled_display_mods oled_display_mod;

	void oled_init();

	enum oled_menu_position { 
	    oled_menu_pos_none,
	    oled_menu_pos_home, 			// demo menu 1
	    oled_menu_pos_home_lighting		// demo menu 2
	};
	extern oled_menu_position oled_menu_getPos;


	//
	// STRUCTURE POUR STOCKER LES ITEMS
	// *****************************************************************************************************

	struct oled_menu_def
	{
		String name;
		void (*func_1)(void);
		void (*func_2)(void);
	};


	//
	// CLASS POUR LA CREATION DES MENU
	// *****************************************************************************************************

	#define OLED_MENU_ITEMS_MAX 25

	class oled_menu_create {
		public:
			oled_menu_def contents[OLED_MENU_ITEMS_MAX];

		public:
			int 				size;
			String 				name;
			oled_menu_position 	getPos;
			String 				subTitle;

			oled_menu_create(String sName);
			void print();
			void create_menu();
			void init_menu();
			void create_menu_items(int startItem, int starMenu, int item_perPage) ;
			void create_menu_header(String title, int currPos, int starMenu, int starHeader, int titleLeft, int subTitleLeft);
	};

	extern oled_menu_create * oled_menu_current;
	extern oled_menu_create * oled_menu_array[];
	extern int oled_menu_array_nbr;

	extern oled_menu_create * oled_menu_home;
	extern oled_menu_create * oled_menu_home_lighting;

	void oled_menu_click_initMenu(oled_menu_create * m);

	void oled_menu_create_items_home();
	void oled_menu_create_items_home_lighting();

	void oled_menu_click_move(boolean up);
	void oled_menu_click_1_set();
	void oled_menu_click_2_set();
	void oled_menu_long_click_1();
	void oled_menu_long_click_2();

	void oled_menu_setOption();
	
	//
	// CLASS POUR LA NAVIGATION
	// *****************************************************************************************************

	class oled_menu_pos {
		public:

			int currseur 		= 0;
			int itemsCnt 		= 0;
			int startItem		= 0;
			int starMenu		= 0;
			int subTitleLeft 	= 0;
			int titleLeft 		= 0;
			int starHeader		= 0;

			int item_perPage	= 0;

			oled_menu_pos();
			void 	print();
			void 	setPos(boolean up, int size);
			int 	getPos();
	};	


#endif