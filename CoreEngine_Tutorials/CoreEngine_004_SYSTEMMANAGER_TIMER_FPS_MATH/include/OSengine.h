// --------------------------------------------------------------------------------------------
// Filename: OSengine.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Add OS engine interfaces
// 
// This file shows the full file version for:
//   WINDOWS_PLATFORM
//   LINUX_PLATFORM
//   ANDROID_PLATFORM
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#if NOTES
                  ----------------  ---------                       ----------------
                  |              |  |       |                       |              |
                  |  SYSTEM MEM  |  | AUDIO |                       |  VIDEO MEM   |
                  |              |  |       |                       |              |
                  ----------------  ---------                       ----------------
                                 |  |                                      |
 ----------                 -------------  DX/DXGI                    -------------
 |HD-Drive| --------------> |           |  ------------------------ > |           |
 ----------                 |    CPU    |  dshow(VIDEO)               |    GPU    |
 ----------                 |           |  ------------------------ > |           |
 |NETWORK | --------------> -------------                             -------------
 ----------                 |    |      |                                   |
                  ----------  -------   ----------                    -------------
                  |Keyboard|  |Mouse|   |JoyStick|                    |           |
                  ----------  -------   ----------                    |  MONITOR  |
                                                                      |           |
                                                                      -------------
#endif

#if NOTES
#-------------------------------------------------------------------------
#THIRDPARTY:
#-------------------------------------------------------------------------
	AUDIO
		|ALUT_LIB|
		|OGG_LIB|
		|OpenAL32_LIB|
		|VORBISFILE_LIB|
		|VORBIS_LIB|

	IMAGES
		|JPG_LIB|
		|TIFF_LIB||tiff-3.8.2-src|
		|PNG_LIB|
		|ZLIB_LIB|

	OPENGL
		|freeglut|
		|GL3Plus|

	IP-GEO-LOCATION
		|GeoLite2PP_LIB|
		|maxminddb_LIB|

	COMUNICATION
		|ultimateTCP_IP_LIB|

	XML
		|TinyXML_LIB|

	(D3D11_SPEC_DATE_YEAR > 2009)
		|DirectXTex|(DX11)
			DX11LoadTexture.cpp
				// TGA:
				DirectX::ScratchImage
				DirectX::LoadFromTGAFile
				DirectX::CreateShaderResourceView

		|DirectXTK|(DX11)
			ALLOW_PRINT_SCREEN_SAVE_PNG
				SaveWICTextureToFile    
			DX11LoadTexture.cpp
				// DDS:
				DirectX::CreateDDSTextureFromFile
				// BMP, JPG, PNG, TIF -> Use: DirectXTK.lib
				DirectX::CreateWICTextureFromFile

	ZipUtils_LIB
#endif

#pragma once
#include "platform.h"
		
#if CORE_ENGINE_LEVEL < 2
#define ENGINE_RESTART 100
#else
#include "stateMachine.h"
#endif

#if !defined NewWomaEngine
#if defined WINDOWS_PLATFORM
	#if CORE_ENGINE_LEVEL < 10 || !defined DX_ENGINE
	#include "winsystemclass.h"
	#define SYSTEM WinSystemClass	        // Are we a Basic Windows Instance?
	#else

	#if defined DX_ENGINE
	#include "dxwinsystemclass.h"
	#define SYSTEM dxWinSystemClass         // Are we a DX Instance?
	#endif
	#endif
#endif

#ifdef LINUX_PLATFORM
	#include "linuxsystemclass.h"
	#define SYSTEM LinuxSystemClass         // Are we a Linux Instance?
	extern LinuxSystemClass* SystemHandle;
#endif

#ifdef ANDROID_PLATFORM
	#include "androidsystemclass.h"
	#define SYSTEM AndroidSystemClass       // Are we a Android Instance?
	extern AndroidSystemClass* SystemHandle;

	#include "AndroidEngine.h"
	void process_events(struct womaengine* engine, struct android_app* app);
#endif
#else
	#include "AndroidNewSystemClass.h"
	#define SYSTEM AndroidNewSystemClass       // Are we a Android Instance?
	extern AndroidNewSystemClass* SystemHandle;
#endif

#if defined WINDOWS_PLATFORM
	#define WOMAOS "WINDOWS"

	#pragma warning(push)
	#pragma warning(disable : 6387)
	#define COMMANDLINE_TO_ARGC_ARGV() {                \
		WOMA::Scmdline = lpCmdLine;                     \
		WOMA::Cmdshow = nShowCmd;                       \
		std::string segment;                            \
		std::stringstream test(lpCmdLine);              \
		for (int i = 0; i < MAX_PARAMS; i++)            \
			argv[i] = (char*)malloc(MAX_STR_LEN);       \
		GetModuleFileName(NULL, argv[argc++], MAX_PATH);\
		while (std::getline(test, segment, ' '))        \
		strcpy(argv[argc++], segment.c_str());          \
	}
	#pragma warning(pop) // _UCRT_DISABLED_WARNINGS
#endif

#if defined ANDROID_PLATFORM
	#define WOMAOS "ANDROID"

	#if defined NewWomaEngine
		#include "Android.h"
		#define WOMAOS "ANDROID"
	#endif
#endif

#if defined LINUX_PLATFORM
	#define WOMAOS "LINUX"
#endif

extern UINT RENDER_PAGE;

#if CORE_ENGINE_LEVEL >= 1
	#include "mem_leak.h"
