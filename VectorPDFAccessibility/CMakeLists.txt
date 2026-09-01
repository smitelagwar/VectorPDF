# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFAccessibility SHARED
    sources/accessibilityglobal.h
    sources/accessibilitytypes.h
    sources/accessibilitytypes.cpp
    sources/accessibilityissue.h
    sources/accessibilityreport.h
    sources/accessibilityreport.cpp
    sources/accessibilitychecker.h
    sources/accessibilitychecker.cpp

    sources/rules/documentrules.h
    sources/rules/documentrules.cpp
    sources/rules/structurerules.h
    sources/rules/structurerules.cpp
    sources/rules/textalternativerules.h
    sources/rules/textalternativerules.cpp
    sources/rules/tablerules.h
    sources/rules/tablerules.cpp
    sources/rules/formrules.h
    sources/rules/formrules.cpp
    sources/rules/navigationrules.h
    sources/rules/navigationrules.cpp

    sources/structure/pdfstructuretreemodel.h
    sources/structure/pdfstructuretreemodel.cpp
    sources/structure/structuretreereader.h
    sources/structure/structuretreereader.cpp
    sources/structure/structuretreewriter.h
    sources/structure/structuretreewriter.cpp
    sources/structure/parenttreemanager.h
    sources/structure/parenttreemanager.cpp
    sources/structure/mcidallocator.h
    sources/structure/mcidallocator.cpp
    sources/structure/tagmutationtransaction.h
    sources/structure/tagmutationtransaction.cpp

    sources/readingorder/readingordermodel.h
    sources/readingorder/readingordermodel.cpp
    sources/readingorder/readingorderoverlay.h
    sources/readingorder/readingorderoverlay.cpp
    sources/readingorder/readingorderanalyzer.h
    sources/readingorder/readingorderanalyzer.cpp

    sources/autotag/autotagcandidate.h
    sources/autotag/autotagplan.h
    sources/autotag/autotagplan.cpp
    sources/autotag/nativeheuristictagger.h
    sources/autotag/nativeheuristictagger.cpp
    sources/autotag/doclingprotocol.h
    sources/autotag/doclingworker.h
    sources/autotag/doclingworker.cpp
    sources/autotag/alttextsuggester.h
    sources/autotag/alttextsuggester.cpp
    sources/autotag/autotaganalyzer.h
    sources/autotag/autotaganalyzer.cpp
    sources/autotag/autotagapplier.h
    sources/autotag/autotagapplier.cpp

    sources/validators/pdfuavalidationreport.h
    sources/validators/verapdfuaworker.h
    sources/validators/verapdfuaworker.cpp
)

add_compile_definitions(VECTORPDF_ACCESSIBILITY_LIBRARY)

target_link_libraries(VectorPDFAccessibility PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

target_include_directories(VectorPDFAccessibility INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFAccessibility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFAccessibility PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFAccessibility
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
