// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: SystemManager_1CheckSYS.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
//-------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// --------------------------------------------------------------------------------------------

#include "WinSystemClass.h"
//#include "SystemPlatform.h"			// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID

//------------------------------------------------------------------
// PRIVATE FUNCTIONS:
//------------------------------------------------------------------
#include <psapi.h>					// PPERFORMANCE_INFORMATION
float SystemManager::GetProcessorSpeed()
{
    LARGE_INTEGER qwWait, qwStart, qwCurrent;

	STRING PROC_FAMILY = processorInfo.processorId;
	std::size_t comma = PROC_FAMILY.find(TEXT(","));
	STRING FAMILY_NAME = PROC_FAMILY.substr(comma+1, PROC_FAMILY.length());

	float Intel = GetProcessorSpeed4Intel((TCHAR*)FAMILY_NAME.c_str());

	if (!Intel) {
		QueryPerformanceCounter(&qwStart);
		QueryPerformanceFrequency(&qwWait);
		qwWait.QuadPart >>= 5;
		unsigned __int64 Start = __rdtsc();

		do {
			QueryPerformanceCounter(&qwCurrent);
		} while(qwCurrent.QuadPart - qwStart.QuadPart < qwWait.QuadPart);

		float AMD = ((__rdtsc() - Start) << 5) / 1000000.0f; // CPUSpeedMHz
		WOMA_LOGManager_DebugMSGW(TEXT("--%s FAMILY --\n"), FAMILY_NAME.c_str());
		WOMA_LOGManager_DebugMSGAUTO("Processor Base Frequency:  %f.0 MHz\n", AMD);
		return AMD;
	} else
	return Intel;
}

#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>
#pragma warning(disable: 4244)
float SystemManager::GetProcessorSpeed4Intel(TCHAR* family_name) {
	std::array<int, 4> cpuInfo;
	__cpuid(cpuInfo.data(), 0);

	if (cpuInfo[0] >= 0x16) {
		__cpuid(cpuInfo.data(), 0x16);

		//Example 1
		//Intel Core i7-6700K Skylake-H/S Family 6 model 94 (506E3)
		//cpuInfo[0] = 0x00000FA0; //= 4000 MHz
		//cpuInfo[1] = 0x00001068; //= 4200 MHz
		//cpuInfo[2] = 0x00000064; //=  100 MHz

		//Example 2
		//Intel Core m3-6Y30 Skylake-U/Y Family 6 model 78 (406E3)
		//cpuInfo[0] = 0x000005DC; //= 1500 MHz
		//cpuInfo[1] = 0x00000898; //= 2200 MHz
		//cpuInfo[2] = 0x00000064; //=  100 MHz

		//Example 3
		//Intel Core i5-7200 Kabylake-U/Y Family 6 model 142 (806E9)
		//cpuInfo[0] = 0x00000A8C; //= 2700 MHz
		//cpuInfo[1] = 0x00000C1C; //= 3100 MHz
		//cpuInfo[2] = 0x00000064; //=  100 MHz

		//printf("EAX: 0x%08x EBX: 0x%08x ECX: %08x\r\n", cpuInfo[0], cpuInfo[1], cpuInfo[2]);
		WOMA_LOGManager_DebugMSGW(TEXT("--%s FAMILY --\n"), family_name);
		WOMA_LOGManager_DebugMSGAUTO("Processor Base Frequency:  %04d MHz\n", cpuInfo[0]);
		WOMA_LOGManager_DebugMSGAUTO("Maximum Frequency:         %04d MHz\n", cpuInfo[1]);
		WOMA_LOGManager_DebugMSGAUTO("Bus (Reference) Frequency: %04d MHz\n", cpuInfo[2]);
		return cpuInfo[0];
	}
	else {
		printf("INTEL CPUID level 16h unsupported\r\n");
		return 0;
	}
}



