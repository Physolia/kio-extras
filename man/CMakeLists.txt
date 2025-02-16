add_definitions(-DTRANSLATION_DOMAIN=\"kio5_man\")

remove_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_USE_QSTRINGBUILDER
)

find_package(KF${QT_MAJOR_VERSION} REQUIRED COMPONENTS Codecs)

# silence warnings for gperf generating code with 0 for null pointers
set(AVOID_NULLPTR_WARNING_FLAG)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(_flag "-Wno-zero-as-null-pointer-constant")
    check_cxx_compiler_flag(${_flag} _flag_present)
    if(_flag_present)
        set(AVOID_NULLPTR_WARNING_FLAG ${_flag})
    endif()
endif()

if(BUILD_TESTING)
    add_subdirectory( tests )
endif()

# intermediate target/library, also used by the tests
add_library(kio_man_debug OBJECT)
target_link_libraries(kio_man_debug
    PRIVATE Qt::Core
)
ecm_qt_declare_logging_category(kio_man_debug
    HEADER kio_man_debug.h
    IDENTIFIER KIO_MAN_LOG
    CATEGORY_NAME kf.kio.workers.man
    OLD_CATEGORY_NAMES log_kio_man kf.kio.slaves.man
    DESCRIPTION "KIO man"
    EXPORT KIO_EXTRAS
)

set_property(TARGET kio_man_debug PROPERTY POSITION_INDEPENDENT_CODE ON)

########### next target ###############

add_library(kio_man MODULE)
set_target_properties(kio_man PROPERTIES
    OUTPUT_NAME "man"
)

set(kio_man_generated_SRCS)
ecm_gperf_generate(requests.gperf ${CMAKE_CURRENT_BINARY_DIR}/request_gperf.h kio_man_generated_SRCS)
target_sources(kio_man PRIVATE
    ${kio_man_generated_SRCS}
    man2html.cpp
    kio_man.cpp
    request_hash.cpp
)

if (AVOID_NULLPTR_WARNING_FLAG)
    target_compile_options(kio_man PRIVATE ${AVOID_NULLPTR_WARNING_FLAG})
endif()

target_link_libraries(kio_man
    PRIVATE KF${QT_MAJOR_VERSION}::KIOCore Qt::Widgets KF${QT_MAJOR_VERSION}::Archive KF${QT_MAJOR_VERSION}::I18n KF${QT_MAJOR_VERSION}::Codecs Qt::Network kio_man_debug
)

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(kio_man PRIVATE Qt::Core5Compat)
endif()

install(TARGETS kio_man DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kio)
