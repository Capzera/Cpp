param(
    [ValidateSet("build", "test", "all", "clean")]
    [string]$Target = "all",
    [int]$Jobs = 4
)

$ErrorActionPreference = "Continue"
$Root = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $Root "build"
$TestDir = Join-Path $BuildDir "tests"
$env:PATH = "D:\QT\Tools\mingw1310_64\bin;D:\QT\6.11.1\mingw_64\bin;$env:PATH"
$Cmake = "D:\QT\Tools\CMake_64\bin\cmake.exe"
$Generator = "MinGW Makefiles"
$ToolchainArgs = @(
    "-G", $Generator,
    "-DCMAKE_CXX_COMPILER=D:/QT/Tools/mingw1310_64/bin/g++.exe",
    "-DCMAKE_MAKE_PROGRAM=D:/QT/Tools/mingw1310_64/bin/mingw32-make.exe",
    "-DCMAKE_PREFIX_PATH=D:/QT/6.11.1/mingw_64"
)

function Write-Step($msg) { Write-Host "=== $msg ===" }

function Invoke-Build {
    Write-Step "CONFIGURE"
    & $Cmake -S $Root -B $BuildDir @ToolchainArgs | Out-Null
    if ($LASTEXITCODE -ne 0) { Write-Host "CONFIGURE FAILED"; exit 1 }
    & $Cmake -S (Join-Path $Root "tests") -B $TestDir @ToolchainArgs | Out-Null
    if ($LASTEXITCODE -ne 0) { Write-Host "TESTS CONFIGURE FAILED"; exit 1 }
    Write-Step "BUILD"
    & $Cmake --build $BuildDir -j $Jobs 2>&1 | Select-String -Pattern "error|Error" -Context 2 | Select-Object -First 25 | Out-String | Write-Host
    if ($LASTEXITCODE -ne 0) { Write-Host "BUILD FAILED"; exit 1 }
    & $Cmake --build $TestDir -j $Jobs 2>&1 | Select-String -Pattern "error|Error" -Context 2 | Select-Object -First 25 | Out-String | Write-Host
    if ($LASTEXITCODE -ne 0) { Write-Host "TEST BUILD FAILED"; exit 1 }
    Write-Host "BUILD OK"
}

function Invoke-Tests {
    Write-Step "RUN ALL TESTS"
    $env:QT_QPA_PLATFORM = "offscreen"
    $fails = @()
    Push-Location $TestDir
    $exes = Get-ChildItem -Path $TestDir -Filter "*.exe" -ErrorAction SilentlyContinue
    foreach ($f in $exes) {
        $out = & $f.FullName 2>&1 | Out-String
        if ($LASTEXITCODE -ne 0) {
            $fails += $f.Name
            Write-Host "FAIL: $($f.Name)"
            Write-Host $out
        }
    }
    Pop-Location
    Write-Host "count=$($exes.Count)"
    if ($fails.Count -eq 0) {
        Write-Host "ALL SUITES PASS"
        exit 0
    }
    Write-Host "FAILED: $($fails -join ', ')"
    exit 1
}

switch ($Target) {
    "build" { Invoke-Build }
    "test" { Invoke-Tests }
    "clean" {
        Write-Step "CLEAN"
        Remove-Item -Recurse -Force (Join-Path $BuildDir "tests") -ErrorAction SilentlyContinue
        Get-ChildItem -Path $Root -Filter "*.png" -File -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
        Get-ChildItem -Path (Join-Path $BuildDir "tests") -Filter "*.png" -File -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
        Write-Host "CLEAN OK"
    }
    default {
        Invoke-Build
        Invoke-Tests
    }
}
