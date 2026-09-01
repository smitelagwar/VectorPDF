# MIT License
#
# Copyright (c) 2026 VectorPDF Contributors
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

add_library(VectorPDFConversion SHARED
    sources/conversionglobal.h
    sources/conversiontypes.h
    sources/conversiontypes.cpp
    sources/conversionrequest.h
    sources/conversionresult.h
    sources/conversioncapability.h
    sources/conversioncapability.cpp
    sources/conversionregistry.h
    sources/conversionregistry.cpp
    sources/conversionservice.h
    sources/conversionservice.cpp
    sources/conversionjob.h
    sources/conversionjob.cpp
    sources/conversionvalidator.h
    sources/conversionvalidator.cpp

    sources/utilities/pagerangeparser.h
    sources/utilities/pagerangeparser.cpp
    sources/utilities/imageprocessing.h
    sources/utilities/imageprocessing.cpp
    sources/utilities/tempfileguard.h
    sources/utilities/tempfileguard.cpp
    sources/utilities/filenamepolicy.h
    sources/utilities/filenamepolicy.cpp
    sources/utilities/mimedetector.h
    sources/utilities/mimedetector.cpp
    sources/utilities/outputverification.h
    sources/utilities/outputverification.cpp

    sources/office/ooxmlpackagewriter.h
    sources/office/ooxmlpackagewriter.cpp
    sources/office/docxpackagewriter.h
    sources/office/docxpackagewriter.cpp
    sources/office/pptxpackagewriter.h
    sources/office/pptxpackagewriter.cpp
    sources/office/xlsxpackagewriter.h
    sources/office/xlsxpackagewriter.cpp
    sources/office/tabledetector.h
    sources/office/tabledetector.cpp

    sources/workers/conversionworkerprotocol.h
    sources/workers/libreofficeworker.h
    sources/workers/libreofficeworker.cpp
    sources/workers/verapdfworker.h
    sources/workers/verapdfworker.cpp

    sources/exporters/pdfimageexporter.h
    sources/exporters/pdfimageexporter.cpp
    sources/exporters/pdfdocxexporter.h
    sources/exporters/pdfdocxexporter.cpp
    sources/exporters/pdfxlsxexporter.h
    sources/exporters/pdfxlsxexporter.cpp
    sources/exporters/pdfpptxexporter.h
    sources/exporters/pdfpptxexporter.cpp
    sources/exporters/pdfaexporter.h
    sources/exporters/pdfaexporter.cpp
    sources/exporters/monochromepdfexporter.h
    sources/exporters/monochromepdfexporter.cpp
    sources/exporters/xfdfexporter.h
    sources/exporters/xfdfexporter.cpp
    sources/exporters/fdfexporter.h
    sources/exporters/fdfexporter.cpp

    sources/creators/imagepdfcreator.h
    sources/creators/imagepdfcreator.cpp
    sources/creators/markdownpdfcreator.h
    sources/creators/markdownpdfcreator.cpp
    sources/creators/htmlpdfcreator.h
    sources/creators/htmlpdfcreator.cpp
    sources/creators/officepdfcreator.h
    sources/creators/officepdfcreator.cpp
    sources/creators/folderpdfcreator.h
    sources/creators/folderpdfcreator.cpp
)

add_compile_definitions(VECTORPDF_CONVERSION_LIBRARY)

target_link_libraries(VectorPDFConversion PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
    Qt6::Xml
    Qt6::PrintSupport
    ZLIB::ZLIB
)

target_include_directories(VectorPDFConversion INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFConversion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFConversion PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFConversion
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
