# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFScanTools SHARED
    sources/scantoolsglobal.h
    sources/scanimage.h
    sources/scanimage.cpp
    sources/scanprocessingprofile.h
    sources/scanprocessingprofile.cpp
    sources/scanprocessingresult.h
    sources/scanprocessingpipeline.h
    sources/scanprocessingpipeline.cpp

    sources/cleanup/orientationdetector.h
    sources/cleanup/orientationdetector.cpp
    sources/cleanup/deskewprocessor.h
    sources/cleanup/deskewprocessor.cpp
    sources/cleanup/autocropdetector.h
    sources/cleanup/autocropdetector.cpp
    sources/cleanup/punchholeremover.h
    sources/cleanup/punchholeremover.cpp
    sources/cleanup/shadowremover.h
    sources/cleanup/shadowremover.cpp

    sources/blankpage/blankpagefeatures.h
    sources/blankpage/blankpagepolicy.h
    sources/blankpage/blankpagepolicy.cpp
    sources/blankpage/blankpagedetector.h
    sources/blankpage/blankpagedetector.cpp

    sources/duplicate/pageduplicatefingerprint.h
    sources/duplicate/pageduplicatefingerprint.cpp
    sources/duplicate/duplicatecluster.h
    sources/duplicate/duplicatecluster.cpp
    sources/duplicate/duplicatepolicy.h
    sources/duplicate/duplicatepolicy.cpp
    sources/duplicate/duplicatepagedetector.h
    sources/duplicate/duplicatepagedetector.cpp
)

add_compile_definitions(VECTORPDF_SCANTOOLS_LIBRARY)

target_link_libraries(VectorPDFScanTools PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

target_include_directories(VectorPDFScanTools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFScanTools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFScanTools PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFScanTools
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