//------------------------------------------------------------------
// PUBLIC FUNCTIONS:
//------------------------------------------------------------------
bool SystemManager::checkCPU ()
{
	processorInfo.cpuCores.GetProcessorInformation();

    // Get CPU Speed:
    CHAR speed[MAX_STR_LEN] = { 0 };
    wtoa(speed, processorInfo.processorName, MAX_STR_LEN); // wchar ==> char
	
    StringCchPrintf(SystemHandle->systemDefinitions.processorName, MAX_STR_LEN, TEXT("Processor Name: %s"), processorInfo.processorName);
    StringCchPrintf(SystemHandle->systemDefinitions.processorId, MAX_STR_LEN, TEXT("Processor ID: %s"), processorInfo.processorId);
	WOMA_LOGManager_DebugMSGAUTO (TEXT("%s\n"), SystemHandle->systemDefinitions.processorName);
	WOMA_LOGManager_DebugMSGAUTO (TEXT("%s\n"), SystemHandle->systemDefinitions.processorId);

	ASSERT(processorInfo.cpuCores.processorCoreCount);
	ASSERT(processorInfo.cpuCores.logicalProcessorCount);

	StringCchPrintf(SystemHandle->systemDefinitions.processorPackageCount, MAX_STR_LEN, TEXT("Number Processor(s): %d"), processorInfo.cpuCores.processorPackageCount);		// Slot
	StringCchPrintf(SystemHandle->systemDefinitions.NumCoreProcessors, MAX_STR_LEN, TEXT("Number Core(s): %d"), processorInfo.cpuCores.processorCoreCount);					// Cores
	StringCchPrintf(SystemHandle->systemDefinitions.logicalProcessorCount, MAX_STR_LEN, TEXT("Logical Processor(s): %d"), processorInfo.cpuCores.logicalProcessorCount);	// Threads

	// Get CPU: Page Size
	SYSTEM_INFO SI;
	GetSystemInfo(&SI);
	WOMA_LOGManager_DebugMSGAUTO( TEXT("CPU Page Size: %i\n"), SI.dwPageSize);

	CPUSpeedMHz = GetProcessorSpeed();

	StringCchPrintf(SystemHandle->systemDefinitions.clockSpeed, MAX_STR_LEN, TEXT("CPU Base Clock Speed: %02.2f GHz"), (float) CPUSpeedMHz/1000);
	WOMA_LOGManager_DebugMSGAUTO (TEXT("%s\n"), SystemHandle->systemDefinitions.clockSpeed);

    return true;
}

#if CORE_ENGINE_LEVEL >= 4 defined WINDOWS_PLATFORM
DWORDLONG SystemManager::getAvailSystemMemory()
{
	//do {
		// GET ullAvailPhys:
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(status);
		GlobalMemoryStatusEx(&status);
		DWORDLONG PhysMemAvail = status.ullAvailPhys;
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.dwLength                = %10u\n",			status.dwLength);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.dwMemoryLoad            = %10u%%\n",		status.dwMemoryLoad);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullTotalPhys            = %10u MBytes\n",	status.ullTotalPhys / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullAvailPhys            = %10u MBytes\n",	status.ullAvailPhys / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullTotalPageFile        = %10u MBytes\n",	status.ullTotalPageFile / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullAvailPageFile        = %10u MBytes\n",	status.ullAvailPageFile / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullTotalVirtual         = %10u MBytes\n",	status.ullTotalVirtual / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullAvailVirtual         = %10u MBytes\n",	status.ullAvailVirtual / MBs);
		WOMA_LOGManager_DebugMSG("\n");

		PERFORMANCE_INFORMATION performanceInformation;
		BOOL res = GetPerformanceInfo(&performanceInformation, sizeof(performanceInformation));
		DWORDLONG CachedMem = performanceInformation.SystemCache;
		WOMA_LOGManager_DebugMSG("PerformanceInfo.CommitTotal         		= %10u MBytes\n",	performanceInformation.CommitTotal * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.CommitLimit         		= %10u MBytes\n",	performanceInformation.CommitLimit * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.CommitPeak          		= %10u MBytes\n",	performanceInformation.CommitPeak * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.PhysicalTotal       		= %10u MBytes\n",	performanceInformation.PhysicalTotal * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.PhysicalAvailable   		= %10u MBytes\n",	performanceInformation.PhysicalAvailable * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.SystemCache         		= %10u MBytes\n",	performanceInformation.SystemCache * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.KernelTotal         		= %10u MBytes\n",	performanceInformation.KernelTotal * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.KernelPaged         		= %10u MBytes\n",	performanceInformation.KernelPaged * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.KernelNonpaged      		= %10u MBytes\n",	performanceInformation.KernelNonpaged * performanceInformation.PageSize / MBs);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.PageSize            		= %10u MBytes\n",	performanceInformation.PageSize / 1024);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.HandleCount         		= %10u\n",			performanceInformation.HandleCount);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.ProcessCount        		= %10u\n",			performanceInformation.ProcessCount);
		WOMA_LOGManager_DebugMSG("PerformanceInfo.ThreadCount         		= %10u\n",			performanceInformation.ThreadCount);
		WOMA_LOGManager_DebugMSG("\n");

		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(memoryStatus);
		res = GlobalMemoryStatusEx(&memoryStatus);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.dwLength                = %10u\n",			memoryStatus.dwLength);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.dwMemoryLoad            = %10u%%\n",		memoryStatus.dwMemoryLoad);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullTotalPhys            = %10u MBytes\n",	memoryStatus.ullTotalPhys / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullAvailPhys            = %10u MBytes\n",	memoryStatus.ullAvailPhys / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullTotalPageFile        = %10u MBytes\n",	memoryStatus.ullTotalPageFile / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullAvailPageFile        = %10u MBytes\n",	memoryStatus.ullAvailPageFile / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullTotalVirtual         = %10u MBytes\n",	memoryStatus.ullTotalVirtual / MBs);
		WOMA_LOGManager_DebugMSG("GlobalMemoryStatusEx.ullAvailVirtual         = %10u MBytes\n",	memoryStatus.ullAvailVirtual / MBs);
		WOMA_LOGManager_DebugMSG("\n");

		//Sleep(5000); } while (true);

	// Real "Free Mem" = PhysMemAvail - Cached Memory
    return PhysMemAvail - CachedMem;
}
#endif

