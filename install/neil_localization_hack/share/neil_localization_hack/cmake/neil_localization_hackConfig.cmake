# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_neil_localization_hack_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED neil_localization_hack_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(neil_localization_hack_FOUND FALSE)
  elseif(NOT neil_localization_hack_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(neil_localization_hack_FOUND FALSE)
  endif()
  return()
endif()
set(_neil_localization_hack_CONFIG_INCLUDED TRUE)

# output package information
if(NOT neil_localization_hack_FIND_QUIETLY)
  message(STATUS "Found neil_localization_hack: 0.0.0 (${neil_localization_hack_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'neil_localization_hack' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${neil_localization_hack_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(neil_localization_hack_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${neil_localization_hack_DIR}/${_extra}")
endforeach()
