#pragma once

#include "NumberUtils.h"
#include <array>

#include "GameConstants.h"
#include <Vector/Vectors.h>
#include <vector>

#include "8bp/Ball.h"

#include <imgui/inc/persistence.h>

#include "8bp/GameManager.h"

static Vec4d table_bounds;
static bool fastCalc = true;

struct Prediction {
    static bool pocketStatus[TABLE_POCKETS_COUNT];

    Prediction() = default;
    ~Prediction() = default;

    bool determineShotResult(bool isAuto, double shotAngle = sharedGameManager.mVisualCue().getShotAngle(), double shotPower = sharedGameManager.mVisualCue().getShotPower(), Vec2d shotSpin = sharedGameManager.getShotSpin(), Candidate cand = {-1});
    bool mockPredictShotResult();

    struct Ball {
        int index; // ball index 0..15
        ::Ball::Classification classification;
        ::Ball::State state;
        bool originalOnTable;
        bool onTable;
        int pocketIndex = -1;

        Point2D velocity;
        Vec3d spin;
        Point2D initialPosition;
        Point2D predictedPosition;
        std::vector<Point2D> positions;

        void findNextCollision(void *pData, double *time);
        void calcVelocity();
        void calcVelocityPostCollision(const double &angle);
        void move(const double &time);
        bool isMovingOrSpinning() const;

        Ball() : index(0), classification(::Ball::Classification::ERR_CLASSIFICATION),
                 state(::Ball::State::ERR_STATE),
                 originalOnTable(false), onTable(false), velocity(Point2D()), spin(Vec3d()),
                 initialPosition(Point2D()), predictedPosition(Point2D()),
                 positions(std::vector<Point2D>()) {}

        ~Ball() = default;

        bool isBallBallCollision(double *smallestTime, Prediction::Ball &otherBall) const; // sub_1C29FA0 5.8.0
        bool willCollideWithTable(const double *smallestTime) const; // sub_1BF9ADC 5.8.0
        void determineBallTableCollision(void *pData, double *smallestTime); // sub_1BF9BD8 5.8.0
        bool isBallLineCollision(double *pTime_1, const Point2D &tableShapePointA, const Point2D &tableShapePointB) const; // sub_1C2A2C0 5.8.0
        bool isBallPointCollision(double *smallestTime, const Point2D &tableShapePoint) const; // sub_1C2A594 5.8.0
    };

    struct Collision {
        Collision() : valid(false), type(Type::POINT), angle(0.0), point{}, ballA(nullptr), ballB(nullptr), firstHitBall(nullptr) {}
        ~Collision() = default;

        enum Type : int {
            BALL,
            LINE,
            POINT
        };

        bool valid;
        Type type;
        double angle;
        Point2D point;
        Prediction::Ball *ballA;
        Prediction::Ball *ballB;
        Prediction::Ball *firstHitBall;
    };

    struct SceneData {
        int ballsCount;
        Ball balls[MAX_BALLS_COUNT];

        Collision collision;
        bool shotState;

        SceneData() : ballsCount(0), balls{}, collision{}, shotState(false) {}
        ~SceneData() = default;
    } guiData;

    int shotResultSize = 0;
    static float shotResult[MAX_SHOT_RESULT_SIZE];

    bool firstHitIsTarget = false;
    Candidate m_candidate = {-1};

    void calculateShotResultSize();
    void initBalls();
    void initCueBall(double shotAngle, double shotPower, const Point2D& shotSpin);
    void mockInitBalls();
    void determineBallsPositions();
    void handleCollision();
    void handleBallBallCollision() const;
    void determineShotState();
};

extern Prediction *gPrediction;
static Prediction prediction;
Prediction *gPrediction = &prediction;

bool Prediction::pocketStatus[] = {};
float Prediction::shotResult[MAX_SHOT_RESULT_SIZE];

static double prevAngle = 0.0;
static double prevPower = 0.0;
static Point2D prevSpin = {0.0, 0.0};

// constexpr double dword_35B7988 = 0.54;
// constexpr double dword_35B7978 = 0.804;

/* PREDICTION PUBLIC METHODS ==================================================================== */

