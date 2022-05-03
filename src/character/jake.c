/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "jake.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//jake character structure
enum
{
	jake_ArcMain_Idle0,
	jake_ArcMain_Idle1,
	jake_ArcMain_Idle2,
	jake_ArcMain_Idle3,
	jake_ArcMain_Idle4,
    jake_ArcMain_Idle5,
	jake_ArcMain_Idle6,
	jake_ArcMain_Left0,
	jake_ArcMain_Left1,
    jake_ArcMain_Left2,
	jake_ArcMain_Left3,
	jake_ArcMain_Down,
	jake_ArcMain_Up0,
	jake_ArcMain_Up1,
	jake_ArcMain_Right0,
	jake_ArcMain_Right1,
	jake_ArcMain_Right2,
	jake_ArcMain_Right3,


	jake_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[jake_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_jake;

//jake character definitions
static const CharFrame char_jake_frame[] = {
	{jake_ArcMain_Idle0, {  0,   0,   0,   0}, {  0,   0}}, //0 idle 1
	{jake_ArcMain_Idle1, {  0,   0,   0,   0}, {  0,   0}}, //1 idle 2
	{jake_ArcMain_Idle2, {  0,   0,   0,   0}, {  0,   0}}, //2 idle 3
	{jake_ArcMain_Idle3, {  0,   0,   0,   0}, {  0,   0}}, //3 idle 4
	{jake_ArcMain_Idle4, {  0,   0,   0,   0}, {  0,   0}}, //4 idle 5
    {jake_ArcMain_Idle5, {  0,   0,   0,   0}, {  0,   0}}, //2 idle 3
	{jake_ArcMain_Idle6, {  0,   0,   0,   0}, {  0,   0}}, //3 idle 4

	{jake_ArcMain_Left0, {  0,   0, 252, 174}, {187, 144}}, //6 left 1
	{jake_ArcMain_Left1, {  0,   0, 248, 176}, {180, 176}}, //7 left 2
    {jake_ArcMain_Left2, {  0,   0, 248, 176}, {180, 176}}, //6 left 1
	{jake_ArcMain_Left3, {  0,   0, 248, 173}, {181, 173}}, //7 left 2
	
	{jake_ArcMain_Down, {  0,   0, 119,  84}, {119,  84}}, //8 down 1
    {jake_ArcMain_Down, {119,   0, 119,  84}, {119,  84}}, //8 down 1
    {jake_ArcMain_Down, {  0,  84, 125,  84}, {125,  84}}, //8 down 1
    {jake_ArcMain_Down, {  0, 168, 136,  87}, {136,  84}}, //8 down 1
	
	{jake_ArcMain_Up0, {  0,   0, 119, 143}, {119, 143}}, //10 up 1
	{jake_ArcMain_Up0, {119,   0, 121, 144}, {119, 144}}, //11 up 2
    {jake_ArcMain_Up1, {  0,   0, 126, 141}, {126, 141}}, //10 up 1
	{jake_ArcMain_Up1, {126,   0, 117, 144}, {117, 144}}, //11 up 2
	
	{jake_ArcMain_Right0, {  0,   0, 176, 160}, {115, 160}}, //12 right 1
	{jake_ArcMain_Right1, {  0,   0, 172, 155}, {115, 155}}, //13 right 2
    {jake_ArcMain_Right2, {  0,   0, 172, 148}, {116, 148}}, //12 right 1
	{jake_ArcMain_Right3, {  0,   0, 176, 148}, {115, 148}}, //13 right 2
};

static const Animation char_jake_anim[CharAnim_Max] = {
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

static void DrawJake(Character *this, Gfx_Tex *tex, const CharFrame *cframe, fixed_t parallax)
{
	//Draw character
	fixed_t x = this->x - FIXED_MUL(stage.camera.x, parallax) - FIXED_DEC(cframe->off[0],1);
	fixed_t y = this->y - FIXED_MUL(stage.camera.y, parallax) - FIXED_DEC(cframe->off[1],1);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {x, y, (src.w + 64) << FIXED_SHIFT, (src.h + 45) << FIXED_SHIFT};
	Stage_DrawTex(tex, &src, &dst, stage.camera.bzoom);
}
void Jake_Draw(Character *this, Gfx_Tex *tex, const CharFrame *cframe)
{
	DrawJake(this, tex, cframe, FIXED_UNIT);
}

//jake character functions
void Char_jake_SetFrame(void *user, u8 frame)
{
	Char_jake *this = (Char_jake*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_jake_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_jake_Tick(Character *character)
{
	Char_jake *this = (Char_jake*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_jake_SetFrame);
	Character_Draw(character, &this->tex, &char_jake_frame[this->frame]);
}


void Char_jake_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_jake_Free(Character *character)
{
	Char_jake *this = (Char_jake*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_jake_New(fixed_t x, fixed_t y)
{
	//Allocate jake object
	Char_jake *this = Mem_Alloc(sizeof(Char_jake));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_jake_New] Failed to allocate jake object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_jake_Tick;
	this->character.set_anim = Char_jake_SetAnim;
	this->character.free = Char_jake_Free;
	
	Animatable_Init(&this->character.animatable, char_jake_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 5;
	
	this->character.focus_x = FIXED_DEC(-142 - -120,1);
	this->character.focus_y = FIXED_DEC(25 - 120,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\JAKE.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //jake_ArcMain_Idle0
		"idle1.tim", //jake_ArcMain_Idle1
		"idle2.tim", //jake_ArcMain_Idle1
		"idle3.tim", //jake_ArcMain_Idle1
		"idle4.tim", //jake_ArcMain_Idle1
        "idle5.tim", //jake_ArcMain_Idle1
        "idle6.tim", //jake_ArcMain_Idle1
		"left0.tim", //jake_ArcMain_Idle1
		"left1.tim", //jake_ArcMain_Idle1
        "left2.tim", //jake_ArcMain_Idle1
        "left3.tim", //jake_ArcMain_Idle1
		"down.tim", //jake_ArcMain_Idle1
		"up0.tim", //jake_ArcMain_Idle1
		"up1.tim", //jake_ArcMain_Idle1
		"right0.tim", //jake_ArcMain_Idle1
		"right1.tim", //jake_ArcMain_Idle1
        "right2.tim", //jake_ArcMain_Idle1
		"right3.tim", //jake_ArcMain_Idle1

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}