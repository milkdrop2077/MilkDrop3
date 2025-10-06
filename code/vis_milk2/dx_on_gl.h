#pragma once

// Dummy structs and typedefs to allow compiling DirectX code on other platforms.
// This is not a full implementation of DirectX on OpenGL.

#include <stdint.h>

typedef struct D3DXVECTOR3 {
    float x, y, z;
} D3DXVECTOR3;

typedef struct D3DXMATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DXMATRIX;

typedef enum _D3DFORMAT {
    D3DFMT_UNKNOWN              =  0,
    D3DFMT_R8G8B8               = 20,
    D3DFMT_A8R8G8B8             = 21,
    D3DFMT_X8R8G8B8             = 22,
    D3DFMT_DXT1                 = 827611204, // MAKEFOURCC('D', 'X', 'T', '1'),
    D3DFMT_DXT2                 = 844388420, // MAKEFOURCC('D', 'X', 'T', '2'),
    D3DFMT_DXT3                 = 861165636, // MAKEFOURCC('D', 'X', 'T', '3'),
    D3DFMT_DXT4                 = 877942852, // MAKEFOURCC('D', 'X', 'T', '4'),
    D3DFMT_DXT5                 = 894720068, // MAKEFOURCC('D', 'X', 'T', '5'),
} D3DFORMAT;

// Dummy interface
typedef void* IDirect3DDevice9;
typedef void* IDirect3DTexture9;
typedef void* IDirect3DBaseTexture9;
typedef void* IDirect3DVertexShader9;
typedef void* IDirect3DPixelShader9;
typedef void* IDirect3DVertexDeclaration9;
typedef void* LPD3DXCONSTANTTABLE;
typedef void* LPD3DXBUFFER;
typedef void* LPD3DXFONT;
typedef void* D3DXHANDLE;
typedef void* LPDIRECT3DDEVICE9;
typedef void* LPDIRECT3DVERTEXBUFFER9;
typedef void* LPDIRECT3DINDEXBUFFER9;
typedef void* LPDIRECT3DSURFACE9;
typedef void* LPDIRECT3DTEXTURE9;
typedef void* LPDIRECT3DBASETEXTURE9;
typedef uint32_t D3DCOLOR;
typedef struct _D3DCAPS9 {
    uint32_t MaxTextureWidth, MaxTextureHeight;
    uint32_t MaxPrimitiveCount;
    uint32_t SrcBlendCaps;
    uint32_t DestBlendCaps;
} D3DCAPS9;
typedef struct _D3DPRESENT_PARAMETERS_ {
    uint32_t BackBufferWidth;
    uint32_t BackBufferHeight;
} D3DPRESENT_PARAMETERS;
typedef struct _D3DSURFACE_DESC {
    D3DFORMAT Format;
    uint32_t Width;
    uint32_t Height;
} D3DSURFACE_DESC;
typedef struct _D3DLOCKED_RECT {
    int Pitch;
    void* pBits;
} D3DLOCKED_RECT;
typedef struct D3DXVECTOR4 {
    float x, y, z, w;
} D3DXVECTOR4;