bool Prediction::determineShotResult(bool isAuto, double shotAngle, double shotPower, Vec2d shotSpin, Candidate cand) { // returns isShouldReDraw
    if (shotAngle == prevAngle && shotPower == prevPower && shotSpin == prevSpin) return false;
    prevAngle = shotAngle, prevPower = shotPower, prevSpin = shotSpin;

    this->m_candidate = cand;
    fastCalc = isAuto;

    this->initBalls();
    this->initCueBall(shotAngle, shotPower, shotSpin);
    this->guiData.collision.firstHitBall = nullptr;
    
    for (bool &_pocketStatus : pocketStatus) _pocketStatus = false;

    this->determineBallsPositions();
    // if (dynamic_bool["isDrawShotStateEnabled", false]) this->determineShotState();

    for (int i = 0; i < this->guiData.ballsCount; i++) {
        Ball &ball = this->guiData.balls[i];
        if (ball.positions.back() != ball.predictedPosition) {
            ball.positions.push_back(ball.predictedPosition);
        }
    }

    return true;
}

/* ============================================================================================== */

/* PREDICTION PRIVATE METHODS =================================================================== */

void Prediction::initBalls() {
    Table table = sharedGameManager.mTable;
    if (!table) return;
    
    auto& balls = table.mBalls();
    if (!balls) return;

    table_bounds = table.mTableCollisionBounds();

    // MemoryManager::Balls::initializeBallsList();
    this->guiData.ballsCount = balls.Count;
    for (int i = 0; i < this->guiData.ballsCount; i++) {
        Ball &ball = this->guiData.balls[i];
        ball.index = i;
        ball.state = balls[i].state();
        ball.originalOnTable = balls[i].isOnTable();
        ball.onTable = ball.originalOnTable;
        ball.classification = balls[i].classification();
        ball.initialPosition = balls[i].position();
        ball.predictedPosition = ball.initialPosition;
        ball.velocity.nullify();
        ball.spin.nullify();
        if (!ball.positions.empty()) ball.positions.clear();
        ball.positions.reserve(20);
        ball.positions.push_back(ball.initialPosition);
    }
}

void Prediction::initCueBall(double shotAngle, double shotPower, const Point2D &shotSpin) {
    double angleCos = round(cos(shotAngle) * 10000.0) / 10000.0;
    double angleSin = round(sin(shotAngle) * 10000.0) / 10000.0;
    Ball &cueBall = this->guiData.balls[0];
    cueBall.velocity.x = shotPower * angleCos;
    cueBall.velocity.y = shotPower * angleSin;
    double spinFactor = shotPower / BALL_RADIUS;
    double v31 = -shotSpin.y * spinFactor;
    cueBall.spin.x = -(angleSin * v31);
    cueBall.spin.y = angleCos * v31;
    cueBall.spin.z = shotSpin.x * spinFactor;
}

void Prediction::determineBallsPositions() {
    int i;
    bool isAnyBallMovingOrSpinning;
    double time;
    double time2;
    do {
        time = TIME_PER_TICK;
        do {
            time2 = time;
            this->guiData.collision.valid = false;
            // find the next collision for each ball
            for (i = 0; i < this->guiData.ballsCount; i++) {
                Ball &ball = this->guiData.balls[i];
                if (ball.onTable) {
                    ball.findNextCollision(&this->guiData, &time2);
                }
            }
            // move all balls to their collision positions
            for (i = 0; i < this->guiData.ballsCount; i++) {
                Ball &ball = this->guiData.balls[i];
                if (ball.onTable && ball.isMovingOrSpinning()) {
                    ball.move(time2);
                }
            }
            if (this->guiData.collision.valid) {
                this->handleCollision();
                if (this->guiData.collision.firstHitBall != nullptr && this->m_candidate.idx != -1) {
                    this->firstHitIsTarget = (this->guiData.collision.firstHitBall->index == this->m_candidate.idx);
                    if (!this->firstHitIsTarget) return;
                }
            }
            time -= time2;
        } while (time > MIN_TIME);
        isAnyBallMovingOrSpinning = false;
        for (i = 0; i < this->guiData.ballsCount; i++) {
            Ball &ball = this->guiData.balls[i];
            if (ball.onTable) {
                ball.calcVelocity();
                if (ball.isMovingOrSpinning()) {
                    isAnyBallMovingOrSpinning = true;
                }
            }
        }
    } while (isAnyBallMovingOrSpinning);
}

