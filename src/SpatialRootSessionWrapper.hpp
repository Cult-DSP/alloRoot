#pragma once

#include "EngineSession.hpp"

#include <string>

class SpatialRootSessionWrapper {
public:
    struct SessionConfig {
        std::string spatialRootDir;
        std::string spatialRootDirSource;

        EngineOptions engineOptions;
        SceneInput sceneInput;
        LayoutInput layoutInput;
        RuntimeParams runtimeParams = RuntimeParams::defaults();

        bool listDevicesOnly = false;
    };

    explicit SpatialRootSessionWrapper(SessionConfig config);
    ~SpatialRootSessionWrapper();

    bool printDevicesIfRequested() const;
    bool canStart() const;
    bool isRunning() const;
    bool isPaused() const;

    bool start();
    void update();
    void togglePause();
    void shutdown();

    std::string statusLine() const;
    std::string startupGuidance() const;

private:
    void setStatus(const std::string& status);
    void printFailureDetails(const char* stage) const;

    SessionConfig mConfig;
    EngineSession mSession;
    std::string mStatus = "Wrapper idle";
    bool mRunning = false;
    bool mPaused = false;
};
