//-----------------------------------------------------------------------------
//
// Header for target library libacs.
//
// Defines from Zandronum's ACS.
//
// This file incorporates definitions from ZDoom's acc's zdefs.acs.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__ACS_Zandronum__defs_h__
#define __GDCC_Header__C__ACS_Zandronum__defs_h__

#include <ACS_ZDoom/defs.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define TEAM_BLUE 0
#define TEAM_RED  1
#define NO_TEAM   2

#define TPROP_Name            0
#define TPROP_Score           1
#define TPROP_IsValid         2
#define TPROP_NumPlayers      3
#define TPROP_NumLivePlayers  4
#define TPROP_TextColor       5
#define TPROP_PlayerStartNum  6
#define TPROP_Spread          7
#define TPROP_Carrier         8
#define TPROP_Assister        9
#define TPROP_FragCount      10
#define TPROP_DeathCount     11
#define TPROP_WinCount       12
#define TPROP_PointCount     13
#define TPROP_ReturnTics     14
#define TPROP_TeamItem       15
#define TPROP_WinnerTheme    16
#define TPROP_LoserTheme     17

#define IS_WAITINGFORPLAYERS 0
#define IS_FIRSTCOUNTDOWN    1
#define IS_INPROGRESS        2
#define IS_BOSSFIGHT         3
#define IS_WAVECOMPLETE      4
#define IS_COUNTDOWN         5

#define T_DARKIMP         155
#define T_BLOODDEMON      156
#define T_SSGGUY          157
#define T_HECTEBUS        158
#define T_CACOLANTERN     159
#define T_PISTOL          162
#define T_GRENADELAUNCHER 163
#define T_RAILGUN         164
#define T_BFG10000        165
#define T_MAXHEALTHBONUS  166
#define T_MAXARMORBONUS   167
#define T_REDARMOR        168
#define T_TURBOSPHERE     169
#define T_ANTIGRAVBELT    170
#define T_TIMEFREEZER     171
#define T_INFRAGOGGLES    172
#define T_INFRATRACKER    173
#define T_TRANSLUCENCY    174
#define T_DOOMSPHERE      175
#define T_RANDOMPOWERUP   176
#define T_BLUEFLAG        177
#define T_REDFLAG         178
#define T_WHITEFLAG       179
#define T_STRENGTH        180
#define T_RAGE            181
#define T_DRAIN           182
#define T_SPREAD          183
#define T_RESISTANCE      184
#define T_REGENERATION    185
#define T_PROSPERITY      186
#define T_REFLECTION      187
#define T_HIGHJUMP        188
#define T_HASTE           189
#define T_MINIGUN         214
#define T_BELPHEGOR       215
#define T_GRENADE         216
#define T_BFG10KSHOT      217
#define T_DARKIMPFIREBALL 218
#define T_CACOLANTERNSHOT 219
#define T_ABADDON         220
#define T_ABADDONSHOT     221

#define DB_ORDER_ASC  0
#define DB_ORDER_DESC 1

#define TM_SECOND  0
#define TM_MINUTE  1
#define TM_HOUR    2
#define TM_DAY     3
#define TM_MONTH   4
#define TM_YEAR    5
#define TM_WEEKDAY 6

#define GAMELIMIT_FRAGS  0
#define GAMELIMIT_TIME   1
#define GAMELIMIT_POINTS 2
#define GAMELIMIT_DUELS  3
#define GAMELIMIT_WINS   4
#define GAMELIMIT_WAVES  5

#define SCORE_FRAGS   0
#define SCORE_POINTS  1
#define SCORE_WINS    2
#define SCORE_DEATHS  3
#define SCORE_KILLS   4
#define SCORE_ITEMS   5
#define SCORE_SECRETS 6
#define SCORE_SPREAD  7
#define SCORE_RANK    8

#define MAPROTATION_NAME       0
#define MAPROTATION_LUMPNAME   1
#define MAPROTATION_USED       2
#define MAPROTATION_MINPLAYERS 3
#define MAPROTATION_MAXPLAYERS 4

#define MAPPOSITION_CURRENT 0
#define MAPPOSITION_NEXT    1

#define LEAVEREASON_LEFT      0
#define LEAVEREASON_KICKED    1
#define LEAVEREASON_ERROR     2
#define LEAVEREASON_TIMEOUT   3
#define LEAVEREASON_RECONNECT 4

#define PLAYERSKIN_USERINFO 0
#define PLAYERSKIN_WEAPON   1
#define PLAYERSKIN_ACS      2
#define PLAYERSKIN_VISIBLE  3

#define POINTINFO_NAME     0
#define POINTINFO_OWNER    1
#define POINTINFO_DISABLED 2

#define SPROP_TYPE_UNKNOWN 0
#define SPROP_TYPE_INT     1
#define SPROP_TYPE_BOOL    2
#define SPROP_TYPE_FLOAT   3
#define SPROP_TYPE_STRING  4

#define GAMESTATE_UNSPECIFIED     -1
#define GAMESTATE_WAITFORPLAYERS   0
#define GAMESTATE_COUNTDOWN        1
#define GAMESTATE_INPROGRESS       2
#define GAMESTATE_INRESULTSEQUENCE 3

#define GAMEEVENT_PLAYERFRAGS            0
#define GAMEEVENT_MEDALS                 1
#define GAMEEVENT_CAPTURES               2
#define GAMEEVENT_TOUCHES                3
#define GAMEEVENT_RETURNS                4
#define GAMEEVENT_ROUND_STARTS           5
#define GAMEEVENT_ROUND_ENDS             6
#define GAMEEVENT_ROUND_ABORTED          7
#define GAMEEVENT_CHAT                   8
#define GAMEEVENT_PLAYERCONNECT          9
#define GAMEEVENT_ACTOR_SPAWNED         10
#define GAMEEVENT_ACTOR_DAMAGED         11
#define GAMEEVENT_ACTOR_ARMORDAMAGED    12
#define GAMEEVENT_ACTOR_DAMAGED_PREMOD  12
#define GAMEEVENT_DOMINATION_CONTROL    13
#define GAMEEVENT_DOMINATION_POINT      14
#define GAMEEVENT_PLAYERLEAVESSERVER    15
#define GAMEEVENT_LEVEL_INIT            16
#define GAMEEVENT_JOINQUEUECHANGED      17
#define GAMEEVENT_DOMINATION_PRECONTROL 18
#define GAMEEVENT_DOMINATION_CONTEST    19

#define GAMEEVENT_SPAWN_LEVELSPAWNED  0x00000001
#define GAMEEVENT_SPAWN_RANDOMSPAWNED 0x00000002

#define AAPTR_PLAYER_GETFLOATYICON 0x04000000
#define AAPTR_PLAYER_GETCAMERA     0x08000000
#define AAPTR_DAMAGE_SOURCE        0x10000000
#define AAPTR_DAMAGE_INFLICTOR     0x40000000
#define AAPTR_DAMAGE_TARGET        0x80000000

#define PLAYERCOUNTRY_ALPHA2 0
#define PLAYERCOUNTRY_ALPHA3 1
#define PLAYERCOUNTRY_NAME   2

#define STATUS_CHATTING  0x00000001
#define STATUS_TALKING   0x00000002
#define STATUS_INCONSOLE 0x00000004
#define STATUS_INMENU    0x00000008
#define STATUS_LAGGING   0x00000010

#endif//__GDCC_Header__C__ACS_Zandronum__defs_h__

