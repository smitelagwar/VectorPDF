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

#include "pdfscreenshotdialog.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCursor>

namespace vectorpdf::gui
{

PDFScreenshotDialog::PDFScreenshotDialog(QWidget* parent)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::BypassWindowManagerHint)
{
    setCursor(Qt::CrossCursor);
    grabVirtualDesktop();
    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

QStringList PDFScreenshotDialog::availableScreenNames()
{
    QStringList names;
    const auto screens = QGuiApplication::screens();
    for (int i = 0; i < screens.size(); ++i)
    {
        names.append(QStringLiteral("Monitor %1 (%2x%3)").arg(i + 1)
                     .arg(screens[i]->geometry().width())
                     .arg(screens[i]->geometry().height()));
    }
    return names;
}

void PDFScreenshotDialog::grabVirtualDesktop()
{
    const auto screens = QGuiApplication::screens();
    if (screens.isEmpty()) return;

    QRect unionGeom = screens.first()->geometry();
    for (QScreen* scr : screens)
    {
        unionGeom = unionGeom.united(scr->geometry());
    }

    m_virtualGeometry = unionGeom;
    setGeometry(m_virtualGeometry);

    QPixmap combined(m_virtualGeometry.size());
    combined.fill(Qt::black);
    QPainter p(&combined);

    for (QScreen* scr : screens)
    {
        QPixmap sp = scr->grabWindow(0);
        QPoint offset = scr->geometry().topLeft() - m_virtualGeometry.topLeft();
        p.drawPixmap(offset, sp);
    }
    p.end();

    m_virtualDesktopPixmap = combined;
}

QImage PDFScreenshotDialog::captureFullScreen(int monitorIndex)
{
    const auto screens = QGuiApplication::screens();
    if (screens.isEmpty()) return QImage();

    if (monitorIndex >= 0 && monitorIndex < screens.size())
    {
        return screens[monitorIndex]->grabWindow(0).toImage();
    }

    // Capture virtual desktop (all monitors)
    QRect unionGeom = screens.first()->geometry();
    for (QScreen* scr : screens)
    {
        unionGeom = unionGeom.united(scr->geometry());
    }

    QImage combined(unionGeom.size(), QImage::Format_RGB32);
    combined.fill(Qt::black);
    QPainter p(&combined);

    for (QScreen* scr : screens)
    {
        QPixmap sp = scr->grabWindow(0);
        QPoint offset = scr->geometry().topLeft() - unionGeom.topLeft();
        p.drawPixmap(offset, sp);
    }
    p.end();

    return combined;
}

QImage PDFScreenshotDialog::captureRegion(QWidget* parent)
{
    PDFScreenshotDialog dlg(parent);
    if (dlg.exec() == QDialog::Accepted)
    {
        return dlg.capturedImage();
    }
    return QImage();
}

QImage PDFScreenshotDialog::capturedImage() const
{
    return m_capturedImage;
}

void PDFScreenshotDialog::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_origin = event->pos();
        m_rubberBand->setGeometry(QRect(m_origin, QSize()));
        m_rubberBand->show();
    }
    else if (event->button() == Qt::RightButton)
    {
        reject();
    }
}

void PDFScreenshotDialog::mouseMoveEvent(QMouseEvent* event)
{
    if (m_rubberBand->isVisible())
    {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
    }
}

void PDFScreenshotDialog::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_rubberBand->isVisible())
    {
        QRect rect = m_rubberBand->geometry();
        m_rubberBand->hide();

        if (rect.width() > 5 && rect.height() > 5)
        {
            m_capturedImage = m_virtualDesktopPixmap.copy(rect).toImage();
            accept();
        }
        else
        {
            reject();
        }
    }
}

void PDFScreenshotDialog::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        reject();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}

void PDFScreenshotDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_virtualDesktopPixmap.isNull())
    {
        painter.drawPixmap(0, 0, m_virtualDesktopPixmap);
        // Dim the background slightly to highlight region selection
        painter.fillRect(rect(), QColor(0, 0, 0, 80));
    }
}

} // namespace vectorpdf::gui
