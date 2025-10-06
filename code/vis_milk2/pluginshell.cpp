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

#include "pluginshell.h"
#include "utility.h"
#include "defines.h"
#include "shell_defines.h"
#include "resource.h"
#include "wasabi.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>

#include <GLFW/glfw3.h>

char* g_szHelp = (char*)"Help text not implemented yet.";
int g_szHelp_W = 0;

CPluginShell::CPluginShell()
{
	// this should remain empty!
}

CPluginShell::~CPluginShell()
{
	// this should remain empty!
}

int       CPluginShell::GetFrame()
{
	return m_frame;
};
float     CPluginShell::GetTime()
{
	return m_time;
};
float     CPluginShell::GetFps()
{
	return m_fps;
};
HWND      CPluginShell::GetPluginWindow()
{
	if (m_lpDX) return (HWND)m_lpDX->GetWindow(); else return NULL;
};
int       CPluginShell::GetWidth()
{
	if (m_lpDX) return m_lpDX->m_client_width;  else return 0;
};
int       CPluginShell::GetHeight()
{
	if (m_lpDX) return m_lpDX->m_client_height; else return 0;
};
int       CPluginShell::GetCanvasMarginX()
{
	if (m_lpDX) return (m_lpDX->m_client_width  - m_lpDX->m_REAL_client_width)/2; else return 0;
};
int       CPluginShell::GetCanvasMarginY()
{
	if (m_lpDX) return (m_lpDX->m_client_height - m_lpDX->m_REAL_client_height)/2; else return 0;
};
HINSTANCE CPluginShell::GetInstance()
{
	return m_hInstance;
};
char* CPluginShell::GetPluginsDirPath()
{
	return m_szPluginsDirPath;
};
char* CPluginShell::GetConfigIniFile()
{
	return m_szConfigIniFile;
};
char* CPluginShell::GetConfigIniFileA()
{
	return m_szConfigIniFileA;
}
int       CPluginShell::GetFontHeight(eFontIndex idx)
{
	if (idx >= 0 && idx < NUM_BASIC_FONTS + NUM_EXTRA_FONTS) return m_fontinfo[idx].nSize; else return 0;
};
int       CPluginShell::GetBitDepth()
{
	if (m_lpDX) return m_lpDX->GetBitDepth(); else return 0;
};
LPDIRECT3DDEVICE9 CPluginShell::GetDevice()
{
	return NULL;
};
D3DCAPS9* CPluginShell::GetCaps()
{
	return NULL;
};
D3DFORMAT CPluginShell::GetBackBufFormat()
{
	return (D3DFORMAT)0;
};
D3DFORMAT CPluginShell::GetBackBufZFormat()
{
	return (D3DFORMAT)0;
};
LPD3DXFONT CPluginShell::GetFont(eFontIndex idx)
{
	return NULL;
};
char* CPluginShell::GetDriverFilename()
{
	if (m_lpDX) return m_lpDX->GetDriver(); else return NULL;
};
char* CPluginShell::GetDriverDescription()
{
	if (m_lpDX) return m_lpDX->GetDesc(); else return NULL;
};

int CPluginShell::InitNondx9Stuff()
{
	m_fftobj.Init(576, NUM_FREQUENCIES);
	if (!InitGDIStuff()) return false;
	return AllocateMyNonDx9Stuff();
}

void CPluginShell::CleanUpNondx9Stuff()
{
	CleanUpMyNonDx9Stuff();
	CleanUpGDIStuff();
	m_fftobj.CleanUp();
}

int CPluginShell::InitGDIStuff()
{
    // Stubbed out
	return true;
}

void CPluginShell::CleanUpGDIStuff()
{
    // Stubbed out
}

int CPluginShell::InitVJStuff(RECT* pClientRect)
{
    // VJ mode not supported in this port
	return true;
}

void CPluginShell::CleanUpVJStuff()
{
    // VJ mode not supported in this port
}

int CPluginShell::AllocateFonts(void* pDevice)
{
    // Stubbed out
	return true;
}

void CPluginShell::CleanUpFonts()
{
    // Stubbed out
}

void CPluginShell::AllocateTextSurface()
{
    // Stubbed out
}

int CPluginShell::AllocateDX9Stuff()
{
	int ret = AllocateMyDX9Stuff();
	m_playlist_top_idx = -1;
	m_text.Finish();
	m_text.Init(GetDevice(), (IDirect3DTexture9*)m_lpDDSText, 1);
	return ret;
}

