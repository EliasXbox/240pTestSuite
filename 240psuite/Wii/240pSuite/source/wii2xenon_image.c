/*
 * 240p Test Suite - Wii2Xenon visual bootstrap
 *
 * Branch-only adapter for the first image path on Xbox 360.
 * Keeps the public ImagePtr / DrawImage shape used by the Wii code while
 * mapping 320x240-style coordinates into Wii2Xenon's current clip-space GX360.
 */

#include "image.h"

void DrawImage(ImagePtr image)
{
    float left;
    float right;
    float top;
    float bottom;

    if(!image)
        return;

    left   = (image->x / 160.0f) - 1.0f;
    right  = ((image->x + image->w) / 160.0f) - 1.0f;
    top    = 1.0f - (image->y / 120.0f);
    bottom = 1.0f - ((image->y + image->h) / 120.0f);

    GX_LoadTexObj(&image->tex, GX_TEXMAP0);

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position2f32(left, top);
        GX_Color4u8(image->r, image->g, image->b, image->alpha);
        GX_TexCoord2f32(image->u1, image->v1);

        GX_Position2f32(right, top);
        GX_Color4u8(image->r, image->g, image->b, image->alpha);
        GX_TexCoord2f32(image->u2, image->v1);

        GX_Position2f32(right, bottom);
        GX_Color4u8(image->r, image->g, image->b, image->alpha);
        GX_TexCoord2f32(image->u2, image->v2);

        GX_Position2f32(left, bottom);
        GX_Color4u8(image->r, image->g, image->b, image->alpha);
        GX_TexCoord2f32(image->u1, image->v2);
    GX_End();
    GX_DrawDone();
}
