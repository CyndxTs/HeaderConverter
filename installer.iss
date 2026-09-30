[Setup]
AppName=HeaderConverter
AppVersion=1.0
AppPublisher=CEDEX
DefaultDirName={autopf}\HeaderConverter
DefaultGroupName=HeaderConverter
OutputBaseFilename=HeaderConverter-Setup
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64compatible
UninstallDisplayName=HeaderConverter
SetupIconFile=header_converter.ico
UninstallDisplayIcon={app}\bin\HeaderConverter.exe

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; Flags: unchecked

[Files]
Source: "deploy\bin\*"; DestDir: "{app}\bin"; Flags: recursesubdirs
Source: "deploy\resources\*"; DestDir: "{app}\resources"

[Icons]
Name: "{group}\HeaderConverter"; Filename: "{app}\bin\HeaderConverter.exe"
Name: "{autodesktop}\HeaderConverter"; Filename: "{app}\bin\HeaderConverter.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\bin\HeaderConverter.exe"; Description: "Launch HeaderConverter"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{userappdata}\CEDEX\HeaderConverter"