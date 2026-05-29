#define DAT_04c8b9a8 2.0 // F(double, libmain + 0x4c8b9a8)
#define DAT_04c8b998 0.0 // F(double, libmain + 0x4c8b998)
#define DAT_04c8bc78 1.0e-11 // F(double, libmain + 0x4c8bc78)
#define DAT_04c8b9c0 1.79769313e308 // F(double, libmain + 0x4c8b9c0)

#define NAN std::isnan

void FUN_02b1b2d0(double *smallestTime, const Vector2D *ball1_position, const Vector2D *ball1_velocity, const Vector2D *ball2_position, const Vector2D *ball2_velocity, double *combinedBallRadiusSquared, double *param_7) {
    Vector2D relativePosition;
    double dVar2;
    double param_7_1;
    double dVar1;
    Vector2D velocityDelta;
    double velocityDeltaSquared;
    
    relativePosition.x = ball2_position->x - ball1_position->x;
    relativePosition.y = ball2_position->y - ball1_position->y;
    velocityDelta.x = ball2_velocity->x - ball1_velocity->x;
    velocityDelta.y = ball2_velocity->y - ball1_velocity->y;
    dVar1 = DAT_04c8b9a8 * (relativePosition.x * velocityDelta.x + relativePosition.y * velocityDelta.y);
    if (dVar1 < DAT_04c8b998 != (NAN(dVar1) || NAN(DAT_04c8b998))) {
        velocityDeltaSquared = velocityDelta.x * velocityDelta.x + velocityDelta.y * velocityDelta.y;
        dVar2 = ((relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y) - *combinedBallRadiusSquared) * velocityDeltaSquared * 4.0;
        if (dVar2 <= dVar1 * dVar1) {
            dVar2 = sqrt(dVar1 * dVar1 - dVar2);
            dVar1 = (-dVar1 - dVar2) / (DAT_04c8b9a8 * velocityDeltaSquared);
            if (DAT_04c8b998 <= dVar1) {
                param_7_1 = *param_7;
                dVar2 = dVar1 - DAT_04c8bc78;
                if (dVar2 == param_7_1 || dVar2 < param_7_1 != (NAN(dVar2) || NAN(param_7_1))) goto LAB_02b1b3b0;
            }
        }
    }
    dVar1 = DAT_04c8b9c0;
LAB_02b1b3b0:
    *smallestTime = dVar1;
    return;
}

bool Prediction::Ball::isBallBallCollision(double *smallestTime, Prediction::Ball &otherBall) const {
    auto& ball1 = *this;
    auto& ball2 = otherBall;

    double balls_radius = BALL_RADIUS + BALL_RADIUS;
    double combinedBallRadiusSquared = balls_radius * balls_radius;

    double tempTime = *smallestTime;
    
    FUN_02b1b2d0(&tempTime, &ball1.predictedPosition, &ball1.velocity, &ball2.predictedPosition, &ball2.velocity, &combinedBallRadiusSquared, &tempTime);
    
    if (tempTime != DAT_04c8b9c0) {
        *smallestTime = tempTime;
        return true;
    }

    return false;
}

bool FUN_03606c80(const Vector2D *position, const Vector2D *velocity, const double *smallestTime, const Vector4D *tableBounds, const double *radius) {
    Vector2D predicted(
        position->x + velocity->x * *smallestTime,
        position->y + velocity->y * *smallestTime
    ); double leftX, rightX, topY, bottomY;

    if (velocity->x > 0.0) {
        leftX = position->x;
        rightX = predicted.x;
    } else {
        leftX = predicted.x;
        rightX = position->x;
    }
    
    if (velocity->y > 0.0) {
        topY = position->y;
        bottomY = predicted.y;
    } else {
        topY = predicted.y;
        bottomY = position->y;
    }

    static auto FUN_034f8f20 = M(bool, libmain + 0x35f8a40, double*, double*, double*, double*, const Vector4D*, const double*);
    return FUN_034f8f20(&leftX, &topY, &rightX, &bottomY, tableBounds, radius);
}

bool Prediction::Ball::willCollideWithTable(const double *smallestTime) const {
    return FUN_03606c80(&this->predictedPosition, &this->velocity, smallestTime, &table_bounds, &BALL_RADIUS);
}

struct pos_vel_rad {
    Vector2D pos;
    Vector2D vel;
    double rad;
};

