#include "AlloRootApp.hpp"

#include <iostream>

AlloRootApp::AlloRootApp(SpatialRootSessionWrapper::SessionConfig config)
    : mWrapper(std::move(config)),
      mStatus("Preparing wrapper") {
    title("alloRoot");
    dimensions(640, 180);
    configureAudio(48000, 256, 0, 0);
}

AlloRootApp::~AlloRootApp() = default;

void AlloRootApp::onCreate() {
    navControl().active(false);
    printBanner();
    attemptStart("startup");
}

void AlloRootApp::onAnimate(double dt) {
    (void)dt;
    mWrapper.update();
}

void AlloRootApp::onDraw(al::Graphics& g) {
    g.clear(0.09f, 0.1f, 0.12f);
}

void AlloRootApp::onExit() {
    std::cout << "[alloRoot] Shutting down API wrapper app." << std::endl;
    mWrapper.shutdown();
}

bool AlloRootApp::onKeyDown(const al::Keyboard& k) {
    if (k.key() == ' ') {
        attemptStart("space key");
        return false;
    }

    if (k.key() == 'p' || k.key() == 'P') {
        mWrapper.togglePause();
        mStatus = mWrapper.statusLine();
        updateWindowTitle();
        return false;
    }

    if (k.key() == 'q' || k.key() == 'Q' || k.key() == al::Keyboard::ESCAPE) {
        std::cout << "[alloRoot] Quit requested from keyboard." << std::endl;
        quit();
        return false;
    }

    return true;
}

void AlloRootApp::attemptStart(const char* reason) {
    std::cout << "[alloRoot] Start attempt triggered by " << reason << "." << std::endl;
    mWrapper.start();
    mStatus = mWrapper.statusLine();
    updateWindowTitle();

    if (!mAttemptedStartupStart) {
        mAttemptedStartupStart = true;
        std::cout << "[alloRoot] Controls: space = start if configured, p = pause/resume, q or escape = quit." << std::endl;
    }
}

void AlloRootApp::printBanner() const {
    std::cout << "[alloRoot] Minimal AlloLib wrapper around Spatial Root's EngineSessionCore API." << std::endl;
    std::cout << "[alloRoot] This host app exercises the public API boundary without changing Spatial Root." << std::endl;
    if (!mWrapper.canStart()) {
        std::cout << mWrapper.startupGuidance() << std::endl;
    }
}

void AlloRootApp::updateWindowTitle() {
    title("alloRoot - " + mStatus);
}
