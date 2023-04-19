// --------------------------------------------------------------------------------------------
// Filename: 8SystemFeatures.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234567831;

#include "WinSystemClass.h"
#include "SystemFeatures.h"
#include "ProcessorInfo.h"

SystemFeatures::SystemFeatures()
{
	CLASSLOADER();
    WomaIntegrityCheck = 1234567831;

    displayAllCpuFeactures = TEXT("");
}

SystemFeatures::~SystemFeatures() {CLASSDELETE();}

#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>
class InstructionSet
{
    // forward declarations
    class InstructionSet_Internal;

public:
    // getters
    static std::string Vendor(void) { return CPU_Rep.vendor_; }
    static std::string Brand(void) { return CPU_Rep.brand_; }

    static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
    static bool PCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }
    static bool MONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }
    static bool VMX(void) { return CPU_Rep.f_1_ECX_[5]; }   //ADDED BY PB
    static bool SSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }
    static bool FMA(void) { return CPU_Rep.f_1_ECX_[12]; }
    static bool CMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }
    static bool SSE41(void) { return CPU_Rep.f_1_ECX_[19]; }
    static bool SSE42(void) { return CPU_Rep.f_1_ECX_[20]; }
    static bool MOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }
    static bool POPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }
    static bool AES(void) { return CPU_Rep.f_1_ECX_[25]; }
    static bool XSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }
    static bool OSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }
    static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }
    static bool F16C(void) { return CPU_Rep.f_1_ECX_[29]; }
    static bool RDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }
    static bool hypervisor(void) { return CPU_Rep.f_1_ECX_[31]; }   //ADDED BY PB

    static bool MSR(void) { return CPU_Rep.f_1_EDX_[5]; }
    static bool CX8(void) { return CPU_Rep.f_1_EDX_[8]; }
    static bool SEP(void) { return CPU_Rep.f_1_EDX_[11]; }
    static bool CMOV(void) { return CPU_Rep.f_1_EDX_[15]; }
    static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }
    static bool MMX(void) { return CPU_Rep.f_1_EDX_[23]; }
    static bool FXSR(void) { return CPU_Rep.f_1_EDX_[24]; }
    static bool SSE(void) { return CPU_Rep.f_1_EDX_[25]; }
    static bool SSE2(void) { return CPU_Rep.f_1_EDX_[26]; }
    static bool HTT(void) { return CPU_Rep.f_1_EDX_[28]; }  //ADDED BY PB

    static bool FSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }
    static bool BMI1(void) { return CPU_Rep.f_7_EBX_[3]; }
    static bool HLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
    static bool AVX2(void) { return CPU_Rep.f_7_EBX_[5]; }
    static bool BMI2(void) { return CPU_Rep.f_7_EBX_[8]; }
    static bool ERMS(void) { return CPU_Rep.f_7_EBX_[9]; }
    static bool INVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }
    static bool RTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
    static bool AVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }
    static bool RDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }
    static bool ADX(void) { return CPU_Rep.f_7_EBX_[19]; }
    static bool AVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }
    static bool AVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }
    static bool AVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }
    static bool SHA(void) { return CPU_Rep.f_7_EBX_[29]; }

    static bool PREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }

    static bool LAHF(void) { return CPU_Rep.f_81_ECX_[0]; }
    static bool SVM(void) { return CPU_Rep.f_81_ECX_[2]; }  //ADDED BY PB
    static bool LZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
    static bool ABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
    static bool SSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
    static bool XOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
    static bool TBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

    static bool SYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
    static bool MMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
    static bool RDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
    static bool _3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
    static bool _3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

private:
    static const InstructionSet_Internal CPU_Rep;

    class InstructionSet_Internal
    {
    public:
        InstructionSet_Internal()
            : nIds_{ 0 },
            nExIds_{ 0 },
            isIntel_{ false },
            isAMD_{ false },
            f_1_ECX_{ 0 },
            f_1_EDX_{ 0 },
            f_7_EBX_{ 0 },
            f_7_ECX_{ 0 },
            f_81_ECX_{ 0 },
            f_81_EDX_{ 0 },
            data_{},
            extdata_{}
        {
            //int cpuInfo[4] = {-1};
            std::array<int, 4> cpui;

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
            __cpuid(cpui.data(), 0);
            nIds_ = cpui[0];

            for (int i = 0; i <= nIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                data_.push_back(cpui);
            }

            // Capture vendor string
            char vendor[0x20];
            memset(vendor, 0, sizeof(vendor));
            *reinterpret_cast<int*>(vendor) = data_[0][1];
            *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
            *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
            vendor_ = vendor;
            if (vendor_ == "GenuineIntel")
            {
                isIntel_ = true;
            }
            else if (vendor_ == "AuthenticAMD")
            {
                isAMD_ = true;
            }

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1)
            {
                f_1_ECX_ = data_[1][2];
                f_1_EDX_ = data_[1][3];
            }

            // load bitset with flags for function 0x00000007
            if (nIds_ >= 7)
            {
                f_7_EBX_ = data_[7][1];
                f_7_ECX_ = data_[7][2];
            }

            // Calling __cpuid with 0x80000000 as the function_id argument
            // gets the number of the highest valid extended ID.
            __cpuid(cpui.data(), 0x80000000);
            nExIds_ = cpui[0];

            char brand[0x40];
            memset(brand, 0, sizeof(brand));

            for (int i = 0x80000000; i <= nExIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                extdata_.push_back(cpui);
            }

            // load bitset with flags for function 0x80000001
            if (nExIds_ >= 0x80000001)
            {
                f_81_ECX_ = extdata_[1][2];
                f_81_EDX_ = extdata_[1][3];
            }

            // Interpret CPU brand string if reported
            if (nExIds_ >= 0x80000004)
            {
                memcpy(brand, extdata_[2].data(), sizeof(cpui));
                memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
                memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
                brand_ = brand;
            }
        };

        int nIds_;
        int nExIds_;
        std::string vendor_;
        std::string brand_;
        bool isIntel_;
        bool isAMD_;
        std::bitset<32> f_1_ECX_;
        std::bitset<32> f_1_EDX_;
        std::bitset<32> f_7_EBX_;
        std::bitset<32> f_7_ECX_;
        std::bitset<32> f_81_ECX_;
        std::bitset<32> f_81_EDX_;
        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;
    };
};

// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;


STRING SystemFeatures::Initialize()
{
    TCHAR cpuFeacture[256];

    // --------------------------------------------------------------------------------------------
    // Collect CPU feactures:
    // --------------------------------------------------------------------------------------------
    int CPUInfo[4] = { -1 };

	#define SHOW_AMD_ONLY

	// INFO: http://en.wikipedia.org/wiki/CPUID

	#define EBX CPUInfo[1]
	#define ECX CPUInfo[2]
	#define EDX CPUInfo[3]

    // Get processor Info and Features:
    cpuID((unsigned*)CPUInfo, 1);			// EAX=1

    htt     = InstructionSet::HTT();        //((EDX & 1<<28) || false);
    mmx     = InstructionSet::MMX();        //((EDX & 1 << 23) || false);
    mmxExt  = InstructionSet::MMXEXT();     //((EDX & 1 << 22) || false);
    sse     = InstructionSet::SSE();        //((EDX & 1<<25) || false);
    sse2    = InstructionSet::SSE2();       //((EDX & 1 << 26) || false);
	sse3    = InstructionSet::SSE3();       //((ECX & 0x1)   || false);
    VMX     = InstructionSet::VMX();        //((ECX & 0x5) || false);
    sse41   = InstructionSet::SSE41();      //((ECX & 1 << 19) || false);
	sse42   = InstructionSet::SSE42();      //((ECX & 1<<20) || false);

    ssse3   = InstructionSet::SSSE3();      //((ECX & 0x9) || false);
	avx     = InstructionSet::AVX();        //((ECX & 1<<28) || false);

    hypervisor = InstructionSet::hypervisor(); // ((ECX & 1 << 31) || false);

    // Get extra processor Info and Features:
//    cpuID((unsigned*) CPUInfo, 0x80000001);	// EAX=0x80000001

    amd_3DNow       = InstructionSet::_3DNOW();     // ((EDX & 1 << 31) || false);	// AMD only!
    amd_3DNowExt    = InstructionSet::_3DNOWEXT();  // ((EDX & 1 << 30) || false);	// AMD only!

    SVM             = InstructionSet::SVM();        //((ECX & 1 << 2) || false);	// AMD only! - AMD virtualization (AMD-V)
    sse4A           = InstructionSet::SSE4a();      //((ECX & 1 << 6) || false);	// AMD only!
	//sse5 =			((ECX & 1<<11) || false);	// AMD only!

    // Get processor Extended Features:
    //cpuID((unsigned*)CPUInfo, 7);			// EAX=7
    avx2 = InstructionSet::AVX2(); // ((EBX & 1 << 5) || false);

    bool virtualization = hypervisor || VMX || SVM;
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("virtualization: %s"), (virtualization) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("hypervisor: %s"), (hypervisor) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("VMX: %s"), (VMX) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SVM: %s"), (SVM) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("Hyper-threading: %s"), (htt) ? TEXT("true") : TEXT("false")); //bMultithreading 
	SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("MMX Instruction Set:  %s"), (mmx) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("MMX Ext. Instruction Set:  %s"), (mmxExt) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

#ifdef SHOW_AMD_ONLY
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("3D Now Instruction Set:  %s  (AMD only)"), (amd_3DNow) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("3D nowExt Instruction Set:  %s  (AMD only)"), (amd_3DNowExt) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
#endif

    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SSE Instruction Set:  %s"), (sse) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SSE2 Instruction Set:  %s"), (sse2) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SSE3 Instruction Set:  %s"), (sse3) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SSSE3 Instruction Set:  %s"), (ssse3) ? TEXT("true") : TEXT("false"));
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SSE41Extensions Set:  %s"), (sse41) ? TEXT("true") : TEXT("false")); //bSSE41
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SSE42Extensions Set:  %s"), (sse42) ? TEXT("true") : TEXT("false")); //bSSE42
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

#ifdef SHOW_AMD_ONLY
    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SSE4A Instruction Set:  %s  (AMD only)"), (sse4A) ? TEXT("true") : TEXT("false")); //bSSE4A
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("SSE5 Instruction Set:  %s  (AMD only)"), (sse5) ? TEXT("true") : TEXT("false")); //bSSE5
    SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);
#endif

    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("AVX Instruction Set: %s"), (avx) ? TEXT("true") : TEXT("false")); //bMultithreading 
	SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

    StringCchPrintf(cpuFeacture, MAX_STR_LEN, TEXT("AVX2 Instruction Set: %s"), (avx2) ? TEXT("true") : TEXT("false")); //bMultithreading 
	SystemHandle->systemDefinitions.cpuFeactures.push_back(cpuFeacture);

    for (UINT i = 0; i < SystemHandle->systemDefinitions.cpuFeactures.size(); i++) {
        displayAllCpuFeactures += SystemHandle->systemDefinitions.cpuFeactures[i];	// append all wstring(s) into "displayAllCpuFeactures"
		displayAllCpuFeactures += TEXT("\n");
	}

    WOMA_LOGManager_DebugMSGAUTO(TEXT("CUP FEATURES:\n%s"), displayAllCpuFeactures.c_str());

    return displayAllCpuFeactures;
}

