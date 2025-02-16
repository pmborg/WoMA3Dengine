// --------------------------------------------------------------------------------------------
// Filename: SystemFeatures.h
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567142;
#pragma once

#include "main.h"
#include <vector>

class SystemFeatures
{
public:
	UINT WomaIntegrityCheck = 1234567142;
    SystemFeatures();
    ~SystemFeatures();

    STRING Initialize();
    STRING displayAllCpuFeactures;


	// Get processor info:
	bool htt;
	bool mmx;
	bool mmxExt;
	bool sse;
	bool sse2;

	bool sse3;
	bool VMX;		//if(!hypervisor) virtualization = "Intel virtualization (VT-x)"
	bool ssse3;
	bool sse41;
	bool sse42;
	bool avx;

	bool hypervisor; //if(hypervisor) virtualization = TRUE

    // Get extra processor info:
	bool amd_3DNow;
	bool amd_3DNowExt;
	bool SVM;		//if(!hypervisor) virtualization = "AMD virtualization (AMD-V)"
	bool sse4A;
	bool sse5;

    // Get processor info:
	bool avx2;
	bool avx512;


private:

};

