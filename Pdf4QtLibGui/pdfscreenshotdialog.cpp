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
#include <QCursor>

namespace vectorpdf::gui
{

PDFScreenshotDialog::PDFScreenshotDialog(QWidget* parent)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setCursor(Qt::CrossCursor);
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        m_fullScreenPixmap = screen->grabWindow(0);
        setGeometry(screen->geometry());
    }

    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

QImage PDFScreenshotDialog::captureFullScreen()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen)
    {
        return screen->grabWindow(0).toImage();
    }
    return QImage();
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
            m_capturedImage = m_fullScreenPixmap.copy(rect).toImage();
            accept();
        }
        else
        {
            reject();
        }
    }
}

void PDFScreenshotDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_fullScreenPixmap.isNull())
    {
        painter.drawPixmap(0, 0, m_fullScreenPixmap);
        // Dim the background slightly to highlight region selection
        painter.fillRect(rect(), QColor(0, 0, 0, 80));
    }
}

} // namespace vectorpdf::gui
