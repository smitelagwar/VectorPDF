<#
.SYNOPSIS
    VectorPDF CLI Toplu İşlem (Batch Processing) Örnek Otomasyon Betiği
.DESCRIPTION
    Bu betik, belirtilen bir dizindeki tüm PDF dosyalarını tarar ve VectorPDF Batch CLI motoru
    üzerinden toplu OCR, optimizasyon veya PDF/A arşiv doğrulaması için komut kuyruğu oluşturur.
.EXAMPLE
    .\examples\automation\batch_process_sample.ps1 -InputDirectory ".\documents" -Action "Optimize"
#>

[CmdletBinding()]
param (
    [string]$InputDirectory = ".\tests\fixtures",
    [string]$OutputDirectory = ".\output",
    [ValidateSet("Convert", "OCR", "Optimize", "Sanitize", "ValidatePDFA")]
    [string]$Action = "Optimize"
)

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "  VectorPDF CLI Toplu İşlem Otomasyonu    " -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan

if (-not (Test-Path $InputDirectory)) {
    Write-Warning "Girdi dizini bulunamadı: $InputDirectory"
    return
}

if (-not (Test-Path $OutputDirectory)) {
    New-Item -ItemType Directory -Path $OutputDirectory -Force | Out-Null
}

$pdfFiles = Get-ChildItem -Path $InputDirectory -Filter "*.pdf"
Write-Host "Tespit edilen PDF dosya sayısı: $($pdfFiles.Count)" -ForegroundColor Green

$jobList = @()
$counter = 1

foreach ($file in $pdfFiles) {
    $outPath = Join-Path $OutputDirectory ("processed_" + $file.Name)
    $job = [PSCustomObject]@{
        JobId = "JOB-$counter"
        Type = $Action
        InputPath = $file.FullName
        OutputPath = $outPath
        Status = "Queued"
    }
    $jobList += $job
    Write-Host "  [$counter] Kuyruğa Eklendi: $($file.Name) -> $Action" -ForegroundColor White
    $counter++
}

Write-Host "`nToplam $($jobList.Count) iş VectorPDF Batch Engine kuyruğuna hazırlandı." -ForegroundColor Cyan
