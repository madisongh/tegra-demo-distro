DESCRIPTION = "Redirect containerd data root to /data/containerd (requires data-partition)"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://config.toml \
    file://containerd-data-root.conf \
"

S = "${UNPACKDIR}"

inherit systemd

do_install() {
    install -d ${D}${sysconfdir}/containerd
    install -m 0644 ${S}/config.toml ${D}${sysconfdir}/containerd/

    install -d ${D}${systemd_system_unitdir}/containerd.service.d
    install -m 0644 ${S}/containerd-data-root.conf ${D}${systemd_system_unitdir}/containerd.service.d/
}

FILES:${PN} = " \
    ${sysconfdir}/containerd/config.toml \
    ${systemd_system_unitdir}/containerd.service.d/containerd-data-root.conf \
"

RDEPENDS:${PN} = "containerd data-partition"
PACKAGE_ARCH = "${MACHINE_ARCH}"
