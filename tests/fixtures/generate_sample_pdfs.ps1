<#
.SYNOPSIS
    VectorPDF Test Armatürü (Fixture) ve Örnek PDF Üreteci
.DESCRIPTION
    Bu betik yerel testler için geçerli ve standartlara uygun örnek PDF dosyaları üretir:
    1. sample_turkish_text.pdf (Türkçe metin ve tipografi)
    2. sample_multipage.pdf (Çok sayfalı belge düzeni)
    3. sample_form.pdf (Etkileşimli AcroForm formu)
.EXAMPLE
    .\tests\fixtures\generate_sample_pdfs.ps1
#>

$ErrorActionPreference = "Stop"
$outDir = $PSScriptRoot

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "  VectorPDF Örnek PDF Üretim Hattı        " -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan

# 1. sample_turkish_text.pdf
$turkishPdf = @"
%PDF-1.7
1 0 obj
<<
  /Type /Catalog
  /Pages 2 0 R
>>
endobj
2 0 obj
<<
  /Type /Pages
  /Kids [3 0 R]
  /Count 1
>>
endobj
3 0 obj
<<
  /Type /Page
  /Parent 2 0 R
  /MediaBox [0 0 612 792]
  /Contents 4 0 R
  /Resources <<
    /Font <<
      /F1 5 0 R
    >>
  >>
>>
endobj
4 0 obj
<<
  /Length 180
>>
stream
BT
/F1 22 Tf
50 720 Td
(VectorPDF - Turkce Belge Testi) Tj
/F1 12 Tf
0 -40 Td
(Bu belge VectorPDF v1.0 goruntuleyici ve duzenleyici testidir.) Tj
0 -25 Td
(Turkce karakterler ve metin secimi sorunsuz calismaktadir.) Tj
ET
endstream
endobj
5 0 obj
<<
  /Type /Font
  /Subtype /Type1
  /BaseFont /Helvetica
>>
endobj
xref
0 6
0000000000 65535 f 
0000000009 00000 n 
0000000058 00000 n 
0000000115 00000 n 
0000000266 00000 n 
0000000497 00000 n 
trailer
<<
  /Size 6
  /Root 1 0 R
>>
startxref
578
%%EOF
"@

Set-Content -Path "$outDir\sample_turkish_text.pdf" -Value $turkishPdf -Encoding Ascii
Write-Host "[1/3] sample_turkish_text.pdf üretildi." -ForegroundColor Green

# 2. sample_multipage.pdf (3 sayfa)
$multipagePdf = @"
%PDF-1.7
1 0 obj
<< /Type /Catalog /Pages 2 0 R >>
endobj
2 0 obj
<< /Type /Pages /Kids [3 0 R 4 0 R 5 0 R] /Count 3 >>
endobj
3 0 obj
<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Contents 6 0 R /Resources << /Font << /F1 9 0 R >> >> >>
endobj
4 0 obj
<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Contents 7 0 R /Resources << /Font << /F1 9 0 R >> >> >>
endobj
5 0 obj
<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Contents 8 0 R /Resources << /Font << /F1 9 0 R >> >> >>
endobj
6 0 obj
<< /Length 75 >>
stream
BT /F1 20 Tf 50 720 Td (VectorPDF Cok Sayfali Test - Sayfa 1) Tj ET
endstream
endobj
7 0 obj
<< /Length 75 >>
stream
BT /F1 20 Tf 50 720 Td (VectorPDF Cok Sayfali Test - Sayfa 2) Tj ET
endstream
endobj
8 0 obj
<< /Length 75 >>
stream
BT /F1 20 Tf 50 720 Td (VectorPDF Cok Sayfali Test - Sayfa 3) Tj ET
endstream
endobj
9 0 obj
<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>
endobj
xref
0 10
0000000000 65535 f 
0000000009 00000 n 
0000000058 00000 n 
0000000133 00000 n 
0000000249 00000 n 
0000000365 00000 n 
0000000481 00000 n 
0000000607 00000 n 
0000000733 00000 n 
0000000859 00000 n 
trailer
<< /Size 10 /Root 1 0 R >>
startxref
940
%%EOF
"@

Set-Content -Path "$outDir\sample_multipage.pdf" -Value $multipagePdf -Encoding Ascii
Write-Host "[2/3] sample_multipage.pdf üretildi." -ForegroundColor Green

# 3. sample_form.pdf (AcroForm)
$formPdf = @"
%PDF-1.7
1 0 obj
<< /Type /Catalog /Pages 2 0 R /AcroForm << /Fields [4 0 R] >> >>
endobj
2 0 obj
<< /Type /Pages /Kids [3 0 R] /Count 1 >>
endobj
3 0 obj
<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Contents 5 0 R /Annots [4 0 R] /Resources << /Font << /F1 6 0 R >> >> >>
endobj
4 0 obj
<<
  /Type /Annot
  /Subtype /Widget
  /FT /Tx
  /T (KullaniciAdi)
  /V (Ahmet Yilmaz)
  /Rect [50 650 250 680]
  /F 4
>>
endobj
5 0 obj
<< /Length 85 >>
stream
BT /F1 18 Tf 50 720 Td (VectorPDF AcroForm Test Formu) Tj ET
endstream
endobj
6 0 obj
<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>
endobj
xref
0 7
0000000000 65535 f 
0000000009 00000 n 
0000000086 00000 n 
0000000143 00000 n 
0000000277 00000 n 
0000000405 00000 n 
0000000541 00000 n 
trailer
<< /Size 7 /Root 1 0 R >>
startxref
622
%%EOF
"@

Set-Content -Path "$outDir\sample_form.pdf" -Value $formPdf -Encoding Ascii
Write-Host "[3/3] sample_form.pdf üretildi." -ForegroundColor Green

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "  Tüm test PDF'leri başarıyla oluşturuldu!" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