void CPluginShell::CleanUpDX9Stuff(int final_cleanup)
{
	CleanUpMyDX9Stuff(final_cleanup);
}

void CPluginShell::OnUserResizeWindow()
{
    // Stubbed out
}

void CPluginShell::StuffParams(void* pParams)
{
    // Stubbed out
}

int CPluginShell::InitDirectX(LPDIRECT3DDEVICE9 device, void* d3dpp, HWND hwnd)
{
    m_lpDX = new GLContext((GLFWwindow*)hwnd, (wchar_t*)m_szConfigIniFile);
	if (!m_lpDX)
	{
		fprintf(stderr, "Unable to init GLContext\n");
		return false;
	}

	GLCONTEXT_PARAMS params = {0};
	if (!m_lpDX->StartOrRestartDevice(&params))
	{
		delete m_lpDX;
		m_lpDX = NULL;
		return false;
	}

	return true;
}

void CPluginShell::CleanUpDirectX()
{
	SafeDelete(m_lpDX);
}

int CPluginShell::PluginPreInitialize(HWND hWinampWnd, HINSTANCE hWinampInstance)
{
	m_start_fullscreen      = 0;
	m_start_desktop         = 0;
	m_fake_fullscreen_mode  = 0;
	m_max_fps_fs            = 60;
	m_max_fps_dm            = 60;
	m_max_fps_w             = 60;
	m_show_press_f1_msg     = 1;
	m_allow_page_tearing_w  = 1;
	m_allow_page_tearing_fs = 0;
	m_allow_page_tearing_dm = 0;
	m_minimize_winamp       = 1;
	m_desktop_show_icons    = 1;
	m_desktop_textlabel_boxes = 1;
	m_desktop_manual_icon_scoot = 0;
	m_desktop_555_fix       = 2;
	m_dualhead_horz         = 2;
	m_dualhead_vert         = 1;
	m_save_cpu              = 1;
	m_skin                  = 1;
	m_fix_slow_text         = 0;
	wcscpy(m_fontinfo[SIMPLE_FONT    ].szFace,        SIMPLE_FONT_DEFAULT_FACE);
	m_fontinfo[SIMPLE_FONT    ].nSize        = SIMPLE_FONT_DEFAULT_SIZE ;
	m_fontinfo[SIMPLE_FONT    ].bBold        = SIMPLE_FONT_DEFAULT_BOLD ;
	m_fontinfo[SIMPLE_FONT    ].bItalic      = SIMPLE_FONT_DEFAULT_ITAL ;
	m_fontinfo[SIMPLE_FONT    ].bAntiAliased = SIMPLE_FONT_DEFAULT_AA   ;
	wcscpy(m_fontinfo[DECORATIVE_FONT].szFace,        DECORATIVE_FONT_DEFAULT_FACE);
	m_fontinfo[DECORATIVE_FONT].nSize        = DECORATIVE_FONT_DEFAULT_SIZE;
	m_fontinfo[DECORATIVE_FONT].bBold        = DECORATIVE_FONT_DEFAULT_BOLD;
	m_fontinfo[DECORATIVE_FONT].bItalic      = DECORATIVE_FONT_DEFAULT_ITAL;
	m_fontinfo[DECORATIVE_FONT].bAntiAliased = DECORATIVE_FONT_DEFAULT_AA  ;
	wcscpy(m_fontinfo[HELPSCREEN_FONT].szFace,        HELPSCREEN_FONT_DEFAULT_FACE);
	m_fontinfo[HELPSCREEN_FONT].nSize        = HELPSCREEN_FONT_DEFAULT_SIZE;
	m_fontinfo[HELPSCREEN_FONT].bBold        = HELPSCREEN_FONT_DEFAULT_BOLD;
	m_fontinfo[HELPSCREEN_FONT].bItalic      = HELPSCREEN_FONT_DEFAULT_ITAL;
	m_fontinfo[HELPSCREEN_FONT].bAntiAliased = HELPSCREEN_FONT_DEFAULT_AA  ;
	wcscpy(m_fontinfo[PLAYLIST_FONT  ].szFace,        PLAYLIST_FONT_DEFAULT_FACE);
	m_fontinfo[PLAYLIST_FONT  ].nSize        = PLAYLIST_FONT_DEFAULT_SIZE;
	m_fontinfo[PLAYLIST_FONT  ].bBold        = PLAYLIST_FONT_DEFAULT_BOLD;
	m_fontinfo[PLAYLIST_FONT  ].bItalic      = PLAYLIST_FONT_DEFAULT_ITAL;
	m_fontinfo[PLAYLIST_FONT  ].bAntiAliased = PLAYLIST_FONT_DEFAULT_AA  ;

	m_disp_mode_fs.Width = DEFAULT_FULLSCREEN_WIDTH;
	m_disp_mode_fs.Height = DEFAULT_FULLSCREEN_HEIGHT;
	m_disp_mode_fs.Format = (D3DFORMAT)0; // D3DFMT_UNKNOWN
	m_disp_mode_fs.RefreshRate = 60;

	m_frame = 0;
	m_time = 0;
	m_fps = 60;
	m_hInstance = hWinampInstance;
	m_lpDX = NULL;

    strcpy(m_szPluginsDirPath, "./");
    sprintf(m_szConfigIniFile, "%smilkdrop.ini", m_szPluginsDirPath);
	strcpy(m_szConfigIniFileA, m_szConfigIniFile);

	m_lost_focus = 0;
	m_hidden     = 0;
	m_resizing   = 0;
	m_show_help  = 0;
	m_show_playlist = 0;
	m_playlist_pos = 0;
	m_playlist_pageups = 0;
	m_playlist_top_idx = -1;
	m_playlist_btm_idx = -1;
	m_exiting    = 0;
	m_upper_left_corner_y = 0;
	m_lower_left_corner_y = 0;
	m_upper_right_corner_y = 0;
	m_lower_right_corner_y = 0;
	m_left_edge = 0;
	m_right_edge = 0;
	m_force_accept_WM_WINDOWPOSCHANGING = 0;
	m_vj_mode       = 0;
	m_hidden_textwnd = 0;
	m_resizing_textwnd = 0;
	m_hTextWnd		= NULL;
	m_nTextWndWidth = 0;
	m_nTextWndHeight = 0;
	m_bTextWindowClassRegistered = false;
	m_vjd3d9        = NULL;
	m_vjd3d9_device = NULL;
	m_last_raw_time = 0;
	memset(m_time_hist, 0, sizeof(m_time_hist));
	m_time_hist_pos = 0;
    m_high_perf_timer_freq = 0;
	m_prev_end_of_frame = 0;
	memset(m_oldwave[0], 0, sizeof(float)*576);
	memset(m_oldwave[1], 0, sizeof(float)*576);
	m_prev_align_offset[0] = 0;
	m_prev_align_offset[1] = 0;
	m_align_weights_ready = 0;
	OverrideDefaults();
	ReadConfig();
	MyPreInitialize();
	MyReadConfig();
	return TRUE;
}

