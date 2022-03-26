/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "mordecai.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//mordecai character structure
enum
{
	mordecai_ArcMain_Idle0,
	mordecai_ArcMain_Idle1,
	mordecai_ArcMain_Idle2,
	mordecai_ArcMain_Idle3,
	mordecai_ArcMain_Idle4,
	mordecai_ArcMain_Idle5,
	mordecai_ArcMain_Idle6,
	mordecai_ArcMain_Idle7,
	mordecai_ArcMain_Left0,
	mordecai_ArcMain_Left1,
	mordecai_ArcMain_Left2,
	mordecai_ArcMain_Left3,
	mordecai_ArcMain_Down0,
	mordecai_ArcMain_Down1,
	mordecai_ArcMain_Down2,
	mordecai_ArcMain_Down3,
	mordecai_ArcMain_Up0,
	mordecai_ArcMain_Up1,
	mordecai_ArcMain_Up2,
	mordecai_ArcMain_Right0,
	mordecai_ArcMain_Right1,
	mordecai_ArcMain_Right2,
	mordecai_ArcMain_Right3,
	
	mordecai_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[mordecai_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_mordecai;

//mordecai character definitions
static const CharFrame char_mordecai_frame[] = {
	{mordecai_ArcMain_Idle0, {  0,   0, 160, 207}, {119,  205}}, //0 idle 1
	{mordecai_ArcMain_Idle1, {  0,   0, 150, 207}, {113,  206}}, //1 idle 2
	{mordecai_ArcMain_Idle2, {  0,   0, 149, 207}, {113,  206}}, //2 idle 3
	{mordecai_ArcMain_Idle3, {  0,   0, 149, 208}, {112,  205}}, //3 idle 4
	{mordecai_ArcMain_Idle4, {  0,   0, 150, 209}, {115,  205}}, //4 idle 5
	{mordecai_ArcMain_Idle5, {  0,   0, 149, 208}, {114,  204}}, //5 idle 6
	{mordecai_ArcMain_Idle6, {  0,   0, 150, 206}, {114,  204}}, //6 idle 7
	{mordecai_ArcMain_Idle7, {  0,   0, 150, 205}, {113,  202}}, //7 idle 8

	{mordecai_ArcMain_Left0, {  0,   0, 153, 182}, {153,  182}}, //4 left 1
	{mordecai_ArcMain_Left1, {  0,   0, 150, 175}, {150,  172}}, //5 left 2
	{mordecai_ArcMain_Left2, {  0,   0, 163, 174}, {163,  171}}, //4 left 1
	{mordecai_ArcMain_Left3, {  0,   0, 148, 172}, {148,  171}}, //5 left 2

	{mordecai_ArcMain_Down0, {  0,   0, 145, 163}, {100,  155}}, //6 down 1
	{mordecai_ArcMain_Down1, {  0,   0, 144, 164}, {100,  155}}, //7 down 2
	{mordecai_ArcMain_Down2, {  0,   0, 145, 165}, {100,  156}}, //6 down 1
	{mordecai_ArcMain_Down3, {  0,   0, 144, 165}, {100,  157}}, //7 down 2

	{mordecai_ArcMain_Up0, {  0,   0, 128, 233}, { 98,  232}}, //8 up 1
	{mordecai_ArcMain_Up0, {128,   0, 128, 233}, { 98,  232}}, //9 up 2
	{mordecai_ArcMain_Up1, {  0,   0, 133, 230}, { 98,  229}}, //8 up 1
	{mordecai_ArcMain_Up2, {  0,   0, 133, 230}, { 98,  229}}, //9 up 2
	
	{mordecai_ArcMain_Right0, {  0,   0, 256, 154}, {134,  141}}, //10 right 1
	{mordecai_ArcMain_Right1, {  0,   0, 256, 130}, {129,  118}}, //11 right 2
	{mordecai_ArcMain_Right2, {  0,   0, 256, 139}, {128,  126}}, //10 right 1
	{mordecai_ArcMain_Right3, {  0,   0, 256, 145}, {125,  133}}, //11 right 2
};

static const Animation char_mordecai_anim[CharAnim_Max] = {
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

//mordecai character functions
void Char_mordecai_SetFrame(void *user, u8 frame)
{
	Char_mordecai *this = (Char_mordecai*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_mordecai_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_mordecai_Tick(Character *character)
{
	Char_mordecai *this = (Char_mordecai*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_mordecai_SetFrame);
	Character_Draw(character, &this->tex, &char_mordecai_frame[this->frame]);
}

void Char_mordecai_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_mordecai_Free(Character *character)
{
	Char_mordecai *this = (Char_mordecai*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_mordecai_New(fixed_t x, fixed_t y)
{
	//Allocate mordecai object
	Char_mordecai *this = Mem_Alloc(sizeof(Char_mordecai));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_mordecai_New] Failed to allocate mordecai object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_mordecai_Tick;
	this->character.set_anim = Char_mordecai_SetAnim;
	this->character.free = Char_mordecai_Free;
	
	Animatable_Init(&this->character.animatable, char_mordecai_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(-142 - -120,1);
	this->character.focus_y = FIXED_DEC(25 - 120,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\MORDECAI.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //mordecai_ArcMain_Idle0
		"idle1.tim", //mordecai_ArcMain_Idle1
		"idle2.tim", //mordecai_ArcMain_Idle1
		"idle3.tim", //mordecai_ArcMain_Idle1
		"idle4.tim", //mordecai_ArcMain_Idle1
		"idle5.tim", //mordecai_ArcMain_Idle1
		"idle6.tim", //mordecai_ArcMain_Idle1
		"idle7.tim", //mordecai_ArcMain_Idle1
		"left0.tim",  //mordecai_ArcMain_Left
		"left1.tim",  //mordecai_ArcMain_Left
		"left2.tim",  //mordecai_ArcMain_Left
		"left3.tim",  //mordecai_ArcMain_Left
		"down0.tim",  //mordecai_ArcMain_Down
		"down1.tim",  //mordecai_ArcMain_Down
		"down2.tim",  //mordecai_ArcMain_Down
		"down3.tim",  //mordecai_ArcMain_Down
		"up0.tim",    //mordecai_ArcMain_Up
		"up1.tim",    //mordecai_ArcMain_Up
		"up2.tim",    //mordecai_ArcMain_Up
		"right0.tim", //mordecai_ArcMain_Right
		"right1.tim", //mordecai_ArcMain_Right
		"right2.tim", //mordecai_ArcMain_Right
		"right3.tim", //mordecai_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
