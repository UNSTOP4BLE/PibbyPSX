/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "steven.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//steven character structure
enum
{
	steven_ArcMain_Idle0,
	steven_ArcMain_Idle1,
	steven_ArcMain_Idle2,
	steven_ArcMain_Idle3,
	steven_ArcMain_Idle4,
	steven_ArcMain_Idle5,
	steven_ArcMain_Idle6,
	steven_ArcMain_Idle7,
	steven_ArcMain_Left0,
	steven_ArcMain_Left1,
	steven_ArcMain_Left2,
	steven_ArcMain_Left3,
	steven_ArcMain_Down0,
	steven_ArcMain_Down1,
	steven_ArcMain_Up0,
	steven_ArcMain_Up1,
	steven_ArcMain_Right0,
	steven_ArcMain_Right1,
	steven_ArcMain_Right2,
	steven_ArcMain_Right3,
	
	steven_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[steven_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_steven;

//steven character definitions
static const CharFrame char_steven_frame[] = {
	{steven_ArcMain_Idle0, {  0,   0, 160, 147}, {138,  140}}, //0 idle 1
	{steven_ArcMain_Idle1, {  0,   0, 159, 145}, {138,  138}}, //1 idle 2
	{steven_ArcMain_Idle2, {  0,   0, 152, 145}, {131,  138}}, //2 idle 3
	{steven_ArcMain_Idle3, {  0,   0, 151, 146}, {131,  139}}, //3 idle 4
	{steven_ArcMain_Idle4, {  0,   0, 157, 148}, {132,  141}}, //4 idle 5
	{steven_ArcMain_Idle5, {  0,   0, 151, 147}, {131,  140}}, //5 idle 6
	{steven_ArcMain_Idle6, {  0,   0, 155, 148}, {132,  141}}, //6 idle 7
	{steven_ArcMain_Idle7, {  0,   0, 152, 148}, {131,  141}}, //7 idle 8

	{steven_ArcMain_Left0, {  0,   0, 137, 156}, {137,  154}}, //4 left 1
	{steven_ArcMain_Left1, {  0,   0, 140, 161}, {140,  158}}, //5 left 2
	{steven_ArcMain_Left2, {  0,   0, 139, 162}, {139,  159}}, //4 left 1
	{steven_ArcMain_Left3, {  0,   0, 139, 163}, {139,  159}}, //5 left 2

	{steven_ArcMain_Down0, {  0,   0, 165, 138}, {136,  112}}, //6 down 1
	{steven_ArcMain_Down0, {100, 113, 149, 137}, {132,  111}}, //7 down 2
	{steven_ArcMain_Down1, {  0,   0, 149, 137}, {132,  111}}, //6 down 1
	{steven_ArcMain_Down1, { 96, 112, 153, 133}, {133,  108}}, //7 down 2

	{steven_ArcMain_Up0, {  0,   0, 123, 174}, {116,  164}}, //8 up 1
	{steven_ArcMain_Up0, {123,   0, 116, 174}, {116,  166}}, //9 up 2
	{steven_ArcMain_Up1, {  0,   0, 115, 173}, {115,  165}}, //8 up 1
	{steven_ArcMain_Up1, {115,   0, 114, 173}, {114,  164}}, //9 up 2
	
	{steven_ArcMain_Right0, {  0,   0, 168, 166}, {139,  158}}, //10 right 1
	{steven_ArcMain_Right1, {  0,   0, 154, 169}, {134,  162}}, //11 right 2
	{steven_ArcMain_Right2, {  0,   0, 154, 169}, {134,  161}}, //10 right 1
	{steven_ArcMain_Right3, {  0,   0, 154, 168}, {135,  160}}, //11 right 2
};

static const Animation char_steven_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, ASCR_CHGANI, CharAnim_Idle}}, //CharAnim_Idle
	{2, (const u8[]){ 8, 9, 10, 11, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 12, 13, 14, 15, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 16, 17, 18, 19, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 20, 21, 22, 23, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//steven character functions
void Char_steven_SetFrame(void *user, u8 frame)
{
	Char_steven *this = (Char_steven*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_steven_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_steven_Tick(Character *character)
{
	Char_steven *this = (Char_steven*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_steven_SetFrame);

	if (stage.stage_id == StageId_2_1 && stage.song_step <= 775)
		Character_Draw(character, &this->tex, &char_steven_frame[this->frame]);
	else if (stage.stage_id == StageId_2_1 && stage.song_step >= 1045)
		Character_Draw(character, &this->tex, &char_steven_frame[this->frame]);
	else if (stage.stage_id != StageId_2_1)
		Character_Draw(character, &this->tex, &char_steven_frame[this->frame]);
}

void Char_steven_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_steven_Free(Character *character)
{
	Char_steven *this = (Char_steven*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_steven_New(fixed_t x, fixed_t y)
{
	//Allocate steven object
	Char_steven *this = Mem_Alloc(sizeof(Char_steven));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_steven_New] Failed to allocate steven object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_steven_Tick;
	this->character.set_anim = Char_steven_SetAnim;
	this->character.free = Char_steven_Free;
	
	Animatable_Init(&this->character.animatable, char_steven_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(-142 - -120,1);
	this->character.focus_y = FIXED_DEC(25 - 120,1);
	this->character.focus_zoom = FIXED_DEC(8,10);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\STEVEN.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //steven_ArcMain_Idle0
		"idle1.tim", //steven_ArcMain_Idle1
		"idle2.tim", //steven_ArcMain_Idle1
		"idle3.tim", //steven_ArcMain_Idle1
		"idle4.tim", //steven_ArcMain_Idle1
		"idle5.tim", //steven_ArcMain_Idle1
		"idle6.tim", //steven_ArcMain_Idle1
		"idle7.tim", //steven_ArcMain_Idle1
		"left0.tim",  //steven_ArcMain_Left
		"left1.tim",  //steven_ArcMain_Left
		"left2.tim",  //steven_ArcMain_Left
		"left3.tim",  //steven_ArcMain_Left
		"down0.tim",  //steven_ArcMain_Down
		"down1.tim",  //steven_ArcMain_Down
		"up0.tim",    //steven_ArcMain_Up
		"up1.tim",    //steven_ArcMain_Up
		"right0.tim", //steven_ArcMain_Right
		"right1.tim", //steven_ArcMain_Right
		"right2.tim", //steven_ArcMain_Right
		"right3.tim", //steven_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
