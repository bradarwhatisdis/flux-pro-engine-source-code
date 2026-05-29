#pragma once

#include "Types.h"

extern ptr libmain;

struct UserInfo : Class {
    Field<0x208, int> coins;
    Field<0x210, int> cash;

    UserInfo(ptr instance = 0) : Class(instance), coins(instance), cash(instance) {}

    /* int getTotalWinnings() {
        static auto GetTotalWinnings = M(int, libmain + 0x3406610, ptr); // totalWinnings
        return GetTotalWinnings(instance);
    } */

    operator bool() { return instance && this->isInstanceOf("UserInfo"); }
};

static UserInfo sharedUserInfo;
