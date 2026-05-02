TEGRA_SIGNING_EXCLUDE_TOOLS:secureboot = "1"
TEGRA_SIGNING_EXTRA_DEPS:secureboot = "${DIGSIG_DEPS} gzip-native:do_populate_sysroot"

TEGRASIGN_EMMC_BCTS = "${EMMC_BCT}${@',' + d.getVar('EMMC_BCT_OVERRIDE') if d.getVar('EMMC_BCT_OVERRIDE') else ''}"
TEGRASIGN_EMMC_BCTS:tegra210 = "${MACHINE}.cfg"

TEGRASIGN_FLASHTOOLS_DIR = "${SOC_FAMILY}-flash"
TEGRASIGN_FLASHTOOLS_DIR:tegra194 = "tegra186-flash"

inherit signing_server l4t_bsp

tegrasign_create_manifest() {
    cat >MANIFEST <<EOF
DTBFILE=${DTBFILE}
ODMDATA=${ODMDATA}
LNXFILE=${LNXFILE}
BOARDID=${TEGRA_BOARDID}
FAB=${TEGRA_FAB}
fuselevel=fuselevel_production
localbootfile=${LNXFILE}
CHIPREV=${TEGRA_CHIPREV}
BOARDSKU=${TEGRA_BOARDSKU}
BOARDREV=${TEGRA_BOARDREV}
EMMC_BCTS=${TEGRASIGN_EMMC_BCTS}
EOF
}

tegraflash_custom_sign_pkg:secureboot() {
    local tarextra mvextra
    tegrasign_create_manifest
    if [ -n "${DATAFILE}" ]; then
        if [ -e "${DATAFILE}" ]; then
            tarextra="--exclude=${DATAFILE}"
            mvextra="${DATAFILE}.img"
	fi
        touch DATAFILE
    fi
    tar -c -h -z -f ${WORKDIR}/tegrasign-in.tar.gz --exclude=${IMAGE_BASENAME}.img --exclude=${IMAGE_BASENAME}.ext4 $tarextra *
    digsig_post sign/tegra -F "machine=${DIGSIG_MACHINE}" -F "soctype=${SOC_FAMILY}" -F "bspversion=${L4T_VERSION}" -F "artifact=@${WORKDIR}/tegrasign-in.tar.gz" --output ${WORKDIR}/tegrasign-out.tar.gz
    tar -x -z -f ${WORKDIR}/tegrasign-out.tar.gz
    [ "${TEGRA_SIGNING_EXCLUDE_TOOLS}" != "1" ] || cp -R ${STAGING_BINDIR_NATIVE}/${FLASHTOOLS_DIR}/* .
    rm doflash.sh
    mv flashcmd.txt doflash.sh
    chmod +x doflash.sh
    rm -f secureflash.sh
    tegraflash_post_sign_pkg
}

tegraflash_custom_sign_bup:secureboot() {
    tegrasign_create_manifest
    echo "BUPGENSPECS=${TEGRA_BUPGEN_SPECS}" >>MANIFEST
    tar -c -h -z -f ${WORKDIR}/tegrasign-bupgen-in.tar.gz *
    digsig_post sign/tegra -F "machine=${DIGSIG_MACHINE}" -F "soctype=${SOC_FAMILY}" -F "bspversion=${L4T_VERSION}" -F "artifact=@${WORKDIR}/tegrasign-bupgen-in.tar.gz" --output ${WORKDIR}/tegrasign-bupgen-out.tar.gz
    tar -x -z -f ${WORKDIR}/tegrasign-bupgen-out.tar.gz
}

do_image_tegraflash[network] = "1"
do_deploy[network] = "1"
