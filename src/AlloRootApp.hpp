#pragma once

#include "SpatialRootSessionWrapper.hpp"

#include "al/app/al_App.hpp"

#include <string>

class AlloRootApp : public al::App {
public:
    explicit AlloRootApp(SpatialRootSessionWrapper::SessionConfig config);
    ~AlloRootApp() override;

    void onCreate() override;
    void onAnimate(double dt) override;
    void onDraw(al::Graphics& g) override;
    void onExit() override;
    bool onKeyDown(const al::Keyboard& k) override;

private:
    void attemptStart(const char* reason);
    void printBanner() const;
    void updateWindowTitle();

    SpatialRootSessionWrapper mWrapper;
    std::string mStatus;
    bool mAttemptedStartupStart = false;
};
