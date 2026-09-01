# MIT License
# Copyright (c) 2018-2026 Jakub Melka and VectorPDF Contributors

add_library(Pdf4QtLibGui SHARED
    pdfcolorbutton.h
    pdfcolorbutton.cpp
    pdfdockwidget.h
    pdfdockwidget.cpp
    pdfdocumentpropertiesdialog.h
    pdfdocumentpropertiesdialog.cpp
    pdfdocumentpropertiesdialog.ui
    pdfgridiconengine.h
    pdfgridiconengine.cpp
    pdfgridview.h
    pdfgridview.cpp
    pdfguiutils.h
    pdfguiutils.cpp
    pdfhoverbutton.h
    pdfhoverbutton.cpp
    pdfhovertoolbutton.h
    pdfhovertoolbutton.cpp
    pdfpageboxeditor.h
    pdfpageboxeditor.cpp
    pdfpageboxeditor.ui
    pdfpageboxwidget.h
    pdfpageboxwidget.cpp
    pdfpagetransitionsdialog.h
    pdfpagetransitionsdialog.cpp
    pdfpagetransitionsdialog.ui
    pdfpageview.h
    pdfpageview.cpp
    pdfpageviewlayout.h
    pdfpageviewlayout.cpp
    pdfpageviewwidget.h
    pdfpageviewwidget.cpp
    pdfpageviewzoomanimation.h
    pdfpageviewzoomanimation.cpp
    pdfpasswordsdialog.h
    pdfpasswordsdialog.cpp
    pdfpasswordsdialog.ui
    pdfprintoptionsdialog.h
    pdfprintoptionsdialog.cpp
    pdfprintoptionsdialog.ui
    pdfprintpreviewwidget.h
    pdfprintpreviewwidget.cpp
    pdfsignaturedialog.h
    pdfsignaturedialog.cpp
    pdfsignaturedialog.ui
    pdfthemewidget.h
    pdfthemewidget.cpp
    pdftipdialog.h
    pdftipdialog.cpp
    pdftipdialog.ui
    pdfunitsprocessor.h
    pdfunitsprocessor.cpp
    pdfviewoptionsdialog.h
    pdfviewoptionsdialog.cpp
    pdfviewoptionsdialog.ui
    pdfviewwidget.h
    pdfviewwidget.cpp
    pdfviewzoomoptionsdialog.h
    pdfviewzoomoptionsdialog.cpp
    pdfviewzoomoptionsdialog.ui
    pdfoutlinemanager.h
    pdfoutlinemanager.cpp
    pdfoutlineui.h
    pdfoutlineui.cpp
    pdfbookmarkmanager.h
    pdfbookmarkmanager.cpp
    pdfbookmarkui.h
    pdfbookmarkui.cpp
    pdfactioncombobox.h
    pdfactioncombobox.cpp
    pdffullscreenwidget.h
    pdffullscreenwidget.cpp
    pdfconversioncenterdialog.h
    pdfconversioncenterdialog.cpp
    pdfconversioncenterdialog.ui
    pdfscreenshotdialog.h
    pdfscreenshotdialog.cpp
    pdfwiascannerbroker.h
    pdfwiascannerbroker.cpp

    # M13
    pdffillsignpanel.h
    pdffillsignpanel.cpp
    pdfheaderfooterdialog.h
    pdfheaderfooterdialog.cpp
    pdfwatermarkdialog.h
    pdfwatermarkdialog.cpp
    pdfbackgrounddialog.h
    pdfbackgrounddialog.cpp
    pdffindreplacedock.h
    pdffindreplacedock.cpp
    pdfsmartredactdialog.h
    pdfsmartredactdialog.cpp
    pdfflattendialog.h
    pdfflattendialog.cpp
    pdfaccessibilitydock.h
    pdfaccessibilitydock.cpp
    pdfreadingorderdock.h
    pdfreadingorderdock.cpp
    pdfautotagdialog.h
    pdfautotagdialog.cpp

    # M14
    pdfsignrequestdialog.h
    pdfsignrequestdialog.cpp
    pdfsignrequeststatusdock.h
    pdfsignrequeststatusdock.cpp
    pdfsharedreviewdialog.h
    pdfsharedreviewdialog.cpp
    pdfreviewdock.h
    pdfreviewdock.cpp
    pdfapprovaldialog.h
    pdfapprovaldialog.cpp
    pdfapprovalstatusdock.h
    pdfapprovalstatusdock.cpp
    pdfbatesdialog.h
    pdfbatesdialog.cpp
    pdfstampdock.h
    pdfstampdock.cpp
    pdfcustomstampdialog.h
    pdfcustomstampdialog.cpp
    pdfsanitizedialog.h
    pdfsanitizedialog.cpp
    pdfadvancedprintdialog.h
    pdfadvancedprintdialog.cpp
    pdfscancleanupstudio.h
    pdfscancleanupstudio.cpp
    pdfblankpagereviewdialog.h
    pdfblankpagereviewdialog.cpp
    pdfduplicatepagereviewdialog.h
    pdfduplicatepagereviewdialog.cpp
    pdfrecoverycenter.h
    pdfrecoverycenter.cpp
)

add_compile_definitions(QT_INSTALL_DIRECTORY="${QT6_INSTALL_PREFIX}")

include(GenerateExportHeader)

GENERATE_EXPORT_HEADER(Pdf4QtLibGui
                       EXPORT_MACRO_NAME
                       PDF4QTLIBGUILIBSHARED_EXPORT
                       EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}/pdf4qtlibgui_export.h")

target_link_libraries(Pdf4QtLibGui PRIVATE
    Pdf4QtLibCore
    Pdf4QtLibWidgets
    VectorPDFConversion
    VectorPDFDocumentTools
    VectorPDFAccessibility
    VectorPDFRecovery
    VectorPDFWorkflow
    VectorPDFScanTools
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::TextToSpeech
    Qt6::Xml
    Qt6::Svg
)

target_include_directories(Pdf4QtLibGui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Pdf4QtLibGui PUBLIC
    ${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../VectorPDFConversion/sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../VectorPDFDocumentTools/sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../VectorPDFAccessibility/sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../VectorPDFRecovery/sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../VectorPDFWorkflow/sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../VectorPDFScanTools/sources
)

set_target_properties(Pdf4QtLibGui PROPERTIES
                      VERSION ${PDF4QT_VERSION}
                      SOVERSION ${PDF4QT_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS Pdf4QtLibGui RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR} LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
