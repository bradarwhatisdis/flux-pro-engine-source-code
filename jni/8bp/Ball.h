#pragma once

#include "Types.h"
#include "Foundation.h"
#include <Vector/Vectors.h>

struct BallEnums {
    enum Classification : int {
        ANY = -1, // 0xFFFFFFFF
        CUE_BALL = 0,
        SOLID = 1,
        STRIPE = 2,
        NINE_BALL_RULE = 3,
        EIGHT_BALL = 4,
        ERR_CLASSIFICATION = -8
    };

    enum State : int {
        DEFAULT = 1,
        IN_POCKET = 2,
        UNKNOWN = 3,
        POTTED = 4,
        ERR_STATE = -8
    };
};

struct Ball : BallEnums, Class {
    Field<0x20, Vec2d> position; // BallPhysicsProperties _physicsProperties
    Field<0x30, Vec2d> velocity;
    Field<0x40, double> radius;
    Field<0x48, Vec3d> spin;
    Field<0x60, double> mass;
    Field<0x68, double> volume;
    
    Field<0xa0, Classification> classification;
    Field<0xa4, State> state;
    
    Ball(ptr instance = 0) : Class(instance), position(instance), velocity(instance), radius(instance), spin(instance), classification(instance), state(instance) {}

    bool isOnTable() { auto state = this->state(); return state == State::DEFAULT || state == State::IN_POCKET; }

    operator bool() { return instance && this->isInstanceOf("Ball"); }
};