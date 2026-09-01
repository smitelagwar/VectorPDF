// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors

#ifndef SAMPLETEXTCOUNTERPLUGIN_H
#define SAMPLETEXTCOUNTERPLUGIN_H

#include "pdfplugin.h"

class SampleTextCounterPlugin : public pdf::PDFPlugin
{
    Q_OBJECT

public:
    explicit SampleTextCounterPlugin(QObject* parent = nullptr);
    ~SampleTextCounterPlugin() override = default;

    QString getPluginMenuName() const override;
    std::vector<QAction*> getActions() const override;

private:
    void calculateStatistics();
};

#endif // SAMPLETEXTCOUNTERPLUGIN_H
