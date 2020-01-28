#ifndef OLED_DISPLAY_H
   	#define OLED_DISPLAY_H
	#include <Arduino.h>

	#include "oled_def.h"    

	enum oled_display_mods { 
	    oled_display_menu,
	    oled_display_disp,
	    oled_display_off,
	    oled_display_time
	};
	extern oled_display_mods oled_display_mod;

	enum oled_menu_position { 
	    oled_menu_pos_none,
	    oled_menu_pos_menu
	};
	extern oled_menu_position oled_menu_getPos;

	enum oled_menu_move { 
	    oled_menu_move_up,
	    oled_menu_move_down,
	    oled_menu_move_next,
	    oled_menu_move_back,
	    oled_menu_move_none
	};

	//
	// STRUCTURE POUR STOCKER LES ITEMS
	// *****************************************************************************************************
	// USER STruCT
	struct oled_menu_item {
		char * name;
		char * subTitle;
		void (*func_1)(boolean exec, oled_menu_move move);
		void (*func_2)(boolean exec, oled_menu_move move);
		#ifdef FUNC_ITEM_3
		void (*func_3)(boolean exec, oled_menu_move move);	
		#endif	
		#ifdef FUNC_ITEM_4
		void (*func_4)(boolean exec, oled_menu_move move);
		#endif		
	} ;

	// OJECT STRUCT
	struct oled_menu_def {
		String name;
		String subTitle;
		void (*func_1)(boolean exec, oled_menu_move move);
		void (*func_2)(boolean exec, oled_menu_move move);
		#ifdef FUNC_ITEM_3
		void (*func_3)(boolean exec, oled_menu_move move);	
		#endif	
		#ifdef FUNC_ITEM_4
		void (*func_4)(boolean exec, oled_menu_move move);
		#endif
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
			char * 				name;
			boolean 			clickmoveUp;
			boolean 			clickmoveDown;
			int 				clickmoveFunc;

			int currseur 		= 0;
			int itemsCnt 		= 0;
			int startItem		= 0;
			
			oled_menu_create(char * sName);
			void set_subtitle (String str);
			void print();
			void create_menu();
			void init_menu();
			void create_menu_items(int startItem, int starMenu, int item_perPage) ;
			void create_menu_header(char * title, int currPos, int starMenu, int starHeader, int titleLeft, int subTitleLeft);
	};


	extern oled_menu_create * oled_menu_lg;
	extern oled_menu_create * oled_menu_current;
	extern oled_menu_create * oled_menu_array[];
	extern int oled_menu_array_nbr;

	void oled_menu_init_menu (oled_menu_create * m);
	void oled_menu_click_initMenu(oled_menu_create * m);

	void oled_menu_set_main(oled_menu_create * m);
	void oled_menu_set_lg(oled_menu_create * m);

	
	struct oled_menu_longClick_1 {
		void (*func_1)(boolean exec, oled_menu_move move);
	} ;
	extern PROGMEM oled_menu_longClick_1 oled_menu_longClick_1_list [];
	extern int oled_menu_longClick_1_count;

	struct oled_menu_longClick_2 {
		void (*func_1)(boolean exec, oled_menu_move move);
	} ;
	extern PROGMEM oled_menu_longClick_2 oled_menu_longClick_2_list [];
	extern int oled_menu_longClick_2_count;
#ifdef FUNC_ITEM_3
	struct oled_menu_longClick_3 {
		void (*func_1)(boolean exec, oled_menu_move move);
	} ;
	extern PROGMEM oled_menu_longClick_3 oled_menu_longClick_3_list [];
	extern int oled_menu_longClick_3_count;
#endif		
#ifdef FUNC_ITEM_4
	struct oled_menu_longClick_4 {
		void (*func_1)(boolean exec, oled_menu_move move);
	} ;
	extern PROGMEM oled_menu_longClick_4 oled_menu_longClick_4_list [];
	extern int oled_menu_longClick_4_count;
#endif	

	void oled_menu_create_items_v2(oled_menu_item list[], int size, oled_menu_create * m);

	void oled_menu_click_move(boolean up);
	void oled_menu_click_1_set(oled_menu_move move);
	void oled_menu_click_2_set(oled_menu_move move);
	void oled_menu_long_click_1(oled_menu_move move);
	void oled_menu_long_click_2(oled_menu_move move);
#ifdef FUNC_ITEM_3
	void oled_menu_click_3_set(oled_menu_move move);
	void oled_menu_long_click_3(oled_menu_move move);
#endif		
#ifdef FUNC_ITEM_4
	void oled_menu_click_4_set(oled_menu_move move);
	void oled_menu_long_click_4(oled_menu_move move);
#endif	
	
	//
	// CLASS POUR LA NAVIGATION
	// *****************************************************************************************************

	class oled_menu_pos {
		public:
			String starterItem 	= "";
			int menu_pos 		= 0;

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
			String 	getItemName();
	};	


	//
	// DIVERS
	// *****************************************************************************************************


	void om_tf(boolean exec, oled_menu_move move);

	void oled_clear();

	#ifdef SD1306WIRE_LIB
		void oled_draw_bmp(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const uint8_t bitmap[]);
	#endif  
	#ifdef ADAFRUIT_SD1306_LIB
		void oled_draw_bmp(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t bitmap[]);
	#endif

#endif