void Prediction::handleCollision() {
    Ball &ballA = *(this->guiData.collision.ballA);
    Ball &ballB = *(this->guiData.collision.ballB);
    if (!fastCalc) ballA.positions.push_back(ballA.predictedPosition);
    
    switch (this->guiData.collision.type) {
        case Collision::Type::BALL:
            this->handleBallBallCollision();
            if (!fastCalc) ballB.positions.push_back(ballB.predictedPosition);
            if (this->guiData.collision.firstHitBall == nullptr) this->guiData.collision.firstHitBall = &ballB;
            break;
        case Collision::Type::LINE:
            ballA.calcVelocityPostCollision(this->guiData.collision.angle);
            break;
        default:
            Point2D delta = {
                this->guiData.collision.point.y - ballA.predictedPosition.y,
                -(this->guiData.collision.point.x - ballA.predictedPosition.x)
            };
            this->guiData.collision.angle = -NumberUtils::calcAngle(delta);
            ballA.calcVelocityPostCollision(this->guiData.collision.angle);
            break;
    }
}

void Prediction::handleBallBallCollision() const {
    Ball &ballA = *(this->guiData.collision.ballA);
    Ball &ballB = *(this->guiData.collision.ballB);
    Point2D relativePosition = ballA.predictedPosition - ballB.predictedPosition;
    double invDistance = 1.0 / sqrt(relativePosition.square());
    Point2D collisionNormal = relativePosition * invDistance;
    double velocityComponentA = ballA.velocity.x * collisionNormal.x + ballA.velocity.y * collisionNormal.y;
    double velocityComponentB = ballB.velocity.x * collisionNormal.x + ballB.velocity.y * collisionNormal.y;
    Point2D velocityA = collisionNormal * velocityComponentA;
    Point2D velocityB = collisionNormal * velocityComponentB;
    ballA.velocity.x = velocityB.x - (velocityA.x - ballA.velocity.x);
    ballA.velocity.y = velocityB.y - (velocityA.y - ballA.velocity.y);
    ballB.velocity.x = velocityA.x - (velocityB.x - ballB.velocity.x);
    ballB.velocity.y = velocityA.y - (velocityB.y - ballB.velocity.y);
}

/* void Prediction::determineShotState() {
    this->guiData.shotState = false;
    // cue ball didn't hit any other ball
    if (this->guiData.collision.firstHitBall == nullptr) {
        return;
    }
    // cue ball potted
    if (!this->guiData.balls[0].onTable) {
        return;
    }
    ::Ball::Classification playerClassification = MemoryManager::GameManager::getPlayerClassification();
    // 8-ball before break
    if (playerClassification == BallClassification::ANY) {
        if (this->guiData.collision.firstHitBall->classification ==
            BallClassification::EIGHT_BALL) {
            return;
        }
        for (int i = 0; i < this->guiData.ballsCount; i++) {
            Ball &ball = this->guiData.balls[i];
            // any ball except 8-ball has been potted during current shot
            if (ball.originalOnTable != ball.onTable) {
                this->guiData.shotState = this->guiData.balls[8].onTable;
                return;
            }
        }
    } else {
        // after break
        if (this->guiData.collision.firstHitBall->classification != playerClassification) {
            return;
        }
        // 9-ball mode
        if (playerClassification == BallClassification::NINE_BALL_RULE) {
            for (int i = 1; i < this->guiData.ballsCount; i++) {
                Ball &ball = this->guiData.balls[i];
                // ball has been potted during current shot
                if (ball.originalOnTable != ball.onTable) {
                    this->guiData.shotState = true;
                    return;
                }
            }
            return;
        }
    }
    // 8-ball mode after break
    if (playerClassification == BallClassification::EIGHT_BALL) {
        // 8-ball has been potted during current shot
        this->guiData.shotState = !this->guiData.balls[8].onTable;
        return;
    }
    // to only check balls with correct classification
    int startBall = (playerClassification == BallClassification::SOLID) ? 1 : 9;
    for (int i = startBall; i < startBall + 7; i++) {
        Ball &ball = this->guiData.balls[i];
        // any ball except 8-ball has been potted during current shot
        if (ball.originalOnTable != ball.onTable) {
            this->guiData.shotState = this->guiData.balls[8].onTable;
            return;
        }
    }
} */

