IMAGE_FEATURES += "ssh-server-openssh"
IMAGE_FSTYPES:append = "${@' tar.gz' if d.getVar('LAYERDIR_meta-swupdate') else ''}"

LICENSE = "MIT"

inherit core-image

CORE_IMAGE_BASE_INSTALL += "packagegroup-demo-base packagegroup-demo-basetests"
CORE_IMAGE_BASE_INSTALL += "${@'packagegroup-demo-systemd' if d.getVar('VIRTUAL-RUNTIME_init_manager') == 'systemd' else ''}"
TOOLCHAIN_HOST_TASK += "nativesdk-packagegroup-cuda-sdk-host"

inherit nopackages l4t_version

configure_issue () {
    cat > ${IMAGE_ROOTFS}${sysconfdir}/issue << ISSUE_EOF
\e{bold}\e{green}\S{NAME}\e{reset}

  \e{cyan}Hostname\e{reset}  :  \e{bold}\n\e{reset}
  \e{cyan}Kernel\e{reset}    :  \s \r
  \e{cyan}L4T\e{reset}       :  R${L4T_VERSION}
  \e{cyan}Version\e{reset}   :  \S{VERSION}
  \e{cyan}eth0\e{reset}      :  \4{eth0}
  \e{cyan}wlan0\e{reset}     :  \4{wlan0}

\e{green}Image '${PN}' ready on \l\e{reset}

ISSUE_EOF
}
ROOTFS_POSTPROCESS_COMMAND += "configure_issue"