int CPluginShell::PluginInitialize(LPDIRECT3DDEVICE9 device, void* d3dpp, HWND hwnd, int iWidth, int iHeight)
{
    if (!InitDirectX(device, d3dpp, hwnd)) return false;
    m_lpDX->m_client_width = iWidth;
    m_lpDX->m_client_height = iHeight;
    m_lpDX->m_REAL_client_height = iHeight;
    m_lpDX->m_REAL_client_width = iWidth;

    if (!InitNondx9Stuff()) return false;
    if (!AllocateDX9Stuff()) return false;
	if (!InitVJStuff()) return false;

    return true;
}

void CPluginShell::PluginQuit()
{
	CleanUpVJStuff();
	CleanUpDX9Stuff(1);
	CleanUpNondx9Stuff();
	CleanUpDirectX();
}

void CPluginShell::ReadConfig()
{
    // Stubbed out
}

void CPluginShell::WriteConfig()
{
    // Stubbed out
}

int CPluginShell::PluginRender(unsigned char *pWaveL, unsigned char *pWaveR)
{
	if (!m_lpDX || !m_lpDX->m_ready)
	{
		m_exiting = 1;
		return false;
	}

	DoTime();
	AnalyzeNewSound(pWaveL, pWaveR);
	AlignWaves();
	DrawAndDisplay(0);
	EnforceMaxFPS();
	m_frame++;
	return true;
}

void CPluginShell::DrawAndDisplay(int redraw)
{
	MyRenderFn(redraw);
}

void CPluginShell::EnforceMaxFPS()
{
    // Stubbed out
}

