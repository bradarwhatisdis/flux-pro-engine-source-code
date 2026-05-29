#pragma once

#include "StateManager.h"
#include "VisualCue.h"
#include "Table.h"
#include "VisualEnglishControl.h"

#include "inc/NumberUtils.h"
#include "Ball.h"
/*
^{Ruleset=Ii{basic_string<char, std::char_traits<char>, std::allocator<char>>={__compressed_pair<std::basic_string<char>::__rep, std::allocator<char>>={__rep=(?={__long=QQ*}{__short=(?=CC)[23C]}{__raw=[3Q]})}}}I{map<std::type_index, rules::IRule *, std::less<std::type_index>, std::allocator<std::pair<const std::type_index, rules::IRule *>>>={__tree<std::__value_type<std::type_index, rules::IRule *>, std::__map_value_compare<std::type_index, std::__value_type<std::type_index, rules::IRule *>, std::less<std::type_index>, true>, std::allocator<std::__value_type<std::type_index, rules::IRule *>>>=^{__tree_end_node<std::__tree_node_base<void *> *>}{__compressed_pair<std::__tree_end_node<std::__tree_node_base<void *> *>, std::allocator<std::__tree_node<std::__value_type<std::type_index, rules::IRule *>, void *>>>={__tree_end_node<std::__tree_node_base<void *> *>=^{__tree_node_base<void *>}}}{__compressed_pair<unsigned long, std::__map_value_compare<std::type_index, std::__value_type<std::type_index, rules::IRule *>, std::less<std::type_index>, true>>=Q}}}{vector<rules::Rules8BallFoulReason, std::allocator<rules::Rules8BallFoulReason>>=^i^i{__compressed_pair<rules::Rules8BallFoulReason *, std::allocator<rules::Rules8BallFoulReason>>=^i}}Bdi@@I@I{vector<BallClassification, std::allocator<BallClassification>>=^i^i{__compressed_pair<BallClassification *, std::allocator<BallClassification>>=^i}}{vector<BallClassification, std::allocator<BallClassification>>=^i^i{__compressed_pair<BallClassification *, std::allocator<BallClassification>>=^i}}{vector<BallClassification, std::allocator<BallClassification>>=^i^i{__compressed_pair<BallClassification *, std::allocator<BallClassification>>=^i}}{vector<PoolPlayer *, std::allocator<PoolPlayer *>>=^@^@{__compressed_pair<PoolPlayer **, std::allocator<PoolPlayer *>>=^@}}iiiiBiccBBBIicIBI}
*/
struct GameManager : Class {
    Field<0x3e0, uintptr_t> _rules;
    Field<0x3e8, Table> mTable;
    Field<0x4b8, VisualCue> mVisualCue;
    Field<0x4c8, VisualEnglishControl> mVisualEnglishControl;
    Field<0x508, GameStateManager> mStateManager;
    Field<0x5c0, int> mGameMode;

    GameManager(ptr instance = 0) : Class(instance), _rules(instance), mTable(instance), mVisualCue(instance), mVisualEnglishControl(instance), mStateManager(instance), mGameMode(instance) {}

    Vec2d getShotSpin() {
        VisualEnglishControl visualEnglishControl = this->mVisualEnglishControl;
        if (!visualEnglishControl) return Vec2d(0, 0);

        Vec2d spin = visualEnglishControl.mEnglish;
        spin.x = NumberUtils::normalizeDoublePrecision(spin.x);
        spin.y = NumberUtils::normalizeDoublePrecision(spin.y);
        return spin ? spin * CUE_PROPERTIES_SPIN : spin;
    }

    // bool isGoldenShotMode() { return mGameMode() == 16; }
    
    BallEnums::Classification getPlayerClassification() {
        auto rules = _rules();
        auto classification_vector = F(uintptr_t, rules + 0xC8);
        return F(BallEnums::Classification, classification_vector + (mStateManager().isPlayerTurn() ? 0 : 4));

        // auto rules = F(ptr, instance + 0x3e0);
        // auto field_0xC8 = F(ptr, rules + 0xC8);
        // LOGI("field_0xC8: %d", F(int, field_0xC8));
        // LOGI("field_0xC8 + 0x4: %d", F(int, field_0xC8 + 0x4));
    }

    bool is9BallGame() {
        return getPlayerClassification() == Ball::Classification::NINE_BALL_RULE;
        // auto rules = _rules();
        // return (rules & 64) == 64;
    }
    
    int getPocketNominationMode() {
        auto rules = _rules();
        return F(int, rules + 0x68);

        // 0: no pocket call
        // 1: call pocket on 8 ball
        // 2: call pocket on all shots
    }
    
    /* uint getPocketNominationMode() {
        auto rules = _rules();
        return F(uint, rules + 0x68) - 1;
    } */

    uint getNominatedPocket() {
        auto rules = _rules();
        return F(uint, rules + 0x118);
    }
    
    void nominatePocket(int pocket) {
        auto rules = _rules();
        F(uint, rules + 0x118) = pocket;
    }

    operator bool() { return instance && isInstanceOf("GameManager"); }
};

static GameManager sharedGameManager;
