@echo off
cd /d "%~dp0"
echo [1/2] 正在挂载 MSVC 环境...
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
if %errorlevel% neq 0 (
    echo [错误] vcvars64.bat 挂载失败，请检查路径是否正确。
    pause
    exit /b 1
)
echo [2/2] 启动编译...
python -m SCons platform=windows target=template_debug debug_symbols=yes -j 2
if %errorlevel% neq 0 (
    echo [错误] 编译失败，请查看上方报错信息。
    pause
    exit /b 1
)
echo ========================================
echo 编译成功！请切换到 Godot 并点击右上角"运行"按钮。
echo ========================================
pause