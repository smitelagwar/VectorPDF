// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <QtTest>
#include <QCoreApplication>
#include <QTemporaryDir>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QCollator>

#include <conversiontypes.h>
#include <conversionrequest.h>
#include <conversionresult.h>
#include <conversioncapability.h>
#include <conversionregistry.h>
#include <conversionservice.h>
#include <conversionvalidator.h>

#include <utilities/pagerangeparser.h>
#include <utilities/tempfileguard.h>
#include <utilities/filenamepolicy.h>
#include <utilities/mimedetector.h>
#include <utilities/imageprocessing.h>
#include <utilities/outputverification.h>

#include <images/multipagetiffwriter.h>
#include <office/ooxmlpackagewriter.h>
#include <office/ooxmlpackagevalidator.h>
#include <office/docxpackagewriter.h>
#include <office/xlsxpackagewriter.h>
#include <office/pptxpackagewriter.h>
#include <office/tabledetector.h>

#include <pdfa/pdfaconformanceprofile.h>
#include <pdfa/pdfaconformanceanalyzer.h>
#include <pdfa/pdfaconformancetransformer.h>
#include <pdfa/pdfaconversionreport.h>
#include <workers/verapdfworker.h>

#include <creators/imagepdfcreator.h>
#include <creators/markdownpdfcreator.h>
#include <creators/htmlpdfcreator.h>
#include <creators/folderpdfcreator.h>

#include <exporters/pdfimageexporter.h>
#include <exporters/pdfdocxexporter.h>
#include <exporters/pdfxlsxexporter.h>
#include <exporters/pdfpptxexporter.h>
#include <exporters/pdfaexporter.h>
#include <exporters/monochromepdfexporter.h>
#include <exporters/xfdfexporter.h>
#include <exporters/fdfexporter.h>

#include <pdfdocument.h>
#include <pdfdocumentreader.h>
#include <pdfdocumentwriter.h>

using namespace vectorpdf::conversion;

class ConversionTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Utility Tests
    void testPageRangeParser();
    void testTempFileGuard();
    void testFilenamePolicy();
    void testMimeDetector();
    void testImageProcessing();

    // OOXML & Office Tests
    void testOoxmlZipWriter();
    void testOoxmlPackageValidator();
    void testDocxPackageWriter();
    void testXlsxPackageWriter();
    void testPptxPackageWriter();
    void testTableDetector();

    // Multi-page TIFF Tests
    void testMultiPageTiffWriter();

    // PDF/A Conformance Tests
    void testPdfAConformanceAnalyzer();
    void testPdfAConformanceTransformer();
    void testVeraPdfHonestUnvalidatedState();

    // Creators Tests
    void testImagePdfCreator();
    void testMarkdownPdfCreatorTurkishUnicode();
    void testHtmlPdfCreator();
    void testFolderPdfCreatorNaturalSort();

    // Exporters & Services Tests
    void testPdfExportToImages();
    void testPdfExportToDocx();
    void testPdfExportToXlsx();
    void testPdfExportToPptx();
    void testPdfExportToPdfA();
    void testPdfExportToMonochrome();
    void testPdfExportToXfdfFdf();
    void testConversionServiceAsyncAndCleanup();
    void testStressMultiPageConversion();

private:
    QString m_tempTestDir;
    QString m_samplePdfPath;
    void createSamplePdf();
};

void ConversionTest::initTestCase()
{
    QTemporaryDir dir;
    dir.setAutoRemove(false);
    m_tempTestDir = dir.path();
    createSamplePdf();
}

void ConversionTest::cleanupTestCase()
{
    if (!m_tempTestDir.isEmpty())
    {
        QDir(m_tempTestDir).removeRecursively();
    }
}

void ConversionTest::createSamplePdf()
{
    m_samplePdfPath = QDir(m_tempTestDir).filePath(QStringLiteral("sample_test.pdf"));

    QList<QImage> images;
    for (int i = 0; i < 3; ++i)
    {
        QImage img(400, 300, QImage::Format_RGB32);
        img.fill(i == 0 ? Qt::white : (i == 1 ? Qt::lightGray : Qt::cyan));
        QPainter p(&img);
        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 16));
        p.drawText(50, 50, QStringLiteral("VectorPDF Test Page %1").arg(i + 1));
        p.drawText(50, 100, QStringLiteral("This is sample paragraph content for conversion testing."));
        p.drawText(50, 150, QStringLiteral("Türkçe karakter testi: ığüşöç İĞÜŞÖÇ"));
        images.append(img);
    }

    QString err;
    bool ok = ImagePdfCreator::createPdfFromImages(images, m_samplePdfPath, 300, true, &err);
    QVERIFY2(ok, qPrintable(err));
    QVERIFY(QFile::exists(m_samplePdfPath));
}

