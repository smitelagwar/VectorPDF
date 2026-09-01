// MIT License
//
// Copyright (c) 2018-2026 Jakub Melka and VectorPDF Contributors
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

#include "pdfplugin.h"
#include "pdfdbgheap.h"

namespace pdf
{

bool PDFPluginManifest::hasCapability(PDFPluginCapability cap) const
{
    return (grantedCapabilities & static_cast<quint32>(cap)) != 0;
}

PDFPluginManifest PDFPluginManifest::fromJson(const QJsonObject& json)
{
    PDFPluginManifest manifest;
    manifest.id = json.value(QLatin1String("Id")).toString();
    manifest.name = json.value(QLatin1String("Name")).toString();
    manifest.author = json.value(QLatin1String("Author")).toString();
    manifest.version = json.value(QLatin1String("Version")).toString();
    manifest.minAppVersion = json.value(QLatin1String("MinAppVersion")).toString();
    manifest.maxAppVersion = json.value(QLatin1String("MaxAppVersion")).toString();
    manifest.license = json.value(QLatin1String("License")).toString();
    manifest.description = json.value(QLatin1String("Description")).toString();
    manifest.signatureDigest = json.value(QLatin1String("SignatureDigest")).toString();
    manifest.isSigned = !manifest.signatureDigest.isEmpty();
    manifest.isTrusted = json.value(QLatin1String("IsTrusted")).toBool(false);

    QJsonArray caps = json.value(QLatin1String("Capabilities")).toArray();
    manifest.grantedCapabilities = static_cast<quint32>(PDFPluginCapability::ReadDocument);
    for (const auto& capVal : caps)
    {
        QString capStr = capVal.toString();
        if (capStr == "ModifyDocument")
        {
            manifest.grantedCapabilities |= static_cast<quint32>(PDFPluginCapability::ModifyDocument);
        }
        else if (capStr == "ExecuteExternalProcess")
        {
            manifest.grantedCapabilities |= static_cast<quint32>(PDFPluginCapability::ExecuteExternalProcess);
        }
        else if (capStr == "AccessSystemInfo")
        {
            manifest.grantedCapabilities |= static_cast<quint32>(PDFPluginCapability::AccessSystemInfo);
        }
    }

    return manifest;
}

QJsonObject PDFPluginManifest::toJson() const
{
    QJsonObject json;
    json[QLatin1String("Id")] = id;
    json[QLatin1String("Name")] = name;
    json[QLatin1String("Author")] = author;
    json[QLatin1String("Version")] = version;
    json[QLatin1String("MinAppVersion")] = minAppVersion;
    json[QLatin1String("MaxAppVersion")] = maxAppVersion;
    json[QLatin1String("License")] = license;
    json[QLatin1String("Description")] = description;
    json[QLatin1String("SignatureDigest")] = signatureDigest;
    json[QLatin1String("IsTrusted")] = isTrusted;

    QJsonArray caps;
    if (hasCapability(PDFPluginCapability::ReadDocument)) caps.append("ReadDocument");
    if (hasCapability(PDFPluginCapability::ModifyDocument)) caps.append("ModifyDocument");
    if (hasCapability(PDFPluginCapability::ExecuteExternalProcess)) caps.append("ExecuteExternalProcess");
    if (hasCapability(PDFPluginCapability::AccessSystemInfo)) caps.append("AccessSystemInfo");
    json[QLatin1String("Capabilities")] = caps;

    return json;
}

bool PDFPluginSecurityGuard::isActionPermitted(const PDFPluginManifest& manifest, PDFPluginCapability requiredCap, QString* denialReason)
{
    if (requiredCap == PDFPluginCapability::None)
    {
        return true;
    }

    if (!manifest.hasCapability(requiredCap))
    {
        if (denialReason)
        {
            *denialReason = QString("Action blocked: Plugin '%1' lacks required capability.").arg(manifest.name);
        }
        return false;
    }

    return true;
}

bool PDFPluginSecurityGuard::verifyManifestIntegrity(const PDFPluginManifest& manifest)
{
    if (manifest.id.isEmpty() || manifest.name.isEmpty())
    {
        return false;
    }
    return true;
}

PDFPlugin::PDFPlugin(QObject* parent) :
    QObject(parent),
    m_dataExchangeInterface(nullptr),
    m_widget(nullptr),
    m_cmsManager(nullptr),
    m_document(nullptr)
{
}

void PDFPlugin::setDataExchangeInterface(IPluginDataExchange* dataExchangeInterface)
{
    m_dataExchangeInterface = dataExchangeInterface;
}

void PDFPlugin::setWidget(PDFWidget* widget)
{
    m_widget = widget;
}

void PDFPlugin::setCMSManager(PDFCMSManager* manager)
{
    m_cmsManager = manager;
}

void PDFPlugin::setDocument(const PDFModifiedDocument& document)
{
    m_document = document;
}

void PDFPlugin::setManifest(const PDFPluginManifest& manifest)
{
    m_manifest = manifest;
}

const PDFPluginManifest& PDFPlugin::getManifest() const
{
    return m_manifest;
}

std::vector<QAction*> PDFPlugin::getActions() const
{
    return std::vector<QAction*>();
}

PDFPluginInfo PDFPluginInfo::loadFromJson(const QJsonObject* json)
{
    PDFPluginInfo result;

    QJsonObject metadata = json->value("MetaData").toObject();
    result.name = metadata.value(QLatin1String("Name")).toString();
    result.author = metadata.value(QLatin1String("Author")).toString();
    result.version = metadata.value(QLatin1String("Version")).toString();
    result.license = metadata.value(QLatin1String("License")).toString();
    result.description = metadata.value(QLatin1String("Description")).toString();
    result.manifest = PDFPluginManifest::fromJson(metadata);

    return result;
}

}   // namespace pdf
