#
# This file is the argos-server recipe.
#

SUMMARY = "Simple argos-server application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://."

# Add libwebsockets library support to the application
IMAGE_INSTALL:append = " libwebsockets libwebsockets-dev"

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/${bindir}
	     install -m 0755 ${S}/argos-server ${D}/${bindir}
}
