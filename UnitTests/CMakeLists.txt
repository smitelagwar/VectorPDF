# MIT License
# Copyright (c) 2018-2026 Jakub Melka and VectorPDF Contributors

add_executable(UnitTests
	tst_lexicalanalyzertest.cpp
)
target_link_libraries(UnitTests PRIVATE Pdf4QtLibCore Qt6::Core Qt6::Gui Qt6::Test)
set_target_properties(UnitTests PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE OFF
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}
)
add_test(UnitTests "${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}/UnitTests")

add_executable(UnitTestsImageOptimizer
	tst_imageoptimizertest.cpp
)
target_link_libraries(UnitTestsImageOptimizer PRIVATE Pdf4QtLibCore Qt6::Core Qt6::Gui Qt6::Test)
set_target_properties(UnitTestsImageOptimizer PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE OFF
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}
)
add_test(UnitTestsImageOptimizer "${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}/UnitTestsImageOptimizer")

add_executable(UnitTestsDocumentSession
	tst_documentsessiontest.cpp
)
target_link_libraries(UnitTestsDocumentSession PRIVATE Pdf4QtLibCore Qt6::Core Qt6::Gui Qt6::Test)
set_target_properties(UnitTestsDocumentSession PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE OFF
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}
)
add_test(UnitTestsDocumentSession "${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}/UnitTestsDocumentSession")

add_executable(UnitTestsM11Ecosystem
	tst_m11ecosystemtest.cpp
)
target_link_libraries(UnitTestsM11Ecosystem PRIVATE Pdf4QtLibCore Qt6::Core Qt6::Gui Qt6::Test)
set_target_properties(UnitTestsM11Ecosystem PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE OFF
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}
)
add_test(UnitTestsM11Ecosystem "${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}/UnitTestsM11Ecosystem")

add_executable(UnitTestsConversion
	tst_conversiontest.cpp
)
target_link_libraries(UnitTestsConversion PRIVATE VectorPDFConversion Pdf4QtLibCore Qt6::Core Qt6::Gui Qt6::Test)
target_include_directories(UnitTestsConversion PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../VectorPDFConversion/sources)
set_target_properties(UnitTestsConversion PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE OFF
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}
)
add_test(UnitTestsConversion "${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}/UnitTestsConversion")

# M13 Unit Tests
macro(add_m13_test TEST_NAME SRC_FILE LINK_LIB INC_DIR)
    add_executable(${TEST_NAME} ${SRC_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE ${LINK_LIB} Pdf4QtLibCore Qt6::Core Qt6::Gui Qt6::Test)
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../${INC_DIR}/sources)
    set_target_properties(${TEST_NAME} PROPERTIES
        WIN32_EXECUTABLE OFF
        MACOSX_BUNDLE OFF
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}
    )
    add_test(${TEST_NAME} "${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR}/${TEST_NAME}")
endmacro()

add_m13_test(UnitTestsDocumentToolsFoundation tst_documenttoolsfoundation.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_m13_test(UnitTestsFillSign tst_fillsigntest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_m13_test(UnitTestsPageDecoration tst_pagedecorationtest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_m13_test(UnitTestsFindReplace tst_findreplacetest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_m13_test(UnitTestsSmartRedact tst_smartredacttest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_m13_test(UnitTestsFlatten tst_flattentest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)

add_m13_test(UnitTestsAccessibilityChecker tst_accessibilitycheckertest.cpp VectorPDFAccessibility VectorPDFAccessibility)
add_m13_test(UnitTestsStructureTree tst_structuretreetest.cpp VectorPDFAccessibility VectorPDFAccessibility)
add_m13_test(UnitTestsAutoTag tst_autotagtest.cpp VectorPDFAccessibility VectorPDFAccessibility)
