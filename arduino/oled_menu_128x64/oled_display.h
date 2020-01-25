#ifndef OLED_DISPLAY_H
   	#define OLED_DISPLAY_H
	#include <Arduino.h>



	enum oled_display_mods { 
	    oled_display_menu,
	    oled_display_off,
	    oled_display_time
	};
	extern oled_display_mods oled_display_mod;

	enum oled_menu_position { 
	    oled_menu_pos_none,
	    oled_menu_pos_menu
	};
	extern oled_menu_position oled_menu_getPos;


	//
	// STRUCTURE POUR STOCKER LES ITEMS
	// *****************************************************************************************************
	
	// USER STruCT
	struct oled_menu_item {
		char * name;
		void (*func_1)(void);
		void (*func_2)(void);		
	} ;

	// OJECT STRUCT
	struct oled_menu_def {
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
			String 				subTitle;
			boolean 			clickmoveUp;
			boolean 			clickmoveDown;
			int 				clickmoveFunc;

			oled_menu_create(String sName);
			void print();
			void create_menu();
			void init_menu();
			void create_menu_items(int startItem, int starMenu, int item_perPage) ;
			void create_menu_header(String title, int currPos, int starMenu, int starHeader, int titleLeft, int subTitleLeft);
	};

	extern oled_menu_create * oled_menu_lg;
	extern oled_menu_create * oled_menu_current;
	extern oled_menu_create * oled_menu_array[];
	extern int oled_menu_array_nbr;

	void oled_menu_init_menu (oled_menu_create * m);
	void oled_menu_click_initMenu(oled_menu_create * m);

	void oled_menu_setOption(oled_menu_create * m , String name);
	void oled_menu_setOption();

	void oled_menu_set_main(oled_menu_create * m);
	void oled_menu_set_lg(oled_menu_create * m);

	struct oled_menu_options {
		void (*option)(void);
	} ;
	extern PROGMEM oled_menu_options oled_menu_options_list [];
	extern int oled_menu_optCount;
	
	struct oled_menu_longClick_1 {
		void (*func_1)(boolean exec);
	} ;
	extern PROGMEM oled_menu_longClick_1 oled_menu_longClick_1_list [];
	extern int oled_menu_longClick_1_count;

	struct oled_menu_longClick_2 {
		void (*func_1)(boolean exec);
	} ;
	extern PROGMEM oled_menu_longClick_2 oled_menu_longClick_2_list [];
	extern int oled_menu_longClick_2_count;
	
	void oled_menu_create_items_v2(oled_menu_item list[], int size, oled_menu_create * m);

	void oled_menu_click_move(boolean up);
	void oled_menu_click_1_set();
	void oled_menu_click_2_set();
	void oled_menu_long_click_1();
	void oled_menu_long_click_2();

	
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


	//
	// DIVERS
	// *****************************************************************************************************

	void test_f1();
	void test_f2();
#endif