/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_MENU_H
#define PSXF_GUARD_MENU_H

#include "stage.h"
#include "font.h"

//Menu enums
typedef enum
{
	MenuPage_Opening,
	MenuPage_Title,
	MenuPage_Main,
	MenuPage_Story,
	MenuPage_Freeplay,
	MenuPage_Mods,
	MenuPage_Options,
	
	#ifdef PSXF_NETWORK
		MenuPage_NetJoin,
		MenuPage_NetHost,
		MenuPage_NetConnect,
		MenuPage_NetFail,
		MenuPage_NetOpWait,
		MenuPage_NetOp,
		MenuPage_NetLobby,
		MenuPage_NetInitFail,
	#endif
	
	MenuPage_Stage, //Changes game loop
} MenuPage;


//Menu state
typedef struct
{
	//Menu state
	u8 page, next_page;
	boolean page_swap;
	u8 select, next_select;
	
	fixed_t scroll;
	fixed_t trans_time;
	
	//Page specific state
	union
	{
		struct
		{
			u8 funny_message;
		} opening;
		struct
		{
			fixed_t logo_bump;
			fixed_t fade, fadespd;
		} title;
		struct
		{
			fixed_t fade, fadespd;
		} story;
		struct
		{
			fixed_t back_r, back_g, back_b;
		} freeplay;
	#ifdef PSXF_NETWORK
		struct
		{
			boolean type;
			MenuStr port;
			MenuStr pass;
		} net_host;
		struct
		{
			boolean type;
			MenuStr ip;
			MenuStr port;
			MenuStr pass;
		} net_join;
		struct
		{
			boolean swap;
		} net_op;
	#endif
	} page_state;
	
	union
	{
		struct
		{
			u8 id, diff;
			boolean story;
		} stage;
	} page_param;
	
	//Menu assets
	Gfx_Tex tex_back, tex_ng, tex_story, tex_title, tex_icon;
	FontData font_bold, font_arial;

	Character *bf; //Title Girlfriend
} Menu;
extern Menu menu;

//Menu functions
void Menu_Load(MenuPage page);
void Menu_Unload();
void Menu_ToStage(StageId id, StageDiff diff, boolean story);
void Menu_Tick();

#endif
