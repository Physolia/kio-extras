include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. )

########### next target ###############

add_executable(kio_man_test)
ecm_mark_as_test(kio_man_test)
set_target_properties(kio_man_test PROPERTIES
    COMPILE_FLAGS "-DSIMPLE_MAN2HTML -DKIO_MAN_TEST"
)

set(kio_man_test_generated_SRCS)
ecm_gperf_generate(../requests.gperf ${CMAKE_CURRENT_BINARY_DIR}/request_gperf.h kio_man_test_generated_SRCS)

target_sources(kio_man_test PRIVATE
    ${kio_man_test_generated_SRCS}
    kio_man_test.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../man2html.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../request_hash.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../kio_man.cpp 
)

target_link_libraries(kio_man_test Qt::Widgets KF${QT_MAJOR_VERSION}::I18n KF${QT_MAJOR_VERSION}::KIOCore KF${QT_MAJOR_VERSION}::Archive KF${QT_MAJOR_VERSION}::Codecs Qt::Network kio_man_debug)

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(kio_man_test Qt::Core5Compat)
endif()

if (AVOID_NULLPTR_WARNING_FLAG)
    target_compile_options(kio_man_test PRIVATE ${AVOID_NULLPTR_WARNING_FLAG})
endif()



########### next target ###############

add_executable(man2html )
ecm_mark_as_test(man2html)
set_target_properties(man2html PROPERTIES
    COMPILE_FLAGS -DSIMPLE_MAN2HTML
)

set(man2html_generated_SRCS)
ecm_gperf_generate(../requests.gperf ${CMAKE_CURRENT_BINARY_DIR}/request_gperf.h man2html_generated_SRCS)

target_sources(man2html PRIVATE
    ${man2html_generated_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../request_hash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../man2html.cpp 
)

target_link_libraries(man2html Qt::Core KF${QT_MAJOR_VERSION}::Codecs KF${QT_MAJOR_VERSION}::Archive kio_man_debug)

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(man2html Qt::Core5Compat)
endif()

if (AVOID_NULLPTR_WARNING_FLAG)
    target_compile_options(man2html PRIVATE ${AVOID_NULLPTR_WARNING_FLAG})
endif()
