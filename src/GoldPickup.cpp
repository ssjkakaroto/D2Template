#include "DLLmain.h"

// Constants for stats and states
#define STAT_GOLD 14
#define STATE_UNINTERRUPTABLE 15
#define PLRMODE_DEATH 11
#define PLRMODE_DEAD 12
#define ITEM_MODE_ON_GROUND 3

// Helper to calculate Chebyshev distance (standard D2 grid distance)
static inline DWORD D2COMMON_GetDistance(int x1, int y1, int x2, int y2) {
    return max(abs(x1 - x2), abs(y1 - y2));
}

/**
 * @brief Inline assembly wrapper for the 1.13c PickGoldPile __usercall
 * @note Original offset 0xD6C80 expects ESI = pPlayer, Stack1 = pGame, Stack2 = pItem
 */
void D2GAME_PickGoldPile_113c(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem) {
    DWORD pfnPickGoldPile = GetDllOffset((char*)"D2Game.dll", DLLBASE_D2GAME, 0xD6C80);

    __asm {
        push esi                 
        
        mov esi, pPlayer         // Pass pPlayer via ESI
        push pItem               // Stack Arg 2
        push pGame               // Stack Arg 1
        
        mov eax, pfnPickGoldPile 
        call eax                 // Callee cleans up 8 bytes via RETN 8
        
        pop esi                  
    }
}

/**
 * @brief Evaluates all items in the player's room and picks up nearby gold
 */
void __fastcall GOLDPICKUP_PickupInRangeGold(D2GameStrc* pGame, D2UnitStrc* pPlayer) {
    if (!pGame || !pPlayer || pPlayer->dwType != UNIT_PLAYER)
        return;

    D2RoomStrc* pRoom = D2COMMON_GetRoomFromUnit(pPlayer);
    if (!pRoom) return;

    if (pPlayer->dwMode == PLRMODE_DEATH || pPlayer->dwMode == PLRMODE_DEAD)
        return;

    if (D2COMMON_GetUnitState(pPlayer, STATE_UNINTERRUPTABLE))
        return;

    for (D2UnitStrc* pUnit = pRoom->pFirstUnit; pUnit; pUnit = pUnit->pRoomNext) {
        if (pUnit->dwType != UNIT_ITEM)
            continue;

        // Trap 2 Fixed: ONLY target items that have physically landed on the floor
        if (pUnit->dwMode != ITEM_MODE_ON_GROUND)
            continue;

        D2ItemsTXT* pItemRecord = D2COMMON_GetItemRecord(pUnit->dwClassId);
        if (!pItemRecord)
            continue;

        // Trap 1 Fixed: Foolproof character check avoids C++ macro endianness bugs
        if (pItemRecord->szCode[0] != 'g' || 
            pItemRecord->szCode[1] != 'l' || 
            pItemRecord->szCode[2] != 'd' || 
            pItemRecord->szCode[3] != ' ') {
            continue;
        }

        D2CoordStrc pPlayerCoords = {0}, pItemCoords = {0};
        D2COMMON_GetUnitCoords(pPlayer, &pPlayerCoords);
        D2COMMON_GetUnitCoords(pUnit, &pItemCoords);

        // Increased pickup radius slightly to account for player run speed / desync
        if (D2COMMON_GetDistance(pPlayerCoords.nX, pPlayerCoords.nY, pItemCoords.nX, pItemCoords.nY) > 20)
            continue;

        DWORD dwCurrentGold = D2COMMON_GetUnitStat(pPlayer, STAT_GOLD, 0);
        if (dwCurrentGold >= D2COMMON_GetInventoryGoldLimit(pPlayer))
            continue;

        // Execute the 1.13c __usercall wrapper
        D2GAME_PickGoldPile_113c(pGame, pPlayer, pUnit);
    }
}

/**
 * @brief Naked hook executed at the beginning of GAME_UpdatePlayer (D2Game+0x79B90)
 */
void __declspec(naked) GAME_UpdatePlayer_Hook() {
    static char szD2Game[] = "D2Game.dll";
    static DWORD dwRetAddr = 0;

    __asm {
        pushad
        
        cmp dword ptr [dwRetAddr], 0
        jne execute_logic
        
        mov ecx, offset szD2Game
        mov edx, DLLBASE_D2GAME
        push 0x79B95
        call GetDllOffset
        mov dword ptr [dwRetAddr], eax

    execute_logic:
        mov ecx, dword ptr [esp + 24] // Original ECX = pGame
        mov edx, dword ptr [esp + 20] // Original EDX = pPlayer

        call GOLDPICKUP_PickupInRangeGold
        
        popad 

        // Execute original instructions overwritten by our 5-byte hook
        sub esp, 0x10
        push ebx
        push ebp

        jmp dword ptr [dwRetAddr]
    }
}