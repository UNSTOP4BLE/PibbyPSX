/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "spinel.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//spinel character structure
enum
{
	spinel_ArcMain_Idle0,
	spinel_ArcMain_Idle1,
	spinel_ArcMain_Idle2,
	spinel_ArcMain_Idle3,
	spinel_ArcMain_Idle4,
	spinel_ArcMain_Idle5,
	spinel_ArcMain_Idle6,
	spinel_ArcMain_Idle7,
	spinel_ArcMain_Left0,
	spinel_ArcMain_Left1,
	spinel_ArcMain_Down0,
	spinel_ArcMain_Down1,
	spinel_ArcMain_Up0,
	spinel_ArcMain_Up1,
	spinel_ArcMain_Right0,
	spinel_ArcMain_Right1,
	
	spinel_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[spinel_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_spinel;

//spinel character definitions
static const CharFrame char_spinel_frame[] = {
	{spinel_ArcMain_Idle0, {  0,   0, 248, 155}, {0,  0}}, //0 idle 1
	{spinel_ArcMain_Idle1, {  0,   0, 247, 153}, {0,  0}}, //1 idle 2
	{spinel_ArcMain_Idle2, {  0,   0, 248, 154}, {0,  0}}, //2 idle 3
	{spinel_ArcMain_Idle3, {  0,   0, 249, 150}, {0,  0}}, //3 idle 4
	{spinel_ArcMain_Idle4, {  0,   0, 247, 154}, {0,  0}}, //4 idle 5
	{spinel_ArcMain_Idle5, {  0,   0, 251, 149}, {0,  0}}, //5 idle 6
	{spinel_ArcMain_Idle6, {  0,   0, 255, 153}, {0,  0}}, //6 idle 7
	{spinel_ArcMain_Idle7, {  0,   0, 247, 155}, {0,  0}}, //7 idle 8

	{spinel_ArcMain_Left0, {  0,   0, 210, 244}, {0,  0}}, //4 left 1
	{spinel_ArcMain_Left1, {  0,   0, 207, 239}, {0,  0}}, //5 left 2

	{spinel_ArcMain_Down0, {  0,   0, 242, 194}, {0,  0}}, //6 down 1
	{spinel_ArcMain_Down0, {  0,   0, 244, 198}, {0,  0}}, //7 down 2

	{spinel_ArcMain_Up0, {  0,   0, 213, 243}, {0,  0}}, //8 up 1
	{spinel_ArcMain_Up0, {  0,   0, 201, 250}, {0,  0}}, //9 up 2
	
	{spinel_ArcMain_Right0, {  0,   0, 185, 246}, {0,  0}}, //10 right 1
	{spinel_ArcMain_Right1, {  0,   0, 185, 246}, {0,  0}}, //11 right 2
};

static const Animation char_spinel_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 8, 9, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 10, 11, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 12, 13, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 14, 15, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//spinel character functions
void Char_spinel_SetFrame(void *user, u8 frame)
{
	Char_spinel *this = (Char_spinel*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_spinel_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_spinel_Tick(Character *character)
{
	Char_spinel *this = (Char_spinel*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_spinel_SetFrame);
	Character_Draw(character, &this->tex, &char_spinel_frame[this->frame]);
}

void Char_spinel_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_spinel_Free(Character *character)
{
	Char_spinel *this = (Char_spinel*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_spinel_New(fixed_t x, fixed_t y)
{
	//Allocate spinel object
	Char_spinel *this = Mem_Alloc(sizeof(Char_spinel));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_spinel_New] Failed to allocate spinel object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_spinel_Tick;
	this->character.set_anim = Char_spinel_SetAnim;
	this->character.free = Char_spinel_Free;
	
	Animatable_Init(&this->character.animatable, char_spinel_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SPINEL.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //spinel_ArcMain_Idle0
		"idle1.tim", //spinel_ArcMain_Idle1
		"idle2.tim", //spinel_ArcMain_Idle1
		"idle3.tim", //spinel_ArcMain_Idle1
		"idle4.tim", //spinel_ArcMain_Idle1
		"idle5.tim", //spinel_ArcMain_Idle1
		"idle6.tim", //spinel_ArcMain_Idle1
		"idle7.tim", //spinel_ArcMain_Idle1
		"left0.tim",  //spinel_ArcMain_Left
		"left1.tim",  //spinel_ArcMain_Left
		"down0.tim",  //spinel_ArcMain_Down
		"down1.tim",  //spinel_ArcMain_Down
		"up0.tim",    //spinel_ArcMain_Up
		"up1.tim",    //spinel_ArcMain_Up
		"right0.tim", //spinel_ArcMain_Right
		"right1.tim", //spinel_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}