bool SystemManager::checkRAM ()
{
#ifdef X64
	#define MIN_RAM 4
#else
	#define MIN_RAM 2
#endif

	wmiUtil.GetTotalPhysicalMemory();
    if (wmiUtil.totalMemoryCapacity < MIN_RAM) 
	{
		TCHAR str[MAX_STR_LEN];
		StringCchPrintf(str, MAX_STR_LEN, TEXT("RAM WARNING: Your RAM memory is very low (< %dGB), this application might not run!\n"), MIN_RAM);
        WOMA::WomaMessageBox(str);
	}

	StringCchPrintf(SystemHandle->systemDefinitions.totalMemoryCapacity, MAX_STR_LEN, TEXT("Total Memory RAM: %d GB\n"), (UINT) wmiUtil.totalMemoryCapacity);

	WOMA_LOGManager_DebugMSGAUTO (TEXT("%s\n"), SystemHandle->systemDefinitions.totalMemoryCapacity);

	// Get Free Mem:
	DWORDLONG availSystemMemory = getAvailSystemMemory(); // in MBs
	StringCchPrintf(SystemHandle->systemDefinitions.freeMemory, MAX_STR_LEN, TEXT ("Avail System Memory: %d MBs free\n"), (UINT) ((float)availSystemMemory / (float)MBs) );
	WOMA_LOGManager_DebugMSGAUTO (TEXT ("%s"), SystemHandle->systemDefinitions.freeMemory); // Already include: \n

	return true;
}

