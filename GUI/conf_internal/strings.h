/*
 * strings.h
 *
 *  Created on: Jan 5, 2021
 *      Author: Kamil Wielgosz
 */

#include "../config.h"

#ifndef STRINGS_H_
#define STRINGS_H_

/*
 * 	STRINGS
 */

#if POLISH == 1
//	INTRO
#define STR_INTRO_SCHOOL 				"PWSZ Tarnów"
#define STR_INTRO_DESC 					"Katedra Informatyki"

// MAIN_MENU
#define STR_MENU_TITLE 					"Kategorie"

// SETTINGS
#define STR_SETTINGS_TITLE 				"Ustawienia"
#define STR_SETTINGS_DISPLAY			"Wyświetlacz"
#define STR_SETTINGS_BRIGHTNESS			"Jasność"
#define STR_SETTINGS_STATUS				"POWIADOMIENIA"
#define STR_SETTINGS_DAYS_TO_END 		"Dni do końca:"
#define STR_SETTINGS_NETWORK			"SIEĆ"
#define STR_SETTINGS_CONNECTEDNETWORK	"Połączono z siecią: "
#define STR_SETTINGS_NETWORK_NONE		"Brak"
#define STR_SETTINGS_NETWORKMSG_TITLE	"Wprowadź hasło:"
#define STR_SETTINGS_MSGBOX_TITLE		"Połączono już z wybraną siecią"
#define STR_SETTINGS_CONNECTING			"Trwa łączenie..."
#define STR_SETTINGS_SAFEMODE			"Tryb awaryjny"

// SHOPPING LIST
#define STR_SHOPLIST_TITLE 				"Lista zakupów"
#define STR_SHOPLIST_CONFIRM_MSG 		"OK"
#define STR_SHOPLIST_LIST_FULL	 		"Lista zakupów jest pełna!"

// TILES
#define STR_TILES_EMPTY_GROUPS			"Brak produktów!"
#define STR_TILES_MSGBTN_TITLE 			"Dodać do listy zakupów:"
#define STR_TILES_MSGBTN_ADD 			"Dodaj"
#define STR_TILES_MSGBTN_DEL 			"Usuń"
#define STR_TILES_MSGBTN_CAL 			"Anuluj"
#define	STR_TILES_MSGBTN_SMODE_TITLE	"Usunąć produkt:"
#define STR_TILES_TAB_COL_NAME			"Produkt"
#define STR_TILES_TAB_COL_COUNT			"Ilość"
#define STR_TILES_TAB_COL_TERM			"Sztuka"

//CATEGORIES
#define STR_TILES_NAMES_DAIRY			"Nabiał"
#define STR_TILES_NAMES_DRINKS			"Napoje"
#define STR_TILES_NAMES_ALCOHOLS		"Alkohole"
#define STR_TILES_NAMES_SAUCES			"Sosy"
#define STR_TILES_NAMES_SWEETS			"Słodycze"
#define STR_TILES_NAMES_FISHES			"Ryby"
#define STR_TILES_NAMES_MEAT			"Mięso"
#define STR_TILES_NAMES_FROZEN			"Mrożonki"
#define STR_TILES_NAMES_PLANTS			"Owoce i warzywa"
#define STR_TILES_NAMES_END				"Kończące się produkty"

#endif

#if ENGLISH == 1
//	INTRO
#define STR_INTRO_SCHOOL 				"PWSZ Tarnów"
#define STR_INTRO_DESC 					"Katedra Informatyki"

// MAIN_MENU
#define STR_MENU_TITLE 					"Categories"

// SETTINGS
#define STR_SETTINGS_TITLE 				"Settings"
#define STR_SETTINGS_DISPLAY			"DISPLAY"
#define STR_SETTINGS_BRIGHTNESS			"Brightness"
#define STR_SETTINGS_STATUS				"NOTIFICATIONS"
#define STR_SETTINGS_DAYS_TO_END 		"Days to expire:"
#define STR_SETTINGS_NETWORK			"NETWORK"
#define STR_SETTINGS_CONNECTEDNETWORK	"Connected with: "
#define STR_SETTINGS_NETWORKMSG_TITLE	"Enter password:"
#define STR_SETTINGS_MSGBOX_TITLE		"Connected already with choosed network."
#define STR_SETTINGS_CONNECTING			"Connecting..."
#define STR_SETTINGS_SAFEMODE			"Deleting mode"
#define STR_SETTINGS_NETWORK_NONE       "none"

// SHOPPING LIST
#define STR_SHOPLIST_TITLE 				"Shopping list"
#define STR_SHOPLIST_CONFIRM_MSG 		"OK"
#define STR_SHOPLIST_LIST_FULL	 		"Shopping list is full!"
#define STR_SHOPLIST_PUTNAME			"Enter name:"

// TILES
#define STR_TILES_EMPTY_GROUPS			"No products!"
#define STR_TILES_MSGBTN_TITLE 			"Add to shopping list:"
#define STR_TILES_MSGBTN_ADD 			"Add"
#define STR_TILES_MSGBTN_DEL 			"Delete"
#define STR_TILES_MSGBTN_CAL 			"Cancel"
#define	STR_TILES_MSGBTN_SMODE_TITLE	"Remove the product:"
#define STR_TILES_TAB_COL_NAME			"Product"
#define STR_TILES_TAB_COL_COUNT			"Count"
#define STR_TILES_TAB_COL_TERM			"Piece"
#define STR_TILES_MSGBOX_CAT			"Category"

//CATEGORIES
#define STR_TILES_NAMES_DAIRY			"Dairy"
#define STR_TILES_NAMES_DRINKS			"Drinks"
#define STR_TILES_NAMES_ALCOHOLS		"Alcohols"
#define STR_TILES_NAMES_SAUCES			"Sauces"
#define STR_TILES_NAMES_SWEETS			"Sweets"
#define STR_TILES_NAMES_FISHES			"Fish"
#define STR_TILES_NAMES_MEAT			"Meat"
#define STR_TILES_NAMES_FROZEN			"Frozen"
#define STR_TILES_NAMES_PLANTS			"Fruits & Vegetables"
#define STR_TILES_NAMES_END				"Near Expire Date"

#endif


#endif /* STRINGS_H_ */
