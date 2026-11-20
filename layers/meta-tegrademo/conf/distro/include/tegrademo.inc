DISTRO = "tegrademo"
DISTRO_NAME = "OE4Tegra Demonstration Distro"
DISTRO_VERSION_BASE = "6.2"
DISTRO_VERSION = "${DISTRO_VERSION_BASE}+snapshot-${METADATA_REVISION}"
DISTRO_CODENAME = "master"
SDK_VENDOR = "-tdsdk"
SDK_VERSION = "${@d.getVar('DISTRO_VERSION').replace('snapshot-${METADATA_REVISION}', 'snapshot')}"
SDK_VERSION[vardepvalue] = "${SDK_VERSION}"

MAINTAINER = "OE4Tegra team <oe4tegra@madison.systems>"

TARGET_VENDOR = "-oe4t"

# New ${DISTRO}-<version> setting for sanity checks.
# Increment version number (and the corresponding
# setting int the template bblayers.conf.sample file)
# each time the layer settings are changed.
REQUIRED_TD_BBLAYERS_CONF_VERSION = "${DISTRO}-7"

LOCALCONF_VERSION = "2"

INHERIT += "tegra-support-sanity"
ESDK_CLASS_INHERIT_DISABLE:append = " tegra-support-sanity"

BB_SIGNATURE_HANDLER ?= "OEEquivHash"
BB_HASHSERVE ??= "auto"

CONTAINER_PROFILE ?= "default"
require conf/distro/include/meta-virt-container-${CONTAINER_PROFILE}.inc

TD_DEFAULT_DISTRO_FEATURES = "pam virtualization glvnd"
DISTRO_FEATURES ?= "${TD_DEFAULT_DISTRO_FEATURES}"
DISTRO_FEATURES_NATIVE += "opengl glvnd wayland x11"

TEGRA_DEFAULT_KERNEL ?= "linux-noble-nvidia-tegra"
PREFERRED_PROVIDER_virtual/kernel:tegra = "${TEGRA_DEFAULT_KERNEL}"
PREFERRED_VERSION_linux-yocto ?= "6.18%"
PREFERRED_VERSION_linux-yocto-rt ?= "6.18%"

SDK_NAME = "${DISTRO}-${TCLIBC}-${SDKMACHINE}-${IMAGE_BASENAME}-${TUNE_PKGARCH}-${MACHINE}"
SDKPATHINSTALL = "/opt/${DISTRO}/${SDK_VERSION}"

PACKAGE_CLASSES ?= "package_rpm"

SANITY_TESTED_DISTROS ?= " \
            ubuntu-22.04 \n \
            ubuntu-24.04 \n \
            ubuntu-26.04 \n \
            "

# Most NVIDIA-supplied services expect systemd
INIT_MANAGER = "systemd"

LICENSE_FLAGS_ACCEPTED += "commercial_faad2 commercial_x264 commercial_gstreamer1.0-libav commercial_ffmpeg"

USE_REDUNDANT_FLASH_LAYOUT_DEFAULT ?= "1"

# for docker-registry-config
DOCKER_OCI_RUNTIMES ?= '{"nvidia":{"args":[],"path":"nvidia-container-runtime"}}'
DOCKER_DEFAULT_RUNTIME ?= "nvidia"

require ${@'dynamic-layers/meta-swupdate/conf/distro/include/tegra-swupdate.inc' if d.getVar('LAYERDIR_meta-swupdate') else ''}
