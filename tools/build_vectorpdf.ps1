<#
.SYNOPSIS
    VectorPDF Windows 11 x64 Tek Tıkla Derleme ve Test Betiği
.DESCRIPTION
    Bu betik ortam değişkenlerini kontrol eder, CMake ile Release hedefini derler ve birim testleri çalıştırır.
.EXAMPLE
    .\tools\build_vectorpdf.ps1
#>

[CmdletBinding()]
param (
    [string]$BuildType = "Release",
    [int]$Jobs = 8,
    [switch]$SkipTests
)

$ErrorActionPreference = "Stop"

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "  VectorPDF Derleme ve Paketleme Hattı    " -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan

# 1. Ortam değişkeni kontrolleri
if (-not $env:VCPKG_ROOT) {
    Write-Warning "UYARI: VCPKG_ROOT ortam değişkeni tanımlanmamış. Varsayılan aranıyor..."
    if (Test-Path "C:\vcpkg") {
        $env:VCPKG_ROOT = "C:\vcpkg"
    } elseif (Test-Path "$env:USERPROFILE\vcpkg") {
        $env:VCPKG_ROOT = "$env:USERPROFILE\vcpkg"
    }
}

Write-Host "[1/4] CMake Yapılandırması başlatılıyor..." -ForegroundColor Green
$cmakeArgs = @(
    "-B", "build",
    "-S", ".",
    "-DCMAKE_BUILD_TYPE=$BuildType",
    "-DPDF4QT_BUILD_TESTS=ON"
)

if ($env:VCPKG_ROOT -and (Test-Path "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake")) {
    $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
}

if ($env:Qt6_DIR) {
    $cmakeArgs += "-DPDF4QT_QT_ROOT=$env:Qt6_DIR"
}

& cmake @cmakeArgs
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake yapılandırması başarısız oldu (Exit code: $LASTEXITCODE)."
}

Write-Host "[2/4] VectorPDF derleniyor (Config: $BuildType, İş Parçacığı: $Jobs)..." -ForegroundColor Green
& cmake --build build --config $BuildType -j $Jobs
if ($LASTEXITCODE -ne 0) {
    Write-Error "Derleme başarısız oldu (Exit code: $LASTEXITCODE)."
}

if (-not $SkipTests) {
    Write-Host "[3/4] Birim testler çalıştırılıyor..." -ForegroundColor Green
    & ctest --test-dir build -C $BuildType --output-on-failure
    if ($LASTEXITCODE -ne 0) {
        Write-Warning "Bazı testler başarısız oldu (Exit code: $LASTEXITCODE)."
    } else {
        Write-Host "Tüm birim testler başarıyla geçti!" -ForegroundColor Green
    }
}

Write-Host "[4/4] VectorPDF derlemesi tamamlandı!" -ForegroundColor Cyan
Write-Host "Çıktı klasörü: build\bin veya build\$BuildType" -ForegroundColor White
