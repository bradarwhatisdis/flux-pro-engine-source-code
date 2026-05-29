#pragma once

// Prediction defined by Prediction.fast.h (included via 8bp.h/AutoPlay.h)
// #include "Prediction.h"
#include <imgui/imgui.h>

using namespace ImGui;

constexpr double maxAngle = 360.0 / (180.0 / M_PI);

double normalizeAngle(double angle) {
    double newAngle = angle;
    if (newAngle >= maxAngle) newAngle = fmod(newAngle, maxAngle);
    else if (newAngle < 0) newAngle = maxAngle - fmod(-newAngle, maxAngle);
    return newAngle;
}

bool ix = true;
namespace AutoAim {
    double lastSetAngle = 0.f;
    bool didSetAngle = false;

    bool shouldAutoAIM() { return !didSetAngle || lastSetAngle == sharedGameManager.mVisualCue().mVisualGuide().mAimAngle(); }

    void setAimAngle(double angle) {
        lastSetAngle = angle;
        sharedGameManager.mVisualCue().mVisualGuide().mAimAngle(angle);
    }

    void AIM(double angleStep = 0.1f) {
        auto startingAngle = NumberUtils::normalizeDoublePrecision(sharedGameManager.mVisualCue().mVisualGuide().mAimAngle());
        
        gPrediction->determineShotResult(true, startingAngle);
        std::vector<int> startingPottedBalls;
        for (int i = 0; i < gPrediction->guiData.ballsCount; i++) {
            Prediction::Ball& ball = gPrediction->guiData.balls[i];
            if (ball.originalOnTable && !ball.onTable) {
                startingPottedBalls.push_back(i);
            }
        }
        
        auto myclass = ix ? Ball::Classification::SOLID : Ball::Classification::STRIPE;
        
        for (double angle = NumberUtils::normalizeDoublePrecision(normalizeAngle(startingAngle + angleStep)); angle != startingAngle; angle = NumberUtils::normalizeDoublePrecision(normalizeAngle(angle + angleStep))) {
            gPrediction->determineShotResult(true, angle);
            
            bool only8BPleft = true;
            for (int i = 1; i < gPrediction->guiData.ballsCount; i++) {
                Prediction::Ball& ball = gPrediction->guiData.balls[i];
                if (ball.classification == myclass) {
                    if (ball.originalOnTable) only8BPleft = false;
                }
            }

            std::vector<int> currentPottedBalls;
            bool isAngleGood = false;
            for (int i = 1; i < gPrediction->guiData.ballsCount; i++) {
                Prediction::Ball& ball = gPrediction->guiData.balls[i];
                if (ball.classification == only8BPleft ? Ball::Classification::EIGHT_BALL : myclass) {
                    if (ball.originalOnTable && !ball.onTable) {
                        currentPottedBalls.push_back(i);
                        isAngleGood = true;
                    }
                }
            }

            if (isAngleGood && gPrediction->guiData.collision.firstHitBall && gPrediction->guiData.collision.firstHitBall->classification != myclass) isAngleGood = false;

            auto& cueBall = gPrediction->guiData.balls[0];
            auto& eightBall = gPrediction->guiData.balls[8];
            if (isAngleGood && cueBall.originalOnTable && !cueBall.onTable) isAngleGood = false;
            if (isAngleGood && !only8BPleft && eightBall.originalOnTable && !eightBall.onTable) isAngleGood = false;
            
            if (!currentPottedBalls.empty() && startingPottedBalls != currentPottedBalls && isAngleGood) {
                setAimAngle(angle);
                // LOGI("potted balls count: %zu", currentPottedBalls.size());
                break;
            }
        }
    }

    void Draw() {
        ImGuiIO& io = GetIO();
        float padding = 30.0f;
        SetNextWindowPos(ImVec2(io.DisplaySize.x - persistent_int["iAIM_WindowX"], persistent_int["iAIM_WindowY"]), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
        SetNextWindowSize(ImVec2(210, 65), ImGuiCond_FirstUseEver);

        if (Begin("AutoAim", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings)) {
            ImVec2 windowSize = GetWindowSize();
            float availableWidth = windowSize.x - GetStyle().WindowPadding.x * 2 - GetStyle().ItemSpacing.x * 2;
            float buttonWidth = availableWidth / 3;
            float availableHeight = windowSize.y - GetStyle().WindowPadding.y * 2;
            float buttonSize = (buttonWidth < availableHeight) ? buttonWidth : availableHeight; // Make square
            
            if (Button(ix ? "x" : "o", ImVec2(buttonSize, buttonSize))) ix = !ix;
            SameLine(); if (Button("<", ImVec2(buttonSize, buttonSize))) AIM(persistent_float["fAIM_AngleStep"]);
            SameLine(); if (Button(">", ImVec2(buttonSize, buttonSize))) AIM(-persistent_float["fAIM_AngleStep"]);
        } End();
    }
};
