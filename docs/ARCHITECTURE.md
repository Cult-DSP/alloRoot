# Architecture

## Purpose

`alloRoot` is an embedding example around Spatial Root's public C++ API.

It is intentionally small:

- a tiny AlloLib host app
- a thin wrapper around `EngineSession`
- no custom engine fork
- no GUI replacement

## Responsibility Split

`alloRoot` owns:

- minimal host app lifecycle
- command-line parsing for example inputs
- calling the Spatial Root API in the documented order
- printing startup and failure diagnostics

Spatial Root owns:

- realtime engine state
- scene loading
- layout loading
- runtime DSP parameters
- playback
- backend diagnostics

## Why A Wrapper

The point of the example is to demonstrate a clean integration boundary.

`SpatialRootSessionWrapper` is intentionally thin. It converts host intent into the ordered `EngineSession` calls:

1. `configureEngine()`
2. `loadScene()`
3. `applyLayout()`
4. `configureRuntime()`
5. `start()`

After startup, it calls `update()`, polls `queryStatus()`, consumes diagnostics, and handles pause and shutdown.

## Why Not Modify Spatial Root

The example should behave like a real external host application.

That means:

- Spatial Root stays a dependency, not a codebase we rewrite
- integration happens through the published API
- any alloRoot-specific build accommodations stay in this repo

## Build Shape

`alloRoot` builds three layers:

1. Spatial Root's vendored dependency stack needed by the API
2. Spatial Root's `EngineSessionCore`
3. the tiny AlloLib host executable

The host links against `EngineSessionCore` and `al`.

## Updating The Example

If Spatial Root's API changes on `devel`, update:

- [src/SpatialRootSessionWrapper.hpp](/Users/lucian/Projects/alloRoot/src/SpatialRootSessionWrapper.hpp)
- [src/SpatialRootSessionWrapper.cpp](/Users/lucian/Projects/alloRoot/src/SpatialRootSessionWrapper.cpp)
- [src/main.cpp](/Users/lucian/Projects/alloRoot/src/main.cpp)
- this documentation

Keep the example focused on the public API surface.

