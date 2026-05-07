#pragma once

#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H

#include <stdint.h>
#include <windows.h>
#include "D2DataTables.h"
#include "D2PacketDef.h"

#pragma pack(1)

/****************************************************************************
*                                                                           *
*   D2Structs.h                                                             *
*                                                                           *
*   Licensed under the Apache License, Version 2.0 (the "License");         *
*   you may not use this file except in compliance with the License.        *
*   You may obtain a copy of the License at                                 *
*                                                                           *
*   http://www.apache.org/licenses/LICENSE-2.0                              *
*                                                                           *
*   Unless required by applicable law or agreed to in writing, software     *
*   distributed under the License is distributed on an "AS IS" BASIS,       *
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
*   See the License for the specific language governing permissions and     *
*   limitations under the License.                                          *
*                                                                           *
*---------------------------------------------------------------------------*
*                                                                           *
*   https://github.com/olivier-verville/D2Template                          *
*                                                                           *
*   This file is used to declare data structures used by the game to        *
*   represent various entities, such as the data structure representing     *
*   a unit entity, or a game entity                                         *
*                                                                           *
*****************************************************************************/

/****************************************************************************
*                                                                           *
* DECLARATIONS                                                              *
*                                                                           *
*****************************************************************************/

struct D2GameStrc;
struct D2UnitStrc;
struct D2RoomStrc;
struct D2CoordStrc;
struct D2ItemsTXT;

struct MonsterData;
struct ObjectData;
struct ItemData;
struct PlayerData;
struct CBPlayerData;
struct CBItemData;
struct Path;
struct Stats;
struct Inventory;
struct Skills;

/****************************************************************************
*                                                                           *
* DEFINITIONS                                                               *
*                                                                           *
*****************************************************************************/

// Game structure (Opaque for this patch, we just pass the pointer around)
struct D2GameStrc {
    DWORD _dummy;
};

struct D2UnitStrc
{										//Offset from Code.		Size: 0xF4+4
	DWORD		dwType;				    //+00
	union{
		DWORD			dwClassId;
		DWORD			nTxtFileNo;
    };									//+04
	DWORD		pMemPool;				//+08
	DWORD		dwUnitId;				//+0C
	DWORD		dwMode;					//+10
	union{
		MonsterData*	ptMonsterData;
		ObjectData*		ptObjectData;
		ItemData*		ptItemData;
		PlayerData*		ptPlayerData;
	};									//+14
	BYTE		act;					//+18
	BYTE		uk12[3];				//+19
	void*		ptAct;					//+1C
	DWORD		SeedLow;				//+20
	DWORD		SeedHi;					//+24
	DWORD		SeedStarting;			//+28
	Path*		path;					//+2C
	BYTE		uk1[0x08];				//+30
	Path*		ptPos;					//+38
	DWORD		animSpeed;				//+3C
	BYTE		uk2[0x04];				//+40
	DWORD		curFrame;				//+44
	DWORD		remainingFrame;			//+48
	WORD		animSpeedW;				//+4C
	BYTE		actionFlag;				//+4E
	BYTE		uk3[0x1];				//+4F
	void*		ptAnimData;				//+50
	BYTE		uk4[0x08];				//+54
	Stats*		ptStats;				//+5C
	Inventory*	ptInventory;		    //+60
	BYTE		uk5[0x1C];				//+64
	D2GameStrc*	ptGame;					//+80
	BYTE		uk6[0x24];				//+84
	Skills*		ptSkills;				//+A8
	void*		ptCombatData;			//+AC
	BYTE		uk7[0x08];				//+B0
	DWORD		itemCode;				//+B8 for an item being dropped by this unit
	BYTE		uk8[0x08];				//+BC
	DWORD		flags1;					//+C4
	union{
		DWORD		flags2;				//+C8	//flags
		struct{
			DWORD	ukf1:25;
			DWORD	isLod:1;
		};
	};
	BYTE		uk9[0x04];				//+CC
	DWORD		clientId;				//+D0
	BYTE		uk10[0x0C];				//+D4
	D2UnitStrc*	ptFirstMonster;			//+E0
	D2UnitStrc*	Unit1;					//+E4
	D2UnitStrc*	pRoomNext;      		//+E8
	BYTE		uk11[0x08];				//+EC
	union{
		CBPlayerData*	ptCBPlayerData;
		CBItemData*		ptCBItemData;
		void*			ptCBData;
	};									//+F4
};

struct D2RoomStrc // size=0x80 (Active Room / Room1)
{
    D2RoomStrc**    pNearRooms;         //+0x00
    DWORD           __04[3];            //+0x04
    void*           pRoomEx;            //+0x10 (Pointer to RoomEx/Room2)
    DWORD           __14[3];            //+0x14
    void*           pCollisionMap;      //+0x20 
    INT32           nNearRooms;         //+0x24
    DWORD           __28[9];            //+0x28
    DWORD           dwStartX;           //+0x4C
    DWORD           dwStartY;           //+0x50
    DWORD           dwSizeX;            //+0x54
    DWORD           dwSizeY;            //+0x58
    DWORD           __5C[6];            //+0x5C
    D2UnitStrc*     pFirstUnit;         //+0x74 (Linked list of units in the room)
    DWORD           __78;               //+0x78
    D2RoomStrc*     pNextRoom;          //+0x7C
};

struct D2CoordStrc
{
    DWORD nX; // +0x00
    DWORD nY; // +0x04
};

