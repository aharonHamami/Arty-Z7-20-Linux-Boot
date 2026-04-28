#
# This file is the rs232-c recipe.
#

SUMMARY = "Simple rs232-c application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://."

inherit systemd

# service configuration
SERVICE_FILE="rs232.service"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "${SERVICE_FILE}"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

S = "${WORKDIR}"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 rs232-c ${D}${bindir}

		 # copy app's .service file to /lib/systemd
         install -d ${D}${systemd_system_unitdir}
         install -m 0644 ${WORKDIR}/${SERVICE_FILE} ${D}${systemd_system_unitdir}
}
