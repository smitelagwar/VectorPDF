# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFWorkflow SHARED
    sources/workflowglobal.h
    sources/workflowtypes.h
    sources/workflowtypes.cpp
    sources/workflowservice.h
    sources/workflowservice.cpp
    sources/workflowtransaction.h
    sources/workflowtransaction.cpp

    sources/identity/workflowparticipant.h
    sources/identity/workflowparticipant.cpp
    sources/identity/participantresolver.h
    sources/identity/participantresolver.cpp

    sources/package/workflowpackagemanifest.h
    sources/package/workflowpackagemanifest.cpp
    sources/package/workflowpackagewriter.h
    sources/package/workflowpackagewriter.cpp
    sources/package/workflowpackagereader.h
    sources/package/workflowpackagereader.cpp
    sources/package/workflowpackagevalidator.h
    sources/package/workflowpackagevalidator.cpp

    sources/signrequest/signrequestfield.h
    sources/signrequestfield.cpp
    sources/signrequest/signrequestdefinition.h
    sources/signrequest/signrequestdefinition.cpp
    sources/signrequest/signrequeststate.h
    sources/signrequest/signrequeststate.cpp
    sources/signrequest/signrequestservice.h
    sources/signrequest/signrequestservice.cpp
    sources/signrequest/signresponseimporter.h
    sources/signrequest/signresponseimporter.cpp

    sources/review/reviewreaction.h
    sources/review/reviewcomment.h
    sources/review/reviewthread.h
    sources/review/reviewdefinition.h
    sources/review/reviewstate.h
    sources/review/reviewservice.h
    sources/review/reviewservice.cpp
    sources/review/reviewmergeengine.h
    sources/review/reviewmergeengine.cpp
    sources/review/reviewxfdfadapter.h
    sources/review/reviewxfdfadapter.cpp
    sources/review/reviewpackagestore.h
    sources/review/reviewpackagestore.cpp

    sources/approval/approvalstep.h
    sources/approval/approvalresponse.h
    sources/approval/approvaldefinition.h
    sources/approval/approvalaudittrail.h
    sources/approval/approvalaudittrail.cpp
    sources/approval/approvalservice.h
    sources/approval/approvalservice.cpp

    sources/transport/iworkflowtransport.h
    sources/transport/filesystemtransport.h
    sources/transport/filesystemtransport.cpp
    sources/transport/sharedfoldertransport.h
    sources/transport/sharedfoldertransport.cpp
    sources/transport/emailhandofftransport.h
    sources/transport/emailhandofftransport.cpp
    sources/transport/webdavtransport.h
    sources/transport/webdavtransport.cpp
)

add_compile_definitions(VECTORPDF_WORKFLOW_LIBRARY)

target_link_libraries(VectorPDFWorkflow PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

target_include_directories(VectorPDFWorkflow INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFWorkflow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFWorkflow PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFWorkflow
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