/* ============================================================================================== */

/* ============================================================================================== */

/* BALL PUBLIC METHODS ========================================================================== */

const std::array<Point2D, TABLE_POCKETS_COUNT>& getPockets() {
    static const std::array<Point2D, TABLE_POCKETS_COUNT> POCKET_POSITIONS = {
            Point2D(-130.8, -67.3),
            Point2D(0, -72),
            Point2D(130.8, -67.3),
            Point2D(130.8, 67.3),
            Point2D(0, 72),
            Point2D(-130.8, 67.3)
    };
    return POCKET_POSITIONS;
}

const std::array<Point2D, TABLE_SHAPE_SIZE>& getTableShape() {
    static const std::array<Point2D, TABLE_SHAPE_SIZE> TABLE_SHAPE = {
            Point2D(-127, 53.5),
            Point2D(-136.9, 64.1),
            Point2D(-138.2, 69.2),
            Point2D(-136.7, 73.2),
            Point2D(-132.7, 74.7),
            Point2D(-127.6, 73.4),
            Point2D(-117, 63.5),
            Point2D(-7.8, 63.5),
            Point2D(-6.1, 68.6),
            Point2D(-5.7, 72.7),
            Point2D(-3.7, 75.4),
            Point2D(0, 76.7),
            Point2D(3.7, 75.4),
            Point2D(5.7, 72.7),
            Point2D(6.1, 68.6),
            Point2D(7.8, 63.5),
            Point2D(117, 63.5),
            Point2D(127.6, 73.4),
            Point2D(132.7, 74.7),
            Point2D(136.7, 73.2),
            Point2D(138.2, 69.2),
            Point2D(136.9, 64.1),
            Point2D(127, 53.5),
            Point2D(127, -53.5),
            Point2D(136.9, -64.1),
            Point2D(138.2, -69.2),
            Point2D(136.7, -73.2),
            Point2D(132.7, -74.7),
            Point2D(127.6, -73.4),
            Point2D(117, -63.5),
            Point2D(7.8, -63.5),
            Point2D(6.1, -68.6),
            Point2D(5.7, -72.7),
            Point2D(3.7, -75.4),
            Point2D(0, -76.7),
            Point2D(-3.7, -75.4),
            Point2D(-5.7, -72.7),
            Point2D(-6.1, -68.6),
            Point2D(-7.8, -63.5),
            Point2D(-117, -63.5),
            Point2D(-127.6, -73.4),
            Point2D(-132.7, -74.7),
            Point2D(-136.7, -73.2),
            Point2D(-138.2, -69.2),
            Point2D(-136.9, -64.1),
            Point2D(-127, -53.5)
    };
    return TABLE_SHAPE;
}

void Prediction::Ball::findNextCollision(void *pData, double *time) {
    auto *data = reinterpret_cast<SceneData *>(pData);
    auto pockets = getPockets();
    // find collisions with other balls
    if (this->state == ::Ball::State::DEFAULT) {
        for (int i = this->index + 1; i < data->ballsCount; i++) {
            Ball &otherBall = data->balls[i];
            if (otherBall.state == ::Ball::State::DEFAULT && this->isBallBallCollision(time, otherBall)) {
                data->collision.valid = true;
                data->collision.ballA = this;
                data->collision.type = Collision::Type::BALL;
                data->collision.ballB = &otherBall;
            }
        }
    }
    if (this->willCollideWithTable(time)) {
        if (this->state == ::Ball::State::IN_POCKET) {
            double unkTime = *time * 1.5; // F(double, libmain + 0x4dae0b8);
            this->velocity.x -= this->predictedPosition.x * unkTime;
            this->velocity.y -= this->predictedPosition.y * unkTime;
        } else if (this->state == ::Ball::State::DEFAULT) { // check if this ball is potted
            double deltaSquare;
            double unkTime;
            Point2D delta;
            for (int i = 0; i < TABLE_POCKETS_COUNT; i++) {
                delta.x = pockets[i].x - this->predictedPosition.x;
                delta.y = pockets[i].y - this->predictedPosition.y;
                deltaSquare = delta.x * delta.x + delta.y * delta.y;
                if (deltaSquare < POCKET_RADIUS_SQUARE) {
                    unkTime = *time * 120.0; // F(double, libmain + 0x4dae0c0);
                    this->velocity.x += delta.x * unkTime;
                    this->velocity.y += delta.y * unkTime;
                    if (deltaSquare < BALL_RADIUS_SQUARE) {
                        this->state = ::Ball::State::IN_POCKET;
                        this->pocketIndex = i;
                        Prediction::pocketStatus[i] = true;
                    }
                }
            }
        }
        this->determineBallTableCollision(pData, time);
    }

    if (this->state == ::Ball::State::IN_POCKET) {
        this->state = ::Ball::State::UNKNOWN;
        this->onTable = false;
        this->velocity.nullify();
        this->spin.nullify();
    }
}

