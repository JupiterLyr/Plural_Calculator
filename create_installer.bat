@echo off
REM 设置为GBK/GB2312代码页
chcp 936 >nul
setlocal EnableDelayedExpansion

REM 配置设置
set "APP_NAME=PluralCalculator"
set "PACKAGE_NAME=com.jupiterlyr.pluralCalculator"
set "BUILD_DIR=build\Release"
set "INSTALLER_DIR=installer"
set "OUTPUT_NAME=%INSTALLER_DIR%\%APP_NAME%_installer.exe"

REM 检查binarycreator
where binarycreator >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    REM 如果不在PATH中，手动指定路径
    REM set "BINARYCREATOR=C:\Qt\Tools\QtInstallerFramework\4.10\bin\binarycreator.exe"
    echo 错误：找不到binarycreator.exe，请将其添加到PATH或修改脚本指定完整路径
    pause
    exit /b 1
)

REM 1. 清空data目录
echo 正在清理data目录...
set "DATA_DIR=%INSTALLER_DIR%\packages\%PACKAGE_NAME%\data"
if exist "%DATA_DIR%" (
    rmdir /s /q "%DATA_DIR%"
    if !ERRORLEVEL! NEQ 0 (
        echo 错误：无法删除 %DATA_DIR%
        pause
        exit /b 1
    )
)
mkdir "%DATA_DIR%"

REM 2. 检查Release目录
if not exist "%BUILD_DIR%" (
    echo 错误：找不到Release目录 %BUILD_DIR%
    pause
    exit /b 1
)

REM 3. 复制Release文件
echo 正在复制Release文件...
xcopy /E /H /I /Y "%BUILD_DIR%\*" "%DATA_DIR%\"
if !ERRORLEVEL! NEQ 0 (
    echo 复制文件时出错
    pause
    exit /b 1
)

REM 4. 生成安装程序
echo 正在创建安装程序...
binarycreator.exe -c "%INSTALLER_DIR%\config\config.xml" -p "%INSTALLER_DIR%\packages" "%OUTPUT_NAME%"
if !ERRORLEVEL! NEQ 0 (
    echo 生成安装程序失败
    pause
    exit /b 1
)

echo.
echo 成功！安装程序已生成: %OUTPUT_NAME%
pause