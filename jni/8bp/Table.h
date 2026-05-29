#pragma once

#include "Types.h"
#include "Foundation.h"
#include "Ball.h"

extern ptr libmain;

struct TableProperties : Class {
    FieldImpl<0x68, Vec2d*, false> mPockets; // TODO: wtf???? 0x38????

    TableProperties(ptr instance = 0) : Class(instance), mPockets(instance) {}

    double getPocketRadius() { return 8.0; } // F(double, libmain + 0x4b6e298); } // 8.0
    double getLength() { return 254.0; } // F(double, libmain + 0x4b6e290); } // 254.0
    double getWidth() { return 127.0; } // F(double, libmain + 0x4b6e288); } // 127.0

    operator bool() { return instance && this->isInstanceOf("TenByFiveNarrowTableProperties"); }
};

#include "FrictionProperties.h"

struct Table : Class {
    Field<0x3b0, TableProperties> mTableProperties;
    Field<0x3c0, FrictionProperties> _frictionProperties;
    Field<0x450, PNSArray<Ball>*> mBalls;
    Field<0x588, Vec4d> mTableCollisionBounds; // x, y, width, height

    Table(ptr instance = 0) : Class(instance), mTableProperties(instance), _frictionProperties(instance), mBalls(instance), mTableCollisionBounds(instance) {}

    operator bool() { return instance && this->isInstanceOf("Table"); }
};
