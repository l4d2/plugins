#include <dhooks>
#include <sourcemod>

public Plugin myinfo = {
    name = "[L4D2] no master check",
    author = "lakwsh",
    version = "1.0.0"
};

public void OnPluginStart() {
    GameData hGameData = new GameData("l4d2_master_chk");
    if(!hGameData) SetFailState("Failed to load 'l4d2_master_chk.txt' gamedata.");
    DHookSetup hDetour = DHookCreateFromConf(hGameData, "CheckRequestRestart");
    CloseHandle(hGameData);
    if(!hDetour || !DHookEnableDetour(hDetour, false, OnCheckRequestRestart)) SetFailState("Failed to hook CheckRequestRestart");
}

public MRESReturn OnCheckRequestRestart() {
    return MRES_Supercede;
}