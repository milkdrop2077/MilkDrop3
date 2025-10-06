/*
  LICENSE
  -------
Copyright 2005-2013 Nullsoft, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of Nullsoft nor the names of its contributors may be used to
    endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "texmgr.h"
#include "../ns-eel2/ns-eel.h"
#include "support.h"
#include "plugin.h"
#include "utility.h"

texmgr::texmgr()
{
}

texmgr::~texmgr()
{
	// DO NOT RELEASE OR DELETE m_lpDD; CLIENT SHOULD DO THIS!
}

void texmgr::Finish()
{
	for (int i=0; i<NUM_TEX; i++)
	{
		KillTex(i);
		NSEEL_VM_free(m_tex[i].tex_eel_ctx);
	}

	// DO NOT RELEASE OR DELETE m_lpDD; CLIENT SHOULD DO THIS!
}

void texmgr::Init(LPDIRECT3DDEVICE9 lpDD)
{
	m_lpDD = lpDD;

	for (int i=0; i<NUM_TEX; i++)
	{
		m_tex[i].pSurface = NULL;
		m_tex[i].szFileName[0] = 0;
		m_tex[i].m_codehandle = NULL;
		m_tex[i].m_szExpr[0] = 0;
		m_tex[i].tex_eel_ctx = NSEEL_VM_alloc();
	}
}

int texmgr::LoadTex(const char *szFilename, int iSlot, char *szInitCode, char *szCode, float time, int frame, unsigned int ck)
{
	if (iSlot < 0) return TEXMGR_ERR_BAD_INDEX;
	if (iSlot >= NUM_TEX) return TEXMGR_ERR_BAD_INDEX;

	// first, if this texture is already loaded, just add another instance.
	bool bTextureInstanced = false;
#ifdef _WIN32
	{
		for (int x=0; x<NUM_TEX; x++)
			if (m_tex[x].pSurface && _stricmp(m_tex[x].szFileName, szFilename)==0)
			{
				memcpy(&m_tex[iSlot], &m_tex[x], sizeof(td_texture_info));
				m_tex[iSlot].m_szExpr[0] = 0;
				m_tex[iSlot].m_codehandle  = 0;

				bTextureInstanced = true;
				break;
			}
	}

	if (!bTextureInstanced)
	{
		// Free old resources:
		KillTex(iSlot);

		strcpy(m_tex[iSlot].szFileName, szFilename);

        D3DXIMAGE_INFO info;
        HRESULT hr = D3DXCreateTextureFromFileExA(
          m_lpDD,
          szFilename,
          D3DX_DEFAULT,
          D3DX_DEFAULT,
          D3DX_DEFAULT, // create a mip chain
          0,
          D3DFMT_UNKNOWN,
          D3DPOOL_DEFAULT,
          D3DX_DEFAULT,
          D3DX_DEFAULT,
          0xFF000000 | ck,
          &info,
          NULL,
          &m_tex[iSlot].pSurface
        );

        if (hr != D3D_OK)
        {
            switch(hr)
            {
            case E_OUTOFMEMORY:
            case D3DERR_OUTOFVIDEOMEMORY:
                return TEXMGR_ERR_OUTOFMEM;
            default:
			    return TEXMGR_ERR_BADFILE;
            }
        }

        m_tex[iSlot].img_w = info.Width;
		m_tex[iSlot].img_h = info.Height;
	}
#endif

	m_tex[iSlot].fStartTime = time;
	m_tex[iSlot].nStartFrame = frame;

	int ret = TEXMGR_ERR_SUCCESS;

	// compile & run init. code:
	if (!RunInitCode(iSlot, szInitCode))
		ret |= TEXMGR_WARN_ERROR_IN_INIT_CODE;

	// compile & save per-frame code:
	strcpy(m_tex[iSlot].m_szExpr, szCode);
	FreeCode(iSlot);
	if (!RecompileExpressions(iSlot))
		ret |= TEXMGR_WARN_ERROR_IN_REG_CODE;

	//g_dumpmsg("texmgr: success");

	return ret;
}

void texmgr::KillTex(int iSlot)
{
	if (iSlot < 0) return;
	if (iSlot >= NUM_TEX) return;

#ifdef _WIN32
	// Free old resources:
	if (m_tex[iSlot].pSurface)
	{
		// first, make sure no other sprites reference this texture!
		int refcount = 0;
		for (int x=0; x<NUM_TEX; x++)
			if (m_tex[x].pSurface == m_tex[iSlot].pSurface)
				refcount++;

		if (refcount==1)
			((LPDIRECT3DTEXTURE9)m_tex[iSlot].pSurface)->Release();
		m_tex[iSlot].pSurface = NULL;
	}
#endif
	m_tex[iSlot].szFileName[0] = 0;

	FreeCode(iSlot);
}

void texmgr::StripLinefeedCharsAndComments(char *src, char *dest)
{
	// replaces all LINEFEED_CONTROL_CHAR characters in src with a space in dest;
	// also strips out all comments (beginning with '//' and going til end of line).
	// Restriction: sizeof(dest) must be >= sizeof(src).

	int i2 = 0;
	int len = strlen(src);
	int bComment = false;
	for (int i=0; i<len; i++)
	{
		if (bComment)
		{
			if (src[i] == LINEFEED_CONTROL_CHAR)
				bComment = false;
		}
		else
		{
			if ((src[i] =='\\' && src[i+1] =='\\') || (src[i] =='/' && src[i+1] =='/'))
				bComment = true;
			else if (src[i] != LINEFEED_CONTROL_CHAR)
				dest[i2++] = src[i];
		}
	}
	dest[i2] = 0;
}

bool texmgr::RunInitCode(int iSlot, char *szInitCode)
{
	// warning: destroys contents of m_tex[iSlot].m_szExpr,
	//   so be sure to call RunInitCode before writing or
	//   compiling that string!

	FreeCode(iSlot);
	FreeVars(iSlot);
	RegisterBuiltInVariables(iSlot);

	strcpy(m_tex[iSlot].m_szExpr, szInitCode);
	bool ret = RecompileExpressions(iSlot);

	// set default values of output variables:
	// (by not setting these every frame, we allow the values to persist from frame-to-frame.)
	*(m_tex[iSlot].var_x)        = 0.5;
	*(m_tex[iSlot].var_y)        = 0.5;
	*(m_tex[iSlot].var_sx)       = 1.0;
	*(m_tex[iSlot].var_sy)       = 1.0;
	*(m_tex[iSlot].var_repeatx)  = 1.0;
	*(m_tex[iSlot].var_repeaty)  = 1.0;
	*(m_tex[iSlot].var_rot)      = 0.0;
	*(m_tex[iSlot].var_flipx)    = 0.0;
	*(m_tex[iSlot].var_flipy)    = 0.0;
	*(m_tex[iSlot].var_r)        = 1.0;
	*(m_tex[iSlot].var_g)        = 1.0;
	*(m_tex[iSlot].var_b)        = 1.0;
	*(m_tex[iSlot].var_a)        = 1.0;
	*(m_tex[iSlot].var_blendmode)= 0.0;
	*(m_tex[iSlot].var_done)     = 0.0;
	*(m_tex[iSlot].var_burn)     = 1.0;

	#ifndef _NO_EXPR_
		if (m_tex[iSlot].m_codehandle)
			NSEEL_code_execute(m_tex[iSlot].m_codehandle);
	#endif

	return ret;
}

bool texmgr::RecompileExpressions(int iSlot)
{
	char *expr = m_tex[iSlot].m_szExpr;

	// QUICK FIX: if the string ONLY has spaces and linefeeds, erase it,
	// because for some strange reason this would cause an error in compileCode().
	{
		char *p = expr;
		while (*p==' ' || *p==LINEFEED_CONTROL_CHAR) p++;
		if (*p == 0) expr[0] = 0;
	}

	// replace linefeed control characters with spaces, so they don't mess up the code compiler,
	// and strip out any comments ('//') before sending to CompileCode().
	char buf[sizeof(m_tex[iSlot].m_szExpr)];
	StripLinefeedCharsAndComments(expr, buf);

	return true;
}

void texmgr::FreeVars(int iSlot)
{
	// free the built-in variables AND any user variables
    if (m_tex[iSlot].tex_eel_ctx)
        NSEEL_VM_freevars(m_tex[iSlot].tex_eel_ctx);
}

void texmgr::FreeCode(int iSlot)
{
	// free the compiled expressions
	if (m_tex[iSlot].m_codehandle)
	{
		NSEEL_code_free(m_tex[iSlot].m_codehandle);
		m_tex[iSlot].m_codehandle = NULL;
	}
}

void texmgr::RegisterBuiltInVariables(int iSlot)
{
	NSEEL_VMCTX eel_ctx = m_tex[iSlot].tex_eel_ctx;
	NSEEL_VM_freevars(eel_ctx);

	// input variables
    m_tex[iSlot].var_time        = NSEEL_VM_regvar(eel_ctx, "time");
	m_tex[iSlot].var_frame       = NSEEL_VM_regvar(eel_ctx, "frame");
	m_tex[iSlot].var_fps         = NSEEL_VM_regvar(eel_ctx, "fps");
	m_tex[iSlot].var_progress    = NSEEL_VM_regvar(eel_ctx, "progress");
	m_tex[iSlot].var_bass        = NSEEL_VM_regvar(eel_ctx, "bass");
	m_tex[iSlot].var_bass_att    = NSEEL_VM_regvar(eel_ctx, "bass_att");
	m_tex[iSlot].var_mid         = NSEEL_VM_regvar(eel_ctx, "mid");
	m_tex[iSlot].var_mid_att     = NSEEL_VM_regvar(eel_ctx, "mid_att");
	m_tex[iSlot].var_treb        = NSEEL_VM_regvar(eel_ctx, "treb");
	m_tex[iSlot].var_treb_att    = NSEEL_VM_regvar(eel_ctx, "treb_att");

	// output variables
	m_tex[iSlot].var_x           = NSEEL_VM_regvar(eel_ctx, "x");
	m_tex[iSlot].var_y           = NSEEL_VM_regvar(eel_ctx, "y");
	m_tex[iSlot].var_sx          = NSEEL_VM_regvar(eel_ctx, "sx");
	m_tex[iSlot].var_sy          = NSEEL_VM_regvar(eel_ctx, "sy");
	m_tex[iSlot].var_repeatx     = NSEEL_VM_regvar(eel_ctx, "repeatx");
	m_tex[iSlot].var_repeaty     = NSEEL_VM_regvar(eel_ctx, "repeaty");
	m_tex[iSlot].var_rot         = NSEEL_VM_regvar(eel_ctx, "rot");
	m_tex[iSlot].var_flipx       = NSEEL_VM_regvar(eel_ctx, "flipx");
	m_tex[iSlot].var_flipy       = NSEEL_VM_regvar(eel_ctx, "flipy");
	m_tex[iSlot].var_r           = NSEEL_VM_regvar(eel_ctx, "r");
	m_tex[iSlot].var_g           = NSEEL_VM_regvar(eel_ctx, "g");
	m_tex[iSlot].var_b           = NSEEL_VM_regvar(eel_ctx, "b");
	m_tex[iSlot].var_a           = NSEEL_VM_regvar(eel_ctx, "a");
	m_tex[iSlot].var_blendmode   = NSEEL_VM_regvar(eel_ctx, "blendmode");
	m_tex[iSlot].var_done        = NSEEL_VM_regvar(eel_ctx, "done");
	m_tex[iSlot].var_burn        = NSEEL_VM_regvar(eel_ctx, "burn");

//	resetVars(NULL);
}