# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

## Configures include paths from all targets within the given dir (and
## subdirectories) as SYSTEM (external) headers, so that warnings should not be
## reported for them.
function(simple_tr8n_mark_dir_targets_as_external dir)
  simple_tr8n_get_all_dir_targets("${dir}" targets)

  foreach(target ${targets})
    get_target_property(targetType ${target} TYPE)

    if (NOT (${targetType} STREQUAL "INTERFACE_LIBRARY"))
      get_target_property(includeDirs
          ${target} INCLUDE_DIRECTORIES)
      set_target_properties(${target} PROPERTIES
          SYSTEM_INCLUDE_DIRECTORIES "${includeDirs}")
    endif()

    get_target_property(interfaceIncludeDirs
        ${target} INTERFACE_INCLUDE_DIRECTORIES)
    set_target_properties(${target} PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${interfaceIncludeDirs}")
  endforeach()
endfunction()

## Outputs all CMake targets within the given dir (and subdirectories) to outVar.
##
## Implementation based on
## https://newbedev.com/how-do-i-iterate-over-all-cmake-targets-programmatically.
function(simple_tr8n_get_all_dir_targets dir outVar)
  set(targets)
  simple_tr8n_get_all_dir_targets_recursive("${dir}" targets)

  set(${outVar} ${targets} PARENT_SCOPE)
endfunction()

macro(simple_tr8n_get_all_dir_targets_recursive dir targets)
  get_property(subDirs
      DIRECTORY "${dir}"
      PROPERTY SUBDIRECTORIES)
  foreach(subDir ${subDirs})
    simple_tr8n_get_all_dir_targets_recursive(${subDir} ${targets})
  endforeach()

  get_property(currentDirTargets
      DIRECTORY "${dir}"
      PROPERTY BUILDSYSTEM_TARGETS)
  list(APPEND ${targets} ${currentDirTargets})
endmacro()