/* void Prediction::Ball::calcVelocity() {
    if (!this->isMovingOrSpinning()) {
        return;
    }
    
    double v15 = BALL_RADIUS * this->spin.x - this->velocity.y;
    double v16 = -this->velocity.x - this->spin.y * BALL_RADIUS;
    double v17 = sqrt(v16 * v16 + v15 * v15);
    double v18 = v17 * 0.0014577259475218659; // _frictionProperties._timeOfequilibriumFactor
    if (v18 > unk_35B3F80) {
        double v20 = (v18 < TIME_PER_TICK) ? (v17 * 0.0014577259475218659) : TIME_PER_TICK;
        double v21 = 196.0 * v20 / v17; // _frictionProperties._velocityReductionSlidingFactor
        double v22 = v16 * v21;
        double v23 = v15 * v21;
        this->velocity.x += v22;
        this->velocity.y += v23;
        this->spin.x -= v23 * 0.6578125102783204; // unk_35B3F88 / BALL_RADIUS
        this->spin.y += v22 * 0.6578125102783204; // unk_35B3F88 / BALL_RADIUS
    }
    if (v18 < TIME_PER_TICK) {
        double v24 = this->velocity.x;
        double v25 = this->velocity.y;
        double v27 = (TIME_PER_TICK - v18) * 10.878; // frictionProperties._velocityReductionRollingFactor
        double v28 = 1.0 - v27 / sqrt(v25 * v25 + v24 * v24);
        v28 = (v28 < 0.0) ? 0.0 : v28;
        this->velocity.x = v24 * v28;
        this->velocity.y = v25 * v28;
        this->spin.x = v25 * v28 / BALL_RADIUS;
        this->spin.y = -(v24 * v28) / BALL_RADIUS;
    }
    constexpr double v29 = 9.8 * TIME_PER_TICK; // frictionProperties._deltaSpinFactor
    this->spin.z = (this->spin.z > 0.0) ? fmax(this->spin.z - v29, 0.0) : fmin(this->spin.z + v29, 0.0);
} */

// DAT_04c8b9a8 2.0E
// FUN_02b1bb3c
/* void Prediction::Ball::calcVelocityPostCollision(const double &angle) {
    double angleCos = round(cos(angle) * 10000.0) / 10000.0;
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
    this->spin.z = newSpinZ;
} */

void Prediction::Ball::move(const double &time) {
    if (this->velocity) {
        this->predictedPosition.x += this->velocity.x * time;
        this->predictedPosition.y += this->velocity.y * time;
        
        if (!fastCalc) {
            auto lastIndex = this->positions.size() - 1;
            if (lastIndex > 1) {
                auto &a = this->positions[lastIndex - 1];
                auto &b = this->positions[lastIndex];
                auto &c = this->predictedPosition;
                if (((b.y - a.y) * (c.x - b.x)) == ((c.y - b.y) * (b.x - a.x))) return;
            } this->positions.push_back(this->predictedPosition);
        }
    }
}

bool Prediction::Ball::isMovingOrSpinning() const {
    return bool(this->velocity) || bool(this->spin);
}

/* ============================================================================================== */

/* BALL PRIVATE METHODS */

#include "Prediction.update.h"

