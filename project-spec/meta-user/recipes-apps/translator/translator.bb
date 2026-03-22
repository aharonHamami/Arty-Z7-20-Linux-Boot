#
# This file is the translator recipe.
#

SUMMARY = "Simple UART to Ethernet parser"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

#
# Debug Variables:
#
DEBUG_FLAGS = "-g3 -O0"
# Specifies to build packages with debugging information
DEBUG_BUILD = "1"
# Do not remove debug symbols
INHIBIT_PACKAGE_STRIP = "1"

SRC_URI += "file://."

S = "${WORKDIR}"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 translator ${D}${bindir}
}
