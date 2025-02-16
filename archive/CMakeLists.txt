add_definitions(-DTRANSLATION_DOMAIN=\"kio5_archive\")

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

set(LIBKIOARCHIVE_VERSION "5.98.0")
set(LIBKIOARCHIVE_SOVERSION "5")
set(LIBKIOARCHIVE_INSTALL_INCLUDEDIR "${KDE_INSTALL_INCLUDEDIR}/KioArchive")

include(ECMSetupVersion)
ecm_setup_version(${LIBKIOARCHIVE_VERSION} VARIABLE_PREFIX "KIOARCHIVE"
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kioarchive_version.h"
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KioArchiveConfigVersion.cmake"
                  SOVERSION ${LIBKIOARCHIVE_SOVERSION})

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KioArchive")

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KioArchiveConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KioArchiveConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KioArchiveConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KioArchiveConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KioArchiveTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KioArchiveTargets.cmake
)

install(FILES
    kio_archivebase.h
    ${CMAKE_CURRENT_BINARY_DIR}/libkioarchive_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/kioarchive_version.h
    DESTINATION ${LIBKIOARCHIVE_INSTALL_INCLUDEDIR}
    COMPONENT Devel
)

ecm_qt_declare_logging_category(kio_archive_debug_SRCS
    HEADER kio_archive_debug.h
    IDENTIFIER KIO_ARCHIVE_LOG
    CATEGORY_NAME kf.kio.workers.archive
    OLD_CATEGORY_NAMES log_kio_archive kf.kio.slaves.archive
    DESCRIPTION "KIO archive"
    EXPORT KIO_EXTRAS
)

############### ArchiveProtocolBase library ###########

add_library(kioarchive kio_archivebase.cpp ${kio_archive_debug_SRCS})

include(GenerateExportHeader)
generate_export_header(kioarchive BASE_NAME libkioarchive EXPORT_FILE_NAME libkioarchive_export.h)

target_compile_definitions(kioarchive
    PRIVATE
        -DKIOCORE_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054400 # need to support KIO::detailsToStatDetails
)

target_link_libraries(kioarchive
    PUBLIC
    KF${QT_MAJOR_VERSION}::Archive
    KF${QT_MAJOR_VERSION}::KIOCore
    PRIVATE
    KF${QT_MAJOR_VERSION}::I18n
    Qt::Network
    Qt::DBus
)

set_target_properties(kioarchive PROPERTIES
    VERSION ${KIOARCHIVE_VERSION}
    SOVERSION ${KIOARCHIVE_SOVERSION}
    EXPORT_NAME KioArchive
)

target_include_directories(kioarchive
    INTERFACE "$<INSTALL_INTERFACE:${LIBKIOARCHIVE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS kioarchive EXPORT KioArchiveTargets LIBRARY NAMELINK_SKIP ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

############### Archive Worker ######################

add_library(kio_archive MODULE kio_archive.cpp ${kio_archive_debug_SRCS})
target_link_libraries(kio_archive KF${QT_MAJOR_VERSION}::KIOCore KF${QT_MAJOR_VERSION}::Archive kioarchive Qt::Network)
set_target_properties(kio_archive PROPERTIES OUTPUT_NAME "archive")
install(TARGETS kio_archive DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kio)
