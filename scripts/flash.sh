#!/usr/bin/env bash

set -euo pipefail

usage() {
    cat <<'USAGE'
Usage:
  scripts/flash.sh <openocd-config> <firmware.elf>

Example:
  scripts/flash.sh \
    platform/stm32f446ze/openocd.cfg \
    build/experiments/001_gpio_output/exp001_gpio_output.elf
USAGE
}

if [[ $# -ne 2 ]]; then
    usage >&2
    exit 2
fi

OPENOCD_CONFIG="$1"
ELF_FILE="$2"

if ! command -v openocd >/dev/null 2>&1; then
    echo "error: openocd not found in PATH" >&2
    exit 1
fi

if [[ ! -f "$OPENOCD_CONFIG" ]]; then
    echo "error: OpenOCD config not found: $OPENOCD_CONFIG" >&2
    exit 1
fi

if [[ ! -f "$ELF_FILE" ]]; then
    echo "error: ELF file not found: $ELF_FILE" >&2
    exit 1
fi

echo "[flash] OpenOCD config: $OPENOCD_CONFIG"
echo "[flash] ELF:            $ELF_FILE"

openocd \
    -f "$OPENOCD_CONFIG" \
    -c "program {$ELF_FILE} verify reset exit"
