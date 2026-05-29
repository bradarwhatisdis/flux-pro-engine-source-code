#pragma once

struct MatchInfo {
    bool set;
    string Tier;
    int32_t arg10, arg11;
} lastMatchInfo;

DEFINE(void*, StartMatch, ptr arg1, int64_t arg2, string arg3, int64_t arg4, int32_t arg5, int32_t arg6, int64_t arg7, int32_t arg8, char arg9, int32_t arg10, int32_t arg11) {
    LOGI("StartMatch");

    LOGI("Tier %s", arg3.c_str());
    LOGI("arg4 %p, arg5 %p, arg6 %p, arg7 %p, arg8 %p, arg9 %p, arg10 %p, arg11 %p", arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
    
    lastMatchInfo = { true, arg3, arg10, arg11 };

    return _StartMatch(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
}

std::map<string, int64_t> modeBets = {
    {"M1", 50},
    {"M2", 100},
    {"M3", 500}, // Call Pocket on 8 Ball
    {"M4", 2500},
    {"M5", 10000}, // Call Pocket on All Shots
    {"M6", 50000},
    {"M7", 100000},
    {"M8", 250000},
    {"M9", 500000},
    {"M10", 1000000}, // Call Pocket on 8 Ball
    {"M11", 2500000}, // Call Pocket on All Shots
    {"M12", 4000000}, // Call Pocket on 8 Ball
    {"M13", 5000000}, // Call Pocket on All Shots
    {"M14", 10000000}, // Call Pocket on All Shots
    {"M15", 15000000}, // Call Pocket on All Shots
    {"M16", 25000000}, // Call Pocket on 8 Ball
    {"M17", 100000000}, // Call Pocket on All Shots + Cushion shot on 8 Ball
};

void StartLastMatch() {
    LOGI("StartLastMatch");

    if (persistent_int["iAutoQueue_Mode"] == 0 && lastMatchInfo.set) {
    _StartMatch(sharedMenuManager.instance, 0, lastMatchInfo.Tier, 0, 0, 0, 0, 0, 0, lastMatchInfo.arg10, lastMatchInfo.arg11);
    } else if (persistent_int["iAutoQueue_Mode"] == 1) {
        auto coins = sharedUserInfo.coins();
        auto maxBet = coins * persistent_int["iAutoQueue_BetPercent"] / 100;
        
        string selectedMode = "M1";
        int64_t selectedBet = 50;
        
        for (const auto& [mode, bet] : modeBets) {
            if (maxBet >= bet) {
                selectedMode = mode;
                selectedBet = bet;
            }
        }
        
        LOGI("Selected mode %s with bet %lld (50%% of %lld coins)", selectedMode.c_str(), selectedBet, coins);
        _StartMatch(sharedMenuManager.instance, 0, selectedMode, 0, 0, 0, 0, 0, 0, 0x7100000001, 0xffffffff);
    }
}

DEFINE(int64_t, popMenuState, int64_t arg1, int64_t arg2, int32_t arg3, int64_t arg4) {
    LOGI("popMenuState arg1 %p, arg2 %p, arg3 %d, arg4 %p", arg1, arg2, arg3, arg4);
    return _popMenuState(arg1, arg2, arg3, arg4);
}

void PopMenuState(int stateId) {
    LOGI("PopMenuState %d", stateId);
    auto _popMenuState = M(int64_t, libmain + 0x3051f00, int64_t, int64_t, int32_t, int64_t); // MenuManager::popMenuState:withScene:
    _popMenuState(sharedMenuManager.instance, 0, stateId, sharedMenuManager.instance);
}