#.rst:
# FindIMobileDevice
# --------
#
# Try to find the imobiledevice library, once done this will define:
#
#  ``IMobileDevice_FOUND``
#      System has libimobiledevice.
#
#  ``IMobileDevice_INCLUDE_DIRS``
#      The libimobiledevice include directory.
#
# ``IMobileDevice_LIBRARIES``
#     The libimobiledevice libraries.
#
# ``IMobileDevice_VERSION``
#     The libimobiledevice version.
#
# If ``IMobileDevice_FOUND`` is TRUE, the following imported target
# will be available:
#
# ``IMobileDevice::IMobileDevice``
#     The libimobiledevice library

#=============================================================================
# SPDX-FileCopyrightText: 2020 Kai Uwe Broulik <kde@broulik.de>
#
# SPDX-License-Identifier: BSD-3-Clause
#=============================================================================

find_package(PkgConfig QUIET)
pkg_check_modules(PC_libimobiledevice QUIET libimobiledevice)

find_path(IMobileDevice_INCLUDE_DIRS NAMES libimobiledevice/libimobiledevice.h HINTS ${PC_libimobiledevice_INCLUDE_DIRS})
find_library(IMobileDevice_LIBRARIES NAMES imobiledevice HINTS ${PC_libimobiledevice_LIBRARY_DIRS})

set(IMobileDevice_VERSION ${PC_libimobiledevice_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IMobileDevice
    FOUND_VAR IMobileDevice_FOUND
    REQUIRED_VARS IMobileDevice_INCLUDE_DIRS IMobileDevice_LIBRARIES
    VERSION_VAR IMobileDevice_VERSION
)

mark_as_advanced(IMobileDevice_INCLUDE_DIRS IMobileDevice_LIBRARIES)

if(IMobileDevice_FOUND AND NOT TARGET IMobileDevice::IMobileDevice)
    add_library(IMobileDevice::IMobileDevice UNKNOWN IMPORTED)
    set_target_properties(IMobileDevice::IMobileDevice PROPERTIES
        IMPORTED_LOCATION "${IMobileDevice_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${IMobileDevice_INCLUDE_DIRS}"
        INTERFACE_COMPILE_DEFINITIONS "${PC_libimobiledevice_CFLAGS_OTHER}"
    )
endif()

include(FeatureSummary)
set_package_properties(IMobileDevice PROPERTIES
    DESCRIPTION "library to communicate with iOS devices"
    URL "https://www.libimobiledevice.org/"
)
