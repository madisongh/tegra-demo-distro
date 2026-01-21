DISTRO = "tegrademo"
DISTRO_NAME = "OE4Tegra Demonstration Distro"
DISTRO_VERSION_BASE = "5.0.18"
DISTRO_VERSION = "${DISTRO_VERSION_BASE}+snapshot-${METADATA_REVISION}"
DISTRO_CODENAME = "scarthgap"
SDK_VENDOR = "-tdsdk"
SDK_VERSION := "${@'${DISTRO_VERSION}'.replace('snapshot-${METADATA_REVISION}','snapshot')}"
SDK_VERSION[vardepvalue] = "${SDK_VERSION}"

MAINTAINER = "OE4Tegra team <oe4tegra@madison.systems>"

TARGET_VENDOR = "-oe4t"

# New ${DISTRO}-<version> setting for sanity checks.
# Increment version number (and the corresponding
# setting int the template bblayers.conf.sample file)
# each time the layer settings are changed.
REQUIRED_TD_BBLAYERS_CONF_VERSION = "${DISTRO}-5-alt"

LOCALCONF_VERSION = "2"

TD_DEFAULT_DISTRO_FEATURES = "largefile opengl ptest multiarch wayland vulkan systemd pam virtualization usrmerge"

DISTRO_FEATURES ?= "${DISTRO_FEATURES_DEFAULT} ${TD_DEFAULT_DISTRO_FEATURES}"

PREFERRED_VERSION_linux-yocto ?= "6.6%"
PREFERRED_VERSION_linux-yocto-rt ?= "6.6%"

SDK_NAME = "${DISTRO}-${TCLIBC}-${SDKMACHINE}-${IMAGE_BASENAME}-${TUNE_PKGARCH}-${MACHINE}"
SDKPATHINSTALL = "/opt/${DISTRO}/${SDK_VERSION}"

TCLIBCAPPEND = ""

PACKAGE_CLASSES ?= "package_rpm"

SANITY_TESTED_DISTROS ?= " \
            ubuntu-20.04 \n \
            ubuntu-22.04 \n \
            ubuntu-24.04 \n \
            "

# Most NVIDIA-supplied services expect systemd
INIT_MANAGER = "systemd"

require conf/distro/include/no-static-libs.inc
require conf/distro/include/yocto-uninative.inc
require conf/distro/include/security_flags.inc
INHERIT += "uninative"

LICENSE_FLAGS_ACCEPTED += "commercial_faad2 commercial_x264"

USE_REDUNDANT_FLASH_LAYOUT_DEFAULT ?= "1"
# Default to putting the device tree in /boot
UBOOT_EXTLINUX_FDT = "${DTBFILE}"
BOOTORDER_INTERNAL = "BootOrderEmmc.dtbo"
BOOTORDER_INTERNAL:orin-nano = "BootOrderSD.dtbo"
TEGRA_BOOTORDER_SETTING = "${@'BootOrderNvme.dtbo' if (d.getVar('TNSPEC_BOOTDEV') or '').startswith('nvme') else '${BOOTORDER_INTERNAL}'}"
TEGRA_BOOTCONTROL_OVERLAYS:append = " ${TEGRA_BOOTORDER_SETTING}"
