// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: language.cpp
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
// PURPOSE: Get User Language
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567311;

#include "OSengine.h"
#include "OSmain_dir.h"
#include "language.h"

#if defined ANDROID_PLATFORM
#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
MainActivity_getSystemLanguage(JNIEnv* env, jobject /* this */) 
{
	// Call the Java method using JNI
	jclass localeClass = env->FindClass("java/util/Locale");
	jmethodID getDefaultMethod = env->GetStaticMethodID(localeClass, "getDefault", "()Ljava/util/Locale;");
	jobject localeObject = env->CallStaticObjectMethod(localeClass, getDefaultMethod);

	// Get the 'getLanguage' method from Locale class
	jmethodID getLanguageMethod = env->GetMethodID(localeClass, "getLanguage", "()Ljava/lang/String;");
	jstring language = (jstring)env->CallObjectMethod(localeObject, getLanguageMethod);

	// Return the language
	return language;
}
#endif

namespace WOMA
{
	// Get language as string
	bool GetLangStringFromLangId(DWORD dwLangID_i)
	{
	#if defined WINDOWS_PLATFORM && !defined ANDROID_PLATFORM 
		const int MAX_LANG_LEN = 50;

		// Prepare LCID
		const LCID lcidLang = MAKELCID(dwLangID_i, SORT_DEFAULT);

		// Will hold language
		TCHAR szLangBuffer[MAX_LANG_LEN] = { 0 };

		// Get language
		DWORD dwCount = GetLocaleInfo(lcidLang, LOCALE_SENGLANGUAGE, szLangBuffer, MAX_LANG_LEN);
		if (!dwCount)
		{
			WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("Failed to get locale language information\n"));
			return false;
		}// End if

		// Will hold country
		TCHAR szCountryBuffer[MAX_LANG_LEN] = { 0 };

		// Get country
		dwCount = GetLocaleInfo(lcidLang, LOCALE_SENGCOUNTRY, szCountryBuffer, MAX_LANG_LEN);

		if (!dwCount)
		{
			WOMA_LOGManager_DebugMSGAUTO((TCHAR*)TEXT("Failed to get locale country information\n"));
			return false;
		}// End if

		 // Prepare language string
		TCHAR str_lang[MAX_STR_LEN]; // Will hold country
		StringCchPrintf(str_lang, sizeof(str_lang), TEXT("Language: %s, %s\n"), szLangBuffer, szCountryBuffer);
		WOMA_LOGManager_DebugMSGAUTO(str_lang);
	#endif

		// Return execution status
		return true;
	}// End GetLangStringFromLangId
}