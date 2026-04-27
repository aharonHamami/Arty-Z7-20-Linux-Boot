#
# This file is the python-rs232 recipe.
#

SUMMARY = "Simple python-rs232 application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://."

S = "${WORKDIR}"

# Enable packaging from python
inherit setuptools3

RDEPENDS:${PN} += "python3-core"

# No need for "do_install". setuptools3 does it for you :D
