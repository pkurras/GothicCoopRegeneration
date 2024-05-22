// This file added in headers queue
// File: "Sources.h"
#include "resource.h"
#include <string>
#include <vector>
#include <unordered_map>

#define MAX_PLAYERS 4

namespace GOTHIC_ENGINE
{
	int32 ManaRegenerationTime = INT_MAX;
	int32 ManaRegenerationPercent = 0;
	int32 HPRegenerationTime = INT_MAX;
	int32 HPRegenerationPercent = 0;
	int32 OutOfCombatMultiplier = 1;
	int32 StreakBonusPercent = 0;
	int32 StreakCount = 0;
	int32 StreakResetTime = 0;

	std::vector<oCNpc*> Npcs;

	void Game_Entry(void)
	{ }

	struct HitStreak
	{
		int32 StreakCount = 0;
		RegenerationTimer HitStreakTimer;
	};

	std::unordered_map<oCNpc*, zCView*> PlayerViews;
	std::unordered_map<oCNpc*, HitStreak> HitStreakMap;
	
	typedef zCView* (*Gothic2SplitscreenCoop_GetPlayerView)(int PlayerID);
	Gothic2SplitscreenCoop_GetPlayerView GetPlayerView = nullptr;
	
	typedef oCNpc* (*Gothic2SplitscreenCoop_GetPlayer)(int PlayerID);
	Gothic2SplitscreenCoop_GetPlayer GetPlayer = nullptr;
	
	typedef zCCamera* (*Gothic2SplitscreenCoop_GetPlayerCam)(int PlayerID);
	Gothic2SplitscreenCoop_GetPlayerCam GetPlayerCam = nullptr;

	bool IsStreakBonusActive(void)
	{
		return StreakBonusPercent > 0 &&
			StreakCount > 0 &&
			StreakResetTime > 0;
	}
	bool IsManaRegenerationActive(void)
	{
		return ManaRegenerationTime > 0 && ManaRegenerationPercent > 0;
	}
	bool IsHPRegenerationActive(void)
	{
		return HPRegenerationTime > 0 && HPRegenerationPercent > 0;
	}

	void Game_Init(void)
	{
		ManaRegenerationTime = zoptions->ReadInt("GOTHIC_COOP_REGENERATION", "ManaRegenerationTime", 5);
		ManaRegenerationPercent = zoptions->ReadInt("GOTHIC_COOP_REGENERATION", "ManaRegenerationPercent", 1);
		HPRegenerationTime = zoptions->ReadInt("GOTHIC_COOP_REGENERATION", "HPRegenerationTime", 5);
		HPRegenerationPercent = zoptions->ReadInt("GOTHIC_COOP_REGENERATION", "HPRegenerationPercent", 1);
		OutOfCombatMultiplier = zoptions->ReadInt("GOTHIC_COOP_REGENERATION", "OutOfCombatMultiplier", 3);
		StreakBonusPercent = zoptions->ReadInt("GOTHIC_COOP_REGENERATION", "StreakBonusPercent", 10);
		StreakCount = zoptions->ReadInt("GOTHIC_COOP_REGENERATION", "StreakCount", 10);
		StreakResetTime = zoptions->ReadInt("GOTHIC_COOP_REGENERATION", "StreakResetTime", 5);
		
		HMODULE CoopHandle = GetModuleHandle(TEXT("Gothic2SplitscreenCoop.dll"));
		
		if (!CoopHandle) return;
		
		GetPlayerView = reinterpret_cast<Gothic2SplitscreenCoop_GetPlayerView>(
			GetProcAddress(CoopHandle, "GetPlayerView")
		);
		GetPlayer = reinterpret_cast<Gothic2SplitscreenCoop_GetPlayer>(
			GetProcAddress(CoopHandle, "GetPlayer")
		);
		GetPlayerCam = reinterpret_cast<Gothic2SplitscreenCoop_GetPlayerCam>(
			GetProcAddress(CoopHandle, "GetPlayerCam")
		);
	}

	void Game_Exit(void)
	{ }

	void Game_PreLoop(void)
	{ }

	RegenerationTimer HealthTimer;
	RegenerationTimer ManaTimer;
	
	template <typename T>
	T Clamp(T value, T min, T max)
	{
		return (value < min) * min +
			(value > max) * max +
			(value >= min && value <= max) * value;
	}