void FUN_02b1b664(double *smallestTime,pos_vel_rad *pos_vel_rad,const Vector2D *tableShapePoint,double *smallestTime_2) {
    double dVar1;
    double dVar2;
    double dVar3;
    double dVar4;
    double dVar5;
    double dVar6;
    
    dVar3 = (pos_vel_rad->vel).x;
    dVar4 = (pos_vel_rad->vel).y;
    dVar2 = tableShapePoint->x - (pos_vel_rad->pos).x;
    dVar5 = tableShapePoint->y - (pos_vel_rad->pos).y;
    dVar1 = DAT_04c8b9a8 * -dVar3 * dVar2 - dVar4 * DAT_04c8b9a8 * dVar5;
    if (dVar1 < DAT_04c8b998 != (NAN(dVar1) || NAN(DAT_04c8b998))) {
        dVar6 = dVar3 * dVar3 + dVar4 * dVar4;
        dVar2 = dVar2 * dVar2 + dVar5 * dVar5;
        dVar3 = dVar6 * 4.0;
        dVar4 = pos_vel_rad->rad * pos_vel_rad->rad;
        dVar5 = dVar2 - (dVar1 * dVar1) / dVar3;
        if (dVar5 < dVar4 != (NAN(dVar5) || NAN(dVar4))) {
            dVar2 = sqrt(dVar1 * dVar1 - dVar3 * (dVar2 - dVar4));
            dVar1 = (-dVar1 - dVar2) / (dVar6 * DAT_04c8b9a8);
            if (DAT_04c8b998 <= dVar1) {
                dVar3 = *smallestTime_2;
                dVar2 = dVar1 - DAT_04c8bc78;
                if (dVar2 == dVar3 || dVar2 < dVar3 != (NAN(dVar2) || NAN(dVar3))) goto LAB_02b1b754;
            }
        }
    }
    dVar1 = DAT_04c8b9c0;
LAB_02b1b754:
    *smallestTime = dVar1;
    return;
}

bool Prediction::Ball::isBallPointCollision(double *smallestTime, const Point2D &tableShapePoint) const {
    pos_vel_rad pos_vel_rad;
    pos_vel_rad.pos = this->predictedPosition;
    pos_vel_rad.vel = this->velocity;
    pos_vel_rad.rad = BALL_RADIUS;
    
    double tempTime = *smallestTime;
    
    FUN_02b1b664(&tempTime, &pos_vel_rad, &tableShapePoint, &tempTime);

    if (tempTime != DAT_04c8b9c0) {
        *smallestTime = tempTime;
        return true;
    }

    return false;
}

#define DAT_04c8b9a0 1.0 // F(double, libmain + 0x4c8b9a0)

void FUN_02b1b3cc(double *param_1, pos_vel_rad *pos_vel_rad, const Vector2D *param_3, const Vector2D *param_4, double *param_5) {
    bool bVar1;
    bool bVar2;
    double dVar3;
    double dVar4;
    double dVar5;
    double dVar6;
    double dVar7;
    double dVar8;
    double dVar9;
    double dVar10;
    double dVar11;
    double dVar12;
    
    dVar11 = (pos_vel_rad->pos).x;
    dVar12 = (pos_vel_rad->pos).y;
    dVar9 = param_4->x - param_3->x;
    dVar10 = param_4->y - param_3->y;
    dVar8 = (pos_vel_rad->vel).x;
    dVar7 = (pos_vel_rad->vel).y;
    dVar3 = sqrt(dVar9 * dVar9 + dVar10 * dVar10);
    dVar5 = dVar8 * dVar10 - dVar7 * dVar9;
    if (dVar5 != DAT_04c8b998) {
        dVar4 = dVar10 * (DAT_04c8b9a0 / dVar3);
        dVar3 = (DAT_04c8b9a0 / dVar3) * -dVar9;
        dVar11 = (dVar11 - param_3->x) - dVar4 * pos_vel_rad->rad;
        dVar12 = (dVar12 - param_3->y) - dVar3 * pos_vel_rad->rad;
        dVar6 = (dVar8 * dVar12 - dVar7 * dVar11) / dVar5;
        bVar1 = false;
        bVar2 = false;
        if (DAT_04c8b998 < dVar6) {
            bVar1 = false;
            bVar2 = true;
            if (!NAN(dVar6) && !NAN(DAT_04c8b9a0)) {
                bVar1 = dVar6 < DAT_04c8b9a0;
                bVar2 = false;
            }
        }
        if ((bVar1 != bVar2) && (dVar5 = (dVar9 * dVar12 - dVar10 * dVar11) / dVar5, DAT_04c8b998 < dVar5)) {
            dVar10 = *param_5;
            dVar9 = dVar5 - DAT_04c8bc78;
            if ((dVar9 == dVar10 || dVar9 < dVar10 != (NAN(dVar9) || NAN(dVar10))) && (dVar3 = dVar8 * dVar4 + dVar7 * dVar3, dVar3 == DAT_04c8b998 || dVar3 < DAT_04c8b998 != (NAN(dVar3) || NAN(DAT_04c8b998)))) goto LAB_02b1b4dc;
        }
    }
    dVar5 = DAT_04c8b9c0;
LAB_02b1b4dc:
    *param_1 = dVar5;
    return;
}

