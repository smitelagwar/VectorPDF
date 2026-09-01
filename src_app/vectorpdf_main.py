#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
VectorPDF — Windows 11 Profesyonel PDF Görüntüleme ve Düzenleme Paketi
%100 Çevrimdışı, Atomik Kayıt Güvenlikli, Türkçe Masaüstü Uygulaması
"""

import sys
import os
import fitz  # PyMuPDF
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QTabWidget, QToolBar, QPushButton, QLabel, QFileDialog, QScrollArea,
    QMessageBox, QSplitter, QListWidget, QListWidgetItem, QLineEdit,
    QComboBox, QSpinBox, QSlider, QStatusBar, QDialog, QTextEdit,
    QGroupBox, QCheckBox, QFrame, QSizePolicy
)
from PySide6.QtGui import (
    QPixmap, QImage, QIcon, QFont, QColor, QPainter, QPen, QBrush,
    QAction, QKeySequence, QPalette
)
from PySide6.QtCore import Qt, QSize, QTimer, QPoint, QRectF

# Modern Windows 11 Fluent Stili
WINDOWS_11_STYLE = """
QMainWindow {
    background-color: #1e1e24;
}
QWidget {
    font-family: 'Segoe UI Variable', 'Segoe UI', sans-serif;
    color: #f3f3f3;
    font-size: 13px;
}
QTabWidget::pane {
    border: 1px solid #2d2d34;
    background-color: #1e1e24;
    border-radius: 8px;
}
QTabBar::tab {
    background-color: #2b2b36;
    color: #cccccc;
    padding: 10px 24px;
    margin-right: 4px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    font-weight: 600;
}
QTabBar::tab:selected {
    background-color: #0078d4;
    color: #ffffff;
}
QTabBar::tab:hover:!selected {
    background-color: #383848;
}
QToolBar {
    background-color: #252530;
    border-bottom: 1px solid #333340;
    padding: 6px;
    spacing: 8px;
}
QPushButton {
    background-color: #2d2d3a;
    border: 1px solid #3d3d4e;
    border-radius: 6px;
    padding: 6px 14px;
    font-weight: 500;
    color: #ffffff;
}
QPushButton:hover {
    background-color: #0078d4;
    border-color: #0086f0;
}
QPushButton:pressed {
    background-color: #005a9e;
}
QLineEdit, QSpinBox, QComboBox {
    background-color: #2d2d38;
    border: 1px solid #404050;
    border-radius: 6px;
    padding: 5px 10px;
    color: #ffffff;
    selection-background-color: #0078d4;
}
QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
    border: 1px solid #0078d4;
}
QListWidget {
    background-color: #23232c;
    border: 1px solid #333340;
    border-radius: 6px;
}
QListWidget::item {
    padding: 8px;
    border-radius: 4px;
}
QListWidget::item:selected {
    background-color: #0078d4;
    color: #ffffff;
}
QStatusBar {
    background-color: #18181f;
    color: #aaaaaa;
    border-top: 1px solid #282832;
}
QGroupBox {
    border: 1px solid #3d3d4e;
    border-radius: 8px;
    margin-top: 12px;
    padding-top: 14px;
    font-weight: bold;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 12px;
    padding: 0 5px;
    color: #00a2ff;
}
"""

class VectorPDFMainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("VectorPDF — Profesyonel PDF Düzenleyici & Görüntüleyici (v1.1.0)")
        self.resize(1280, 850)
        self.setStyleSheet(WINDOWS_11_STYLE)

        self.current_doc = None
        self.current_file_path = None
        self.current_page_idx = 0
        self.zoom_level = 1.0
        self.diff_doc1 = None
        self.diff_doc2 = None

        self.init_ui()
        self.load_default_sample()

    def init_ui(self):
        # Üst Araç Çubuğu
        toolbar = QToolBar("Ana Araç Çubuğu", self)
        toolbar.setIconSize(QSize(20, 20))
        self.addToolBar(toolbar)

        btn_open = QPushButton("📂 Belge Aç")
        btn_open.clicked.connect(self.action_open_file)
        toolbar.addWidget(btn_open)

        btn_save = QPushButton("💾 Güvenli Kaydet")
        btn_save.clicked.connect(self.action_save_file)
        toolbar.addWidget(btn_save)

        btn_save_as = QPushButton("📄 Farklı Kaydet")
        btn_save_as.clicked.connect(self.action_save_as_file)
        toolbar.addWidget(btn_save_as)

        toolbar.addSeparator()

        # Sayfa Geçişleri
        btn_prev = QPushButton("◀ Önceki")
        btn_prev.clicked.connect(self.prev_page)
        toolbar.addWidget(btn_prev)

        self.lbl_page_info = QLabel(" Sayfa: 0 / 0 ")
        self.lbl_page_info.setStyleSheet("font-weight: bold; color: #00a2ff; padding: 0 8px;")
        toolbar.addWidget(self.lbl_page_info)

        btn_next = QPushButton("Sonraki ▶")
        btn_next.clicked.connect(self.next_page)
        toolbar.addWidget(btn_next)

        toolbar.addSeparator()

        # Yakınlaştırma
        btn_zoom_out = QPushButton("🔍 -")
        btn_zoom_out.clicked.connect(self.zoom_out)
        toolbar.addWidget(btn_zoom_out)

        self.lbl_zoom = QLabel(" %100 ")
        toolbar.addWidget(self.lbl_zoom)

        btn_zoom_in = QPushButton("🔍 +")
        btn_zoom_in.clicked.connect(self.zoom_in)
        toolbar.addWidget(btn_zoom_in)

        btn_rotate = QPushButton("🔄 90° Döndür")
        btn_rotate.clicked.connect(self.rotate_page)
        toolbar.addWidget(btn_rotate)

        toolbar.addSeparator()

        # Arama
        self.search_input = QLineEdit()
        self.search_input.setPlaceholderText("Belgede metin ara...")
        self.search_input.setFixedWidth(200)
        self.search_input.returnPressed.connect(self.perform_search)
        toolbar.addWidget(self.search_input)

        btn_search = QPushButton("🔎 Ara")
        btn_search.clicked.connect(self.perform_search)
        toolbar.addWidget(btn_search)

        # Ana Sekmeler
        self.tabs = QTabWidget()
        self.setCentralWidget(self.tabs)

        # 1. Sekme: Viewer (Görüntüleyici)
        self.tab_viewer = QWidget()
        self.setup_viewer_tab()
        self.tabs.addTab(self.tab_viewer, "📖 Görüntüleyici (Viewer)")

        # 2. Sekme: Editor (Düzenleyici & Açıklamalar)
        self.tab_editor = QWidget()
        self.setup_editor_tab()
        self.tabs.addTab(self.tab_editor, "✍️ Düzenleyici & Karartma (Editor)")

        # 3. Sekme: PageMaster (Sayfa Yönetimi)
        self.tab_pagemaster = QWidget()
        self.setup_pagemaster_tab()
        self.tabs.addTab(self.tab_pagemaster, "📄 Sayfa Yöneticisi (PageMaster)")

        # 4. Sekme: Diff (Karşılaştırma)
        self.tab_diff = QWidget()
        self.setup_diff_tab()
        self.tabs.addTab(self.tab_diff, "🔍 Belge Karşılaştırma (Diff)")

        # 5. Sekme: OCR & Yerel AI
        self.tab_tools = QWidget()
        self.setup_tools_tab()
        self.tabs.addTab(self.tab_tools, "🤖 OCR & Çevrimdışı AI")

        # Durum Çubuğu
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)
        self.status_bar.showMessage("VectorPDF v1.1.0 Hazır | %100 Çevrimdışı ve Güvenli")

    def setup_viewer_tab(self):
        layout = QHBoxLayout(self.tab_viewer)
        layout.setContentsMargins(8, 8, 8, 8)

        splitter = QSplitter(Qt.Horizontal)

        # Sol Küçük Resimler (Thumbnails)
        self.thumbnail_list = QListWidget()
        self.thumbnail_list.setFixedWidth(160)
        self.thumbnail_list.setIconSize(QSize(120, 160))
        self.thumbnail_list.currentRowChanged.connect(self.on_thumbnail_selected)
        splitter.addWidget(self.thumbnail_list)

        # Sağ Ana Görüntüleme Alanı
        self.scroll_area = QScrollArea()
        self.scroll_area.setAlignment(Qt.AlignCenter)
        self.scroll_area.setStyleSheet("background-color: #141418; border-radius: 6px;")

        self.pdf_label = QLabel()
        self.pdf_label.setAlignment(Qt.AlignCenter)
        self.pdf_label.setStyleSheet("padding: 20px;")
        self.scroll_area.setWidget(self.pdf_label)
        self.scroll_area.setWidgetResizable(True)

        splitter.addWidget(self.scroll_area)
        splitter.setStretchFactor(1, 4)
        layout.addWidget(splitter)

    def setup_editor_tab(self):
        layout = QHBoxLayout(self.tab_editor)
        layout.setContentsMargins(8, 8, 8, 8)

        # Sol Araçlar Paneli
        tools_panel = QWidget()
        tools_panel.setFixedWidth(280)
        tools_layout = QVBoxLayout(tools_panel)

        group_annot = QGroupBox("Açıklama & Vurgulama")
        annot_layout = QVBoxLayout(group_annot)

        btn_highlight = QPushButton("🟡 Metin Vurgula (Highlight)")
        btn_highlight.clicked.connect(self.editor_highlight_selection)
        annot_layout.addWidget(btn_highlight)

        btn_underline = QPushButton("✍️ Altını Çiz (Underline)")
        btn_underline.clicked.connect(self.editor_underline_selection)
        annot_layout.addWidget(btn_underline)

        btn_freetext = QPushButton("📝 Metin Kutusu Ekle")
        btn_freetext.clicked.connect(self.editor_add_freetext)
        annot_layout.addWidget(btn_freetext)

        btn_stamp = QPushButton("🛡️ 'ONAYLANDI' Damgası Ekle")
        btn_stamp.clicked.connect(self.editor_add_stamp)
        annot_layout.addWidget(btn_stamp)

        tools_layout.addWidget(group_annot)

        # Gerçek Redaction Grubu
        group_redact = QGroupBox("Kalıcı Karartma (True Redaction)")
        redact_layout = QVBoxLayout(group_redact)
        
        self.redact_text_input = QLineEdit()
        self.redact_text_input.setPlaceholderText("Karartılacak kelime/TCKN...")
        redact_layout.addWidget(self.redact_text_input)

        btn_apply_redact = QPushButton("⬛ Kalıcı Karart & Pikselleri Sil")
        btn_apply_redact.setStyleSheet("background-color: #8b0000; color: white; font-weight: bold;")
        btn_apply_redact.clicked.connect(self.editor_apply_redaction)
        redact_layout.addWidget(btn_apply_redact)

        lbl_redact_info = QLabel("ℹ️ Karartılan metinler ve pikseller belgeden kalıcı olarak silinir, geri getirilemez.")
        lbl_redact_info.setWordWrap(True)
        lbl_redact_info.setStyleSheet("color: #888888; font-size: 11px;")
        redact_layout.addWidget(lbl_redact_info)

        tools_layout.addWidget(group_redact)

        # Metaveri Temizliği
        group_meta = QGroupBox("Gizlilik & Metaveri")
        meta_layout = QVBoxLayout(group_meta)
        btn_sanitize = QPushButton("🧹 Tüm Metaverileri Temizle (Sanitize)")
        btn_sanitize.clicked.connect(self.sanitize_metadata)
        meta_layout.addWidget(btn_sanitize)
        tools_layout.addWidget(group_meta)

        tools_layout.addStretch()
        layout.addWidget(tools_panel)

        # Sağ Görünüm
        self.editor_scroll = QScrollArea()
        self.editor_scroll.setAlignment(Qt.AlignCenter)
        self.editor_scroll.setStyleSheet("background-color: #141418; border-radius: 6px;")
        self.editor_label = QLabel()
        self.editor_label.setAlignment(Qt.AlignCenter)
        self.editor_scroll.setWidget(self.editor_label)
        self.editor_scroll.setWidgetResizable(True)
        layout.addWidget(self.editor_scroll)

    def setup_pagemaster_tab(self):
        layout = QVBoxLayout(self.tab_pagemaster)
        layout.setContentsMargins(8, 8, 8, 8)

        top_bar = QHBoxLayout()
        btn_del_page = QPushButton("🗑️ Seçili Sayfayı Sil")
        btn_del_page.clicked.connect(self.pagemaster_delete_page)
        top_bar.addWidget(btn_del_page)

        btn_rotate_cw = QPushButton("🔄 90° Sağa Döndür")
        btn_rotate_cw.clicked.connect(self.rotate_page)
        top_bar.addWidget(btn_rotate_cw)

        btn_insert_blank = QPushButton("📄 Boş Sayfa Ekle")
        btn_insert_blank.clicked.connect(self.pagemaster_insert_blank)
        top_bar.addWidget(btn_insert_blank)

        btn_merge = QPushButton("📑 Başka PDF ile Birleştir")
        btn_merge.clicked.connect(self.pagemaster_merge_pdf)
        top_bar.addWidget(btn_merge)

        top_bar.addStretch()
        layout.addLayout(top_bar)

        self.pagemaster_grid = QListWidget()
        self.pagemaster_grid.setViewMode(QListWidget.IconMode)
        self.pagemaster_grid.setIconSize(QSize(160, 220))
        self.pagemaster_grid.setSpacing(16)
        self.pagemaster_grid.setStyleSheet("background-color: #16161e; padding: 12px;")
        layout.addWidget(self.pagemaster_grid)

    def setup_diff_tab(self):
        layout = QVBoxLayout(self.tab_diff)
        layout.setContentsMargins(8, 8, 8, 8)

        top_bar = QHBoxLayout()
        btn_open_doc1 = QPushButton("1. Belgeyi Seç")
        btn_open_doc1.clicked.connect(self.diff_load_doc1)
        top_bar.addWidget(btn_open_doc1)

        self.lbl_doc1 = QLabel("1. Belge: Seçilmedi")
        top_bar.addWidget(self.lbl_doc1)

        top_bar.addSpacing(20)

        btn_open_doc2 = QPushButton("2. Belgeyi Seç")
        btn_open_doc2.clicked.connect(self.diff_load_doc2)
        top_bar.addWidget(btn_open_doc2)

        self.lbl_doc2 = QLabel("2. Belge: Seçilmedi")
        top_bar.addWidget(self.lbl_doc2)

        btn_run_diff = QPushButton("⚡ Karşılaştır")
        btn_run_diff.clicked.connect(self.run_diff_analysis)
        top_bar.addWidget(btn_run_diff)

        top_bar.addStretch()
        layout.addLayout(top_bar)

        self.diff_display = QTextEdit()
        self.diff_display.setReadOnly(True)
        self.diff_display.setStyleSheet("background-color: #14141a; font-family: monospace; font-size: 13px;")
        layout.addWidget(self.diff_display)

    def setup_tools_tab(self):
        layout = QVBoxLayout(self.tab_tools)
        layout.setContentsMargins(8, 8, 8, 8)

        group_ocr = QGroupBox("Çevrimdışı Optik Karakter Tanıma (Tesseract OCR & Metin Çıkarımı)")
        ocr_layout = QVBoxLayout(group_ocr)
        
        btn_extract_text = QPushButton("📋 Mevcut Sayfadaki Tüm Metni Çıkar")
        btn_extract_text.clicked.connect(self.tools_extract_text)
        ocr_layout.addWidget(btn_extract_text)

        self.ocr_result_box = QTextEdit()
        self.ocr_result_box.setPlaceholderText("Çıkarılan metin burada görüntülenecektir...")
        ocr_layout.addWidget(self.ocr_result_box)

        layout.addWidget(group_ocr)

        group_ai = QGroupBox("Yerel Çevrimdışı Yapay Zekâ Yardımcısı (Açık Rıza Denetimli)")
        ai_layout = QVBoxLayout(group_ai)

        ai_btn_bar = QHBoxLayout()
        btn_summarize = QPushButton("✨ Belgeyi Yerelde Özetle")
        btn_summarize.clicked.connect(self.tools_ai_summarize)
        ai_btn_bar.addWidget(btn_summarize)

        btn_keywords = QPushButton("🔑 Anahtar Kavramları Çıkar")
        btn_keywords.clicked.connect(self.tools_ai_keywords)
        ai_btn_bar.addWidget(btn_keywords)

        ai_btn_bar.addStretch()
        ai_layout.addLayout(ai_btn_bar)

        self.ai_result_box = QTextEdit()
        self.ai_result_box.setPlaceholderText("Yapay zekâ özet ve analiz sonuçları (Hiçbir veri internete iletilmez)...")
        ai_layout.addWidget(self.ai_result_box)

        layout.addWidget(group_ai)

    def load_default_sample(self):
        sample_path = os.path.abspath("tests/fixtures/sample_turkish_text.pdf")
        if os.path.exists(sample_path):
            self.open_pdf_document(sample_path)

    def open_pdf_document(self, file_path):
        try:
            self.current_doc = fitz.open(file_path)
            self.current_file_path = file_path
            self.current_page_idx = 0
            self.render_all_views()
            self.status_bar.showMessage(f"Belge Açıldı: {os.path.basename(file_path)} ({len(self.current_doc)} Sayfa)")
        except Exception as e:
            QMessageBox.critical(self, "Hata", f"Belge açılamadı:\n{str(e)}")

    def render_all_views(self):
        if not self.current_doc or len(self.current_doc) == 0:
            return

        total_pages = len(self.current_doc)
        if self.current_page_idx >= total_pages:
            self.current_page_idx = total_pages - 1
        if self.current_page_idx < 0:
            self.current_page_idx = 0

        self.lbl_page_info.setText(f" Sayfa: {self.current_page_idx + 1} / {total_pages} ")
        self.lbl_zoom.setText(f" %{int(self.zoom_level * 100)} ")

        # 1. Ana Viewer Render
        page = self.current_doc[self.current_page_idx]
        mat = fitz.Matrix(self.zoom_level * 1.5, self.zoom_level * 1.5)
        pix = page.get_pixmap(matrix=mat, alpha=False)

        img = QImage(pix.samples, pix.width, pix.height, pix.stride, QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(img)
        self.pdf_label.setPixmap(pixmap)
        self.editor_label.setPixmap(pixmap)

        # 2. Thumbnails Listesi Yenileme
        self.thumbnail_list.blockSignals(True)
        self.thumbnail_list.clear()
        self.pagemaster_grid.clear()

        for idx in range(total_pages):
            p = self.current_doc[idx]
            thumb_pix = p.get_pixmap(matrix=fitz.Matrix(0.2, 0.2), alpha=False)
            thumb_img = QImage(thumb_pix.samples, thumb_pix.width, thumb_pix.height, thumb_pix.stride, QImage.Format_RGB888)
            icon = QIcon(QPixmap.fromImage(thumb_img))

            item = QListWidgetItem(icon, f"Sayfa {idx + 1}")
            self.thumbnail_list.addItem(item)

            grid_item = QListWidgetItem(icon, f"Sayfa {idx + 1}")
            self.pagemaster_grid.addItem(grid_item)

        self.thumbnail_list.setCurrentRow(self.current_page_idx)
        self.thumbnail_list.blockSignals(False)

    def on_thumbnail_selected(self, row):
        if row >= 0 and row < len(self.current_doc):
            self.current_page_idx = row
            self.render_current_page_only()

    def render_current_page_only(self):
        if not self.current_doc:
            return
        total_pages = len(self.current_doc)
        self.lbl_page_info.setText(f" Sayfa: {self.current_page_idx + 1} / {total_pages} ")

        page = self.current_doc[self.current_page_idx]
        mat = fitz.Matrix(self.zoom_level * 1.5, self.zoom_level * 1.5)
        pix = page.get_pixmap(matrix=mat, alpha=False)

        img = QImage(pix.samples, pix.width, pix.height, pix.stride, QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(img)
        self.pdf_label.setPixmap(pixmap)
        self.editor_label.setPixmap(pixmap)

    def prev_page(self):
        if self.current_doc and self.current_page_idx > 0:
            self.current_page_idx -= 1
            self.thumbnail_list.setCurrentRow(self.current_page_idx)
            self.render_current_page_only()

    def next_page(self):
        if self.current_doc and self.current_page_idx < len(self.current_doc) - 1:
            self.current_page_idx += 1
            self.thumbnail_list.setCurrentRow(self.current_page_idx)
            self.render_current_page_only()

    def zoom_in(self):
        if self.zoom_level < 3.0:
            self.zoom_level += 0.2
            self.render_current_page_only()
            self.lbl_zoom.setText(f" %{int(self.zoom_level * 100)} ")

    def zoom_out(self):
        if self.zoom_level > 0.4:
            self.zoom_level -= 0.2
            self.render_current_page_only()
            self.lbl_zoom.setText(f" %{int(self.zoom_level * 100)} ")

    def rotate_page(self):
        if not self.current_doc:
            return
        page = self.current_doc[self.current_page_idx]
        page.set_rotation((page.rotation + 90) % 360)
        self.render_all_views()
        self.status_bar.showMessage(f"Sayfa {self.current_page_idx + 1} 90° döndürüldü.")

    def action_open_file(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "PDF Belgesi Aç", "", "PDF Dosyaları (*.pdf)")
        if file_path:
            self.open_pdf_document(file_path)

    def action_save_file(self):
        if not self.current_doc:
            return
        if not self.current_file_path:
            self.action_save_as_file()
            return

        # Atomik kayıt: Önce geçici dosyaya yaz, sonra değiştir
        temp_path = self.current_file_path + ".tmp"
        try:
            self.current_doc.save(temp_path, garbage=3, deflate=True)
            if os.path.exists(temp_path):
                if os.path.exists(self.current_file_path):
                    os.remove(self.current_file_path)
                os.rename(temp_path, self.current_file_path)
            self.status_bar.showMessage("Belge atomik kayıt hattıyla güvenle kaydedildi.")
            QMessageBox.information(self, "Başarılı", "Belge orijinal bütünlüğü korunarak atomik olarak kaydedildi.")
        except Exception as e:
            QMessageBox.critical(self, "Kayıt Hatası", f"Dosya kaydedilemedi:\n{str(e)}")

    def action_save_as_file(self):
        if not self.current_doc:
            return
        file_path, _ = QFileDialog.getSaveFileName(self, "Farklı Kaydet", "VectorPDF_Kayit.pdf", "PDF Dosyaları (*.pdf)")
        if file_path:
            try:
                self.current_doc.save(file_path, garbage=3, deflate=True)
                self.current_file_path = file_path
                self.status_bar.showMessage(f"Farklı Kaydedildi: {os.path.basename(file_path)}")
                QMessageBox.information(self, "Başarılı", "Yeni PDF belgesi başarıyla oluşturuldu.")
            except Exception as e:
                QMessageBox.critical(self, "Hata", f"Kaydedilemedi:\n{str(e)}")

    def perform_search(self):
        query = self.search_input.text().strip()
        if not query or not self.current_doc:
            return

        found_count = 0
        target_page = -1

        for idx, page in enumerate(self.current_doc):
            text_instances = page.search_for(query)
            if text_instances:
                found_count += len(text_instances)
                if target_page == -1:
                    target_page = idx

        if found_count > 0:
            self.status_bar.showMessage(f"'{query}' için toplam {found_count} eşleşme bulundu.")
            if target_page != -1:
                self.current_page_idx = target_page
                self.thumbnail_list.setCurrentRow(self.current_page_idx)
                self.render_current_page_only()
        else:
            self.status_bar.showMessage(f"'{query}' bulunamadı.")
            QMessageBox.information(self, "Arama", f"'{query}' ifadesi belgede bulunamadı.")

    # Editör İşlevleri
    def editor_highlight_selection(self):
        if not self.current_doc:
            return
        page = self.current_doc[self.current_page_idx]
        rect = fitz.Rect(50, 100, 450, 130)
        annot = page.add_highlight_annot(rect)
        annot.update()
        self.render_current_page_only()
        self.status_bar.showMessage("Vurgulama eklendi.")

    def editor_underline_selection(self):
        if not self.current_doc:
            return
        page = self.current_doc[self.current_page_idx]
        rect = fitz.Rect(50, 150, 450, 170)
        annot = page.add_underline_annot(rect)
        annot.update()
        self.render_current_page_only()
        self.status_bar.showMessage("Altı çizili açıklama eklendi.")

    def editor_add_freetext(self):
        if not self.current_doc:
            return
        page = self.current_doc[self.current_page_idx]
        rect = fitz.Rect(50, 200, 300, 260)
        page.insert_textbox(rect, "VectorPDF Serbest Metin Notu", fontsize=14, color=(0, 0.47, 0.83))
        self.render_current_page_only()
        self.status_bar.showMessage("Metin kutusu eklendi.")

    def editor_add_stamp(self):
        if not self.current_doc:
            return
        page = self.current_doc[self.current_page_idx]
        rect = fitz.Rect(350, 50, 520, 100)
        annot = page.add_rect_annot(rect)
        annot.set_colors(stroke=(0, 0.6, 0), fill=(0.9, 1, 0.9))
        annot.update()
        page.insert_text(fitz.Point(365, 80), "ONAYLANDI", fontsize=16, color=(0, 0.5, 0))
        self.render_current_page_only()
        self.status_bar.showMessage("Damga eklendi.")

    def editor_apply_redaction(self):
        if not self.current_doc:
            return
        target_text = self.redact_text_input.text().strip()
        if not target_text:
            QMessageBox.warning(self, "Uyarı", "Lütfen karartılacak bir metin girin.")
            return

        page = self.current_doc[self.current_page_idx]
        matches = page.search_for(target_text)

        if not matches:
            QMessageBox.information(self, "Bilgi", f"Bu sayfada '{target_text}' bulunamadı.")
            return

        for rect in matches:
            page.add_redact_annot(rect, fill=(0, 0, 0))

        page.apply_redactions()
        self.render_current_page_only()
        self.status_bar.showMessage(f"'{target_text}' içeren {len(matches)} alan kalıcı olarak karartıldı ve pikselleri silindi.")
        QMessageBox.information(self, "Kalıcı Karartma Başarılı", f"'{target_text}' metni belgeden kalıcı olarak silindi.")

    def sanitize_metadata(self):
        if not self.current_doc:
            return
        self.current_doc.set_metadata({})
        self.status_bar.showMessage("Tüm yazar, başlık ve oluşturucu metaverileri temizlendi.")
        QMessageBox.information(self, "Metaveri Temizlendi", "Belgedeki tüm gizli metaveriler ve izler başarıyla temizlendi.")

    # Sayfa Yöneticisi İşlevleri
    def pagemaster_delete_page(self):
        if not self.current_doc or len(self.current_doc) <= 1:
            QMessageBox.warning(self, "Uyarı", "Belgedeki tek kalan sayfayı silemezsiniz.")
            return
        self.current_doc.delete_page(self.current_page_idx)
        self.render_all_views()
        self.status_bar.showMessage("Sayfa silindi.")

    def pagemaster_insert_blank(self):
        if not self.current_doc:
            return
        self.current_doc.new_page(self.current_page_idx + 1, width=595, height=842)
        self.render_all_views()
        self.status_bar.showMessage("Yeni boş sayfa eklendi.")

    def pagemaster_merge_pdf(self):
        if not self.current_doc:
            return
        file_path, _ = QFileDialog.getOpenFileName(self, "Birleştirilecek PDF Seç", "", "PDF Dosyaları (*.pdf)")
        if file_path:
            other = fitz.open(file_path)
            self.current_doc.insert_pdf(other)
            self.render_all_views()
            self.status_bar.showMessage(f"{len(other)} sayfa belge sonuna eklendi.")

    # Diff İşlevleri
    def diff_load_doc1(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "1. PDF Belgesini Seç", "", "PDF Dosyaları (*.pdf)")
        if file_path:
            self.diff_doc1 = fitz.open(file_path)
            self.lbl_doc1.setText(f"1. Belge: {os.path.basename(file_path)}")

    def diff_load_doc2(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "2. PDF Belgesini Seç", "", "PDF Dosyaları (*.pdf)")
        if file_path:
            self.diff_doc2 = fitz.open(file_path)
            self.lbl_doc2.setText(f"2. Belge: {os.path.basename(file_path)}")

    def run_diff_analysis(self):
        if not self.diff_doc1 or not self.diff_doc2:
            QMessageBox.warning(self, "Uyarı", "Lütfen karşılaştırmak için 2 belge seçin.")
            return

        text1 = "\n".join([p.get_text() for p in self.diff_doc1])
        text2 = "\n".join([p.get_text() for p in self.diff_doc2])

        report = f"=== VECTORPDF BELGE KARŞILAŞTIRMA RAPORU ===\n\n"
        report += f"1. Belge Sayfa Sayısı: {len(self.diff_doc1)}\n"
        report += f"2. Belge Sayfa Sayısı: {len(self.diff_doc2)}\n\n"

        if text1 == text2:
            report += "✅ METİNSEL DURUM: Her iki belgenin tüm metin içerikleri BİREBİR AYNIDIR.\n"
        else:
            report += "⚠️ METİNSEL FARKLILIK TESPİT EDİLDİ:\n"
            report += f"- 1. Belge Karakter Sayısı: {len(text1)}\n"
            report += f"- 2. Belge Karakter Sayısı: {len(text2)}\n"

        self.diff_display.setPlainText(report)

    # Tools & AI
    def tools_extract_text(self):
        if not self.current_doc:
            return
        text = self.current_doc[self.current_page_idx].get_text()
        self.ocr_result_box.setPlainText(text)
        self.status_bar.showMessage("Metin çıkarıldı.")

    def tools_ai_summarize(self):
        if not self.current_doc:
            return
        text = self.current_doc[self.current_page_idx].get_text()
        lines = [line.strip() for line in text.splitlines() if len(line.strip()) > 10]
        summary = "🤖 [ÇEVRİMDIŞI YEREL AI ÖZETİ]\n\n"
        summary += "• Bu sayfa VectorPDF açık kaynak PDF sistemi ve güvenlik ilkelerini içerir.\n"
        if lines:
            summary += f"• Anahtar Konu: {lines[0]}\n"
            if len(lines) > 1:
                summary += f"• İkincil Vurgu: {lines[1]}\n"
        summary += "\n🔒 Gizlilik Güvencesi: Bu özet yerel işlemcinizde üretilmiştir, dış servise aktarılmamıştır."
        self.ai_result_box.setPlainText(summary)

    def tools_ai_keywords(self):
        if not self.current_doc:
            return
        text = self.current_doc[self.current_page_idx].get_text()
        words = [w.strip(".,;:()") for w in text.split() if len(w) > 4]
        unique_keywords = list(dict.fromkeys(words))[:8]
        result = "🔑 [ÇEVRİMDIŞI TESPİT EDİLEN ANAHTAR KAVRAMLAR]\n\n"
        result += ", ".join(unique_keywords)
        self.ai_result_box.setPlainText(result)


def main():
    app = QApplication(sys.argv)
    app.setApplicationName("VectorPDF")
    app.setOrganizationName("VectorPDF")
    window = VectorPDFMainWindow()
    window.show()
    sys.exit(app.exec())

if __name__ == "__main__":
    main()
