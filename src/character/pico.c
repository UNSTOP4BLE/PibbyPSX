/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "pico.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//pico character structure
enum
{
	pico_ArcMain_Idle0,
	pico_ArcMain_Idle1,
	pico_ArcMain_Hit0,
	pico_ArcMain_Hit1,
	pico_ArcMain_Hit2,
	pico_ArcMain_Hit3,
	pico_ArcMain_Hit4,
	
	pico_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[pico_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_pico;

//pico character definitions
static const CharFrame char_pico_frame[] = {
	{pico_ArcMain_Idle0, {  0,   0, 120, 117}, {0,  117}}, //0 idle 1
	{pico_ArcMain_Idle0, {120,   0, 122, 120}, {0,  120}}, //1 idle 2
	{pico_ArcMain_Idle0, {  0, 117, 123, 120}, {0,  119}}, //2 idle 3
	{pico_ArcMain_Idle0, {123, 120, 124, 121}, {0,  121}}, //3 idle 4
	{pico_ArcMain_Idle1, {  0,   0, 125, 121}, {0,  121}}, //4 idle 5
	{pico_ArcMain_Idle1, {125,   0, 124, 121}, {0,  121}}, //5 idle 6
	{pico_ArcMain_Idle1, {  0, 121, 124, 123}, {0,  122}}, //6 idle 7

	{pico_ArcMain_Hit2, { 91,  123, 106,  121}, {0,  121}}, //4 left 1
	{pico_ArcMain_Hit3, {  0,   0,  102,  120}, {0,  120}}, //5 left 2
	{pico_ArcMain_Hit3, {102,   0,  100,  120}, {0,  120}}, //4 left 1
	{pico_ArcMain_Hit3, {  0, 120,  100,  122}, {0,  121}}, //5 left 2

	{pico_ArcMain_Hit0, {  0,   0, 146,  97}, {0,  97}}, //6 down 1
	{pico_ArcMain_Hit0, {  0,  97, 148,  99}, {0,  99}}, //7 down 2
	{pico_ArcMain_Hit1, {  0,   0, 147,  99}, {0,  99}}, //6 down 1
	{pico_ArcMain_Hit1, {  0,  99, 147,  101}, {0,  101}}, //7 down 2

	{pico_ArcMain_Hit3, {100, 120, 112,  123}, {0, 123}}, //8 up 1
	{pico_ArcMain_Hit4, {  0,   0, 114,  123}, {0,  122}}, //9 up 2
	{pico_ArcMain_Hit4, {114,   0, 115,  121}, {0,  121}}, //8 up 1
	{pico_ArcMain_Hit4, {  0, 123, 114,  121}, {0,  0}}, //9 up 2
	
	{pico_ArcMain_Idle1,  {124, 121, 87, 123}, {0,  117}}, //10 right 1
	{pico_ArcMain_Hit2, {  0,   0, 91, 122}, {0,  119}}, //11 right 2
	{pico_ArcMain_Hit2, { 91,   0, 93, 120}, {0,  118}}, //10 right 1
	{pico_ArcMain_Hit2, {  0, 122, 91, 125}, {0,  120}}, //11 right 2
};

static const Animation char_pico_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 8, 9, 10, 11, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 12, 13, 14, 15, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 16, 17, 18, 19, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 20, 21, 22, 23, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//pico character functions
void Char_pico_SetFrame(void *user, u8 frame)
{
	Char_pico *this = (Char_pico*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_pico_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_pico_Tick(Character *character)
{
	Char_pico *this = (Char_pico*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_pico_SetFrame);
	Character_CharacterFlip(character, &this->tex, &char_pico_frame[this->frame], FIXED_UNIT);
}

void Char_pico_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_pico_Free(Character *character)
{
	Char_pico *this = (Char_pico*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_pico_New(fixed_t x, fixed_t y)
{
	//Allocate pico object
	Char_pico *this = Mem_Alloc(sizeof(Char_pico));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_pico_New] Failed to allocate pico object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_pico_Tick;
	this->character.set_anim = Char_pico_SetAnim;
	this->character.free = Char_pico_Free;
	
	Animatable_Init(&this->character.animatable, char_pico_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(-142 - -120,1);
	this->character.focus_y = FIXED_DEC(25 - 120,1);
	this->character.focus_zoom = FIXED_DEC(8,10);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\PICO.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //pico_ArcMain_Idle0
		"idle1.tim", //pico_ArcMain_Idle1
		"hit0.tim",
		"hit1.tim",
		"hit2.tim",
		"hit3.tim",
		"hit4.tim",
		
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
