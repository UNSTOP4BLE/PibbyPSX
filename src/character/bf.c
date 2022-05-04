/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bf.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../random.h"
#include "../main.h"

//Boyfriend skull fragments
static SkullFragment char_bf_skull[15] = {
	{ 1 * 8, -87 * 8, -13, -13},
	{ 9 * 8, -88 * 8,   5, -22},
	{18 * 8, -87 * 8,   9, -22},
	{26 * 8, -85 * 8,  13, -13},
	
	{-3 * 8, -82 * 8, -13, -11},
	{ 8 * 8, -85 * 8,  -9, -15},
	{20 * 8, -82 * 8,   9, -15},
	{30 * 8, -79 * 8,  13, -11},
	
	{-1 * 8, -74 * 8, -13, -5},
	{ 8 * 8, -77 * 8,  -9, -9},
	{19 * 8, -75 * 8,   9, -9},
	{26 * 8, -74 * 8,  13, -5},
	
	{ 5 * 8, -73 * 8, -5, -3},
	{14 * 8, -76 * 8,  9, -6},
	{26 * 8, -67 * 8, 15, -3},
};

//Boyfriend player types
enum
{
	BF_ArcMain_BF0,
	BF_ArcMain_BF1,
	BF_ArcMain_BF2,
	BF_ArcMain_BF3,
	BF_ArcMain_BF4,
	BF_ArcMain_BF5,
	BF_ArcMain_Dead0, //BREAK
	BF_ArcMain_Dead1, //Mic Drop
	BF_ArcMain_Dead2, //Twitch
	BF_ArcMain_Retry, //Retry prompt

	BF_ArcMain_Max,
};


	

#define BF_Arc_Max BF_ArcMain_Max

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	CdlFILE file_dead_arc; //dead.arc file position
	IO_Data arc_ptr[BF_Arc_Max];
	
	Gfx_Tex tex, tex_retry;
	u8 frame, tex_id;
	
	u8 retry_bump;
	
	SkullFragment skull[COUNT_OF(char_bf_skull)];
	u8 skull_scale;
} Char_BF;

//Boyfriend player definitions
static const CharFrame char_bf_frame[] = {
	{BF_ArcMain_BF0, {  0,   0, 116,  97}, { 77,  96}}, //0 idle 1
	{BF_ArcMain_BF0, {116,   0, 114,  98}, { 77,  97}}, //1 idle 2
	{BF_ArcMain_BF0, {  0,  98, 109,  99}, { 74,  98}}, //2 idle 3
	{BF_ArcMain_BF0, {109,  98, 107, 102}, { 72, 102}}, //3 idle 4
	{BF_ArcMain_BF1, {  0,   0, 107, 103}, { 72, 102}}, //4 idle 5
	
	{BF_ArcMain_BF1, {107,   0,  97,  99}, { 72,  98}}, //5 left 1
	{BF_ArcMain_BF1, {  0, 103,  96, 101}, { 70,  100}}, //6 left 2
	
	{BF_ArcMain_BF1, { 96,  99, 106,  91}, { 81,  90}}, //7 down 1
	{BF_ArcMain_BF2, {  0,   0, 107,  90}, { 81,  90}}, //8 down 2
	
	{BF_ArcMain_BF2, {107,   0, 101, 106}, { 75, 105}}, //9 up 1
	{BF_ArcMain_BF2, {  0,  90, 102, 105}, { 77, 105}}, //10 up 2
	
	{BF_ArcMain_BF2, {102, 106,  95, 102}, { 68,  101}}, //11 right 1
	{BF_ArcMain_BF3, {  0,   0,  95, 102}, { 69,  101}}, //12 right 2
	
	{BF_ArcMain_BF4, {  0,   0, 107,  99}, { 82, 98}}, //13 left miss 1
	{BF_ArcMain_BF4, {107,   0, 104,  99}, { 79, 98}}, //14 left miss 2
	
	{BF_ArcMain_BF4, {  0,  99, 105,  88}, { 79, 87}}, //15 down miss 1
	{BF_ArcMain_BF4, {105,  99, 106,  89}, { 81,  88}}, //16 down miss 2
	
	{BF_ArcMain_BF5, {  0, 105, 100, 105}, { 75, 105}}, //17 up miss 1
	{BF_ArcMain_BF5, {100, 106, 100, 106}, { 75, 105}}, //18 up miss 2
	
	{BF_ArcMain_BF5, {  0,   1, 106, 104}, { 75, 104}}, //19 right miss 1
	{BF_ArcMain_BF5, {106,   0, 102, 106}, { 74, 105}}, //20 right miss 2

	{BF_ArcMain_Dead0, {  0,   0, 133, 108}, { 89,  107}}, //21 dead0 0
	{BF_ArcMain_Dead0, {  0, 108, 131, 108}, { 85,  107}}, //22 dead0 1
	{BF_ArcMain_Dead1, {  0,   0, 138, 107}, { 92,  107}}, //23 dead1 0
	{BF_ArcMain_Dead1, {  0, 107, 138, 108}, { 83,  107}}, //24 dead1 1
	
	{BF_ArcMain_Dead2, {  0,   0, 119, 108}, {108 - 42,  107}}, //25 dead2 body twitch 0
	{BF_ArcMain_Dead2, {119,   0, 123, 107}, {110 - 42,  106}}, //26 dead2 body twitch 1
	{BF_ArcMain_Dead2, {  0, 108, 122, 108}, {112 - 42,  107}}, //27 dead2 balls twitch 0
	{BF_ArcMain_Dead2, {122, 107, 122, 107}, {112 - 42,  106}}, //28 dead2 balls twitch 1

	{BF_ArcMain_BF3, { 95,   0, 117, 101}, { 71,  100}}, //12 right 2
	{BF_ArcMain_BF3, {  0, 102, 119, 102}, { 77,  102}}, //12 right 2
	{BF_ArcMain_BF3, {119, 101, 114, 102}, { 72,  101}}, //12 right 2

};

