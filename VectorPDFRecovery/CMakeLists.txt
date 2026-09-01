# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFRecovery SHARED
    sources/recoveryglobal.h
    sources/recoverytypes.h
    sources/recoverytypes.cpp
    sources/recoverycandidate.h
    sources/recoverycrypto.h
    sources/recoverycrypto.cpp
    sources/recoverymanifest.h
    sources/recoverymanifest.cpp
    sources/recoverystore.h
    sources/recoverystore.cpp
    sources/recoveryjournal.h
    sources/recoveryjournal.cpp
    sources/recoverysnapshotwriter.h
    sources/recoverysnapshotwriter.cpp
    sources/recoveryvalidator.h
    sources/recoveryvalidator.cpp
    sources/recoverycleanupservice.h
    sources/recoverycleanupservice.cpp
    sources/recoverysession.h
    sources/recoverysession.cpp
    sources/recoverylaunchscanner.h
    sources/recoverylaunchscanner.cpp
    sources/recoverymanager.h
    sources/recoverymanager.cpp
)

add_compile_definitions(VECTORPDF_RECOVERY_LIBRARY)

target_link_libraries(VectorPDFRecovery PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

if (WIN32)
    target_link_libraries(VectorPDFRecovery PRIVATE Crypt32)
endif()

target_include_directories(VectorPDFRecovery INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFRecovery PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFRecovery PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFRecovery
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
