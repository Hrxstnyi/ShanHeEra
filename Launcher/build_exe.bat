@echo off
chcp 65001 >nul
echo ========================================
echo   山河纪元启动器 - 打包为EXE
echo ========================================
echo.
echo 正在检查 PyInstaller...
pip install pyinstaller --quiet
echo.
echo 正在打包...
pyinstaller --onefile --windowed --name "ShanHeEra_Launcher" --icon=NONE launcher.py
echo.
echo 打包完成！可执行文件在 dist 目录下。
pause