void ConversionTest::testPageRangeParser()
{
    QList<int> pages = PageRangeParser::parse(QStringLiteral("1-3, 5, 7"), 10);
    QCOMPARE(pages.size(), 5);
    QCOMPARE(pages[0], 0);
    QCOMPARE(pages[1], 1);
    QCOMPARE(pages[2], 2);
    QCOMPARE(pages[3], 4);
    QCOMPARE(pages[4], 6);

    QList<int> allPages = PageRangeParser::parse(QStringLiteral("all"), 4);
    QCOMPARE(allPages.size(), 4);
    QCOMPARE(allPages, QList<int>({0, 1, 2, 3}));

    QString str = PageRangeParser::format({0, 1, 2, 4, 6});
    QCOMPARE(str, QStringLiteral("1-3, 5, 7"));
}

void ConversionTest::testTempFileGuard()
{
    QString tempPath;
    {
        TempFileGuard guard(QStringLiteral("test_guard_"));
        QVERIFY(guard.isValid());
        tempPath = guard.createTempFilePath(QStringLiteral("txt"));
        QFile f(tempPath);
        QVERIFY(f.open(QIODevice::WriteOnly));
        f.write("Hello Temporary Guard");
        f.close();
        QVERIFY(QFile::exists(tempPath));
    }
    QVERIFY(!QFile::exists(tempPath));

    TempFileGuard guard2(QStringLiteral("test_commit_"));
    QString src = guard2.createTempFilePath(QStringLiteral("dat"));
    QFile f(src);
    f.open(QIODevice::WriteOnly);
    f.write("Payload Data");
    f.close();

    QString dest = QDir(m_tempTestDir).filePath(QStringLiteral("committed_file.dat"));
    QString commitErr;
    bool ok = TempFileGuard::atomicCommit(src, dest, &commitErr);
    QVERIFY2(ok, qPrintable(commitErr));
    QVERIFY(QFile::exists(dest));
    QCOMPARE(QFileInfo(dest).size(), 12);
}

void ConversionTest::testFilenamePolicy()
{
    QString base = QDir(m_tempTestDir).filePath(QStringLiteral("document.pdf"));
    QString docxOut = FilenamePolicy::computeDefaultOutputPath(base, ConversionFormat::Docx);
    QVERIFY(docxOut.endsWith(QStringLiteral(".docx")));

    QString dummy = QDir(m_tempTestDir).filePath(QStringLiteral("existing.txt"));
    QFile f(dummy);
    f.open(QIODevice::WriteOnly);
    f.write("exists");
    f.close();

    QString nonColliding = FilenamePolicy::resolveCollision(dummy, false);
    QVERIFY(nonColliding != dummy);
    QVERIFY(nonColliding.contains(QStringLiteral("(1)")));

    QString overwriting = FilenamePolicy::resolveCollision(dummy, true);
    QCOMPARE(overwriting, dummy);
}

void ConversionTest::testMimeDetector()
{
    QCOMPARE(MimeDetector::detectFileKind(QStringLiteral("doc.docx")), FileKind::OfficeDocument);
    QCOMPARE(MimeDetector::detectFileKind(QStringLiteral("file.md")), FileKind::Markdown);
    QCOMPARE(MimeDetector::detectFileKind(QStringLiteral("page.html")), FileKind::Html);
    QCOMPARE(MimeDetector::detectFileKind(QStringLiteral("photo.png")), FileKind::Image);
    QCOMPARE(MimeDetector::detectFileKind(QStringLiteral("doc.pdf")), FileKind::Pdf);
}