static const Animation char_bf_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6, ASCR_BACK, 1}},             //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_LeftAlt
	{2, (const u8[]){ 7,  8, ASCR_BACK, 1}},             //CharAnim_Down
	{2, (const u8[]){ 29, 30, 31, ASCR_BACK, 0}},       //CharAnim_DownAlt
	{2, (const u8[]){ 9, 10, ASCR_BACK, 1}},             //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UpAlt
	{2, (const u8[]){11, 12, ASCR_BACK, 1}},             //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_RightAlt
	
	{1, (const u8[]){ 5, 13, 13, 14, ASCR_BACK, 1}},     //PlayerAnim_LeftMiss
	{1, (const u8[]){ 7, 15, 15, 16, ASCR_BACK, 1}},     //PlayerAnim_DownMiss
	{1, (const u8[]){ 9, 17, 17, 18, ASCR_BACK, 1}},     //PlayerAnim_UpMiss
	{1, (const u8[]){11, 19, 19, 20, ASCR_BACK, 1}},     //PlayerAnim_RightMiss
	
	{5, (const u8[]){28-7, 29-7, 30-7, 31-7, 31-7, 31-7, 31-7, 31-7, 31-7, 31-7, ASCR_CHGANI, PlayerAnim_Dead1}}, //PlayerAnim_Dead0
	{5, (const u8[]){31-7, ASCR_REPEAT}},                                                       //PlayerAnim_Dead1
	{3, (const u8[]){32-7, 33-7, 34-7, 35-7, 35-7, 35-7, 35-7, 35-7, 35-7, 35-7, ASCR_CHGANI, PlayerAnim_Dead3}}, //PlayerAnim_Dead2
	
	{4, (const u8[]){ 33-7, 34-7, 35-7, ASCR_CHGANI, PlayerAnim_Dead3}}, //PlayerAnim_Dead2

};

