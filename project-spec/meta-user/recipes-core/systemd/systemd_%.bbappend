do_install:append() {
    install -d ${D}${sysconfdir}/systemd/system/
    ln -s /dev/null ${D}${sysconfdir}/systemd/system/serial-getty@ttyPS0.service
}
