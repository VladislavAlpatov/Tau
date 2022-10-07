#pragma once
#include "Hooks.h"
#include <MinHook.h>
#include "../../Utils/memory.h"
#include <d3d9.h>	
#include <memory>
#include "../../Overlay/COverlay.h"
#include "../../SSDK/SSDK.h"
#include "../Aimbot/AimBot.h"

enum ETFCond
{
	TF_COND_INVALID = -1,
	TF_COND_AIMING = 0,		// Sniper aiming, Heavy minigun.
	TF_COND_ZOOMED,
	TF_COND_DISGUISING,
	TF_COND_DISGUISED,
	TF_COND_STEALTHED,		// Spy specific
	TF_COND_INVULNERABLE,
	TF_COND_TELEPORTED,
	TF_COND_TAUNTING,
	TF_COND_INVULNERABLE_WEARINGOFF,
	TF_COND_STEALTHED_BLINK,
	TF_COND_SELECTED_TO_TELEPORT,
	TF_COND_CRITBOOSTED,	// DO NOT RE-USE THIS -- THIS IS FOR KRITZKRIEG AND REVENGE CRITS ONLY
	TF_COND_TMPDAMAGEBONUS,
	TF_COND_FEIGN_DEATH,
	TF_COND_PHASE,
	TF_COND_STUNNED,		// Any type of stun. Check iStunFlags for more info.
	TF_COND_OFFENSEBUFF,
	TF_COND_SHIELD_CHARGE,
	TF_COND_DEMO_BUFF,
	TF_COND_ENERGY_BUFF,
	TF_COND_RADIUSHEAL,
	TF_COND_HEALTH_BUFF,
	TF_COND_BURNING,
	TF_COND_HEALTH_OVERHEALED,
	TF_COND_URINE,
	TF_COND_BLEEDING,
	TF_COND_DEFENSEBUFF,	// 35% defense! No crit damage.
	TF_COND_MAD_MILK,
	TF_COND_MEGAHEAL,
	TF_COND_REGENONDAMAGEBUFF,
	TF_COND_MARKEDFORDEATH,
	TF_COND_NOHEALINGDAMAGEBUFF,
	TF_COND_SPEED_BOOST,				// = 32
	TF_COND_CRITBOOSTED_PUMPKIN,		// Brandon hates bits
	TF_COND_CRITBOOSTED_USER_BUFF,
	TF_COND_CRITBOOSTED_DEMO_CHARGE,
	TF_COND_SODAPOPPER_HYPE,
	TF_COND_CRITBOOSTED_FIRST_BLOOD,	// arena mode first blood
	TF_COND_CRITBOOSTED_BONUS_TIME,
	TF_COND_CRITBOOSTED_CTF_CAPTURE,
	TF_COND_CRITBOOSTED_ON_KILL,		// =40. KGB, etc.
	TF_COND_CANNOT_SWITCH_FROM_MELEE,
	TF_COND_DEFENSEBUFF_NO_CRIT_BLOCK,	// 35% defense! Still damaged by crits.
	TF_COND_REPROGRAMMED,				// Bots only
	TF_COND_CRITBOOSTED_RAGE_BUFF,
	TF_COND_DEFENSEBUFF_HIGH,			// 75% defense! Still damaged by crits.
	TF_COND_SNIPERCHARGE_RAGE_BUFF,		// Sniper Rage - Charge time speed up
	TF_COND_DISGUISE_WEARINGOFF,		// Applied for half-second post-disguise
	TF_COND_MARKEDFORDEATH_SILENT,		// Sans sound
	TF_COND_DISGUISED_AS_DISPENSER,
	TF_COND_SAPPED,						// =50. Bots only
	TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGED,
	TF_COND_INVULNERABLE_USER_BUFF,
	TF_COND_HALLOWEEN_BOMB_HEAD,
	TF_COND_HALLOWEEN_THRILLER,
	TF_COND_RADIUSHEAL_ON_DAMAGE,
	TF_COND_CRITBOOSTED_CARD_EFFECT,
	TF_COND_INVULNERABLE_CARD_EFFECT,
	TF_COND_MEDIGUN_UBER_BULLET_RESIST,
	TF_COND_MEDIGUN_UBER_BLAST_RESIST,
	TF_COND_MEDIGUN_UBER_FIRE_RESIST,		// =60
	TF_COND_MEDIGUN_SMALL_BULLET_RESIST,
	TF_COND_MEDIGUN_SMALL_BLAST_RESIST,
	TF_COND_MEDIGUN_SMALL_FIRE_RESIST,
	TF_COND_STEALTHED_USER_BUFF,			// Any class can have this
	TF_COND_MEDIGUN_DEBUFF,
	TF_COND_STEALTHED_USER_BUFF_FADING,
	TF_COND_BULLET_IMMUNE,
	TF_COND_BLAST_IMMUNE,
	TF_COND_FIRE_IMMUNE,
	TF_COND_PREVENT_DEATH,					// =70
	TF_COND_MVM_BOT_STUN_RADIOWAVE, 		// Bots only
	TF_COND_HALLOWEEN_SPEED_BOOST,
	TF_COND_HALLOWEEN_QUICK_HEAL,
	TF_COND_HALLOWEEN_GIANT,
	TF_COND_HALLOWEEN_TINY,
	TF_COND_HALLOWEEN_IN_HELL,
	TF_COND_HALLOWEEN_GHOST_MODE,			// =77
	TF_COND_MINICRITBOOSTED_ON_KILL,
	TF_COND_OBSCURED_SMOKE,
	TF_COND_PARACHUTE_DEPLOYED,				// =80
	TF_COND_BLASTJUMPING,
	TF_COND_HALLOWEEN_KART,
	TF_COND_HALLOWEEN_KART_DASH,
	TF_COND_BALLOON_HEAD,					// =84 larger head, lower-gravity-feeling jumps
	TF_COND_MELEE_ONLY,						// =85 melee only
	TF_COND_SWIMMING_CURSE,					// player movement become swimming movement
	TF_COND_FREEZE_INPUT,					// freezes player input
	TF_COND_HALLOWEEN_KART_CAGE,			// attach cage model to player while in kart
	TF_COND_DONOTUSE_0,
	TF_COND_RUNE_STRENGTH,
	TF_COND_RUNE_HASTE,
	TF_COND_RUNE_REGEN,
	TF_COND_RUNE_RESIST,
	TF_COND_RUNE_VAMPIRE,
	TF_COND_RUNE_REFLECT,
	TF_COND_RUNE_PRECISION,
	TF_COND_RUNE_AGILITY,
	TF_COND_GRAPPLINGHOOK,
	TF_COND_GRAPPLINGHOOK_SAFEFALL,
	TF_COND_GRAPPLINGHOOK_LATCHED,
	TF_COND_GRAPPLINGHOOK_BLEEDING,
	TF_COND_AFTERBURN_IMMUNE,
	TF_COND_RUNE_KNOCKOUT,
	TF_COND_RUNE_IMBALANCE,
	TF_COND_CRITBOOSTED_RUNE_TEMP,
	TF_COND_PASSTIME_INTERCEPTION,
	TF_COND_SWIMMING_NO_EFFECTS,			// =107_DNOC_FT
	TF_COND_PURGATORY,
	TF_COND_RUNE_KING,
	TF_COND_RUNE_PLAGUE,
	TF_COND_RUNE_SUPERNOVA,
	TF_COND_PLAGUE,
	TF_COND_KING_BUFFED,
	TF_COND_TEAM_GLOWS,						// used to show team glows to living players
	TF_COND_KNOCKED_INTO_AIR,
	TF_COND_COMPETITIVE_WINNER,
	TF_COND_COMPETITIVE_LOSER,
	TF_COND_HEALING_DEBUFF,
	TF_COND_PASSTIME_PENALTY_DEBUFF,		// when carrying the ball without any teammates nearby	
	TF_COND_GRAPPLED_TO_PLAYER,
	TF_COND_GRAPPLED_BY_PLAYER,
	//
	// ADD NEW ITEMS HERE TO AVOID BREAKING DEMOS
	//

