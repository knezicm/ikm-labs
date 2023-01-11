PACKAGECONFIG:append = " networkd resolved"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://10-eth0.network"

FILES:${PN} += "${sysconfdir}/systemd/network/10-eth0.network"

do_install:append() {
    install -d ${D}${sysconfdir}/systemd/network
    install -m 0644 ${WORKDIR}/10-eth0.network ${D}${sysconfdir}/systemd/network
}
