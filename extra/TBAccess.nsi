;NSIS Modern User Interface
;Welcome/Finish Page Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General
  !define MUI_ICON "Logo.ico"
  !define MUI_UNICON "trash-o.ico"
  !define MUI_PRODUCT "TBAccess"
  !define MUI_COMPANYNAME "BJ Devices"
  
  Icon "${MUI_ICON}"
  UninstallIcon "${MUI_UNICON}"

  ;Name and file
  Name "${MUI_PRODUCT} x64"
  OutFile "${MUI_PRODUCT}-x64.exe"
  Unicode True

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\${MUI_PRODUCT}"

  ;Get installation folder from registry if available
  ;InstallDirRegKey HKCU "Software\${MUI_PRODUCT}" ""

  ;Request application privileges for Windows Vista
  ;RequestExecutionLevel user
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
;  !insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
;  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES

Function finishpageaction
	CreateShortcut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_PRODUCT}.exe" ""
FunctionEnd
  !define MUI_FINISHPAGE_RUN
  !define MUI_FINISHPAGE_RUN_TEXT "Create desktop shortcut"
  !define MUI_FINISHPAGE_RUN_FUNCTION "finishpageaction"
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Install"

  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...
 
  File "${MUI_PRODUCT}.exe"
  File "qtlogging.ini"
  File "D3Dcompiler_47.dll"
  File "libEGL.dll"
  File "libgcc_s_seh-1.dll"
  File "libGLESV2.dll"
  File "libstdc++-6.dll"
  File "libwinpthread-1.dll"
  File "opengl32sw.dll"
  File "Qt5Core.dll"
  File "Qt5Gui.dll"
  File "Qt5Svg.dll"
  File "Qt5Widgets.dll"
  File "${MUI_ICON}"
  File "${MUI_UNICON}"
  
  SetOutPath "$INSTDIR\translations"
  File "translations\qt_ar.qm"
  File "translations\qt_bg.qm"
  File "translations\qt_ca.qm"
  File "translations\qt_cs.qm"
  File "translations\qt_da.qm"
  File "translations\qt_de.qm"
  File "translations\qt_en.qm"
  File "translations\qt_es.qm"
  File "translations\qt_fi.qm"
  File "translations\qt_fr.qm"
  File "translations\qt_gd.qm"
  File "translations\qt_he.qm"
  File "translations\qt_hu.qm"
  File "translations\qt_it.qm"
  File "translations\qt_ja.qm"
  File "translations\qt_ko.qm"
  File "translations\qt_lv.qm"
  File "translations\qt_pl.qm"
  File "translations\qt_ru.qm"
  File "translations\qt_sk.qm"
  File "translations\qt_uk.qm"
  File "translations\qt_zh_TW.qm"
  
  SetOutPath "$INSTDIR\styles"
  File "styles\qwindowsvistastyle.dll"
  
  SetOutPath "$INSTDIR\platforms"
  File "platforms\qwindows.dll"
  
  SetOutPath "$INSTDIR\imageformats"
  File "imageformats\qgif.dll"
  File "imageformats\qicns.dll"
  File "imageformats\qico.dll"
  File "imageformats\qjpeg.dll"
  File "imageformats\qsvg.dll"
  File "imageformats\qtga.dll"
  File "imageformats\qtiff.dll"
  File "imageformats\qwbmp.dll"
  File "imageformats\qwebp.dll"
  
  SetOutPath "$INSTDIR\iconengines"
  File "iconengines\qsvgicon.dll"
  
  ;create desktop shortcut
  ;CreateShortCut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_PRODUCT}.exe" ""
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  ;Start Menu
  CreateDirectory "$SMPROGRAMS\${MUI_COMPANYNAME}"
  CreateShortCut "$SMPROGRAMS\${MUI_COMPANYNAME}\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_PRODUCT}.exe" "" "$INSTDIR\${MUI_ICON}"
  CreateShortCut "$SMPROGRAMS\${MUI_COMPANYNAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\${MUI_UNICON}"
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BJDevices" "DisplayName" "TBAccess -- BJ Devices"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BJDevices" "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BJDevices" "QuietUninstallString" "$\"$INSTDIR\Uninstall.exe$\" /S"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BJDevices" "DisplayIcon" "$\"$INSTDIR\Logo.ico$\""

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  Delete "$DESKTOP\${MUI_PRODUCT}.lnk"
  RMDir /r "$SMPROGRAMS\${MUI_COMPANYNAME}"
  
  RMDir /r "$INSTDIR\translations"
  RMDir /r "$INSTDIR\styles"
  RMDir /r "$INSTDIR\platforms"
  RMDir /r "$INSTDIR\imageformats"
  RMDir /r "$INSTDIR\iconengines"

  Delete "$INSTDIR\${MUI_PRODUCT}.exe"
  Delete "$INSTDIR\qtlogging.ini"
  Delete "$INSTDIR\D3Dcompiler_47.dll"
  Delete "$INSTDIR\libEGL.dll"
  Delete "$INSTDIR\libgcc_s_seh-1.dll"
  Delete "$INSTDIR\libGLESV2.dll"
  Delete "$INSTDIR\libstdc++-6.dll"
  Delete "$INSTDIR\libwinpthread-1.dll"
  Delete "$INSTDIR\opengl32sw.dll"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Svg.dll"
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "${MUI_ICON}"
  Delete "${MUI_UNICON}"
  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r "$INSTDIR"
  
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BJDevices"
  
  Sleep 1000

  ;DeleteRegKey /ifempty HKCU "Software\Modern UI Test"

SectionEnd