	// ******** Keep this block last! ********
	// Keep experimental conditions below and graduate out of it before shipping
#ifdef STAGING_ONLY
	TF_COND_NO_COMBAT_SPEED_BOOST,		// STAGING_ENGY
	TF_COND_TRANQ_SPY_BOOST,			// STAGING_SPY
	TF_COND_TRANQ_MARKED,
	//	TF_COND_SPACE_GRAVITY,
	//	TF_COND_SELF_CONC,
	TF_COND_ROCKETPACK,
	TF_COND_STEALTHED_PHASE,
	TF_COND_CLIP_OVERLOAD,
	TF_COND_SPY_CLASS_STEAL,
#endif // STAGING_ONLY

	TF_COND_LAST
};


static void* oPresentAddr = NULL;
static void* oCreateMove  = NULL;
static void* oInCond     = NULL;
static std::unique_ptr<COverlay> pOverlay;

int __stdcall  hkPresent(LPDIRECT3DDEVICE9 pDevice, int a2, int a3, int a4, int a5)
{
	if (not pOverlay)
		pOverlay = std::unique_ptr<COverlay>(new COverlay(pDevice));

	pOverlay->Render();


	typedef int(__stdcall* Present)(LPDIRECT3DDEVICE9, int, int, int, int);
	return reinterpret_cast<Present>(oPresentAddr)(pDevice, a2, a3, a4, a5);
}

bool __stdcall hkCreateMove(float fSampleTime, SSDK::CUserCmd* pUserCmd)
{
	if (!SSDK::GetLocalPlayer())
		return false;

	typedef bool(__stdcall* CreateMovet)(float, void*);
	// reinterpret_cast<CreateMovet>(oCreateMove)(fSampleTime, pUserCmd);

	SSDK::fix_usercmd guard(pUserCmd);
	Hacks::CAimBot(pUserCmd).Work();

	return false;
}

void Hooks::Attach()
{
	MH_Initialize();
	auto present = (LPVOID)Memory::FindPattern("d3d9.dll", "?? ?? ?? ?? ?? 83 E4 F8 51 51 56 8B 75 08 8B CE F7 D9 57 1B C9 8D 46 04 23 C8 6A ?? 51 8D 4C 24 10 E8 ?? ?? ?? ?? F7 46 ?? ?? ?? ?? ?? 74 07 BF ?? ?? ?? ?? EB 17");
	auto createMove = (LPVOID)Memory::FindPattern("client.dll", "55 8B EC E8 ? ? ? ? 8B C8 85 C9 75 06");


	MH_CreateHook(present,    hkPresent,    &oPresentAddr);
	MH_CreateHook(createMove, hkCreateMove, &oCreateMove);
	// MH_CreateHook( (BYTE*)(GetModuleHandleA("client.dll")) + 0x4BF040, hInCond, &oInCond);
	MH_EnableHook(MH_ALL_HOOKS);

}

void Hooks::Detach()
{
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}
