#include "AlloRootApp.hpp"
#include "BuildConfig.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

namespace {

struct Options {
    SpatialRootSessionWrapper::SessionConfig sessionConfig;
};

std::string getArgString(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == flag) {
            return argv[i + 1];
        }
    }
    return "";
}

int getArgInt(int argc, char* argv[], const std::string& flag, int defaultValue) {
    const std::string value = getArgString(argc, argv, flag);
    if (value.empty()) {
        return defaultValue;
    }

    try {
        return std::stoi(value);
    } catch (...) {
        return defaultValue;
    }
}

float getArgFloat(int argc, char* argv[], const std::string& flag, float defaultValue) {
    const std::string value = getArgString(argc, argv, flag);
    if (value.empty()) {
        return defaultValue;
    }

    try {
        return std::stof(value);
    } catch (...) {
        return defaultValue;
    }
}

bool hasArg(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == flag) {
            return true;
        }
    }
    return false;
}

void printUsage(const char* programName) {
    std::cout
        << "Usage: " << programName << " [options]\n"
        << "\n"
        << "Spatial Root API inputs:\n"
        << "  --scene <path>           LUSID scene JSON path\n"
        << "  --layout <path>          Speaker layout JSON path\n"
        << "  --sources <path>         Folder containing mono sources\n"
        << "  --adm <path>             ADM WAV file\n"
        << "\n"
        << "Engine options:\n"
        << "  --device <name>          Exact output device name\n"
        << "  --samplerate <int>       Sample rate in Hz (default: 48000)\n"
        << "  --buffersize <int>       Buffer size in frames (default: 512)\n"
        << "  --osc-port <int>         OSC port (default: 0 for embedding)\n"
        << "  --elevation-mode <n>     0=RescaleAtmosUp, 1=RescaleFullSphere, 2=Clamp\n"
        << "\n"
        << "Runtime params:\n"
        << "  --gain <dB>              Master gain in dB\n"
        << "  --focus <float>          DBAP focus\n"
        << "  --speaker-mix <dB>       Loudspeaker trim in dB\n"
        << "  --sub-mix <dB>           Subwoofer trim in dB\n"
        << "\n"
        << "Utility:\n"
        << "  --list-devices           List output devices and exit\n"
        << "  --help                   Print this message\n";
}

Options parseArgs(int argc, char* argv[]) {
    Options options;
    options.sessionConfig.spatialRootDir = ALLOWROOT_SPATIAL_ROOT_DIR;
    options.sessionConfig.spatialRootDirSource = ALLOWROOT_SPATIAL_ROOT_SOURCE;

    options.sessionConfig.engineOptions.sampleRate = getArgInt(argc, argv, "--samplerate", 48000);
    options.sessionConfig.engineOptions.bufferSize = getArgInt(argc, argv, "--buffersize", 512);
    options.sessionConfig.engineOptions.outputDeviceName = getArgString(argc, argv, "--device");
    options.sessionConfig.engineOptions.oscPort = getArgInt(argc, argv, "--osc-port", 0);
    options.sessionConfig.engineOptions.elevationMode =
        static_cast<ElevationMode>(std::max(0, std::min(2, getArgInt(argc, argv, "--elevation-mode", 0))));

    options.sessionConfig.sceneInput.scenePath = getArgString(argc, argv, "--scene");
    options.sessionConfig.sceneInput.sourcesFolder = getArgString(argc, argv, "--sources");
    options.sessionConfig.sceneInput.admFile = getArgString(argc, argv, "--adm");
    options.sessionConfig.layoutInput.layoutPath = getArgString(argc, argv, "--layout");

    options.sessionConfig.runtimeParams = RuntimeParams::defaults();
    options.sessionConfig.runtimeParams.masterGainDb =
        getArgFloat(argc, argv, "--gain", options.sessionConfig.runtimeParams.masterGainDb);
    options.sessionConfig.runtimeParams.dbapFocus =
        getArgFloat(argc, argv, "--focus", options.sessionConfig.runtimeParams.dbapFocus);
    options.sessionConfig.runtimeParams.speakerMixDb =
        getArgFloat(argc, argv, "--speaker-mix", options.sessionConfig.runtimeParams.speakerMixDb);
    options.sessionConfig.runtimeParams.subMixDb =
        getArgFloat(argc, argv, "--sub-mix", options.sessionConfig.runtimeParams.subMixDb);

    options.sessionConfig.listDevicesOnly = hasArg(argc, argv, "--list-devices");
    return options;
}

} // namespace

int main(int argc, char* argv[]) {
    if (hasArg(argc, argv, "--help") || hasArg(argc, argv, "-h")) {
        printUsage(argv[0]);
        return 0;
    }

    const Options options = parseArgs(argc, argv);

    std::cout << "[alloRoot] Spatial Root source: " << options.sessionConfig.spatialRootDirSource << std::endl;
    std::cout << "[alloRoot] Spatial Root directory: " << options.sessionConfig.spatialRootDir << std::endl;

    SpatialRootSessionWrapper wrapper(options.sessionConfig);
    if (wrapper.printDevicesIfRequested()) {
        return 0;
    }

    AlloRootApp app(options.sessionConfig);
    app.start();
    return 0;
}
