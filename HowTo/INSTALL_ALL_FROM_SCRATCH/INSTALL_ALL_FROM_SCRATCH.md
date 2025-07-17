# WoMA3Dengine - Clean Installation Log
> Fresh install of all components, validated step-by-step  
> ✅ Items completed | ☐ Items pending  

---

## 🖥️ Operating System

- [ ] **Windows 11 PRO 24H2**  
      Version: `26100.4652`

---

## 🔧 Motherboard Drivers — ASUS ROG Crosshair X670E Hero

Website: [ASUS Drivers Download](https://rog.asus.com/motherboards/rog-crosshair/rog-crosshair-x670e-hero-model/helpdesk_download/)

- [ ] `DRV_Audio_RTK_USB_CNT_DTSP_SP_W11_64_V6396002342_20230330B.zip`
- [ ] `DRV_Bluetooth_Intel_AX210_TP_W11_64_V238003_20241101R.zip`
- [ ] `DRV_Chipset_AMD_AM5_TP_W11_64_V70108129_20250113B.zip`
- [ ] `DRV_LAN_Intel_I226_UWD_TP_W10_64_V11443_20240710R.zip`
- [ ] `DRV_Thunderbolt_Intel_UWD_TSD_W11_64_V14114120_20250516R.zip`
- [ ] `DRV_VGA_AMD_AM5_TP_TSD_W11_64_V320130364_20250314B.zip`
- [ ] `DRV_WiFi_Intel_TP_W11_64_V238007_20241101R.zip`

---

## 🎮 GPU Drivers

- [ ] `546.65-notebook-win10-win11-64bit-international-dch-whql.exe`
- [ ] `GeForce_Experience_v3.27.0.120.exe`

---

## 🔧 Git & Tools

- [ ] `Git-2.23.0-64-bit.exe`
- [ ] `TortoiseGit-2.9.0.0-64bit.msi`
- [ ] `git-lfs-windows-v3.7.0.exe`

---

## ☕ Java JDK 8 Setup (Java 1.8 Is mandatory for Android Builds)

- [ ] Download and install `jdk-8u202-windows-x64.exe` from:  
  [Oracle JDK 8 Downloads](https://www.oracle.com/java/technologies/downloads/#license-lightbox)

### Environment Variables

- `JAVA_HOME = C:\Program Files\Java\jdk1.8.0_202`
- Add to `PATH`:  
  `C:\Program Files\Java\jdk1.8.0_202\bin`

### Validation

#```cmd
echo %JAVA_HOME%
java -version

---
	#6.1
	Follow:
	C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\6.1 ActivateHiperV.png
	
	#PRE-REQUIREMENT STEPS for VS2019 Android Env.:
	-----------------------------------------------------------------------------------------------
	#[1] Open Power Shell as Administrator: (%SystemRoot%\system32\WindowsPowerShell\v1.0\powershell.exe)
	#just to check current state, RUN:
	Get-WindowsOptionalFeature -FeatureName HypervisorPlatform -Online

	#OUTPUT FOR HYPER-V INSTALLED: (so go to STEP[2])
	FeatureName      : HypervisorPlatform
	DisplayName      : Windows Hypervisor Platform
	Description      : Enables virtualization software to run on the Windows hypervisor
	RestartRequired  : Possible
	State            : Enabled		<-------------- (we want Enabled!)
	CustomProperties :

	#OUTPUT FOR HYPER-V NOT INSTALLED: (so do the next 2 commands:)
	FeatureName      : HypervisorPlatform
	DisplayName      : Windows Hypervisor Platform
	Description      : Enables virtualization software to run on the Windows hypervisor
	RestartRequired  : Possible
	State            : Disabled		<-------------- (If is disabled run these 2 commands:)
	CustomProperties :

	#To enable the features needed, RUN these two commands:
	Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Hyper-V -All
		[Y] Yes  [N] No  [?] Help (default is "Y"): N  									(when asked say N)
		
	Enable-WindowsOptionalFeature -Online -FeatureName HypervisorPlatform -All
	[Y] Yes  [N] No  [?] Help (default is "Y"): Y 										(when asked say N)
	(rebooting now...)
---

[click on power button] to reboot, where you should see... (update and restart) option.

---
<!--
[ ] Install VS2017 Professional
-->



	
[ ] Installed VS2019 Community: (Used by Android Emulators)<br>
        <b>NOTE:</b> Need to be this version: 16.11.42 After this one Microsfot ruin everything, and dont do updated after having this one.<br>
	"Select options on Snapshot": <br>
	C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\Install_VS_2019.png<br>
	
---

	#[7] Start VS2019 for the first time:

	Click on: "continue without code"

	#7.0
	TOOLS / ANDROID / Android SDK Manager / Platforms
	SDK need repair: Repair... Accept: "Repair"
	"Accept"
	...wait for background install...
	
	#7.1
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\7.1.png
	need SDK...
		click here to install..
	...wait for background install...
		
	(open again)
	TOOLS / ANDROID / Android SDK Manager / Tools

	#7.2 
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\7.2.png
	[X] Install Intel x86 Emulator

	#7.3 Install ANDROID Sdk-25:
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\7.3.png
	
	#7.4
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\7.4.png
	<b>IMPORTANT Note!</b>
	The default windows location to install AndroidSDK have spaces and that cause bugs!
	Copy all directory from: 
		C:\Program Files (x86)\Android\AndroidSDK\25
	TO:
		C:\Microsoft\AndroidSDK\25
---
	#[8.1] Create a Android Emulator
	TOOLS / ANDROID / Android Device Manager...
	Create as Snapshot image 8.1:
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\8.1.png
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\8.2.png
	(Dont Run it yet!)
	
---
	Before install "Android Studio" (Dependency for MS ANDROID emulators.)
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\9.1.png
	#[9.1] Setup Environment:
	SET PATH=
	C:\Program Files\Java\jdk1.8.0_25\bin
	C:\Program Files (x86)\Android\android-sdk\platform-tools\
	C:\Microsoft\AndroidSDK\25\tools
	C:\Microsoft\AndroidSDK\25\tools\bin
	C:\Microsoft\AndroidSDK\25\platform-tools
	%USERPROFILE%\AppData\Local\Microsoft\WindowsApps
	...	

	#[9.2]
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\9.2.png
	ANDROID_HOME=C:\Microsoft\AndroidSDK\25
	
---
	#[9.3]
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019
		9.3.1.png
		9.3.2.png
		9.3.3.png
		9.3.4.png
		9.3.5.png
	
	#Install Android Studio (android-studio-2022.2.1.18-windows.exe):
	or 				latest:	(android-studio-2025.1.1.14-windows.exe)
	https://developer.android.com/studio
	Install and Run for the first time "Android Studio"
	(CLOSE it!)
	
	#[9.4] Define Android SDK and JDK path.
	Visual Studio 2019 / Tools / Options
	Cross Platform
		C++
			Android	
				C:\Microsoft\AndroidSDK\25
				C:\Program Files (x86)\Android\AndroidNDK64\android-ndk-r16b
				C:\Program Files\Eclipse Foundation\jdk-8.0.302.8-hotspot
				c:\program files (x86)\microsoft visual studio\2019\community\Apps\apache-ant-1.9.3
				
	Follow: C:\WoMA3Dengine\HowTo\INSTALL_ALL_FROM_SCRATCH\Install_VS_COMINUTY_2019\9.4.png
	
---
	#For Troubleshooting:
	https://learn.microsoft.com/en-us/previous-versions/xamarin/android/get-started/installation/android-emulator/hardware-acceleration?pivots=windows

---
	Start emulator like Snapshot image 8.2:
	
	CLOSE
	
---
[ ] Installed VS2022 Community: (Used by Windows, Linux and Android builds)	
	//HOW TO DOWNLOAD AND INSTALL VISUAL STUDIO SETUP:

	//Step-1: DOWNLOAD Visual Studio 2022 Community Version:
	https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&passive=false&cid=2030

	//And copy VisualStudioSetup.exe file to this directory: C:\WoMA3Dengine\HowTo\HowToInstallVisualStudio2022Community

	//Step-2:  and then Run (at directory: C:\WoMA3Dengine\HowTo\HowToInstallVisualStudio2022Community)
	InstallVisualStudioForDirectX12.bat
	
	COMPILE WINDOWS  WITH SUCCESS!
	
--- 
	#Install android-ndk-r15c
	Download: https://dl.google.com/android/repository/android-ndk-r15c-windows-x86_64.zip
	Copy zip content to: "C:\Program Files (x86)\Android\AndroidNDK"
	
	Final location: C:\Program Files (x86)\Android\AndroidNDK\android-ndk-r15c-windows-x86_64
	
	
	TOOLS / OPTIONS / CROSS Platform / C++
	
	C:\ProgramData\Microsoft\AndroidSDK\25
	C:\Program Files (x86)\Android\AndroidNDK\android-ndk-r15c-windows-x86_64\android-ndk-r15c
	C:\Program Files\Android\jdk\jdk-8.0.302.8-hotspot\jdk8u302-b08
	C:\program files (x86)\microsoft visual studio\2019\community\Apps\apache-ant-1.9.3
	
	COMPILE ANDROID WITH SUCCESS!
---
	#For WSL need to COmpile Linux:
		Follow: "C:\WoMA3Dengine\HowTo\HowToWSL2\What to install on WSL LINUX.txt"

	#Move WSL to default path:
		Export the existing distro to a .tar file:
		PS C:\WINDOWS\system32> <b>wsl --export Ubuntu-22.04 Ubuntu.tar</b>
		Export in progress, this may take a few minutes. (9085 MB)

		Unregister the current distro (this deletes it, so be sure your export was successful):
		wsl --unregister Ubuntu-22.04

		Import the distro back under the name Ubuntu:
		mkdir C:\WSL\Ubuntu
		wsl --import Ubuntu C:\WSL\Ubuntu Ubuntu.tar

		Now you will see:
		\\wsl.localhost\Ubuntu

	# for Linux Audio:
		sudo apt update
		sudo apt install -y \
		pulseaudio \
		libsdl2-dev \
		alsa-utils \
		sox \
		libsox-fmt-all \
		vlc-bin \
		ffmpeg \
		libopenal-dev \
		libasound2-dev \
		libasound2-plugins
			
		vi /etc/environment
		...
		set PULSE_SERVER=localhost

		vi ~/.asoundrc
		pcm.default pulse
		ctl.default pulse

		vi ~/.bashrc
		...
		export HOST_IP="$(ip route |awk '/^default/{print $3}')"
		export PULSE_SERVER="tcp:$HOST_IP"

		#START-SOUND-WSL2toWINDOWS:
		C:\WoMA3Dengine\ThirdParty\pulse\bin\pulseaudio.exe

		#START-WSL:
			Windows Subsystem for Linux
			
	COMPILE ANDROID WITH SUCCESS!

---
	#Also needed to compile assimp lib: (need to be this version!)
	Install cmake for windows: cmake-3.31.4-windows-x86_64.msi
	https://cmake.org/cmake/help/latest/release/3.31.html
	
	
---

## 🧰 Blender

- [ ] Install Blender 4.5 (or newer):  
	[Download](https://www.blender.org/download/)  
	File: `blender-4.5.0-windows-x64.msi`

## Python Setup (For Asset Tools and Automation)
- [ ] Install Python from Microsoft Store.
	OPEN CMD:
	Just type: python
	close (shell)

- [ ] Install Script dependencies:	
	OPEN CMD:
	pip install numpy
	pip install imageio

## Video player to read mp4:
- [ ] Install Video MP4 deps. for player:
	https://files2.codecguide.com/K-Lite_Codec_Pack_1905_Standard.exe
	

### Validate:
	CMD
	python --version
	pip --version
