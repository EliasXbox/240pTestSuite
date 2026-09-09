# Wii2Xenon bootstrap branch

This branch is the experimental Xbox 360 client side for the 240p Test Suite.
The upstream-style `master` branch remains untouched.

## First bootstrap target

The first goal is intentionally small: compile the existing 240p controller layer against Wii2Xenon without rewriting it into Xbox-specific input code.

Current target files:

- `240psuite/Wii/240pSuite/source/controller.c`
- `240psuite/Wii/240pSuite/source/controller.h`

For the first pass, build without `WII_VERSION` so the module uses the PAD/GameCube-style path only. Wii2Xenon's `WiiXInput` already provides the PAD API used here, including button events, sticks, triggers and rumble.

The Wii2Xenon branch `feature/240p-bootstrap` now supplies a minimal `gccore.h` compatibility shim. The intention is that existing source can keep including `<gccore.h>` while the compatibility implementation lives in Wii2Xenon.

## Rules for this port

1. Prefer implementing reusable Wii/libogc behavior in Wii2Xenon instead of scattering Xbox APIs through 240p source.
2. Keep this branch as close to the original Wii source as practical.
3. Add compatibility only when a real compile/runtime dependency asks for it.
4. Do not pretend unsupported APIs exist: compiler/linker failures are our dependency list.

## Next checkpoint

Get the original controller module compiling in the Xbox 360 Visual Studio project against Wii2Xenon's headers. After that, move to the smallest video/GX scene path needed for the first visible 240p-derived frame.
