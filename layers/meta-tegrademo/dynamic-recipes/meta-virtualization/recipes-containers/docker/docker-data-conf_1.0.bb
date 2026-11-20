DESCRIPTION = "Redirect Docker data root to /data/docker (requires data-partition)"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://docker-data-root.conf"

S = "${UNPACKDIR}"

inherit systemd

do_install() {
    install -d ${D}${systemd_system_unitdir}/docker.service.d
    install -m 0644 ${S}/docker-data-root.conf ${D}${systemd_system_unitdir}/docker.service.d/
}

FILES:${PN} = "${systemd_system_unitdir}/docker.service.d/docker-data-root.conf"

RDEPENDS:${PN} = "docker data-partition"
PACKAGE_ARCH = "${MACHINE_ARCH}"
