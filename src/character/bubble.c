/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bubble.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//bubble character structure
enum
{
	bubble_ArcMain_Idle0,
	bubble_ArcMain_Idle1,
	bubble_ArcMain_Idle2,
	bubble_ArcMain_Idle3,
	bubble_ArcMain_Left0,
	bubble_ArcMain_Left1,
	bubble_ArcMain_Down0,
    bubble_ArcMain_Down1,
    bubble_ArcMain_Down2,
	bubble_ArcMain_Up0,
	bubble_ArcMain_Up1,
	bubble_ArcMain_Right0,
	bubble_ArcMain_Right1,


	bubble_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[bubble_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_bubble;

//bubble character definitions
static const CharFrame char_bubble_frame[] = {
	{bubble_ArcMain_Idle0, {  0,   0, 148, 134}, {119, 129}}, //0 idle 1
    {bubble_ArcMain_Idle0, {  0,   0, 148, 134}, {119, 129}}, //0 idle 1
	{bubble_ArcMain_Idle1, {  0,   0, 152, 137}, {120, 132}}, //1 idle 2
    {bubble_ArcMain_Idle1, {  0,   0, 152, 137}, {120, 132}}, //1 idle 2
	{bubble_ArcMain_Idle2, {  0,   0, 160, 133}, {125, 128}}, //2 idle 3
    {bubble_ArcMain_Idle2, {  0,   0, 160, 133}, {125, 128}}, //2 idle 3
	{bubble_ArcMain_Idle3, {  0,   0, 168, 137}, {136, 132}}, //3 idle 4
    {bubble_ArcMain_Idle3, {  0,   0, 168, 137}, {136, 132}}, //3 idle 4

	{bubble_ArcMain_Left0, {  0,   0, 252, 174}, {187, 169}}, //6 left 1
	{bubble_ArcMain_Left0, {  0,   0, 248, 176}, {180, 170}}, //7 left 2
    {bubble_ArcMain_Left1, {  0,   0, 248, 176}, {180, 171}}, //6 left 1
	{bubble_ArcMain_Left1, {  0,   0, 248, 173}, {181, 168}}, //7 left 2
	
	{bubble_ArcMain_Down0, {  0,   0, 119,  84}, {119,  80}}, //8 down 1
    {bubble_ArcMain_Down0, {119,   0, 119,  84}, {119,  80}}, //8 down 1
    {bubble_ArcMain_Down1, {  0,  84, 125,  84}, {125,  80}}, //8 down 1
    {bubble_ArcMain_Down2, {  0, 168, 136,  87}, {136,  80}}, //8 down 1
	
	{bubble_ArcMain_Up0, {  0,   0, 119, 143}, {119, 138}}, //10 up 1
	{bubble_ArcMain_Up0, {119,   0, 121, 144}, {119, 139}}, //11 up 2
    {bubble_ArcMain_Up1, {  0,   0, 126, 141}, {126, 136}}, //10 up 1
	{bubble_ArcMain_Up1, {126,   0, 117, 144}, {117, 139}}, //11 up 2
	
	{bubble_ArcMain_Right0, {  0,   0, 176, 160}, {115, 155}}, //12 right 1
	{bubble_ArcMain_Right0, {  0,   0, 172, 155}, {115, 150}}, //13 right 2
    {bubble_ArcMain_Right1, {  0,   0, 172, 148}, {116, 143}}, //12 right 1
	{bubble_ArcMain_Right1, {  0,   0, 176, 148}, {115, 143}}, //13 right 2
};

static const Animation char_bubble_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 7, 8, 9, 10, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 11, 12, 13, 14, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 15, 16, 17, 18, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 19, 20, 21, 22, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//bubble character functions
void Char_bubble_SetFrame(void *user, u8 frame)
{
	Char_bubble *this = (Char_bubble*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bubble_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_bubble_Tick(Character *character)
{
	Char_bubble *this = (Char_bubble*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_bubble_SetFrame);
	Character_Draw(character, &this->tex, &char_bubble_frame[this->frame]);
}


void Char_bubble_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_bubble_Free(Character *character)
{
	Char_bubble *this = (Char_bubble*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_bubble_New(fixed_t x, fixed_t y)
{
	//Allocate bubble object
	Char_bubble *this = Mem_Alloc(sizeof(Char_bubble));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_bubble_New] Failed to allocate bubble object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_bubble_Tick;
	this->character.set_anim = Char_bubble_SetAnim;
	this->character.free = Char_bubble_Free;
	
	Animatable_Init(&this->character.animatable, char_bubble_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 5;
	
	this->character.focus_x = FIXED_DEC(-142 - -120,1);
	this->character.focus_y = FIXED_DEC(25 - 120,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BUBBLE.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //bubble_ArcMain_Idle0
		"idle1.tim", //bubble_ArcMain_Idle1
		"idle2.tim", //bubble_ArcMain_Idle1
		"idle3.tim", //bubble_ArcMain_Idle1
		"left0.tim", //bubble_ArcMain_Idle1
		"left1.tim", //bubble_ArcMain_Idle1
		"down0.tim", //bubble_ArcMain_Idle1
        "down1.tim", //bubble_ArcMain_Idle1
        "down2.tim", //bubble_ArcMain_Idle1
		"up0.tim", //bubble_ArcMain_Idle1
		"up1.tim", //bubble_ArcMain_Idle1
		"right0.tim", //bubble_ArcMain_Idle1
		"right1.tim", //bubble_ArcMain_Idle1

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}