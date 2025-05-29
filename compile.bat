@echo off
echo 正在尝试编译停车场管理系统...

REM 尝试使用GCC编译器
gcc -Wall -o parking_system.exe main.c parking.c
if %ERRORLEVEL% EQU 0 (
    echo 编译成功！使用GCC编译器
    goto run
)

REM 尝试使用Visual Studio的CL编译器
cl /W4 /Fe:parking_system.exe main.c parking.c
if %ERRORLEVEL% EQU 0 (
    echo 编译成功！使用CL编译器
    goto run
)

echo 编译失败！请确保您已安装GCC或Visual Studio。
echo 您可以下载并安装MinGW-w64或TDM-GCC来获取GCC编译器。
echo 下载链接: https://www.mingw-w64.org/ 或 https://jmeubank.github.io/tdm-gcc/
goto end

:run
echo.
echo 正在运行停车场管理系统...
parking_system.exe

:end
pause