//Boyfriend player functions
void Char_BF_SetFrame(void *user, u8 frame)
{
	Char_BF *this = (Char_BF*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bf_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_BF_Tick(Character *character)
{
	Char_BF *this = (Char_BF*)character;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

	fixed_t beat_bop;
	if ((stage.song_step & 0x3) == 0)
		beat_bop = FIXED_UNIT - ((stage.note_scroll / 24) & FIXED_LAND);
	else
		beat_bop = 0;

	//Draw boppers
	static const struct pibby
	{
		RECT src;
		RECT_FIXED dst;
	} lbop_piece[] = {
		{{0, 0, 76, 85}, {FIXED_DEC(10,1), FIXED_DEC(-70,1), FIXED_DEC(76,1), FIXED_DEC(85,1)}},
	};
	
	const struct pibby *lbop_p = lbop_piece;
	for (size_t i = 0; i < COUNT_OF(lbop_piece); i++, lbop_p++)
	{
		RECT_FIXED lbop_dst = {
			lbop_p->dst.x - fx - (beat_bop << 1) + character->x,
			lbop_p->dst.y - fy + (beat_bop << 3) + character->y,
			lbop_p->dst.w + (beat_bop << 2),
			lbop_p->dst.h - (beat_bop << 3),
		};
		if (stage.player_state[0].health != 0 && stage.mode != StageMode_2P)
			Stage_DrawTex(&stage.tex_pibby, &lbop_p->src, &lbop_dst, stage.camera.bzoom);
	}

	//Handle animation updates
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0 ||
	    (character->animatable.anim != CharAnim_Left &&
	     character->animatable.anim != CharAnim_LeftAlt &&
	     character->animatable.anim != CharAnim_Down &&
	     character->animatable.anim != CharAnim_DownAlt &&
	     character->animatable.anim != CharAnim_Up &&
	     character->animatable.anim != CharAnim_UpAlt &&
	     character->animatable.anim != CharAnim_Right &&
	     character->animatable.anim != CharAnim_RightAlt))
		Character_CheckEndSing(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		//Perform idle dance
		if (Animatable_Ended(&character->animatable) &&
			(character->animatable.anim != CharAnim_Left &&
		     character->animatable.anim != CharAnim_LeftAlt &&
		     character->animatable.anim != PlayerAnim_LeftMiss &&
		     character->animatable.anim != CharAnim_Down &&
		     character->animatable.anim != CharAnim_DownAlt &&
		     character->animatable.anim != PlayerAnim_DownMiss &&
		     character->animatable.anim != CharAnim_Up &&
		     character->animatable.anim != CharAnim_UpAlt &&
		     character->animatable.anim != PlayerAnim_UpMiss &&
		     character->animatable.anim != CharAnim_Right &&
		     character->animatable.anim != CharAnim_RightAlt &&
		     character->animatable.anim != PlayerAnim_RightMiss) &&
			(stage.song_step & 0x7) == 0)
			character->set_anim(character, CharAnim_Idle);
	}
	
	//Retry screen
	if (character->animatable.anim >= PlayerAnim_Dead1)
	{
		//Tick skull fragments
		if (this->skull_scale)
		{
			SkullFragment *frag = this->skull;
			for (size_t i = 0; i < COUNT_OF_MEMBER(Char_BF, skull); i++, frag++)
			{
				//Draw fragment
				RECT frag_src = {
					(i & 1) ? 112 : 96,
					(i >> 1) << 4,
					16,
					16
				};
				fixed_t skull_dim = (FIXED_DEC(16,1) * this->skull_scale) >> 6;
				fixed_t skull_rad = skull_dim >> 1;
				RECT_FIXED frag_dst = {
					character->x + (((fixed_t)frag->x << FIXED_SHIFT) >> 3) - skull_rad - stage.camera.x,
					character->y + (((fixed_t)frag->y << FIXED_SHIFT) >> 3) - skull_rad - stage.camera.y,
					skull_dim,
					skull_dim,
				};
				Stage_DrawTex(&this->tex_retry, &frag_src, &frag_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
				
				//Move fragment
				frag->x += frag->xsp;
				frag->y += ++frag->ysp;
			}
			
			//Decrease scale
			this->skull_scale--;
		}
		
		//Draw input options
		u8 input_scale = 16 - this->skull_scale;
		if (input_scale > 16)
			input_scale = 0;
		
		RECT button_src = {
			 0, 96,
			16, 16
		};
		RECT_FIXED button_dst = {
			character->x - FIXED_DEC(32,1) - stage.camera.x,
			character->y - FIXED_DEC(88,1) - stage.camera.y,
			(FIXED_DEC(16,1) * input_scale) >> 4,
			FIXED_DEC(16,1),
		};
		
		//Cross - Retry
		Stage_DrawTex(&this->tex_retry, &button_src, &button_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
		
		//Circle - Blueball
		button_src.x = 16;
		button_dst.y += FIXED_DEC(56,1);
		Stage_DrawTex(&this->tex_retry, &button_src, &button_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
		
		//Draw 'RETRY'
		u8 retry_frame;
		
		if (character->animatable.anim == PlayerAnim_Dead2)
		{
			//Selected retry
			retry_frame = 2 - (this->retry_bump >> 3);
			if (retry_frame >= 3)
				retry_frame = 0;
			if (this->retry_bump & 2)
				retry_frame += 3;
			
			if (++this->retry_bump == 0xFF)
				this->retry_bump = 0xFD;
		}
		else
		{
			//Idle
			retry_frame = 1 +  (this->retry_bump >> 3);
			if (retry_frame >= 3)
				retry_frame = 0;
			
			if (++this->retry_bump >= 55)
				this->retry_bump = 0;
		}
		
		RECT retry_src = {
			(retry_frame & 1) ? 48 : 0,
			(retry_frame >> 1) << 5,
			48,
			32
		};
		RECT_FIXED retry_dst = {
			character->x -  FIXED_DEC(7,1) - stage.camera.x,
			character->y - FIXED_DEC(92,1) - stage.camera.y,
			FIXED_DEC(48,1),
			FIXED_DEC(32,1),
		};
		Stage_DrawTex(&this->tex_retry, &retry_src, &retry_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
	}
	
	//Animate and draw character
	Animatable_Animate(&character->animatable, (void*)this, Char_BF_SetFrame);
	Character_Draw(character, &this->tex, &char_bf_frame[this->frame]);
}

void Char_BF_SetAnim(Character *character, u8 anim)
{
	Char_BF *this = (Char_BF*)character;
	
	//Perform animation checks
	switch (anim)
	{
		case PlayerAnim_Dead0:
			character->focus_x = FIXED_DEC(0,1);
			character->focus_y = FIXED_DEC(-40,1);
			character->focus_zoom = FIXED_DEC(125,100);
			break;
		case PlayerAnim_Dead2:
			//Load retry art
			Gfx_LoadTex(&this->tex_retry, this->arc_ptr[BF_ArcMain_Retry], 0);
			break;
	}
	
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_BF_Free(Character *character)
{
	Char_BF *this = (Char_BF*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_BF_New(fixed_t x, fixed_t y)
{
	//Allocate boyfriend object
	Char_BF *this = Mem_Alloc(sizeof(Char_BF));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_BF_New] Failed to allocate boyfriend object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_BF_Tick;
	this->character.set_anim = Char_BF_SetAnim;
	this->character.free = Char_BF_Free;
	
	Animatable_Init(&this->character.animatable, char_bf_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = CHAR_SPEC_MISSANIM;
	
	this->character.health_i = 0;
	
	this->character.focus_x = FIXED_DEC(-50,1);
	this->character.focus_y = (stage.stage_id == StageId_2_3) ? FIXED_DEC(40 - 120,1) : FIXED_DEC(-65,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BF.ARC;1");

	const char **pathp = (const char *[]){
		"bf0.tim",   //BF_ArcMain_BF0
		"bf1.tim",   //BF_ArcMain_BF1
		"bf2.tim",   //BF_ArcMain_BF2
		"bf3.tim",   //BF_ArcMain_BF3
		"bf4.tim",   //BF_ArcMain_BF4
		"bf5.tim",   //BF_ArcMain_BF5
		"dead0.tim", //BF_ArcMain_Dead0
		"dead1.tim", //BF_ArcDead_Dead1
		"dead2.tim", //BF_ArcDead_Dead2
		"retry.tim", //BF_ArcDead_Retry
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	//Initialize player state
	this->retry_bump = 0;
	
	//Copy skull fragments
	memcpy(this->skull, char_bf_skull, sizeof(char_bf_skull));
	this->skull_scale = 64;
	
	SkullFragment *frag = this->skull;
	for (size_t i = 0; i < COUNT_OF_MEMBER(Char_BF, skull); i++, frag++)
	{
		//Randomize trajectory
		frag->xsp += RandomRange(-4, 4);
		frag->ysp += RandomRange(-2, 2);
	}
	
	return (Character*)this;
}
