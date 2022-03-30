/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_PLAYER_H
#define PSXF_GUARD_PLAYER_H

#include "character.h"

//Player enums
typedef enum
{
	PlayerAnim_LeftMiss = CharAnim_Max,
	PlayerAnim_DownMiss,
	PlayerAnim_UpMiss,
	PlayerAnim_RightMiss,
	PlayerAnim_Dead0, //BREAK
	PlayerAnim_Dead1, //Idle with mic
	PlayerAnim_Dead2, //Mic Drop
	PlayerAnim_Dead3, //Mic Drop
	

	PlayerAnim_Max,
} PlayerAnim;

//Player structures
typedef struct
{
	s16 x, y;
	s16 xsp, ysp;
} SkullFragment;

#endif
