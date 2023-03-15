// --------------------------------------------------------------------------------------------
// Filename: SystemFeatures.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// ********************************************************************************************
#pragma once

// --------------------------------------------------------------------------------------------
// Includes:
// --------------------------------------------------------------------------------------------
#include "main.h"
#include <vector>

class SystemFeatures
{
public:
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
	//bool sse5;

    // Get processor info:
	bool avx2;


private:

};

