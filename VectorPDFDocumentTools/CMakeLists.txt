# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFDocumentTools SHARED
    sources/documenttoolsglobal.h
    sources/documenttooltypes.h
    sources/documenttooltypes.cpp
    sources/documenttoolresult.h
    sources/documenttooltransaction.h
    sources/documenttooltransaction.cpp
    sources/documenttoolvalidator.h
    sources/documenttoolvalidator.cpp

    sources/fillsign/signatureasset.h
    sources/fillsign/signatureasset.cpp
    sources/fillsign/signatureassetstore.h
    sources/fillsign/signatureassetstore.cpp
    sources/fillsign/fillsignappearancebuilder.h
    sources/fillsign/fillsignappearancebuilder.cpp
    sources/fillsign/fillsignplacementmodel.h
    sources/fillsign/fillsignplacementmodel.cpp
    sources/fillsign/fillsigntransaction.h
    sources/fillsign/fillsigntransaction.cpp

    sources/decorations/pagedecorationdefinition.h
    sources/decorations/pagedecorationdefinition.cpp
    sources/decorations/decorationtemplateengine.h
    sources/decorations/decorationtemplateengine.cpp
    sources/decorations/pagedecorationrenderer.h
    sources/decorations/pagedecorationrenderer.cpp
    sources/decorations/pagedecorationmanager.h
    sources/decorations/pagedecorationmanager.cpp
    sources/decorations/headerfooterdefinition.h
    sources/decorations/watermarkdefinition.h
    sources/decorations/backgrounddefinition.h

    sources/contenttools/findreplacequery.h
    sources/contenttools/findreplacematch.h
    sources/contenttools/findreplaceanalyzer.h
    sources/contenttools/findreplaceanalyzer.cpp
    sources/contenttools/findreplacetransaction.h
    sources/contenttools/findreplacetransaction.cpp

    sources/redact/redactionpatternregistry.h
    sources/redactionpatternregistry.cpp
    sources/redact/iredactiondetector.h
    sources/redact/redactioncandidate.h
    sources/redact/redactionreviewmodel.h
    sources/redact/redactionreviewmodel.cpp
    sources/redact/detectors/emaildetector.h
    sources/redact/detectors/emaildetector.cpp
    sources/redact/detectors/phonedetector.h
    sources/redact/detectors/phonedetector.cpp
    sources/redact/detectors/tckimlikdetector.h
    sources/redact/detectors/tckimlikdetector.cpp
    sources/redact/detectors/ibandetector.h
    sources/redact/detectors/ibandetector.cpp
    sources/redact/detectors/paymentcarddetector.h
    sources/redact/detectors/paymentcarddetector.cpp
    sources/redact/detectors/ipdetector.h
    sources/redact/detectors/ipdetector.cpp
    sources/redact/detectors/urldetector.h
    sources/redact/detectors/urldetector.cpp
    sources/redact/detectors/datedetector.h
    sources/redact/detectors/datedetector.cpp
    sources/redact/detectors/turkishplatedetector.h
    sources/redact/detectors/turkishplatedetector.cpp

    sources/flatten/flattenpolicy.h
    sources/flatten/flattenplan.h
    sources/flatten/flattenreport.h
    sources/flatten/flattenengine.h
    sources/flatten/flattenengine.cpp
    sources/flatten/annotationflattener.h
    sources/flatten/annotationflattener.cpp
    sources/flatten/formflattener.h
    sources/flatten/formflattener.cpp
    sources/flatten/optionalcontentflattener.h
    sources/flatten/optionalcontentflattener.cpp
)

add_compile_definitions(VECTORPDF_DOCUMENTTOOLS_LIBRARY)

target_link_libraries(VectorPDFDocumentTools PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

if (WIN32)
    target_link_libraries(VectorPDFDocumentTools PRIVATE Crypt32)
endif()

target_include_directories(VectorPDFDocumentTools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFDocumentTools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFDocumentTools PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFDocumentTools
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
