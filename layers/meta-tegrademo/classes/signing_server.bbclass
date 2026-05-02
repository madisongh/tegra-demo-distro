DIGSIG_SERVER ??= "http://127.0.0.1:9999"
DIGSIG_SERVER[vardepvalue] = ""
DIGSIG_DEPS = "curl-native:do_populate_sysroot"
DIGSIG_MACHINE ?= "${MACHINE}"

# Set some reasonably long timeout and retry values, in case
# the signing server is handling multiple requests in parallel.
DIGSIG_POST_ARGS ??= "--connect-timeout 30 --max-time 1800 --retry 4"
DIGSIG_POST_ARGS[vardepvalue] = ""

digsig_post() {
    local endpoint="$1"
    shift
    [ -n "${DIGSIG_SERVER}" -a -n "$endpoint" ] || exit 1
    curl ${DIGSIG_POST_ARGS} --silent --show-error --fail -X POST "$@" "${DIGSIG_SERVER}/$endpoint"
}
