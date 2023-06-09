; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "WoMA3Dengine029"
#define MyAppVersion "1.0"
#define MyAppPublisher "Pedro Borges"
#define MyAppURL "https://github.com/pmborg/WoMA3Dengine"
#define MyAppExeName "CoreEngine_029.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{373AF537-B375-4B9C-B303-0E90496CF034}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputBaseFilename=WoMAsetup029
SetupIconFile=C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\WoMA.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Code]
procedure RunOtherInstaller;
var
  ResultCode: Integer;
begin
  if not Exec(ExpandConstant('{app}\VC_redist.x64.exe'), '', '', SW_SHOWNORMAL,
    ewWaitUntilTerminated, ResultCode)
  then
    MsgBox('VC_redist.x64.exe installer failed to run!' + #13#10 +
      SysErrorMessage(ResultCode), mbError, MB_OK);
end;

[Files]
Source: "C:\Windows\System32\ucrtbased.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64\ucrt\ucrtbased.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\x64\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\x64\Release\freeglut.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\settings.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\WoMA.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\hlsl\*"; DestDir: "{app}\hlsl"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\GLengine\*"; DestDir: "{app}\GLengine"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\woma_engine\woma.pck"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\windows_engine\windows.pck"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\WoMA3Dengine\CoreEngine_Tutorials\DXENGINE_029\GeoLite2-City.mmdb"; DestDir: "{app}"; Flags: ignoreversion
Source: "output\VC_redist.x64.exe"; DestDir: "{app}"; AfterInstall: RunOtherInstaller
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

