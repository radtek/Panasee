# Install script for directory: E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "E:/DcmTK/dcmtk-3.5.4/../dcmtk-3.5.4-win32-i386")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/dcmtk/dcmdata" TYPE FILE FILES
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/cmdlnarg.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcbytstr.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcchrstr.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dccodec.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcdatset.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcddirif.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcdebug.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcdefine.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcdeftag.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcdicdir.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcdicent.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcdict.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcdirrec.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcelem.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcerror.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcfilefo.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dchashdi.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcistrma.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcistrmb.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcistrmf.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcistrmz.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcitem.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dclist.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcmetinf.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcobject.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcofsetl.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcostrma.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcostrmb.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcostrmf.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcostrmz.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcovlay.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcpcache.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcpixel.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcpixseq.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcpxitem.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcrleccd.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcrlecce.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcrlecp.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcrledec.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcrledrg.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcrleenc.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcrleerg.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcrlerp.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcsequen.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcstack.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcswap.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dctag.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dctagkey.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dctk.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dctypes.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcuid.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvm.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvr.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrae.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvras.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrat.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrcs.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrda.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrds.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrdt.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrfd.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrfl.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvris.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrlo.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrlt.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrobow.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrof.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrpn.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrpobw.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrsh.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrsl.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrss.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrst.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrtm.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrui.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrul.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrulup.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrus.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcvrut.h"
    "E:/DcmTK/dcmtk-3.5.4/dcmdata/include/dcmtk/dcmdata/dcxfer.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

