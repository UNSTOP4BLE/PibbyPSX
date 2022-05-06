	{ //StageId_1_1 (SAVE-THE-DAY)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_steven_New, FIXED_DEC(-120,1),  FIXED_DEC(120,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(28,10),FIXED_DEC(1,1)},
		1, 1,
		XA_Bopeebo, 0,
		
		StageId_1_2, STAGE_LOAD_FLAG
	},
	{ //StageId_1_2 (glitched-gem)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_steven_New, FIXED_DEC(-120,1),  FIXED_DEC(120,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(27,10),FIXED_DEC(1,1)},
		1, 2,
		XA_Fresh, 2,
		
		StageId_1_3, STAGE_LOAD_OPPONENT
	},
	{ //StageId_1_3 (my-friends)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_spinel_New, FIXED_DEC(-230,1),  FIXED_DEC(80,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		1, 3,
		XA_Dadbattle, 0,
		
		StageId_2_1, STAGE_LOAD_OPPONENT | STAGE_LOAD_OPPONENT2
	},
	{ //StageId_1_4 (NULL)
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_steven_New,  FIXED_DEC(-90,1),   FIXED_DEC(85,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(1,1),FIXED_DEC(1,1)},
		2, 1,
		XA_Spookeez, 0,
		
		StageId_2_2, STAGE_LOAD_FLAG
	},
	
	{ //StageId_2_1 (the-change)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_spinel_New, FIXED_DEC(-180,1),  FIXED_DEC(80,1)},
		{Char_steven_New, FIXED_DEC(-120,1),  FIXED_DEC(120,1)},	
		{NULL},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		2, 1,
		XA_Spookeez, 0,
		
		StageId_2_1, 0
	},
	{ //StageId_2_2 (overworked)
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_mordecai_New,  FIXED_DEC(-90,1),   FIXED_DEC(85,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_park_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(33,10),FIXED_DEC(1,1)},
		2, 2,
		XA_South, 2,
		
		StageId_2_2, 0
	},
	{ //StageId_2_3 (last-spooktober)
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(120,1)},
		{Char_Spook_New,  FIXED_DEC(-90,1),   FIXED_DEC(120,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		2, 3,
		XA_Monster, 0,
		
		StageId_3_1, STAGE_LOAD_OPPONENT | STAGE_LOAD_STAGE
	},
	{ //StageId_3_1 (gunshot)
		//Characters
		{Char_BF_New,     FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_pico_New, FIXED_DEC(-105,1),   FIXED_DEC(85,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_Week3_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(28,10),FIXED_DEC(1,1)},
		3, 1,
		XA_Pico, 0,
		
		StageId_3_2, STAGE_LOAD_OPPONENT | STAGE_LOAD_STAGE
	},
	{ //StageId_3_2 (blue-balled)
		//Characters
		{Char_BF_New,     FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_bfc_New, FIXED_DEC(-70,1),   FIXED_DEC(85,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_Week1_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(28,10),FIXED_DEC(1,1)},
		3, 2,
		XA_Philly, 2,
		
		StageId_3_2, 0
	},
	{ //StageId_3_3 (Corrupted-Hero)
		//Characters
		{Char_BF_New,     FIXED_DEC(56 + 20,1),   FIXED_DEC(85 + 40,1)},
		{Char_finn_New, FIXED_DEC(-105 + 30,1),   FIXED_DEC(85 + 40,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_school_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		3, 3,
		XA_Blammed, 0,
		
		StageId_3_3, 0
	},
	
	{ //StageId_4_1 (Brotherly-Bond)
		//Characters
		{Char_BF_New,     FIXED_DEC(56 + 20,1),   FIXED_DEC(85 + 40,1)},
		{Char_finn_New, FIXED_DEC(-105 + 30,1),   FIXED_DEC(85 + 40,1)},
		{Char_jake_New, FIXED_DEC(-105,1),   FIXED_DEC(85 + 40,1)},
		{NULL},
		
		//Stage background
		Back_school_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		4, 1,
		XA_SatinPanties, 0,
		
		StageId_4_2, STAGE_LOAD_FLAG
	},
	{ //StageId_4_2 (Gummy-Substance)
		//Characters
		{Char_BF_New,     FIXED_DEC(56 + 20,1),   FIXED_DEC(85 + 40,1)},
		{Char_bubble_New, FIXED_DEC(-105 + 30,1),   FIXED_DEC(85 + 40,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_school_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		4, 2,
		XA_High, 2,
		
		StageId_4_3, STAGE_LOAD_FLAG
	},
	{ //StageId_4_3 (together-forever)
		//Characters
		{Char_BF_New,   FIXED_DEC(120,1),   FIXED_DEC(40,1)},
		{Char_steven_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(32,10),FIXED_DEC(1,1)},
		4, 3,
		XA_MILF, 0,
		
		StageId_4_3, 0
	},
	{ //StageId_4_4 (NULL)
		//Characters
		{Char_BF_New,  FIXED_DEC(90,1),   FIXED_DEC(85,1)},
		{Char_steven_New, FIXED_DEC(-190,1),   FIXED_DEC(90,1)},
		{NULL},
		{NULL,              FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(1,1),FIXED_DEC(1,1)},
		5, 1,
		XA_Cocoa, 0,
		
		StageId_5_2, STAGE_LOAD_FLAG
	},
	
	{ //StageId_5_1 (no-hero-remastered)
		//Characters
		{Char_BF_New,  FIXED_DEC(90,1),   FIXED_DEC(85,1)},
		{Char_steven_New, FIXED_DEC(-190,1),   FIXED_DEC(90,1)},
		{NULL},
		{NULL,              FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(32,10),FIXED_DEC(1,1)},
		5, 1,
		XA_Cocoa, 0,
		
		StageId_5_2, STAGE_LOAD_FLAG
	},
	{ //StageId_5_2 (lost-legend)
		//Characters
		{Char_BF_New,  FIXED_DEC(90,1),   FIXED_DEC(85,1)},
		{Char_steven_New, FIXED_DEC(-190,1),   FIXED_DEC(90,1)},
		{NULL},
		{NULL,              FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		5, 2,
		XA_Eggnog, 2,
		
		StageId_5_3, STAGE_LOAD_FLAG
	},
	{ //StageId_5_3 (elastic-destruction)
		//Characters
		{Char_BF_New, FIXED_DEC(105,1),  FIXED_DEC(100,1)},
		{Char_steven_New,   FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		{NULL,              FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		5, 3,
		XA_WinterHorrorland, 0,
		
		StageId_5_3, 0
	},
	
	{ //StageId_6_1 (glitched-duo)
		//Characters
		{Char_BF_New,  FIXED_DEC(52,1),  FIXED_DEC(50,1)},
		{Char_steven_New, FIXED_DEC(-60,1),  FIXED_DEC(50,1)},
		{NULL},
		{NULL,   FIXED_DEC(0,1),  FIXED_DEC(45,1)},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(31,10),FIXED_DEC(1,1)},
		6, 1,
		XA_Senpai, 0,
		
		StageId_6_2, STAGE_LOAD_FLAG | STAGE_LOAD_OPPONENT
	},
	{ //StageId_6_2 (NO-HERO)
		//Characters
		{Char_BF_New,   FIXED_DEC(52,1),  FIXED_DEC(50,1)},
		{Char_steven_New, FIXED_DEC(-60,1),  FIXED_DEC(50,1)},
		{NULL},
		{NULL,    FIXED_DEC(0,1),  FIXED_DEC(45,1)},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(3,1),FIXED_DEC(1,1)},
		6, 2,
		XA_Roses, 2,
		
		StageId_6_3, 0
	},
	{ //StageId_6_3 (you-can't-save-them)
		//Characters
		{Char_BF_New,  FIXED_DEC(52,1),  FIXED_DEC(50,1)},
		{Char_steven_New, FIXED_DEC(-60,1),  FIXED_DEC(50,1)},
		{NULL},
		{NULL,   FIXED_DEC(0,1),  FIXED_DEC(45,1)},
		
		//Stage background
		Back_beach_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(33,10),FIXED_DEC(1,1)},
		6, 3,
		XA_Thorns, 0,
		
		StageId_6_3, 0
	},