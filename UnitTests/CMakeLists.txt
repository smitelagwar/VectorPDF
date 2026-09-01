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

macro(add_module_test TEST_NAME SRC_FILE LINK_LIB INC_DIR)
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

# M13
add_module_test(UnitTestsDocumentToolsFoundation tst_documenttoolsfoundation.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsFillSign tst_fillsigntest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsPageDecoration tst_pagedecorationtest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsFindReplace tst_findreplacetest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsSmartRedact tst_smartredacttest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsFlatten tst_flattentest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)

add_module_test(UnitTestsAccessibilityChecker tst_accessibilitycheckertest.cpp VectorPDFAccessibility VectorPDFAccessibility)
add_module_test(UnitTestsStructureTree tst_structuretreetest.cpp VectorPDFAccessibility VectorPDFAccessibility)
add_module_test(UnitTestsAutoTag tst_autotagtest.cpp VectorPDFAccessibility VectorPDFAccessibility)

# M14
add_module_test(UnitTestsRecovery tst_recoverytest.cpp VectorPDFRecovery VectorPDFRecovery)
add_module_test(UnitTestsWorkflow tst_workflowtest.cpp VectorPDFWorkflow VectorPDFWorkflow)
add_module_test(UnitTestsBates tst_batestest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsStamp tst_stamptest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsSanitize tst_sanitizetest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsPrint tst_printtest.cpp VectorPDFDocumentTools VectorPDFDocumentTools)
add_module_test(UnitTestsScanCleanup tst_scancleanuptest.cpp VectorPDFScanTools VectorPDFScanTools)
add_module_test(UnitTestsBlankPageDetection tst_blankpagedetectiontest.cpp VectorPDFScanTools VectorPDFScanTools)
add_module_test(UnitTestsDuplicatePageDetection tst_duplicatepagedetectiontest.cpp VectorPDFScanTools VectorPDFScanTools)
