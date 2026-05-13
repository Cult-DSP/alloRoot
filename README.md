# alloRoot

`alloRoot` is a small developer example that shows how an external AlloLib-based application can wrap Spatial Root's public `EngineSessionCore` API from the `devel` branch.

It is not a new Spatial Root frontend, not a replacement for the existing GUI, and not a reimplementation of the audio engine. It is a minimal embedding example around the published C++ API boundary.

## What alloRoot Does

- builds a tiny AlloLib host app
- links against Spatial Root's `EngineSessionCore`
- configures `EngineSession` through a thin wrapper class
- starts the realtime engine when valid scene and layout inputs are provided
- prints clear diagnostics from `getLastError()` and `getFailureDiagnostics()`

## What alloRoot Does Not Do

- it does not modify Spatial Root
- it does not launch or replace the Spatial Root GUI
- it does not reimplement DBAP, layout parsing, LUSID parsing, device routing, or transcoding
- it does not introduce a second engine architecture

## Spatial Root Source

This example targets the Spatial Root `devel` branch only.

Preferred setup:

```bash
git submodule add -b devel https://github.com/Cult-DSP/spatialroot.git thirdparty/spatialroot
```

Build-time override:

```bash
cmake -S . -B build -DSPATIAL_ROOT_DIR=/path/to/spatialroot
```

If both are available, `SPATIAL_ROOT_DIR` wins.

## Clone And Initialize

```bash
git clone https://github.com/Cult-DSP/alloRoot.git
cd alloRoot
git submodule update --init --recursive
```

## Build

`alloRoot` builds the wrapper app and Spatial Root's realtime engine library in one CMake build. No Spatial Root source edits are required.

```bash
cmake -S . -B build
cmake --build build
```

## Run

The example needs a scene, a layout, and exactly one source mode.

```bash
./build/alloRoot \
  --scene /path/to/scene.lusid.json \
  --layout /path/to/layout.json \
  --sources /path/to/source-package
```

Or with ADM input:

```bash
./build/alloRoot \
  --scene /path/to/scene.lusid.json \
  --layout /path/to/layout.json \
  --adm /path/to/input.wav
```

The repo root wrapper also works:

```bash
./alloRoot --scene /path/to/scene.lusid.json --layout /path/to/layout.json --sources /path/to/source-package
```

## Useful Options

```bash
./build/alloRoot --help
./build/alloRoot --list-devices
```

Other supported flags:

- `--device <name>`
- `--samplerate <int>`
- `--buffersize <int>`
- `--osc-port <int>`
- `--elevation-mode <0|1|2>`
- `--gain <dB>`
- `--focus <float>`
- `--speaker-mix <dB>`
- `--sub-mix <dB>`

## Controls

- `space`: attempt to start the Spatial Root session if the required inputs were provided
- `p`: pause or resume the running session
- `q`: quit
- `escape`: quit

## Architecture Summary

`alloRoot` keeps the integration boundary narrow:

- `AlloRootApp` owns the tiny host window and keyboard lifecycle
- `SpatialRootSessionWrapper` owns the calls into `EngineSession`
- Spatial Root owns scene loading, layout handling, runtime DSP, playback, and diagnostics

This example stays on the public API boundary rather than reaching into GUI-specific or engine-internal implementation details.

## Troubleshooting

- `Spatial Root directory not found`
  Initialize the submodule or pass `-DSPATIAL_ROOT_DIR=/path/to/spatialroot`.

- `Session configuration incomplete`
  Provide `--scene`, `--layout`, and exactly one of `--sources` or `--adm`.

- `loadScene failed` or `applyLayout failed`
  Check the input paths and inspect the emitted failure diagnostics.

- nested Spatial Root submodules are missing
  Run `git submodule update --init --recursive`.

## Known Limitations On Current devel

- The example builds against Spatial Root's vendored `cult-allolib` and `EngineSessionCore` targets directly from the submodule checkout.
- The current `devel` vendored AlloLib setup needs small alloRoot-side build shims to expose two upstream vendored include paths and one missing source file while keeping Spatial Root itself unchanged.
- Windows support is not the focus of this example.

See [docs/ARCHITECTURE.md](/Users/lucian/Projects/alloRoot/docs/ARCHITECTURE.md) and [docs/SPATIAL_ROOT_API_WRAPPING.md](/Users/lucian/Projects/alloRoot/docs/SPATIAL_ROOT_API_WRAPPING.md).
