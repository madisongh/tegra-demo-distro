# tegra-demo-distro fork for demonstrating A/B OTA update improvements

This repo is **not** what you want to clone if you are looking for the
OE4T Project's reference distro. For that, please visit https://github.com/OE4T/tegra-demo-distro .

Metadata layers are brought in as git submodules:

| Layer Repo            | Branch         | Description                                         |
| --------------------- | ---------------|---------------------------------------------------- |
| poky                  | scarthgap      | OE-Core from poky repo at yoctoproject.org          |
| meta-tegra            | scarthgap      | L4T BSP layer - L4T R36.5.0/JetPack 6.2.2           |
| meta-tegra-community  | scarthgap      | OE4T layer with additions from the community        |
| meta-openembedded     | scarthgap      | OpenEmbedded layers                                 |
| meta-swupdate         | scarthgap      | swupdate layer                                      |
| meta-virtualization   | scarthgap      | Virtualization layer for docker support             |

## Changes from the stock OE4T demo distro

1. The meta-swupdate layer is included, for testing OTA updates.

2. UEFI patches are applied, see description below.

3. The distro config sets `UBOOT_EXTLINUX_FDT` to place the device tree in `/boot`,
   and adds a boot order overlay to `TEGRA_BOOTCONTROL_OVERLAYS` to tell UEFI which
   device to boot from (currently needed in conjunction with the applied patches).

4. The demo images include `swupdate`, and create `tar.gz` tarballs for forming swupdate
   packages.

## UEFI patches

The patches applied to UEFI enable cleaner A/B OTA updates by building the L4TLauncher EFI application into the UEFI image.
This eliminates the need to update the ESP at all during OTA updates, eliminating a possible failure/bricking incident if
power is lost during an update due to lack of ESP redundancy. The ESP is still made available for locating update capsules.

The patches also improve capsule update speeds, and reduce wear on the QSPI flash, by only applying updates to flash
partitions that are changed by the capsule update.

To use these patches, you *must* (currently) use a boot order DTB overlay to tell UEFI which device you are booting from.
They also restrict you to using the L4TLauncher EFI application.

The modified UEFI configuration here is patched to further optimize boot time by eliminating unneeded features and
drivers. It set up to use L4TLauncher's extlinux-like boot support **only**, since the intent is to simplify the
flash layout to remove the Android kernel and DTB partitions.

## Testing

Testing so far has only been with `MACHINE="jetson-orin-nano-devkit-nvme"`.

## Future work

1. Custom, simplified flash layouts.

2. Test other machines.

3. Rework flash layouts to separate the `/boot` partition from the main rootfs. This will allow for
   having encrypted rootfs setups, and will also let us format the `/boot` file system in a way that
   UEFI's ext4 filesystem driver will always understand it, even if the Linux kernel starts adding ext4
   features in a way that the UEFI driver can't deal with.

4. If possible, migrate to other R36.4.4-based branches for further testing.

There are some other possible examples around LUKS and DM-Verity setups that I have in mind.

