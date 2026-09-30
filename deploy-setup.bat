@echo off
setlocal
rem ============================================================
rem  HeaderConverter - Compilacion Release + preparacion de deploy
rem
rem  Uso: deploy.bat          Compila, actualiza el exe y los resources
rem        deploy.bat clean    Igual, pero vacia deploy\bin antes
rem ============================================================

rem ---- Configuracion (editar solo si cambian las rutas) ----
set "QT_DIR=C:\Qt\6.11.0\mingw_64"
set "BUILD_DIR=cmake-build-release"
set "EXE_NAME=HeaderConverter.exe"
set "CMAKE_EXE=cmake"
rem Si 'cmake' no esta en el PATH, apunta al que trae CLion. Ejemplo:
rem set "CMAKE_EXE=C:\Program Files\JetBrains\CLion 2025.2\bin\cmake\win\x64\bin\cmake.exe"

rem ---- Preparacion de entorno ----
cd /d "%~dp0"
if not exist "%QT_DIR%\bin\qtenv2.bat" (
    echo [ERROR] No se encontro Qt en "%QT_DIR%".
    goto :error
)
call "%QT_DIR%\bin\qtenv2.bat"
cd /d "%~dp0"

"%CMAKE_EXE%" --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] No se encontro cmake. Edita CMAKE_EXE al inicio de este script.
    goto :error
)
if not exist "%BUILD_DIR%\CMakeCache.txt" (
    echo [ERROR] No existe "%BUILD_DIR%". Crea el perfil Release en CLion y compilalo una vez.
    goto :error
)

rem ---- 1. Compilacion ----
echo.
echo [1/4] Compilando en Release...
"%CMAKE_EXE%" --build "%BUILD_DIR%"
if errorlevel 1 goto :error

rem ---- 2. Copia del ejecutable ----
echo.
echo [2/4] Copiando ejecutable a deploy\bin...
if /i "%~1"=="clean" if exist "deploy\bin" rmdir /s /q "deploy\bin"
if not exist "deploy\bin" mkdir "deploy\bin"
copy /y "%BUILD_DIR%\%EXE_NAME%" "deploy\bin\%EXE_NAME%" >nul
if errorlevel 1 goto :error

rem ---- 3. Dependencias de Qt ----
echo.
echo [3/4] Actualizando dependencias de Qt (windeployqt)...
windeployqt --release "deploy\bin\%EXE_NAME%"
if errorlevel 1 goto :error

rem ---- 4. Recursos por defecto ----
rem Se omiten Source.txt y Conversion.txt (archivos de prueba); la app los crea vacios.
echo.
echo [4/4] Sincronizando resources a deploy\resources...
robocopy "resources" "deploy\resources" /MIR /XF Source.txt Conversion.txt /NFL /NDL /NJH /NJS /NP
if errorlevel 8 goto :error

rem ---- Generar el instalador automaticamente ----
echo.
echo ============================================================
echo Todo lo anterior se completo correctamente.
echo El deploy esta listo.
echo.
echo A continuacion se intentara generar el instalador con
echo Inno Setup. Si Inno Setup esta instalado, el instalador
echo se generara automaticamente.
echo ============================================================
echo.

"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" installer.iss
if errorlevel 1 goto :inno_error

echo.
echo ============================================================
echo Instalador generado correctamente.
echo Todo el proceso finalizo correctamente.
echo ============================================================
echo.
pause
exit /b 0

:inno_error
echo.
echo ============================================================
echo El deploy se preparo correctamente, pero no se pudo
echo ejecutar installer.iss.
echo.
echo Esto normalmente significa que Inno Setup no esta
echo instalado en el equipo o no se encuentra en la ruta:
echo C:\Program Files (x86)\Inno Setup 6\ISCC.exe
echo.
echo Debes instalar Inno Setup para poder generar o actualizar
echo automaticamente el instalador.
echo ============================================================
echo.
pause
exit /b 0

:error
echo.
echo *** El proceso fallo. Revisa los mensajes de arriba. ***
pause
exit /b 1