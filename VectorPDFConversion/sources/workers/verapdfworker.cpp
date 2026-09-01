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

#include "verapdfworker.h"
#include "../conversioncapability.h"
#include <QProcess>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QFile>

namespace vectorpdf::conversion
{

VeraPdfWorker::VeraPdfWorker(const QString& executablePath)
    : m_executablePath(executablePath)
{
    if (m_executablePath.isEmpty())
    {
        m_executablePath = ConversionCapability::instance().getVeraPdfPath();
    }
}

bool VeraPdfWorker::isAvailable() const
{
    return !m_executablePath.isEmpty() && QFileInfo::exists(m_executablePath);
}

QString VeraPdfWorker::executablePath() const
{
    return m_executablePath;
}

VeraPdfValidationReport VeraPdfWorker::validate(const QString& pdfFilePath, ConversionFormat profile, CancelToken* cancelToken)
{
    VeraPdfValidationReport report;
    report.profile = formatToString(profile);

    if (!isAvailable())
    {
        report.availability = ValidationAvailability::Unavailable;
        report.conformance = ConformanceState::Unknown;
        report.isValidated = false;
        report.isCompliant = false;
        report.statement = QStringLiteral("veraPDF validator CLI is not installed or configured. Strict PDF/A conformance certification could not be executed.");
        return report;
    }

    QString profileArg;
    switch (profile)
    {
    case ConversionFormat::PdfA1: profileArg = QStringLiteral("1b"); break;
    case ConversionFormat::PdfA2: profileArg = QStringLiteral("2b"); break;
    case ConversionFormat::PdfA3: profileArg = QStringLiteral("3b"); break;
    case ConversionFormat::PdfA4: profileArg = QStringLiteral("4"); break;
    default:                      profileArg = QStringLiteral("2b"); break;
    }

    QStringList args;
    args << QStringLiteral("--format") << QStringLiteral("xml");
    args << QStringLiteral("--flavour") << profileArg;
    args << pdfFilePath;

    QProcess process;
    process.setProgram(m_executablePath);
    process.setArguments(args);
    process.start();

    if (!process.waitForStarted(5000))
    {
        report.availability = ValidationAvailability::FailedToRun;
        report.conformance = ConformanceState::Unknown;
        report.isValidated = false;
        report.isCompliant = false;
        report.statement = QStringLiteral("Failed to launch veraPDF process: %1").arg(process.errorString());
        return report;
    }

    while (!process.waitForFinished(500))
    {
        if (cancelToken && cancelToken->load())
        {
            process.kill();
            process.waitForFinished(1000);
            report.availability = ValidationAvailability::FailedToRun;
            report.conformance = ConformanceState::Unknown;
            report.statement = QStringLiteral("Validation cancelled by user.");
            return report;
        }
    }

    const QByteArray xmlOutput = process.readAllStandardOutput();
    if (xmlOutput.isEmpty())
    {
        report.availability = ValidationAvailability::FailedToRun;
        report.conformance = ConformanceState::Unknown;
        report.isValidated = false;
        report.isCompliant = false;
        report.statement = QStringLiteral("veraPDF did not produce standard output report.");
        return report;
    }

    report.availability = ValidationAvailability::Available;
    report.isValidated = true;

    QXmlStreamReader xml(xmlOutput);
    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            if (xml.name() == QLatin1String("validationResult"))
            {
                const QString comp = xml.attributes().value(QStringLiteral("isCompliant")).toString();
                report.isCompliant = (comp.compare(QLatin1String("true"), Qt::CaseInsensitive) == 0);
                report.conformance = report.isCompliant ? ConformanceState::Conformant : ConformanceState::NonConformant;
            }
            else if (xml.name() == QLatin1String("statement"))
            {
                report.statement = xml.readElementText();
            }
            else if (xml.name() == QLatin1String("failedRule"))
            {
                report.failedRules.append(xml.attributes().value(QStringLiteral("ruleId")).toString());
            }
        }
    }

    return report;
}

} // namespace vectorpdf::conversion
