#include "SpatialRootSessionWrapper.hpp"

#include "al/io/al_AudioIO.hpp"

#include <iostream>
#include <sstream>

SpatialRootSessionWrapper::SpatialRootSessionWrapper(SessionConfig config)
    : mConfig(std::move(config)) {}

SpatialRootSessionWrapper::~SpatialRootSessionWrapper() {
    shutdown();
}

bool SpatialRootSessionWrapper::printDevicesIfRequested() const {
    if (!mConfig.listDevicesOnly) {
        return false;
    }

    std::cout << "[alloRoot] Available output devices:" << std::endl;
    const int count = al::AudioDevice::numDevices();
    for (int i = 0; i < count; ++i) {
        al::AudioDevice device(i);
        if (!device.valid() || device.channelsOutMax() <= 0) {
            continue;
        }

        std::cout << "  [" << i << "] " << device.name()
                  << " (outputs=" << device.channelsOutMax()
                  << ", preferredRate=" << device.defaultSampleRate() << ")"
                  << std::endl;
    }
    return true;
}

bool SpatialRootSessionWrapper::canStart() const {
    const bool hasScene = !mConfig.sceneInput.scenePath.empty();
    const bool hasLayout = !mConfig.layoutInput.layoutPath.empty();
    const bool hasAdm = !mConfig.sceneInput.admFile.empty();
    const bool hasSources = !mConfig.sceneInput.sourcesFolder.empty();
    return hasScene && hasLayout && (hasAdm != hasSources);
}

bool SpatialRootSessionWrapper::isRunning() const {
    return mRunning;
}

bool SpatialRootSessionWrapper::isPaused() const {
    return mPaused;
}

bool SpatialRootSessionWrapper::start() {
    if (mRunning) {
        setStatus("Spatial Root session already running");
        return true;
    }

    if (!canStart()) {
        setStatus("Session configuration incomplete");
        std::cout << startupGuidance() << std::endl;
        return false;
    }

    std::cout << "[alloRoot] Configuring Spatial Root API wrapper." << std::endl;

    if (!mSession.configureEngine(mConfig.engineOptions)) {
        setStatus("configureEngine failed");
        printFailureDetails("configureEngine");
        return false;
    }

    if (!mSession.loadScene(mConfig.sceneInput)) {
        setStatus("loadScene failed");
        printFailureDetails("loadScene");
        return false;
    }

    if (!mSession.applyLayout(mConfig.layoutInput)) {
        setStatus("applyLayout failed");
        printFailureDetails("applyLayout");
        return false;
    }

    if (!mSession.configureRuntime(mConfig.runtimeParams)) {
        setStatus("configureRuntime failed");
        printFailureDetails("configureRuntime");
        return false;
    }

    if (!mSession.start()) {
        setStatus("start failed");
        printFailureDetails("start");
        return false;
    }

    mRunning = true;
    mPaused = false;
    setStatus("Spatial Root session running");
    std::cout << "[alloRoot] Spatial Root session started through EngineSessionCore." << std::endl;
    return true;
}

void SpatialRootSessionWrapper::update() {
    if (!mRunning) {
        return;
    }

    mSession.update();
    const EngineStatus status = mSession.queryStatus();
    if (status.isExitRequested) {
        std::cout << "[alloRoot] Spatial Root requested exit." << std::endl;
        shutdown();
        return;
    }

    mPaused = status.paused;

    const DiagnosticEvents events = mSession.consumeDiagnostics();
    if (events.renderRelocEvent || events.deviceRelocEvent ||
        events.renderDomRelocEvent || events.deviceDomRelocEvent ||
        events.renderClusterEvent || events.deviceClusterEvent) {
        std::cout << "[alloRoot] Diagnostic event received from Spatial Root." << std::endl;
    }
}

void SpatialRootSessionWrapper::togglePause() {
    if (!mRunning) {
        setStatus("Cannot pause before start");
        return;
    }

    mPaused = !mPaused;
    mSession.setPaused(mPaused);
    setStatus(mPaused ? "Spatial Root session paused" : "Spatial Root session running");
    std::cout << "[alloRoot] " << statusLine() << std::endl;
}

void SpatialRootSessionWrapper::shutdown() {
    if (!mRunning) {
        return;
    }

    mSession.shutdown();
    mRunning = false;
    mPaused = false;
    setStatus("Spatial Root session stopped");
}

std::string SpatialRootSessionWrapper::statusLine() const {
    return mStatus;
}

std::string SpatialRootSessionWrapper::startupGuidance() const {
    std::ostringstream stream;
    stream << "[alloRoot] Spatial Root API wrapper is ready, but startup data is incomplete.\n"
           << "[alloRoot] Required:\n"
           << "  --scene /path/to/scene.lusid.json\n"
           << "  --layout /path/to/layout.json\n"
           << "  and exactly one of:\n"
           << "  --sources /path/to/package-folder\n"
           << "  --adm /path/to/file.wav\n"
           << "[alloRoot] Spatial Root checkout source: " << mConfig.spatialRootDirSource << "\n"
           << "[alloRoot] Spatial Root checkout path: " << mConfig.spatialRootDir;
    return stream.str();
}

void SpatialRootSessionWrapper::setStatus(const std::string& status) {
    mStatus = status;
}

void SpatialRootSessionWrapper::printFailureDetails(const char* stage) const {
    std::cout << "[alloRoot] " << stage << " failed: " << mSession.getLastError() << std::endl;
    const std::string diagnostics = mSession.getFailureDiagnostics();
    if (!diagnostics.empty()) {
        std::cout << diagnostics << std::endl;
    }
}
