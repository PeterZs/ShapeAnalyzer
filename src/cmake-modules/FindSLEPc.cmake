# -*- mode: cmake -*-
#
#  This file is part of the Feel library
#
#  Author(s): Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr>
#       Date: 2010-01-22
#
#  Copyright (C) 2010 Universit� Joseph Fourier
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 3.0 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

INCLUDE(CheckIncludeFile)

foreach( debian_arches linux kfreebsd )
  IF ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    set( DEBIAN_FLAVORS ${debian_arches}-gnu-c-debug ${debian_arches}-gnu-c-opt ${DEBIAN_FLAVORS})
  ELSE()
    set( DEBIAN_FLAVORS ${debian_arches}-gnu-c-opt ${debian_arches}-gnu-c-debug ${DEBIAN_FLAVORS})
  ENDIF()
endforeach()


find_path (SLEPC_DIR include/slepc.h
  HINTS ENV SLEPC_DIR
  PATHS
  /usr/lib/slepcdir/3.2 # Debian
  /usr/lib/slepcdir/3.1 # Debian
  /usr/lib/slepcdir/3.0.0 # Debian
  /opt/local/lib/petsc # macports
  $ENV{HOME}/slepc
  DOC "SLEPc Directory")


SET(SLEPC_INCLUDE_DIR "${SLEPC_DIR}/include/")
CHECK_INCLUDE_FILE( ${SLEPC_INCLUDE_DIR}slepc.h HAS_SLEPC_H )

if (SLEPC_DIR AND NOT PETSC_ARCH)
  set (_slepc_arches
    $ENV{PETSC_ARCH}                   # If set, use environment variable first
    ${DEBIAN_FLAVORS}  # Debian defaults
    x86_64-unknown-linux-gnu i386-unknown-linux-gnu)
  set (slepcconf "NOTFOUND" CACHE FILEPATH "Cleared" FORCE)
  foreach (arch ${_slepc_arches})
    if (NOT PETSC_ARCH)
      find_path (slepcconf slepcconf.h
	HINTS ${SLEPC_DIR}
	PATH_SUFFIXES ${arch}/include bmake/${arch}
	NO_DEFAULT_PATH)
      if (slepcconf)
	set (PETSC_ARCH "${arch}" CACHE STRING "PETSc build architecture")
      endif (slepcconf)
    endif (NOT PETSC_ARCH)
  endforeach (arch)
  set (slepcconf "NOTFOUND" CACHE INTERNAL "Scratch variable" FORCE)
endif (SLEPC_DIR AND NOT PETSC_ARCH)
message(STATUS "SLEPc arch: ${PETSC_ARCH}")

CHECK_INCLUDE_FILE( ${SLEPC_DIR}/$ENV{PETSC_ARCH}/include/slepcconf.h HAS_SLEPCCONF_H )
if (HAS_SLEPCCONF_H)
  SET(SLEPC_INCLUDE_DIR ${SLEPC_DIR}/${PETSC_ARCH}/include ${SLEPC_INCLUDE_DIR})
endif()
FIND_LIBRARY(SLEPC_LIB_SLEPC slepc
  HINTS
  ${SLEPC_DIR}/${PETSC_ARCH}/lib
  ${SLEPC_DIR}/lib )
SET(SLEPC_LIBRARIES ${SLEPC_LIB_SLEPC} CACHE STRING "SLEPc libraries" FORCE)
# handle the QUIETLY and REQUIRED arguments and set Madlib_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS (SLEPC DEFAULT_MSG
  SLEPC_DIR SLEPC_LIBRARIES
  )

if ( SLEPC_FOUND )
  set(HAS_SLEPC 1)
  set(SLEPC_INCLUDES ${SLEPC_INCLUDE_DIR} CACHE STRING "SLEPc include path" FORCE)
endif()

MARK_AS_ADVANCED( SLEPC_DIR SLEPC_LIB_SLEPC SLEPC_INCLUDES SLEPC_LIBRARIES )
