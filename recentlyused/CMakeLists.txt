add_definitions(-DTRANSLATION_DOMAIN=\"kio5_recentlyused\")

add_library(recentlyused MODULE)
set_target_properties(recentlyused PROPERTIES
    OUTPUT_NAME "recentlyused"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kio"
)
target_sources(recentlyused PRIVATE
    recentlyused.cpp
)
# recentlyused.h has examples in the comment secions which match /*,
# so triggering the preprocessor to warn about nested comments.
# Using a pragma to ignore that diagnostic does not work for preprocessor warnings
# so for now simply that warning is completly disabled
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(_flag "-Wno-comment")
    check_cxx_compiler_flag(${_flag} _flag_present)
    if(_flag_present)
        target_compile_options(recentlyused PRIVATE "${_flag}")
    endif()
endif()

ecm_qt_declare_logging_category(recentlyused
    HEADER recentlyused-logsettings.h
    IDENTIFIER KIO_RECENTLYUSED_LOG
    CATEGORY_NAME kf.kio.workers.recentlyused
    OLD_CATEGORY_NAMES log_kio_recentlyused kf.kio.slaves.recentlyused
    DESCRIPTION "KIO recentlyused"
    EXPORT KIO_EXTRAS
)

target_link_libraries(recentlyused
    KF${QT_MAJOR_VERSION}::KIOCore
    KF${QT_MAJOR_VERSION}::I18n
    KF${QT_MAJOR_VERSION}::Activities
    KF${QT_MAJOR_VERSION}::ActivitiesStats)

install(TARGETS recentlyused DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kio)

# forgetfileitemaction plugin
kcoreaddons_add_plugin(forgetfileitemaction
    SOURCES forgetfileitemaction.cpp
    INSTALL_NAMESPACE "kf${QT_MAJOR_VERSION}/kfileitemaction")

target_link_libraries(forgetfileitemaction
    KF${QT_MAJOR_VERSION}::I18n
    KF${QT_MAJOR_VERSION}::KIOWidgets)