bool Prediction::Ball::isBallLineCollision(double *smallestTime, const Point2D &tableShapePointA, const Point2D &tableShapePointB) const {
    if (!this->velocity) return false;

    pos_vel_rad pos_vel_rad;
    pos_vel_rad.pos = this->predictedPosition;
    pos_vel_rad.vel = this->velocity;
    pos_vel_rad.rad = BALL_RADIUS;
    
    double tempTime = *smallestTime;
    
    FUN_02b1b3cc(&tempTime, &pos_vel_rad, &tableShapePointA, &tableShapePointB, &tempTime);

    if (tempTime != DAT_04c8b9c0) {
        *smallestTime = tempTime;
        return true;
    }

    return false;
}

// _frictionProperties._timeOfequilibriumFactor 0.00145772594752187
// getDefaultLogicalFrameTime 0.005
// 0x4DAE0D0 2.5E
// 0x4dadc00 1.0E
// 0x4dadbf8 0.0E

void Prediction::Ball::calcVelocity() {
    Table table = sharedGameManager.mTable;
    if (!table) return;

    auto& balls = table.mBalls();
    auto ball = balls[this->index];

    auto& _frictionProperties = table._frictionProperties();

    auto bak_velocity = ball.velocity();
    auto bak_spin = ball.spin();

    ball.velocity() = this->velocity;
    ball.spin() = this->spin;

    static auto FUN_03608724 = M(void, libmain + 0x3725a34, uintptr_t, FrictionProperties*, const double*);
    FUN_03608724(ball.instance, &_frictionProperties, &TIME_PER_TICK);

    if (ball.velocity() != this->velocity || ball.spin() != this->spin) {
        this->velocity = ball.velocity();
        this->spin = ball.spin();
    }

    ball.velocity() = bak_velocity;
    ball.spin() = bak_spin;
}

void Prediction::Ball::calcVelocityPostCollision(const double &angle) {
    Table table = sharedGameManager.mTable;
    if (!table) return;

    auto& balls = table.mBalls();
    auto ball = balls[this->index];

    auto& _frictionProperties = table._frictionProperties();

    auto bak_velocity = ball.velocity();
    auto bak_spin = ball.spin();

    ball.velocity() = this->velocity;
    ball.spin() = this->spin;

    static auto FUN_02b1bb3c = M(int64_t, libmain + 0x2ca7064, uintptr_t, FrictionProperties*, const double*);
    FUN_02b1bb3c(ball.instance, &_frictionProperties, &angle);

    if (ball.velocity() != this->velocity || ball.spin() != this->spin) {
        this->velocity = ball.velocity();
        this->spin = ball.spin();
    }

    ball.velocity() = bak_velocity;
    ball.spin() = bak_spin;


    /* double angleCos = round(cos(angle) * 10000.0) / 10000.0;
    double angleSin = round(sin(angle) * 10000.0) / 10000.0;
    double velocityX = angleCos * this->velocity.x - angleSin * this->velocity.y;
    double velocityY = angleSin * this->velocity.x + angleCos * this->velocity.y;
    double spinFactor = velocityX - BALL_RADIUS * this->spin.z;
    double absSpinFactor = (spinFactor > 0.0) ? spinFactor : -spinFactor;
    double velocityFactor = absSpinFactor / 2.5;
    double absVelocityY = (velocityY > 0.0) ? velocityY : -velocityY;
    double spinDirection = (spinFactor > 0.0) ? 1.0 : -1.0; // DAT_04c8b9a0 1.0E
    double minSpinFactor = 0.4 * absVelocityY;
    if (velocityFactor < minSpinFactor) minSpinFactor = velocityFactor;
    double spinChange = spinDirection * minSpinFactor;
    double newVelocityX = velocityX - spinChange / 2.5; // DAT_04c8bc80 2.5E
    double newVelocityY = -0.804 * velocityY; // -(velocityY * dword_35B7978) // DAT_04cb4410 0.804E
    this->velocity.x = angleSin * newVelocityY + angleCos * newVelocityX;
    this->velocity.y = angleCos * newVelocityY - newVelocityX * angleSin;
    double newSpinX = angleSin * this->spin.x + angleCos * this->spin.y;
    double newSpinY = angleCos * this->spin.x - angleSin * this->spin.y - velocityY * 0.1420875022201172; // dword_35B7988 / BALL_RADIUS   DAT_04cb4420 0.54E
    double newSpinZ = this->spin.z + spinChange * 0.6578125102783204; // unk_35B7A28 / BALL_RADIUS
    this->spin.x = angleSin * newSpinX + angleCos * newSpinY;
    this->spin.y = angleCos * newSpinX - newSpinY * angleSin;
    this->spin.z = newSpinZ; */
}