#if defined _l4d2_ems_hud_included
 #endinput
#endif
#define _l4d2_ems_hud_included

// ====================================================================================================
// "L4D2 EMS HUD Functions" by "sorallll, lakwsh"
//	Version: 1.1.0
// ====================================================================================================

#include <sdktools>

// CDirectorChallengeMode::AddEnumsToScope
#define HUD_LEFT_TOP	0
#define HUD_LEFT_BOT	1
#define HUD_MID_TOP		2
#define HUD_MID_BOT		3
#define HUD_RIGHT_TOP	4
#define HUD_RIGHT_BOT	5
#define HUD_TICKER		6
#define HUD_FAR_LEFT	7
#define HUD_FAR_RIGHT	8
#define HUD_MID_BOX		9
#define HUD_SCORE_TITLE	10
#define HUD_SCORE_1		11
#define HUD_SCORE_2		12
#define HUD_SCORE_3		13
#define HUD_SCORE_4		14
#define HUD_COUNT		15

// custom flags for background, time, alignment, which team, pre or postfix, etc
#define HUD_FLAG_PRESTR			(1<<0)	//	do you want a string/value pair to start(pre) or end(post) with the static string (default is PRE)
#define HUD_FLAG_POSTSTR		(1<<1)	//	ditto
#define HUD_FLAG_BEEP			(1<<2)	//	Makes a countdown timer blink
#define HUD_FLAG_BLINK			(1<<3)  //	do you want this field to be blinking
#define HUD_FLAG_AS_TIME		(1<<4)	//	to do..
#define HUD_FLAG_COUNTDOWN_WARN	(1<<5)	//	auto blink when the timer gets under 10 seconds
#define HUD_FLAG_NOBG			(1<<6) 	//	dont draw the background box for this UI element
#define HUD_FLAG_ALLOWNEGTIMER	(1<<7) 	//	by default Timers stop on 0:00 to avoid briefly going negative over network, this keeps that from happening
#define HUD_FLAG_ALIGN_LEFT		(1<<8)
#define HUD_FLAG_ALIGN_CENTER	(1<<9)
#define HUD_FLAG_ALIGN_RIGHT	(1<<8)|(1<<9)
#define HUD_FLAG_TEAM_SURVIVORS	(1<<10) //	only show to the survivor team
#define HUD_FLAG_TEAM_INFECTED	(1<<11) //	only show to the special infected team
#define HUD_FLAG_TEAM_MASK		(1<<10)|(1<<11)
#define HUD_FLAG_STR			(1<<13)
#define HUD_FLAG_NOTVISIBLE		(1<<14)

/**
 * Passes a table that defines your in-game HUD to the engine.
 * CScriptHud::HUDSetLayout(CScriptHud *, int).
 *
 * @param slot			HUD slot.
 * @param flags			flags
 * @param dataval		used for data of destination string buffer.
 * @param ...			Variable number of format parameters.
 * @noreturn
 * @error				Invalid HUD slot.
 */
stock void HUDSetLayout(int slot, int flags, const char[] dataval, any ...) {
	static char str[128];
	VFormat(str, sizeof str, dataval, 4);
	ServerCommand("sv_hud %d %d", slot, flags|HUD_FLAG_STR);
	GameRules_SetPropString("m_szScriptedHUDStringSet", str, true, slot);
}

/**
 * Place a slot in game.
 * CScriptHud::HUDSetPosition(CScriptHud *__hidden this, int, float, float, float, float).
 *
 * @param slot			HUD slot.
 * @param x				screen x position.
 * @param y				screen y position.
 * @param width			screen slot width.
 * @param height		screen slot height.
 * @noreturn
 * @error				Invalid HUD slot.
 */
stock void HUDPlace(int slot, float x, float y, float width, float height) {
	GameRules_SetPropFloat("m_fScriptedHUDPosX", x, slot, true);
	GameRules_SetPropFloat("m_fScriptedHUDPosY", y, slot, true);
	GameRules_SetPropFloat("m_fScriptedHUDWidth", width, slot, true);
	GameRules_SetPropFloat("m_fScriptedHUDHeight", height, slot, true);
}

/**
 * Removes a slot from game.
 * CTerrorGameRules::ResetScriptedHUD(CTerrorGameRules *__hidden this).
 *
 * @param slot			HUD slot.
 * @noreturn
 * @error				Invalid HUD slot.
 */
stock void RemoveHUD(int slot) {
	ServerCommand("sv_hud %d %d", slot, HUD_FLAG_NOTVISIBLE);
}

/**
 * Returns if a Slot is Used
 *
 * @param slot			HUD slot.
 * @return				True if a Slot is Used, false otherwise.
 * @error				Invalid HUD slot.
 */
stock bool HUDSlotIsUsed(int slot) {
	return !(GameRules_GetProp("m_iScriptedHUDFlags", _, slot)&HUD_FLAG_NOTVISIBLE);
}

/**
 * Remove All slot from game.
 */
stock void RemoveAllHUD() {
	for (int slot; slot < HUD_COUNT; slot++) RemoveHUD(slot);
}