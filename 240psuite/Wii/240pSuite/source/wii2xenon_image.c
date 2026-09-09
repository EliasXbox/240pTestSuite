/*
 * 240p Test Suite - Wii2Xenon visual bootstrap
 *
 * P0.4 Native DrawImage bootstrap.
 * StartScene/EndScene remain the small Xbox-facing scene adapter, while
 * DrawImage now mirrors the original 240p Suite DrawImage path and feeds
 * native 240p coordinates directly into Wii2Xenon's GX compatibility layer.
 */

#include "image.h"

/* Minimal mode state required by the original DrawImage control flow.
 * P0.4 runs in the normal 320x240 path: no PAL offset and no double-scale
 * video mode. These values can later move to the VIDEO compatibility layer.
 */
static float wii2xenon_offsetY = 0.0f;
static int wii2xenon_doubleScaleMode = 0;

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

/*
 * Kept intentionally line-for-line close to image.c::DrawImage().
 * The only branch-specific substitution is the temporary video-mode state
 * above; the actual GX draw sequence is the original 240p path.
 */
void DrawImage(ImagePtr image)
{
    float x, y, w, h;

    if(!image)
        return;

    x = image->x;
    y = image->y;
    w = image->w;
    h = image->h;

    // Original 240p behavior: center vertically when the current mode asks
    // for it. P0.4's NTSC 240p bootstrap keeps this at zero.
    if(!image->IgnoreOffsetY)
        y += wii2xenon_offsetY;

    // Original 240p behavior doubles coordinates in its special 480/576
    // scanline modes. P0.4 deliberately stays in the native 320x240 path.
    if(image->scale && wii2xenon_doubleScaleMode)
    {
        x *= 2;
        y *= 2;
        w *= 2;
        h *= 2;
    }

    GX_LoadTexObj(&image->tex, GX_TEXMAP0);

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position2f32(x, y);
        GX_Color4u8(image->r, image->g, image->b, image->alpha);
        GX_TexCoord2f32(image->u1, image->v1);

        GX_Position2f32(x+w, y);
        GX_Color4u8(image->r, image->g, image->b, image->alpha);
        GX_TexCoord2f32(image->u2, image->v1);

        GX_Position2f32(x+w, y+h);
        GX_Color4u8(image->r, image->g, image->b, image->alpha);
        GX_TexCoord2f32(image->u2, image->v2);

        GX_Position2f32(x, y+h);
        GX_Color4u8(image->r, image->g, image->b, image->alpha);
        GX_TexCoord2f32(image->u1, image->v2);
    GX_End();
    GX_DrawDone();
}
