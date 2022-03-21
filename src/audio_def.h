#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(11295)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3840)},  //XA_GameOver
	//WEEK1A.XA
	{XA_Week1A, XA_LENGTH(12900)}, //XA_Bopeebo
	{XA_Week1A, XA_LENGTH(11100)}, //XA_Fresh
	//WEEK1B.XA
	{XA_Week1B, XA_LENGTH(13400)}, //XA_Dadbattle
	//WEEK2A.XA
	{XA_Week2A, XA_LENGTH(15000)}, //XA_Spookeez
	{XA_Week2A, XA_LENGTH(14300)}, //XA_South
	//WEEK2B.XA
	{XA_Week2B, XA_LENGTH(13900)}, //XA_Monster
	//WEEK3A.XA
	{XA_Week3A, XA_LENGTH(13100)},  //XA_Pico
	{XA_Week3A, XA_LENGTH(14800)}, //XA_Philly
	//WEEK3B.XA
	{XA_Week3B, XA_LENGTH(15900)}, //XA_Blammed
	//WEEK4A.XA
	{XA_Week4A, XA_LENGTH(14300)},  //XA_SatinPanties
	{XA_Week4A, XA_LENGTH(11400)}, //XA_High
	//WEEK4B.XA
	{XA_Week4B, XA_LENGTH(21700)}, //XA_MILF
	//WEEK5A.XA
	{XA_Week5A, XA_LENGTH(19300)}, //XA_Cocoa
	{XA_Week5A, XA_LENGTH(12800)},  //XA_Eggnog
	//WEEK5B.XA
	{XA_Week5B, XA_LENGTH(20500)}, //XA_WinterHorrorland
	//WEEK6A.XA
	{XA_Week6A, XA_LENGTH(19700)}, //XA_Senpai
	{XA_Week6A, XA_LENGTH(17800)}, //XA_Roses
	//WEEK6B.XA
	{XA_Week6B, XA_LENGTH(15900)}, //XA_Thorns
};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\WEEK1A.XA;1", //XA_Week1A
	"\\MUSIC\\WEEK1B.XA;1", //XA_Week1B
	"\\MUSIC\\WEEK2A.XA;1", //XA_Week2A
	"\\MUSIC\\WEEK2B.XA;1", //XA_Week2B
	"\\MUSIC\\WEEK3A.XA;1", //XA_Week3A
	"\\MUSIC\\WEEK3B.XA;1", //XA_Week3B
	"\\MUSIC\\WEEK4A.XA;1", //XA_Week4A
	"\\MUSIC\\WEEK4B.XA;1", //XA_Week4B
	"\\MUSIC\\WEEK5A.XA;1", //XA_Week5A
	"\\MUSIC\\WEEK5B.XA;1", //XA_Week5B
	"\\MUSIC\\WEEK6A.XA;1", //XA_Week6A
	"\\MUSIC\\WEEK6B.XA;1", //XA_Week6B
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freaky", false},   //XA_GettinFreaky
	{"gameover", false}, //XA_GameOver
	//WEEK1A.XA
	{"bopeebo", true}, //XA_Bopeebo
	{"fresh", true},   //XA_Fresh
	//WEEK1B.XA
	{"dadbattle", true}, //XA_Dadbattle
	//WEEK2A.XA
	{"spookeez", true}, //XA_Spookeez
	{"south", true},    //XA_South
	//WEEK2B.XA
	{"monster", true}, //XA_Monster
	//WEEK3A.XA
	{"pico", true},   //XA_Pico
	{"philly", true}, //XA_Philly
	//WEEK3B.XA
	{"blammed", true}, //XA_Blammed
	//WEEK4A.XA
	{"satinpanties", true}, //XA_SatinPanties
	{"high", true},         //XA_High
	//WEEK4B.XA
	{"milf", true}, //XA_MILF
	//WEEK5A.XA
	{"cocoa", true},  //XA_Cocoa
	{"eggnog", true}, //XA_Eggnog
	//WEEK5B.XA
	{"winterhorrorland", true}, //XA_WinterHorrorland
	//WEEK6A.XA
	{"senpai", true}, //XA_Senpai
	{"roses", true},  //XA_Roses
	//WEEK6B.XA
	{"thorns", true}, //XA_Thorns
	
	{NULL, false}
};
