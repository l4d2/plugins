#include <l4d2_ems_hud>
#include <sdkhooks>
#include <sourcemod>

#define TEAM_SURVIVOR 2
#define TEAM_INFECTED 3

bool Enable;

public Plugin myinfo = {
    name = "[L4D2] Hud",
    description = "L4D2 Hud Plugin",
    author = "lakwsh",
    version = "1.0.4",
    url = "https://github.com/lakwsh/l4d2_rmc"
};

public void OnPluginStart() {
    HookEvent("round_start", OnRoundStart, EventHookMode_PostNoCopy);
    HookEvent("player_death", OnPlayerEvent, EventHookMode_PostNoCopy);
    HookEvent("player_hurt", OnPlayerEvent, EventHookMode_PostNoCopy);
    HookEvent("heal_success", OnPlayerEvent, EventHookMode_PostNoCopy);
    HookEvent("revive_success", OnPlayerEvent, EventHookMode_PostNoCopy);
}

public void OnMapStart() {
    char mode[16] = {0};
    FindConVar("mp_gamemode").GetString(mode, sizeof(mode));
    Enable = StrEqual(mode, "coop") || StrEqual(mode, "realism") || StrEqual(mode, "survival");
    HudInit();
}

public void OnRoundStart(Event event, const char[] name, bool dontBroadcast) {
    HudInit();
}

public void OnPlayerEvent(Event event, const char[] name, bool dontBroadcast) {
    if(!Enable) return;
    UpdateHud();
}

void HudInit() {
    if(!Enable) return;
    RemoveAllHUD();
    for(int i = 0; i < HUD_COUNT; i++) HUDPlace(i, 0.0, 0.06 + i * 0.03, 0.5, 0.04);
}

void PadString(char[] buf, int maxlen, int need) {
    int len = strlen(buf);
    int space = (need - len);
    if(space < 1 || space * 2 + len > maxlen - 1) return;
    for(int i = len + space - 1; i < len + space * 2; i++) buf[i] = ' ';
    for(int i = len - 1; i >= 0; i--) buf[i + space] = buf[i];
    for(int i = 0; i < space; i++) buf[i] = ' ';
    buf[len + space * 2] = 0;
}

void UpdateHud() {
    int slot = 0;
    for(int i = 1; i <= MaxClients && slot < HUD_COUNT; i++) {
        if(!IsClientInGame(i) || GetClientTeam(i) != TEAM_SURVIVOR) continue;
        char sHp[2 + 1 + 2 + 1] = {0};
        int hp = IsPlayerAlive(i) ? GetClientHealth(i) : 0;
        IntToString(hp, sHp, sizeof(sHp));
        PadString(sHp, sizeof(sHp), 3);
        int rev = hp ? GetEntProp(i, Prop_Send, "m_currentReviveCount") : 0;
        int flag = HUD_FLAG_ALIGN_LEFT | HUD_FLAG_NOBG | HUD_FLAG_TEAM_SURVIVORS;
        if(rev || GetEntProp(i, Prop_Send, "m_isIncapacitated")) flag |= HUD_FLAG_BLINK;
        HUDSetLayout(slot++, flag, "%s/%d %N", sHp, rev, i);
    }
    for(int i = slot; i < HUD_COUNT; i++) RemoveHUD(i);
}