struct D2ItemsTXT
{
	char szFlippyFile[32];					//0x00
	char szInvFile[32];						//0x20
	char szUniqueInvFile[32];				//0x40
	char szSetInvFile[32];					//0x60
	union
	{
		uint32_t dwCode;					//0x80
		char szCode[4];						//0x80
	};
	uint32_t dwNormCode;					//0x84
	uint32_t dwUberCode;					//0x88
	uint32_t dwUltraCode;					//0x8C
	uint32_t dwAlternateGfx;				//0x90
	uint32_t dwPspell;						//0x94
	uint16_t wState;						//0x98
	uint16_t wCurseState[2];				//0x9A
	uint16_t wStat[3];						//0x9E
	uint32_t dwCalc[3];						//0xA4
	uint32_t dwLen;							//0xB0
	uint8_t nSpellDesc;						//0xB4
	uint8_t pad0xB5;						//0xB5
	uint16_t wSpellDescStr;					//0xB6
	uint32_t dwSpellDescCalc;				//0xB8
	uint32_t dwBetterGem;					//0xBC
	uint32_t dwWeapClass;					//0xC0
	uint32_t dwWeapClass2Hand;				//0xC4
	uint32_t dwTransmogrifyType;			//0xC8
	int32_t dwMinAc;						//0xCC
	int32_t dwMaxAc;						//0xD0
	uint32_t dwGambleCost;					//0xD4
	int32_t dwSpeed;						//0xD8
	uint32_t dwBitField1;					//0xDC
	uint32_t dwCost;						//0xE0
	uint32_t dwMinStack;					//0xE4
	uint32_t dwMaxStack;					//0xE8
	uint32_t dwSpawnStack;					//0xEC
	uint32_t dwGemOffset;					//0xF0
	uint16_t wNameStr;						//0xF4
	uint16_t wVersion;						//0xF6
	uint16_t wAutoPrefix;					//0xF8
	uint16_t wMissileType;					//0xFA
	uint8_t nRarity;						//0xFC
	uint8_t nLevel;							//0xFD
	uint8_t nMinDam;						//0xFE
	uint8_t nMaxDam;						//0xFF
	uint8_t nMinMisDam;						//0x100
	uint8_t nMaxMisDam;						//0x101
	uint8_t n2HandMinDam;					//0x102
	uint8_t n2HandMaxDam;					//0x103
	uint8_t nRangeAdder;					//0x104
	uint8_t unk0x105;						//0x105
	int16_t nStrBonus;						//0x106
	int16_t nDexBonus;						//0x108
	uint16_t wReqStr;						//0x10A
	uint16_t wReqDex;						//0x10C
	uint8_t nAbsorb;						//0x10E
	uint8_t nInvWidth;						//0x10F
	uint8_t nInvHeight;						//0x110
	uint8_t nBlock;							//0x111
	uint8_t nDurability;					//0x112
	uint8_t nNoDurability;					//0x113
	uint8_t nMissile;						//0x114
	uint8_t nComponent;						//0x115
	uint8_t nArmorComp[6];					//0x116
	uint8_t n2Handed;						//0x11C
	uint8_t nUseable;						//0x11D
	int16_t wType[2];						//0x11E
	uint8_t nSubType;						//0x122
	uint8_t unk0x123;						//0x123
	uint16_t wDropSound;					//0x124
	uint16_t wUseSound;						//0x126
	uint8_t nDropSfxFrame;					//0x128
	uint8_t nUnique;						//0x129
	uint8_t nQuest;							//0x12A
	uint8_t nQuestDiffCheck;				//0x12B
	uint8_t nTransparent;					//0x12C
	uint8_t nTransTbl;						//0x12D
	uint8_t pad0x12E;						//0x12E
	uint8_t nLightRadius;					//0x12F
	uint8_t nBelt;							//0x130
	uint8_t nAutoBelt;						//0x131
	uint8_t nStackable;						//0x132
	uint8_t nSpawnable;						//0x133
	uint8_t nSpellIcon;						//0x134
	uint8_t nDurWarning;					//0x135
	uint8_t nQuantityWarning;				//0x136
	uint8_t nHasInv;						//0x137
	uint8_t nGemSockets;					//0x138
	uint8_t nTransmogrify;					//0x139
	uint8_t nTmogMin;						//0x13A
	uint8_t nTmogMax;						//0x13B
	uint8_t nHitClass;						//0x13C
	uint8_t n1or2Handed;					//0x13D
	uint8_t nGemApplyType;					//0x13E
	uint8_t nLevelReq;						//0x13F
	uint8_t nMagicLevel;					//0x140
	int8_t nTransform;						//0x141
	int8_t nInvTrans;						//0x142
	uint8_t nCompactSave;					//0x143
	uint8_t nSkipName;						//0x144
	uint8_t nNameable;						//0x145
	uint8_t nVendorMin[17];					//0x146
	uint8_t nVendorMax[17];					//0x157
	uint8_t nVendorMagicMin[17];			//0x168
	uint8_t nVendorMagicMax[17];			//0x179
	uint8_t nVendorMagicLvl[17];			//0x18A
	uint8_t pad0x19B;						//0x19B
	uint32_t dwNightmareUpgrade;			//0x19C
	uint32_t dwHellUpgrade;					//0x1A0
	uint8_t nPermStoreItem;					//0x1A4
	uint8_t nMultibuy;						//0x1A5
	uint16_t pad0x1A6;						//0x1A6
};

// end of file --------------------------------------------------------------
#pragma pack()
#endif