	void Game_Loop(void)
	{
		if (IsHPRegenerationActive() && (HealthTimer.Elapsed() >= HPRegenerationTime))
		{
			for (auto& Npc : Npcs)
			{
				if (!Npc || Npc->IsDead() || Npc->IsUnconscious()) continue;
		
				float Multiplier = 1.0f;
				if (OutOfCombatMultiplier > 0 && Npc->IsInFightMode_S(NPC_WEAPON_NONE))
					Multiplier = static_cast<float>(OutOfCombatMultiplier);
		
				auto Value = static_cast<int32>(roundf((Npc->attribute[NPC_ATR_HITPOINTSMAX] * 0.01f * HPRegenerationPercent * Multiplier)));
				Value = Clamp(Value, 1, Npc->attribute[NPC_ATR_HITPOINTSMAX]);
				Npc->attribute[NPC_ATR_HITPOINTS] = Clamp(Npc->attribute[NPC_ATR_HITPOINTS] + Value, 0, Npc->attribute[NPC_ATR_HITPOINTSMAX]);
			}
		
			HealthTimer.Reset();
		}
		if (IsManaRegenerationActive() && (ManaTimer.Elapsed() >= ManaRegenerationTime))
		{
			for (auto& Npc : Npcs)
			{
				if (!Npc || Npc->IsDead() || Npc->IsUnconscious()) continue;
		
				float Multiplier = 1.0f;
				if (OutOfCombatMultiplier > 0 && Npc->IsInFightMode_S(NPC_WEAPON_NONE))
					Multiplier = static_cast<float>(OutOfCombatMultiplier);
		
				auto Value = static_cast<int32>(roundf((Npc->attribute[NPC_ATR_MANAMAX] * 0.01f * ManaRegenerationPercent * Multiplier)));
				Value = Clamp(Value, 1, Npc->attribute[NPC_ATR_MANAMAX]);
				Npc->attribute[NPC_ATR_MANA] = Clamp(Npc->attribute[NPC_ATR_MANA] + Value, 0, Npc->attribute[NPC_ATR_MANAMAX]);
			}
		
			ManaTimer.Reset();
		}
		
		if (!IsStreakBonusActive()) return;

		for (auto& StreakMapPair : HitStreakMap)
		{
			// check if hit streak timer is greater than StreakResetTime
			if (StreakResetTime >= 1 &&
				StreakMapPair.second.HitStreakTimer.Elapsed() >= StreakResetTime &&
				StreakMapPair.second.StreakCount > 0)
			{
				// reset hit streak
				StreakMapPair.second.StreakCount = 0;
				StreakMapPair.second.HitStreakTimer.Reset();

				zCView* View = PlayerViews[StreakMapPair.first];
				if (View) View->PrintTimed(100, 100, string::Combine("Kombo: %ix", StreakMapPair.second.StreakCount), 1000.0f, nullptr);
			}

			// check if hit streak is greater than StreakCount
			if (StreakMapPair.second.StreakCount >= StreakCount)
			{
				// check if player is alive
				auto Player = StreakMapPair.first;
				if (!Player || Player->IsDead() || Player->IsUnconscious()) continue;

				// check if player is in fight mode (without magic)
				if (!Player->IsInFightMode_S(NPC_WEAPON_NONE) && !Player->IsInFightMode_S(NPC_WEAPON_MAG))
				{
					// add 10% to health
					auto Value = static_cast<int32>(roundf((Player->attribute[NPC_ATR_HITPOINTSMAX] * StreakBonusPercent * 0.01f)));
					Value = Clamp(Player->attribute[NPC_ATR_HITPOINTS] + Value, 1, Player->attribute[NPC_ATR_HITPOINTSMAX]);
					Player->attribute[NPC_ATR_HITPOINTS] = Value;

					zCView* View = PlayerViews[Player];
					if (View) View->PrintTimed(100, 300, string::Combine("+%i%% Lebensenergie-Bonus für %ier Kombo", StreakBonusPercent, StreakMapPair.second), 3000.0f, nullptr);
				}
				else if (Player->IsInFightMode_S(NPC_WEAPON_MAG))
				{
					// add 10% to mana
					auto Value = static_cast<int32>(roundf((Player->attribute[NPC_ATR_MANAMAX] * StreakBonusPercent * 0.01f)));
					Value = Clamp(Player->attribute[NPC_ATR_MANA] + Value, 1, Player->attribute[NPC_ATR_MANAMAX]);
					Player->attribute[NPC_ATR_MANA] = Value;

					zCView* View = PlayerViews[Player];
					if (View) View->PrintTimed(100, 300, string::Combine("+%i%% Mana-Bonus für %ier Kombo", StreakBonusPercent, StreakMapPair.second), 3000.0f, nullptr);
				}

				// reset hit streak
				StreakMapPair.second.StreakCount = 0;
			}
		}
	}

	void Game_PostLoop(void)
	{ }

	void Game_MenuLoop(void)
	{ }

	// Information about current saving or loading world
	TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

	void Game_SaveBegin(void)
	{ }

	void Game_SaveEnd(void)
	{ }

	void LoadBegin(void)
	{ }