/* bool Prediction::Ball::willCollideWithTable(const double *smallestTime) const {
    double currentX = this->predictedPosition.x;
    double currentY = this->predictedPosition.y;
    double predictedX = currentX + this->velocity.x * *smallestTime;
    double predictedY = currentY + this->velocity.y * *smallestTime;
    double leftX;
    double rightX;
    double bottomY;
    double topY;
    if (this->velocity.x > 0.0) {
        leftX = currentX;
        rightX = predictedX;
    } else {
        leftX = predictedX;
        rightX = currentX;
    }
    if (this->velocity.y > 0.0) {
        topY = currentY;
        bottomY = predictedY;
    } else {
        topY = predictedY;
        bottomY = currentY;
    }
    return (leftX < TABLE_BOUND_LEFT || rightX > TABLE_BOUND_RIGHT || topY < TABLE_BOUND_TOP ||
            bottomY > TABLE_BOUND_BOTTOM);
} */

void Prediction::Ball::determineBallTableCollision(void *pData, double *smallestTime) {
    double angle;
    auto *data = reinterpret_cast<Prediction::SceneData *>(pData);
    auto tableShape = getTableShape();
    for (int i = 0; i < TABLE_SHAPE_SIZE; i++) {
        const Point2D &point = tableShape[i];
        const Point2D &nextPoint = tableShape[(i + 1) % TABLE_SHAPE_SIZE];
        if (this->isBallLineCollision(smallestTime, point, nextPoint)) {
            angle = NumberUtils::calcAngle(nextPoint, point);
            data->collision.valid = true;
            data->collision.ballA = this;
            data->collision.type = Collision::Type::LINE;
            data->collision.angle = -angle;
        } else if (this->isBallPointCollision(smallestTime, point)) {
            data->collision.valid = true;
            data->collision.ballA = this;
            data->collision.point = point;
            data->collision.type = Collision::Type::POINT;
        }
    }
}

/* bool Prediction::Ball::isBallLineCollision(double *pTime_1, const Point2D &tableShapePointA,
                                           const Point2D &tableShapePointB) const {
    if (!this->velocity) {
        return false;
    }
    Point2D delta = tableShapePointB - tableShapePointA;
    double v17 = delta.y * this->velocity.x - delta.x * this->velocity.y;
    if (v17 == 0.0) {
        return false;
    }
    double invDistance = 1.0 / sqrt(delta.square());
    double v21 = invDistance * BALL_RADIUS;
    double v22 = this->predictedPosition.x - tableShapePointA.x - delta.y * v21;
    double v23 = this->predictedPosition.y - tableShapePointA.y + delta.x * v21;
    double v24 = (v22 * -this->velocity.y - v23 * -this->velocity.x) / v17;
    if (v24 <= 0.0 || v24 >= 1.0) {
        return false;
    }
    double time = (delta.x * v23 - delta.y * v22) / v17;
    if (time <= 0.0 || (time - 1E-11 > *pTime_1)) {
        return false;
    }
    if (this->velocity.x * (delta.y * invDistance) + this->velocity.y * -(delta.x * invDistance) >
        0.0) {
        return false;
    }
    *pTime_1 = time;
    return true;
} */

/* bool Prediction::Ball::isBallPointCollision(double *smallestTime, const Point2D &tableShapePoint) const {
    Point2D delta = tableShapePoint - this->predictedPosition;
    double v16 = -(this->velocity.x * delta.x * 2.0) - (this->velocity.y * delta.y * 2.0);
    if (v16 >= 0.0) {
        return false;
    }
    double velocitySquare = this->velocity.square();
    double distanceSquare = delta.square();
    double unkSquare = v16 * v16;
    if (distanceSquare - unkSquare / (velocitySquare * 4.0) >= BALL_RADIUS_SQUARE) {
        return false;
    }
    double v22 = (-v16 -
                  sqrt(unkSquare - velocitySquare * 4.0 * (distanceSquare - BALL_RADIUS_SQUARE))) /
                 (velocitySquare * 2.0);
    if (v22 < 0.0) {
        return false;
    }
    if (v22 - unk_35B7A20 > *smallestTime) {
        return false;
    }
    *smallestTime = v22;
    return true;
} */
