/*
 * 240p Test Suite - Wii2Xenon visual bootstrap
 *
 * Branch-only adapter for the first image/scene path on Xbox 360.
 * Keeps the public ImagePtr / DrawImage / StartScene / EndScene shape used by
 * the Wii code while mapping the minimum required behavior into GX360.
 */

#include "image.h"

/*
 * P0.2 scene bootstrap.
 * The original Wii StartScene configures viewport, vertex descriptors and a
 * model-view matrix. GX360 currently owns a fixed compatible immediate-mode
 * pipeline, so those state calls are intentionally absorbed here for now.
 */
void StartScene(void)
{
    GX_SetViewport(0.0f, 0.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    GX_InvVtxCache();
    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
}

/*
 * The original Wii EndScene also handles scanlines, EFB/XFB copying and VIDEO
 * synchronization. P0.2 maps only render state + presentation; framebuffer
 * emulation remains a later compatibility slice.
 */
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
