# Disable the Getty service from running (to free up the serial port for other applications that use it)

ROOTFS_POSTPROCESS_COMMAND += "disable_serial_getty; "

disable_serial_getty() {
    ln -sf /dev/null ${IMAGE_ROOTFS}${systemd_system_unitdir}/serial-getty@ttyPS0.service
}
