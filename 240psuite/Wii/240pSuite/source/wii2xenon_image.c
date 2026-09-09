/*
 * 240p Test Suite - Wii2Xenon visual bootstrap
 *
 * P0.3: keep the public ImagePtr / DrawImage / StartScene / EndScene shape,
 * but stop converting 320x240 coordinates in this adapter. Wii2Xenon's GX
 * compatibility layer now owns the orthographic projection step.
 */

#include "image.h"

void StartScene(void)
{
    Mtx44 projection;

    GX_SetViewport(0.0f, 0.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    GX_InvVtxCache();
    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    // Same coordinate convention used by the 240p Suite: origin at top-left,
    // 320x240 logical drawing area. GX_Position2f32 now applies this matrix.
    guOrtho(projection, 0.0f, 240.0f, 0.0f, 320.0f, 0.0f, 300.0f);
    GX_LoadProjectionMtx(projection, GX_ORTHOGRAPHIC);
}

void EndScene(void)
{
    GX_SetZMode(GX_DISABLE, GX_LEQUAL, GX_FALSE);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    GX_DrawDone();
    GX360_Present();
}

void DrawImage(ImagePtr image)
{
    float left;
    float right;
    float top;
    float bottom;

    if(!image)
        return;

    // P0.3 milestone: these are now native 240p coordinates. No clip-space
    // conversion lives in the 240p adapter anymore.
    left   = image->x;
    right  = image->x + image->w;
    top    = image->y;
    bottom = image->y + image->h;

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