#endif

extern int APPLICATION_MAIN(int argc, char* argv[]);
extern void APPLICATION_STOP();
extern void APPLICATION_STARTUP(int argc, char* argv[], int Command);

extern void ShowFPS();

#if defined NewWomaEngine
	extern struct engine engine;
#else
	extern struct womaengine engine;
#endif

#if !defined MB_OK
#define MB_OK 0
#endif

#if CORE_ENGINE_LEVEL >= 2
extern int WomaMessageBox(TCHAR* lpText);
extern int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption);
extern int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption, bool yesORno);
#else
extern int WomaMessageBox(TCHAR* lpText, TCHAR* lpCaption, bool yesORno);
#endif

namespace WOMA
{
	// State Vars:
	//---------------------------------------------------
	extern int		game_state;
	extern int		previous_game_state;

	extern Settings settings;				// Main Settings: settings.h
	extern UINT		ENGINE_LEVEL_USED;
	extern int		main_loop_state;

#if defined WINDOWS_PLATFORM // SUBSYSTEM:WINDOWS
	extern PSTR		Scmdline;
	extern int		Cmdshow;
#endif

	// SUBSYSTEM:CONSOLE
	extern BOOL		UseWarpDevice;
	extern int		ARGc;
	extern CHAR**	ARGv;

	extern TCHAR strConsoleTitle[MAX_STR_LEN];
#if defined USE_LOG_MANAGER //1
	extern bool dirExists(STRING& dirName_in);
	extern int getTaskBarHeight();
#endif

#if defined USE_LOADING_THREADS || defined USE_MAIN_THREAD //extern
	extern UINT		num_running_THREADS;
#endif
#if defined USE_LOADING_THREADS || DX_ENGINE_LEVEL >= 37
	extern UINT		num_loading_objects;
#endif

#if CORE_ENGINE_LEVEL >= 1
	extern TCHAR	APP_NAME[MAX_STR_LEN];	// "Aplication Name"
#endif

#if CORE_ENGINE_LEVEL >= 2
	extern TCHAR	APP_COMPANY_NAME[];	// "Company" Directory Name: 1st lvl
	extern TCHAR	APP_PROJECT_NAME[];	// "Project" Directory Name: 2nd lvl
	extern TCHAR	APP_FULLNAME[MAX_STR_LEN];	// "Aplication FullName"
	extern bool		fileExists(STRING Filename);

	#if defined WINDOWS_PLATFORM
	extern TCHAR	APP_ICO[];					// "Icon" for this aplication
	#endif
#endif

#if defined USE_MINIDUMPER	//3
	extern MiniDumper* miniDumper;
#endif

#if CORE_ENGINE_LEVEL >= 4
	// Defined at: main_settings.cpp
	extern STRING	filename;			// CMD line: filename
#endif

#if defined USE_TINYXML_LOADER //#if CORE_ENGINE_LEVEL >= 5
	extern TCHAR	APP_SETTINGS_FILE[];
#endif
}


#if CORE_ENGINE_LEVEL >= 1 && !defined NewWomaEngine
extern int Command;

extern TCHAR* DEMO_NAME[];

#if CORE_ENGINE_LEVEL >= 4
TCHAR* getComputerName();
TCHAR* getUserName();
#endif

#if CORE_ENGINE_LEVEL >= 10
#define m_Driver  driverList[SystemHandle->AppSettings->DRIVER]
extern std::vector<WomaDriverClass*> driverList;
extern WomaDriverClass* g_contextDriver;
#endif

namespace WOMA
{
	extern int endian();
}

#if defined WINDOWS_PLATFORM
#define gettid() 0
#endif

#if defined ANDROID_PLATFORM
#define FCLOSE(A) file.close()
#else
#define FCLOSE(A) fclose(A)
#endif

#if CORE_ENGINE_LEVEL >= 10 && CORE_ENGINE_LEVEL == 10	//DX9 DX11+DX10 DX12 OPENGL3
//extern int USE_THIS_GRAPHIC_CARD_ADAPTER;
#endif

#if defined ANDROID_PLATFORM
#if defined USE_ANDROID_SOUND
extern int m_main_music_id;
extern int playAudio(const char* message);
extern void stopAudio(const int audioFileIdx);
#endif

static int androidRead(void* cookie, char* buf, int size) {
	return AAsset_read((AAsset*)cookie, buf, size);
}

static int androidWrite(void* cookie, const char* buf, int size) {
	return -1;//can't provide write access to the apk
}

static fpos_t androidSeek(void* cookie, fpos_t offset, int whence) {
	return AAsset_seek((AAsset*)cookie, offset, whence);
}

static int androidClose(void* cookie) {
	AAsset_close((AAsset*)cookie);
	return 0;
}

//module file.h
struct File {
	FILE* _File;
	AAsset* _A;
	File()
		: _File(nullptr)
		, _A(nullptr)
	{}
	bool open(const char* path, const char* mode);
	void close() {
		fclose(_File);
	}
	size_t read(void* buf, size_t size) {
		size_t res = fread(buf, 1, size, _File);
		return res;
	}
	void seek(fpos_t offset) {
		androidSeek(_A, offset, SEEK_CUR);
	}
};

#endif

#if defined ANDROID_PLATFORM
extern STRING LOAD_ASSET_SAVE_TO_CACHE(TCHAR* XMLFILE);
#endif

#endif
