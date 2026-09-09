/*
 * 240p Test Suite - Wii2Xenon bootstrap
 *
 * This branch-only source is the first Xbox 360 bootstrap client for the
 * Wii2Xenon compatibility layer. It intentionally exercises the same
 * libogc-style PAD surface used by the Wii controller code before the rest
 * of the 240p application is brought over.
 */

#include <gccore.h>
#include "controller.h"

u8 EndProgram = 0;

s32 Wii2Xenon_240p_ControllerBootstrap(void)
{
    return ControllerInit();
}

u32 Wii2Xenon_240p_ControllerFrame(void)
{
    ControllerScan();
    return Controller_ButtonsDown(0);
}