void ConversionTest::testImageProcessing()
{
    QImage img(100, 100, QImage::Format_RGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    p.fillRect(10, 10, 80, 80, Qt::black);
    p.end();

    QImage monoOtsu = ImageProcessing::toMonochrome(img, MonochromeThresholdMethod::Otsu);
    QVERIFY(!monoOtsu.isNull());
    QCOMPARE(monoOtsu.format(), QImage::Format_Mono);

    QImage monoFixed = ImageProcessing::toMonochrome(img, MonochromeThresholdMethod::Fixed, 128);
    QVERIFY(!monoFixed.isNull());
    QCOMPARE(monoFixed.format(), QImage::Format_Mono);
}

void ConversionTest::testOoxmlZipWriter()
{
    QString zipPath = QDir(m_tempTestDir).filePath(QStringLiteral("test_archive.zip"));
    OoxmlPackageWriter writer;
    writer.addFile(QStringLiteral("[Content_Types].xml"), QByteArray("<?xml version=\"1.0\"?><Types/>"));
    writer.addFile(QStringLiteral("_rels/.rels"), QByteArray("<?xml version=\"1.0\"?><Relationships/>"));

    QString err;
    bool ok = writer.save(zipPath, &err);
    QVERIFY2(ok, qPrintable(err));
    QVERIFY(QFile::exists(zipPath));
    QVERIFY(QFileInfo(zipPath).size() > 50);
}

void ConversionTest::testOoxmlPackageValidator()
{
    // Test 1: Valid DOCX validation
    QString docxPath = QDir(m_tempTestDir).filePath(QStringLiteral("validator_test.docx"));
    DocxPackageWriter writer;
    writer.addHeading(QStringLiteral("Test Document"), 1);
    writer.addParagraph(QStringLiteral("Testing OOXML Package Validator."));
    QString err;
    QVERIFY(writer.save(docxPath, &err));

    OoxmlValidationResult validRes = OoxmlPackageValidator::validatePackage(docxPath, ConversionFormat::Docx);
    QVERIFY2(validRes.isValid, qPrintable(validRes.errorMessage));

    // Test 2: Incomplete archive (missing word/document.xml)
    QString badZip = QDir(m_tempTestDir).filePath(QStringLiteral("incomplete.docx"));
    OoxmlPackageWriter badWriter;
    badWriter.addFile(QStringLiteral("[Content_Types].xml"), QByteArray("<?xml version=\"1.0\"?><Types/>"));
    badWriter.addFile(QStringLiteral("_rels/.rels"), QByteArray("<?xml version=\"1.0\"?><Relationships/>"));
    badWriter.save(badZip, &err);

    OoxmlValidationResult invalidRes = OoxmlPackageValidator::validatePackage(badZip, ConversionFormat::Docx);
    QVERIFY(!invalidRes.isValid);
    QVERIFY(!invalidRes.missingRequiredParts.isEmpty());
}

void ConversionTest::testDocxPackageWriter()
{
    QString docxPath = QDir(m_tempTestDir).filePath(QStringLiteral("test_out.docx"));
    DocxPackageWriter writer;
    writer.addHeading(QStringLiteral("VectorPDF Document"), 1);
    writer.addParagraph(QStringLiteral("Türkçe paragraf: Çağdaş yazılım mimarisi ve belgeler."));
    writer.addPageBreak();
    writer.addHeading(QStringLiteral("Section 2"), 2);
    writer.addParagraph(QStringLiteral("Second page text."));

    QString err;
    bool ok = writer.save(docxPath, &err);
    QVERIFY2(ok, qPrintable(err));
    QVERIFY(QFile::exists(docxPath));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyDocx(docxPath, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testXlsxPackageWriter()
{
    QString xlsxPath = QDir(m_tempTestDir).filePath(QStringLiteral("test_out.xlsx"));
    XlsxPackageWriter writer;

    QList<DetectedTable> tables;
    DetectedTable table;
    table.pageIndex = 0;
    table.headers = { QStringLiteral("ID"), QStringLiteral("Ürün Adı"), QStringLiteral("Fiyat") };

    DetectedRow r1;
    r1.cells = { QStringLiteral("101"), QStringLiteral("VectorPDF Pro"), QStringLiteral("49.99") };
    table.rows.append(r1);

    DetectedRow r2;
    r2.cells = { QStringLiteral("102"), QStringLiteral("Kurumsal Lisans"), QStringLiteral("199.00") };
    table.rows.append(r2);

    tables.append(table);

    QString err;
    bool ok = writer.saveTables(tables, xlsxPath, &err);
    QVERIFY2(ok, qPrintable(err));
    QVERIFY(QFile::exists(xlsxPath));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyXlsx(xlsxPath, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testPptxPackageWriter()
{
    QString pptxPath = QDir(m_tempTestDir).filePath(QStringLiteral("test_out.pptx"));
    PptxPackageWriter writer;

    QImage slideImg(800, 600, QImage::Format_RGB32);
    slideImg.fill(Qt::white);
    QPainter p(&slideImg);
    p.drawText(100, 100, "Slide 1 Sunum Başlığı");
    p.end();

    writer.addVisualSlide(slideImg, QSizeF(720, 540));

    QString err;
    bool ok = writer.save(pptxPath, &err);
    QVERIFY2(ok, qPrintable(err));
    QVERIFY(QFile::exists(pptxPath));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyPptx(pptxPath, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testTableDetector()
{
    QList<DetectedTableItem> items;
    items.append({ QStringLiteral("Name"), QRectF(50, 100, 100, 20) });
    items.append({ QStringLiteral("Score"), QRectF(200, 100, 60, 20) });
    items.append({ QStringLiteral("Alice"), QRectF(50, 130, 100, 20) });
    items.append({ QStringLiteral("95"), QRectF(200, 130, 60, 20) });
    items.append({ QStringLiteral("Bob"), QRectF(50, 160, 100, 20) });
    items.append({ QStringLiteral("88"), QRectF(200, 160, 60, 20) });

    QList<DetectedTable> tables = TableDetector::detectTables(items, 0);
    QVERIFY(!tables.isEmpty());
    QCOMPARE(tables.first().headers.size(), 2);
    QCOMPARE(tables.first().rows.size(), 2);
}

void ConversionTest::testMultiPageTiffWriter()
{
    QList<QImage> pages;
    for (int i = 0; i < 5; ++i)
    {
        QImage page(300, 200, QImage::Format_RGB32);
        page.fill(i % 2 == 0 ? Qt::white : Qt::lightGray);
        QPainter p(&page);
        p.drawText(20, 30, QStringLiteral("TIFF Directory Page %1").arg(i + 1));
        pages.append(page);
    }

    QString tiffPath = QDir(m_tempTestDir).filePath(QStringLiteral("multi_page_test.tiff"));
    QString err;
    bool ok = MultiPageTiffWriter::writeMultiPageTiff(pages, tiffPath, 300, &err);
    QVERIFY2(ok, qPrintable(err));
    QVERIFY(QFile::exists(tiffPath));

    int dirCount = MultiPageTiffWriter::countDirectories(tiffPath, &err);
    QCOMPARE(dirCount, 5);
}

void ConversionTest::testPdfAConformanceAnalyzer()
{
    QFile f(m_samplePdfPath);
    f.open(QIODevice::ReadOnly);
    QByteArray data = f.readAll();
    f.close();

    pdf::PDFDocument doc;
    pdf::PDFDocumentReader reader(&doc);
    reader.read(data);

    PdfAAnalysisReport report = PdfAConformanceAnalyzer::analyze(&doc, PdfAProfile::PdfA2b);
    QVERIFY(report.canConvertPreservingVector);
    QVERIFY(!report.remediations.isEmpty());
}

void ConversionTest::testPdfAConformanceTransformer()
{
    QFile f(m_samplePdfPath);
    f.open(QIODevice::ReadOnly);
    QByteArray data = f.readAll();
    f.close();

    pdf::PDFDocument doc;
    pdf::PDFDocumentReader reader(&doc);
    reader.read(data);

    QString outPdfAPath = QDir(m_tempTestDir).filePath(QStringLiteral("transformer_pdfa2.pdf"));
    PdfAConversionReport report;
    QString err;
    bool ok = PdfAConformanceTransformer::transform(&doc, PdfAProfile::PdfA2b, PdfATransformationMode::PreserveVector, outPdfAPath, &report, &err);
    QVERIFY2(ok, qPrintable(err));
    QVERIFY(QFile::exists(outPdfAPath));
    QVERIFY(report.conversionSuccessful);

    QByteArray xmpPacket = PdfAConformanceTransformer::generatePdfAXmpPacket(PdfAProfile::PdfA2b, QStringLiteral("Test"), QStringLiteral("VectorPDF"));
    QVERIFY(xmpPacket.contains("pdfaid:part"));
    QVERIFY(xmpPacket.contains("pdfaid:conformance"));
}

void ConversionTest::testVeraPdfHonestUnvalidatedState()
{
    // If veraPDF path is invalid/empty, it must honestly report ValidationAvailability::Unavailable and isCompliant=false
    VeraPdfWorker fakeVera(QStringLiteral("C:/non_existent_verapdf_binary.exe"));
    QVERIFY(!fakeVera.isAvailable());

    VeraPdfValidationReport rep = fakeVera.validate(m_samplePdfPath, ConversionFormat::PdfA2);
    QCOMPARE(rep.availability, ValidationAvailability::Unavailable);
    QCOMPARE(rep.conformance, ConformanceState::Unknown);
    QCOMPARE(rep.isCompliant, false);
    QCOMPARE(rep.isValidated, false);
}

void ConversionTest::testImagePdfCreator()
{
    QList<QImage> images;
    for (int i = 0; i < 2; ++i)
    {
        QImage img(200, 200, QImage::Format_RGB32);
        img.fill(Qt::yellow);
        images.append(img);
    }

    QString outPdf = QDir(m_tempTestDir).filePath(QStringLiteral("created_from_images.pdf"));
    QString err;
    bool ok = ImagePdfCreator::createPdfFromImages(images, outPdf, 300, true, &err);
    QVERIFY2(ok, qPrintable(err));
    QVERIFY(QFile::exists(outPdf));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyPdf(outPdf, 2, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testMarkdownPdfCreatorTurkishUnicode()
{
    QString mdFile = QDir(m_tempTestDir).filePath(QStringLiteral("test_turkish.md"));
    QFile f(mdFile);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    f.write("# Türkçe Başlık: İĞÜŞÖÇ ığüşöç\n\n"
            "Bu bir **Markdown → PDF** dönüştürme testidir.\n\n"
            "| Şehir | Değer |\n"
            "| --- | --- |\n"
            "| İstanbul | 100 |\n"
            "| Ankara | 200 |\n\n"
            "```cpp\n"
            "QString test = \"Çağdaş Tipografi\";\n"
            "```\n");
    f.close();

    QString outPdf = QDir(m_tempTestDir).filePath(QStringLiteral("markdown_turkish_out.pdf"));
    ConversionRequest req;
    req.sourcePath = mdFile;
    req.outputPath = outPdf;
    req.markdownTheme = MarkdownTheme::Clean;

    MarkdownPdfCreator creator;
    ConversionResult res = creator.execute(req);
    QVERIFY2(res.isSuccess(), qPrintable(res.safeMessage));
    QVERIFY(QFile::exists(outPdf));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyPdf(outPdf, 0, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testHtmlPdfCreator()
{
    QString htmlFile = QDir(m_tempTestDir).filePath(QStringLiteral("test_doc.html"));
    QFile f(htmlFile);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    f.write("<html><head><title>Test HTML</title></head>"
            "<body><h1>HTML to PDF Test</h1><p>Converted safely without external network requests.</p></body></html>");
    f.close();

    QString outPdf = QDir(m_tempTestDir).filePath(QStringLiteral("html_out.pdf"));
    ConversionRequest req;
    req.sourcePath = htmlFile;
    req.outputPath = outPdf;

    HtmlPdfCreator creator;
    ConversionResult res = creator.execute(req);
    QVERIFY2(res.isSuccess(), qPrintable(res.safeMessage));
    QVERIFY(QFile::exists(outPdf));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyPdf(outPdf, 0, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testFolderPdfCreatorNaturalSort()
{
    QString folderPath = QDir(m_tempTestDir).filePath(QStringLiteral("natural_sort_folder"));
    QDir().mkpath(folderPath);

    // Create 1.md, 2.md, 10.md, 11.md
    for (const QString& name : { "1.md", "2.md", "10.md", "11.md" })
    {
        QFile f(QDir(folderPath).filePath(name));
        f.open(QIODevice::WriteOnly | QIODevice::Text);
        f.write(QStringLiteral("# Document %1").arg(name).toUtf8());
        f.close();
    }

    auto items = FolderPdfCreator::scanFolder(folderPath, false);
    QCOMPARE(items.size(), 4);
    // Numeric order verification: 1.md -> 2.md -> 10.md -> 11.md
    QCOMPARE(items[0].fileName, QStringLiteral("1.md"));
    QCOMPARE(items[1].fileName, QStringLiteral("2.md"));
    QCOMPARE(items[2].fileName, QStringLiteral("10.md"));
    QCOMPARE(items[3].fileName, QStringLiteral("11.md"));
}

void ConversionTest::testPdfExportToImages()
{
    ConversionRequest req;
    req.sourcePath = m_samplePdfPath;
    req.format = ConversionFormat::Png;
    req.outputDirectory = m_tempTestDir;
    req.dpi = 150;

    PdfImageExporter exporter;
    ConversionResult res = exporter.execute(req);
    QVERIFY2(res.isSuccess(), qPrintable(res.safeMessage));
    QVERIFY(!res.outputFiles.isEmpty());

    for (const QString& imgFile : res.outputFiles)
    {
        QString verifyErr;
        QVERIFY2(OutputVerification::verifyImage(imgFile, QStringLiteral("png"), &verifyErr), qPrintable(verifyErr));
    }
}

void ConversionTest::testPdfExportToDocx()
{
    QString outDocx = QDir(m_tempTestDir).filePath(QStringLiteral("sample_pdf_out.docx"));
    ConversionRequest req;
    req.sourcePath = m_samplePdfPath;
    req.outputPath = outDocx;
    req.format = ConversionFormat::Docx;
    req.docxMode = DocxExportMode::Reflow;

    PdfDocxExporter exporter;
    ConversionResult res = exporter.execute(req);
    QVERIFY2(res.isSuccess(), qPrintable(res.safeMessage));
    QVERIFY(QFile::exists(outDocx));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyDocx(outDocx, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testPdfExportToXlsx()
{
    QString outXlsx = QDir(m_tempTestDir).filePath(QStringLiteral("sample_pdf_out.xlsx"));
    ConversionRequest req;
    req.sourcePath = m_samplePdfPath;
    req.outputPath = outXlsx;
    req.format = ConversionFormat::Xlsx;
    req.xlsxMode = XlsxExportMode::PageLayout;

    PdfXlsxExporter exporter;
    ConversionResult res = exporter.execute(req);
    QVERIFY2(res.isSuccess(), qPrintable(res.safeMessage));
    QVERIFY(QFile::exists(outXlsx));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyXlsx(outXlsx, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testPdfExportToPptx()
{
    QString outPptx = QDir(m_tempTestDir).filePath(QStringLiteral("sample_pdf_out.pptx"));
    ConversionRequest req;
    req.sourcePath = m_samplePdfPath;
    req.outputPath = outPptx;
    req.format = ConversionFormat::Pptx;
    req.pptxMode = PptxExportMode::Visual;
    req.dpi = 150;

    PdfPptxExporter exporter;
    ConversionResult res = exporter.execute(req);
    QVERIFY2(res.isSuccess(), qPrintable(res.safeMessage));
    QVERIFY(QFile::exists(outPptx));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyPptx(outPptx, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testPdfExportToPdfA()
{
    QString outPdfA = QDir(m_tempTestDir).filePath(QStringLiteral("sample_pdfa.pdf"));
    ConversionRequest req;
    req.sourcePath = m_samplePdfPath;
    req.outputPath = outPdfA;
    req.format = ConversionFormat::PdfA2;

    PdfAExporter exporter;
    ConversionResult res = exporter.execute(req);
    QVERIFY2(res.isSuccess(), qPrintable(res.safeMessage));
    QVERIFY(QFile::exists(outPdfA));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyPdf(outPdfA, 3, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testPdfExportToMonochrome()
{
    QString outMono = QDir(m_tempTestDir).filePath(QStringLiteral("sample_mono.pdf"));
    ConversionRequest req;
    req.sourcePath = m_samplePdfPath;
    req.outputPath = outMono;
    req.format = ConversionFormat::MonochromePdf;
    req.dpi = 150;

    MonochromePdfExporter exporter;
    ConversionResult res = exporter.execute(req);
    QVERIFY2(res.isSuccess(), qPrintable(res.safeMessage));
    QVERIFY(QFile::exists(outMono));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyPdf(outMono, 3, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testPdfExportToXfdfFdf()
{
    QString outXfdf = QDir(m_tempTestDir).filePath(QStringLiteral("sample_form.xfdf"));
    ConversionRequest reqXfdf;
    reqXfdf.sourcePath = m_samplePdfPath;
    reqXfdf.outputPath = outXfdf;
    reqXfdf.format = ConversionFormat::Xfdf;

    XfdfExporter xfdfExp;
    ConversionResult resXfdf = xfdfExp.execute(reqXfdf);
    QVERIFY2(resXfdf.isSuccess(), qPrintable(resXfdf.safeMessage));
    QVERIFY(QFile::exists(outXfdf));

    QString verifyErr;
    QVERIFY2(OutputVerification::verifyXfdf(outXfdf, &verifyErr), qPrintable(verifyErr));

    QString outFdf = QDir(m_tempTestDir).filePath(QStringLiteral("sample_form.fdf"));
    ConversionRequest reqFdf;
    reqFdf.sourcePath = m_samplePdfPath;
    reqFdf.outputPath = outFdf;
    reqFdf.format = ConversionFormat::Fdf;

    FdfExporter fdfExp;
    ConversionResult resFdf = fdfExp.execute(reqFdf);
    QVERIFY2(resFdf.isSuccess(), qPrintable(resFdf.safeMessage));
    QVERIFY(QFile::exists(outFdf));

    QVERIFY2(OutputVerification::verifyFdf(outFdf, &verifyErr), qPrintable(verifyErr));
}

void ConversionTest::testConversionServiceAsyncAndCleanup()
{
    TempFileGuard guard(QStringLiteral("service_temp_"));
    QString tempInput = guard.createTempFilePath(QStringLiteral("png"));
    QImage dummy(10, 10, QImage::Format_RGB32);
    dummy.fill(Qt::black);
    dummy.save(tempInput, "PNG");
    guard.release();
    QVERIFY(QFile::exists(tempInput));

    ConversionRequest req;
    req.sourcePath = tempInput;
    req.format = ConversionFormat::Pdf;
    req.outputPath = QDir(m_tempTestDir).filePath(QStringLiteral("temp_cleaned_out.pdf"));
    req.ownedTemporaryInputPaths.append(tempInput);

    QSignalSpy spyFinished(&ConversionService::instance(), &ConversionService::jobFinished);

    ConversionJob* job = ConversionService::instance().enqueue(req);
    QVERIFY(job != nullptr);

    QTRY_VERIFY_WITH_TIMEOUT(job->status() == ConversionStatus::Success, 10000);
    QVERIFY(spyFinished.count() >= 1);
    QVERIFY(job->result().isSuccess());

    // Verify that owned temporary input was automatically cleaned up
    QVERIFY(!QFile::exists(tempInput));

    // Verify active jobs map pruned completed job
    QVERIFY(!ConversionService::instance().getActiveJobs().contains(job));
}

void ConversionTest::testStressMultiPageConversion()
{
    // Generate a multi-page PDF document
    QList<QImage> pages;
    for (int i = 0; i < 20; ++i)
    {
        QImage page(150, 150, QImage::Format_RGB32);
        page.fill(i % 2 == 0 ? Qt::white : Qt::lightGray);
        pages.append(page);
    }

    QString multiPdfPath = QDir(m_tempTestDir).filePath(QStringLiteral("stress_20p.pdf"));
    QString err;
    QVERIFY(ImagePdfCreator::createPdfFromImages(pages, multiPdfPath, 150, true, &err));

    // Export 20 pages to PNG
    ConversionRequest reqPng;
    reqPng.sourcePath = multiPdfPath;
    reqPng.format = ConversionFormat::Png;
    reqPng.outputDirectory = m_tempTestDir;
    reqPng.dpi = 100;

    PdfImageExporter imgExp;
    ConversionResult resPng = imgExp.execute(reqPng);
    QVERIFY2(resPng.isSuccess(), qPrintable(resPng.safeMessage));
    QCOMPARE(resPng.totalPagesProcessed, 20);
    QCOMPARE(resPng.outputFiles.size(), 20);

    // Export 20 pages to Multi-Page TIFF
    ConversionRequest reqTiff;
    reqTiff.sourcePath = multiPdfPath;
    reqTiff.format = ConversionFormat::Tiff;
    reqTiff.multiPageTiff = true;
    reqTiff.outputPath = QDir(m_tempTestDir).filePath(QStringLiteral("stress_20p.tiff"));
    reqTiff.dpi = 100;

    ConversionResult resTiff = imgExp.execute(reqTiff);
    QVERIFY2(resTiff.isSuccess(), qPrintable(resTiff.safeMessage));
    QCOMPARE(resTiff.totalPagesProcessed, 20);

    int tiffDirs = MultiPageTiffWriter::countDirectories(resTiff.outputPath, &err);
    QCOMPARE(tiffDirs, 20);
}

QTEST_MAIN(ConversionTest)
#include "tst_conversiontest.moc"
