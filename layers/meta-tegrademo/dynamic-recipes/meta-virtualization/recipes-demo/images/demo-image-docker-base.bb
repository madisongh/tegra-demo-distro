SUMMARY = "Tegra demo base image with docker"
DESCRIPTION = """A base image with docker, suitable for running basic docker container GPU passthrough tests. \
For a full featured image with all possible pass through library files included, use demo-image-full instead."""

require recipes-demo/images/demo-image-common.inc
require demo-image-docker-minimal.inc
