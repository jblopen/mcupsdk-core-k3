
const staticConfig =
{
    baseAddr: 0x80000000,
    size: 0x100000000,
    sdramIdx: Math.log2(0x100000000) - 16,
    sdramIdxMax: 0x13,
    clockIds: [ "TISCI_DEV_DDR32SS0", "TISCI_DEV_EMIF_DATA_ISO_VD" ],
};

let defaultDdrConfigFile = "drivers/ddr/v1/soc/am62dx/board_ddrReginit.h"

function getDefaultDdrConfigFileName() {
    return defaultDdrConfigFile;
}

function getStaticConfig()
{
    return staticConfig;
}

exports = {
    getDefaultDdrConfigFileName,
    getStaticConfig,
};
