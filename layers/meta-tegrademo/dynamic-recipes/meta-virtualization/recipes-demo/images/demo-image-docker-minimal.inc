REQUIRED_DISTRO_FEATURES:append = " virtualization"
IMAGE_FEATURES += "container-registry"
CORE_IMAGE_BASE_INSTALL += "docker nvidia-container-toolkit docker-registry-config"