	void LoadEnd(void)
	{
		Npcs.clear();
		if (IsStreakBonusActive())
			HitStreakMap.clear();
		PlayerViews.clear();

		Npcs.push_back(oCNpc::player);

		if (GetPlayer)
		{
			for (uint32 idx = 1; idx <= MAX_PLAYERS - 1; idx++)
			{
				oCNpc* PlayerNpc = GetPlayer(idx);
				if (PlayerNpc) Npcs.push_back(PlayerNpc);
			}
		}

		// Main player hit streak insertion
		if (IsStreakBonusActive())
		{
			HitStreak MainPlayerCombo{ };
			HitStreakMap.emplace(oCNpc::player, MainPlayerCombo);
			PlayerViews.emplace(oCNpc::player, ogame->array_view[oCGame::oEGameDialogView::GAME_VIEW_SCREEN]);
		}
		
		for (uint32 idx = 1; idx <= MAX_PLAYERS - 1; idx++)
		{
			if (!GetPlayer) continue;
		
			oCNpc* PlayerNpc = GetPlayer(idx);
			if (!PlayerNpc) continue;
		
			// coop player hit streak insertion
			if (IsStreakBonusActive())
			{
				HitStreak SubPlayerCombo{ };
				HitStreakMap.emplace(PlayerNpc, SubPlayerCombo);
			}
		
			// Try to find coop player views
			if (!GetPlayerView) continue;
		
			// coop player views found! add them to the map.
			zCView* PlayerView = GetPlayerView(idx);
			PlayerViews.emplace(PlayerNpc, PlayerView);
		}
	}

	void Game_LoadBegin_NewGame(void)
	{
		LoadBegin();
	}

	void Game_LoadEnd_NewGame(void)
	{
		LoadEnd();
	}

	void Game_LoadBegin_SaveGame(void)
	{
		LoadBegin();
	}

	void Game_LoadEnd_SaveGame(void)
	{
		LoadEnd();
	}

	void Game_LoadBegin_ChangeLevel(void)
	{
		LoadBegin();
	}

	void Game_LoadEnd_ChangeLevel(void)
	{
		LoadEnd();
	}

	void Game_LoadBegin_Trigger(void)
	{ }

	void Game_LoadEnd_Trigger(void)
	{ }

	void Game_Pause(void)
	{ }

	void Game_Unpause(void)
	{ }

	void Game_DefineExternals(void)
	{ }

	void Game_ApplyOptions(void)
	{ }

	/*
	Functions call order on Game initialization:
	  - Game_Entry           * Gothic entry point
	  - Game_DefineExternals * Define external script functions
	  - Game_Init            * After DAT files init

	Functions call order on Change level:
	  - Game_LoadBegin_Trigger     * Entry in trigger
	  - Game_LoadEnd_Trigger       *
	  - Game_Loop                  * Frame call window
	  - Game_LoadBegin_ChangeLevel * Load begin
	  - Game_SaveBegin             * Save previous level information
	  - Game_SaveEnd               *
	  - Game_LoadEnd_ChangeLevel   *

	Functions call order on Save game:
	  - Game_Pause     * Open menu
	  - Game_Unpause   * Click on save
	  - Game_Loop      * Frame call window
	  - Game_SaveBegin * Save begin
	  - Game_SaveEnd   *

	Functions call order on Load game:
	  - Game_Pause              * Open menu
	  - Game_Unpause            * Click on load
	  - Game_LoadBegin_SaveGame * Load begin
	  - Game_LoadEnd_SaveGame   *
	*/

	HOOK Hook_oCNpc_OnDamage_Hit PATCH(&oCNpc::OnDamage_Hit, &oCNpc::OnDamage_Hit_Union);
	void oCNpc::OnDamage_Hit_Union(oSDamageDescriptor& descDamage)
	{
		THISCALL(Hook_oCNpc_OnDamage_Hit)(descDamage);
	
		if (!IsStreakBonusActive()) return;
	
		if (HitStreakMap.count(descDamage.pNpcAttacker) != 0)
		{
			HitStreakMap.at(descDamage.pNpcAttacker).StreakCount += 1;
			zCView* View = PlayerViews[descDamage.pNpcAttacker];
			if (View) View->PrintTimed(100, 100, string::Combine("Kombo: %ix", HitStreakMap.at(descDamage.pNpcAttacker).StreakCount), 1000.0f, nullptr);
			HitStreakMap.at(descDamage.pNpcAttacker).HitStreakTimer.Reset();
		}
	
		if (HitStreakMap.count(this) != 0)
			HitStreakMap[this].StreakCount = 0;
	}

#define AppDefault True
	CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
		Enabled(FALSE) Game_Entry,
		Enabled(TRUE) Game_Init,
		Enabled(FALSE) Game_Exit,
		Enabled(FALSE) Game_PreLoop,
		Enabled(TRUE) Game_Loop,
		Enabled(FALSE) Game_PostLoop,
		Enabled(FALSE) Game_MenuLoop,
		Enabled(FALSE) Game_SaveBegin,
		Enabled(FALSE) Game_SaveEnd,
		Enabled(FALSE) Game_LoadBegin_NewGame,
		Enabled(TRUE) Game_LoadEnd_NewGame,
		Enabled(FALSE) Game_LoadBegin_SaveGame,
		Enabled(TRUE) Game_LoadEnd_SaveGame,
		Enabled(FALSE) Game_LoadBegin_ChangeLevel,
		Enabled(TRUE) Game_LoadEnd_ChangeLevel,
		Enabled(FALSE) Game_LoadBegin_Trigger,
		Enabled(FALSE) Game_LoadEnd_Trigger,
		Enabled(FALSE) Game_Pause,
		Enabled(FALSE) Game_Unpause,
		Enabled(FALSE) Game_DefineExternals,
		Enabled(FALSE) Game_ApplyOptions
	);
}