bool SystemManager::checkDiskFreeSpace ()
{
	WOMA_LOGManager_DebugMSGAUTO (TEXT("Disk Free:\n") );

  // Get list of all Drives: [C:\ D:\ ...]
    DWORD mydrives = MAX_STR_LEN; // buffer length
    TCHAR lpBuffer[MAX_STR_LEN];  // buffer for drive string storage
    
    DWORD test = GetLogicalDriveStrings(mydrives, lpBuffer);
    if (test <= 0)
        { WomaFatalException("Getting Logical Drives!"); }

    // Get a list of all drives with free space: drivesList
    DriveList drive;
    BOOL success;
    TCHAR* cpBuffer = lpBuffer;
    __int64 lpFreeBytesAvailable, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes = 0;

    while (cpBuffer[0]) 
	{
        TCHAR* pszDrive = cpBuffer;

		UINT unittype = GetDriveType(pszDrive);
		switch (unittype)
		{
			case DRIVE_UNKNOWN:
				WOMA_LOGManager_DebugMSG("Drive %s of unknown type\n", pszDrive);
			break;
			case DRIVE_NO_ROOT_DIR:
				WOMA_LOGManager_DebugMSG("Drive %s is invalid\n", pszDrive);
			break;
			case DRIVE_REMOVABLE:
				WOMA_LOGManager_DebugMSG("Drive %s is a removable drive\n", pszDrive);
			break;
			case DRIVE_FIXED: //3
				WOMA_LOGManager_DebugMSG("Drive %s is a hard disk\n", pszDrive);
			break;
			case DRIVE_REMOTE:
				WOMA_LOGManager_DebugMSG("Drive %s is a network drive\n", pszDrive);
			break;
			case DRIVE_CDROM: //5
				WOMA_LOGManager_DebugMSG("Drive %s is a CD-ROM drive\n", pszDrive);
			break;
			case DRIVE_RAMDISK:
				WOMA_LOGManager_DebugMSG("Drive %s is a RAM disk\n", pszDrive);
			break;
			default:
				WOMA_LOGManager_DebugMSG("Drive %s has an unknown %u drive type\n", pszDrive, unittype);
		}

		if (unittype != DRIVE_CDROM)
		{
			success = GetDiskFreeSpaceEx( pszDrive,
										(PULARGE_INTEGER)&lpFreeBytesAvailable,
										(PULARGE_INTEGER)&lpTotalNumberOfBytes,
										(PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);

			drive.drive = cpBuffer[0];
			drive.freeBytesAvailable = (success) ? lpFreeBytesAvailable : 0; 

			if (drive.freeBytesAvailable > 0)   //0 Means: CD-W, DVD-W, BD-W, etc...
				drivesList.push_back(drive);
		}

        cpBuffer+=4;
    } 

    // Check a drive that have at least: 512 MB
    for (driveLetter = 0; driveLetter < drivesList.size(); driveLetter++) {
        if (drivesList[driveLetter].freeBytesAvailable >= 512 * MBs)
            break;
    }

    if (driveLetter == drivesList.size())
        WOMA::WomaMessageBox(TEXT("WARNING: You dont have 512MB of free disk space to Install Woma on all drives!")); //Change Drive...

	
	// SAVE SCAN RESULTS:
	TCHAR temp [MAX_STR_LEN];
    for (UINT driveLetter = 0; driveLetter < drivesList.size(); driveLetter++) 
	{
		StringCchPrintf(temp, MAX_STR_LEN, TEXT("%c: %02.1f GB"), drivesList[driveLetter].drive , (float) drivesList[driveLetter].freeBytesAvailable / ((float)GBs));
		SystemHandle->systemDefinitions.drives_List.push_back(temp);
		WOMA_LOGManager_DebugMSGAUTO (TEXT("%s\n"), temp);
    }
	
    return true;
}

bool SystemManager::checkCPUFeatures ()
{
    SystemHandle->systemDefinitions.systemFeatures = systemFeatures.Initialize();

	return true;
}

bool CheckDEVICEinfo() 
{
	// get mouse speed
	int mouseSpeed;
	mouseSpeed = 0;
	SystemParametersInfo(SPI_GETMOUSESPEED, 0, &mouseSpeed, 0); // 1 (slowest) and 20 (fastest). A value of 10 is the default.

	UINT nDevices = 0;
	PRAWINPUTDEVICELIST pRawInputDeviceList;
	WOMA_LOGManager_DebugMSG ("-------------------------------------------------------------------------------\n");

	if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
	{
		//"Errors...";
		return false;
	}

	if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)) == NULL)
	{
		//"Initialization failed...";
		return false;
	}

	int nNoOfDevices = 0;
	if ((nNoOfDevices = GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST))) == ((UINT) - 1))
	{
		// Error
		return false;
	}

	RID_DEVICE_INFO rdi;
	rdi.cbSize = sizeof(RID_DEVICE_INFO);

	for(int i = 0; i < nNoOfDevices; i++)
	{
		UINT size = 256;
		TCHAR tBuffer[256] = {0};
		tBuffer[0] = '\0';
		if(GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_DEVICENAME, tBuffer, &size) < 0)
		{
			// Error in reading device name
			//"error reading device name";
		}

		UINT cbSize = rdi.cbSize;
		if(GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_DEVICEINFO, &rdi, &cbSize) < 0)
		{
			// Error in reading information
		}

		if(rdi.dwType == RIM_TYPEMOUSE)
		{
			STRING buf = tBuffer;
			if ((int)buf.find(TEXT("#VID")) > 0)
			{
			//WOMA_LOGManager_DebugMSGAUTO (TEXT("Device Name: %s\n"), tBuffer);
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "ID for Mouse: %d\n"), rdi.mouse.dwId);
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "Number of Buttons: %d\n"), rdi.mouse.dwNumberOfButtons);

			if (rdi.mouse.dwSampleRate > 0)
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "Sample rate(Number of data points): %d\n"), rdi.mouse.dwSampleRate); //This value is not used for USB devices.
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "Mouse Speed (1 to 20): %d\n"), mouseSpeed);

			WOMA_LOGManager_DebugMSG ("-------------------------------------------------------------------------------\n");
			}
		}

		if(rdi.dwType == RIM_TYPEKEYBOARD)
		{
			STRING buf = tBuffer;
			if ((int)buf.find(TEXT("#VID")) > 0)
			{
				//WOMA_LOGManager_DebugMSGAUTO (TEXT("Device Name: %s\n"), tBuffer);
				WOMA_LOGManager_DebugMSGAUTO (TEXT( "Keyboard Mode: %d\n"), rdi.keyboard.dwKeyboardMode);
				WOMA_LOGManager_DebugMSGAUTO (TEXT( "Number of function keys: %d\n"), rdi.keyboard.dwNumberOfFunctionKeys);
				WOMA_LOGManager_DebugMSGAUTO (TEXT( "Number of indicators: %d\n"), rdi.keyboard.dwNumberOfIndicators);
				WOMA_LOGManager_DebugMSGAUTO (TEXT( "Number of keys total: %d\n"), rdi.keyboard.dwNumberOfKeysTotal);
				WOMA_LOGManager_DebugMSGAUTO (TEXT( "Type of the keyboard: %d\n"), rdi.keyboard.dwType);
				WOMA_LOGManager_DebugMSGAUTO (TEXT( "Subtype of the keyboard: %d\n"), rdi.keyboard.dwSubType);
				WOMA_LOGManager_DebugMSG ("-------------------------------------------------------------------------------\n");
			}
		}
		/*
		if(rdi.dwType == RIM_TYPEHID)
		{
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "Vendor Id: %x\n"), rdi.hid.dwVendorId);
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "Product Id: %x\n"), rdi.hid.dwProductId);
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "Version No: %d\n"), rdi.hid.dwVersionNumber);
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "Usage for the device: %d\n"), rdi.hid.usUsage);
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "Usage Page for the device: %d\n"), rdi.hid.usUsagePage);
			WOMA_LOGManager_DebugMSGAUTO (TEXT( "**************************\n"));
		}
		*/
	}

	free(pRawInputDeviceList);

	return true;
}


