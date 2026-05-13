# Spatial Root API Wrapping

## Expected Branch

`alloRoot` targets the Spatial Root `devel` branch.

The wrapper is based on the public API documented in:

- `PUBLIC_DOCS/API.md`
- `source/spatial_engine/realtimeEngine/src/EngineSession.hpp`

## Wrapped API Surface

The example wraps these `EngineSession` stages:

1. `configureEngine(const EngineOptions&)`
2. `loadScene(const SceneInput&)`
3. `applyLayout(const LayoutInput&)`
4. `configureRuntime(const RuntimeParams&)`
5. `start()`

During runtime it uses:

- `update()`
- `queryStatus()`
- `consumeDiagnostics()`
- `setPaused(bool)`
- `shutdown()`

## Input Contract In alloRoot

`alloRoot` expects:

- `--scene <path>`
- `--layout <path>`
- exactly one of:
  `--sources <path>`
  `--adm <path>`

Optional engine controls map directly to API fields:

- sample rate
- buffer size
- device name
- OSC port
- elevation mode
- runtime gain and focus parameters

For automated verification, `alloRoot` also supports `--headless --run-seconds <float>`. That mode uses the same wrapper and `EngineSession` calls, but skips the persistent AlloLib window loop.

## Wrapper Behavior

`SpatialRootSessionWrapper` does four things:

- stores the example's startup configuration
- validates whether enough input exists to call `start()`
- executes the documented API sequence
- forwards failures through developer-friendly console output

It does not parse Spatial Root internals itself.

## Diagnostics

When a stage fails, `alloRoot` reports:

- the failed API stage
- `EngineSession::getLastError()`
- `EngineSession::getFailureDiagnostics()` when available

That keeps debugging at the official API boundary.

## Current Build Notes

The example does not modify the Spatial Root checkout.

However, the current vendored AlloLib setup on `devel` needs a few alloRoot-side build accommodations so an external host can use the same vendored targets cleanly:

- extra include visibility for vendored `cpptoml`
- extra include visibility for vendored `RtMidi`
- one missing `al_ControlNav.cpp` source added on the alloRoot side

Those shims live only in `alloRoot`'s CMake and are not changes to Spatial Root source.

## If The API Changes

If `EngineSession` or its typed structs change on `devel`, update:

1. argument parsing in [src/main.cpp](/Users/lucian/Projects/alloRoot/src/main.cpp)
2. wrapper calls in [src/SpatialRootSessionWrapper.cpp](/Users/lucian/Projects/alloRoot/src/SpatialRootSessionWrapper.cpp)
3. this document

Avoid drifting into undocumented internals.
