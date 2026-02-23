FILESEXTRAPATHS:prepend := "${THISDIR}/${BPN}:"

# Patches for:
#  0001: only writing changed partitions to SPI flash in capsule
#        updates
#  0002: allow for capsule updates when using CONFIG_SINGLE_BOOT_L4T_LAUNCHER,
#        which embeds L4TLauncher in the UEFI image in SPI flash
SRC_URI += "\
    file://0001-TegraFmp-write-only-changed-partitions-during-fw-upd.patch;patchdir=../edk2-nvidia \
    file://0002-feat-capsule-updates-with-single-boot-configuration.patch;patchdir=../edk2-nvidia \
"

# Set EDK2_VERBOSE_LOGGING = "1" in your local.conf to get more logs
# out of UEFI during boot
EDK2_VERBOSE_LOGGING ??= "0"
DEBUGPRINTCFG = "${@'file://debugprint.cfg' if bb.utils.to_boolean(d.getVar('EDK2_VERBOSE_LOGGING')) else ''}"

# Stripped-down configurations that:
#  - include minimal set of drivers needed for booting
#  - disable the video display and boot logo
#  - disable the boot menu and shell
#  - disable Android-style booting
#  - configure L4T Launcher as the boot application, embedding it in
#    the UEFI image
def omit_nonboot_drivers(d):
    # For targets with no on-board eMMC or SDcard slot, or where the
    # boot device is explicitly set to NVMe, disable the eMMC and
    # SDcard drivers.
    if bb.utils.to_boolean(d.getVar('TEGRAFLASH_NO_INTERNAL_STORAGE') or '0') or d.getVar('TNSPEC_BOOTDEV').startswith('nvme'):
        return "file://disable-emmc-sdcard.cfg"
    # If the boot device is eMMC or SDcard, disable NVMe (and PCIe)
    if d.getVar('TNSPEC_BOOTDEV').startswith('mmcblk'):
        return "file://disable-nvme.cfg"
    return ""

SRC_URI += "\
    file://disable-unused-hardware.cfg \
    ${@omit_nonboot_drivers(d)} \
    file://disable-unused-features.cfg \
    ${DEBUGPRINTCFG} \
"

# The Kconfig/Kbuild files NVIDIA provides don't get the logic for
# setting the default boot timeout quite right, so just hack in a
# hard-coded zero.
fix_boot_timeout() {
    sed -i -e's,\$(CONFIG_BOOT_DEFAULT_TIMEOUT),0,' ${S}/../edk2-nvidia/Platform/NVIDIA/NVIDIA.common.dsc.inc
}
do_patch[postfuncs] += "fix_boot_timeout"

# Since L4T Launcher is embedded in the UEFI image, we don't install
# it into the ESP partition. The ESP is used only to store update capsules.
do_install:append() {
    find ${D}${EFIDIR} -type f -delete || true
}