bool SystemManager::CheckIO ()
{
	CheckDEVICEinfo();
	STRING KeyboardStrDescription[] = { TEXT("none"),
		TEXT("IBM PC/XT or compatible (83-key) keyboard"),	//	1	
		TEXT("Olivetti \"ICO\" (102-key) keyboard"),			//	2	
		TEXT("IBM PC/AT (84-key) or similar keyboard"),		//	3	
		TEXT("IBM enhanced (101- or 102-key) keyboard"),		//	4	
		TEXT("Nokia 1050 and similar keyboards"),				//	5	
		TEXT("Nokia 9140 and similar keyboards"),				//	6	
		TEXT("Laptop Keyboard / Japanese Keyboard"),			//	7
	};


	int KeyboardType =  GetKeyboardType(0);
	WOMA_LOGManager_DebugMSG ("Keyboard - Type: %s\n", KeyboardStrDescription[KeyboardType].c_str());
	//----------------------------------------------------------------------------

	int numberoffunctionkeys =  GetKeyboardType(2);
	WOMA_LOGManager_DebugMSG ("Keyboard - Number of function keys: %d\n", numberoffunctionkeys);
	//----------------------------------------------------------------------------
	TCHAR pwszKLID[KL_NAMELENGTH];
	BOOL b = GetKeyboardLayoutName(pwszKLID);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("Keyboard - Layout: %s\n"), pwszKLID);
	//----------------------------------------------------------------------------

	HKL lpList[1];
	int r = GetKeyboardLayoutList(1, lpList);

	return true;
}

bool SystemManager::checkBenchMarkSpeed(TimerClass* m_Timer)
{
	SystemHandle->systemDefinitions.benchMarkMathSpeed = SystemHandle->m_math.testMathSpeed(m_Timer);
	WOMA_LOGManager_DebugMSGAUTO(TEXT("%s\n"), SystemHandle->systemDefinitions.benchMarkMathSpeed.c_str());

	return true;
}