void CPluginShell::DoTime()
{
	if (m_frame==0)
	{
		m_fps = 60;
		m_time = 0;
		m_time_hist_pos = 0;
	}

    double new_raw_time = glfwGetTime();
	float elapsed = (float)(new_raw_time - m_last_raw_time);
	m_last_raw_time = new_raw_time;

	if (m_frame > 0)
    {
        if (elapsed > 0)
        {
            m_fps = 0.9f * m_fps + 0.1f * (1.0f / elapsed);
        }
    }
    m_time += 1.0f / m_fps;
}

void CPluginShell::AnalyzeNewSound(unsigned char *pWaveL, unsigned char *pWaveR)
{
	int i;
	float temp_wave[2][576];
	int old_i = 0;
	for (i=0; i<576; i++)
	{
		m_sound.fWaveform[0][i] = (float)((pWaveL[i] ^ 128) - 128);
		m_sound.fWaveform[1][i] = (float)((pWaveR[i] ^ 128) - 128);
		temp_wave[0][i] = 0.5f*(m_sound.fWaveform[0][i] + m_sound.fWaveform[0][old_i]);
		temp_wave[1][i] = 0.5f*(m_sound.fWaveform[1][i] + m_sound.fWaveform[1][old_i]);
		old_i = i;
	}
	m_fftobj.time_to_frequency_domain(temp_wave[0], m_sound.fSpectrum[0]);
	m_fftobj.time_to_frequency_domain(temp_wave[1], m_sound.fSpectrum[1]);
	for (int ch=0; ch<2; ch++)
	{
		for (i=0; i<3; i++)
		{
			int start = (int)(NUM_FREQUENCIES * 200.0f*powf(3.8058f, (float)i)/11025.0f);
			int end   = (int)(NUM_FREQUENCIES * 200.0f*powf(3.8058f, (float)(i+1))/11025.0f);
			if (start < 0) start = 0;
			if (end > NUM_FREQUENCIES) end = NUM_FREQUENCIES;
			m_sound.imm[ch][i] = 0;
			for (int j=start; j<end; j++)
				m_sound.imm[ch][i] += m_sound.fSpectrum[ch][j];
			m_sound.imm[ch][i] /= (float)(end-start);
		}
	}
	for (int ch=0; ch<2; ch++)
	{
		m_sound.imm[ch][0] /= 0.326781557f;
		m_sound.imm[ch][1] /= 0.380873770f;
		m_sound.imm[ch][2] /= 0.199888934f;
	}
	for (int ch=0; ch<2; ch++)
	{
		for (i=0; i<3; i++)
		{
			float avg_mix = (m_sound.imm[ch][i] > m_sound.avg[ch][i]) ? AdjustRateToFPS(0.2f, 14.0f, m_fps) : AdjustRateToFPS(0.5f, 14.0f, m_fps);
			m_sound.avg[ch][i] = m_sound.avg[ch][i]*avg_mix + m_sound.imm[ch][i]*(1-avg_mix);
			float med_mix  = AdjustRateToFPS(0.91f, 14.0f, m_fps);
			float long_mix = AdjustRateToFPS(0.96f, 14.0f, m_fps);
			m_sound.med_avg[ch][i]  =  m_sound.med_avg[ch][i]*(med_mix) + m_sound.imm[ch][i]*(1-med_mix);
			m_sound.long_avg[ch][i] = m_sound.long_avg[ch][i]*(long_mix) + m_sound.imm[ch][i]*(1-long_mix);
		}
	}
}

void CPluginShell::AlignWaves()
{
    // Simplified for now
	memcpy(m_oldwave[0], m_sound.fWaveform[0], sizeof(float)*576);
	memcpy(m_oldwave[1], m_sound.fWaveform[1], sizeof(float)*576);
}

LRESULT CPluginShell::PluginShellWindowProc(HWND hWnd, unsigned uMsg, WPARAM wParam, LPARAM lParam)
{
	return MyWindowProc(hWnd, uMsg, wParam, lParam);
}

void CPluginShell::ToggleHelp()
{
    m_show_help = 1 - m_show_help;
}

void CPluginShell::SuggestHowToFreeSomeMem()
{
    printf("To free up video memory, try...\n");
    printf(" - restarting the application\n");
    printf(" - closing other applications\n");
}

void CPluginShell::RenderBuiltInTextMsgs()
{
    // Stubbed out
}

void CPluginShell::RenderPlaylist()
{
    // Stubbed out
}