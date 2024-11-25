let path = require('path');
const _ = require('lodash');

const files = {
    common: [
        "unibase.c"           ,
        "ub_logging.c"        ,
        "ub_esarray.c"        ,
        "ub_strutils.c"       ,
        "ub_confutils.c"      ,
        "ub_llist.c"          ,
        "ub_fileio.c"         ,
        "ub_getmem.c"         ,
        "getopt.c"         ,
    ],
};
const filedirs = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_unibase",
    ],
};

const includes = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/board/ethphy/enet/rtos_drivers/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_unibase",

    ],
};

const includes_a53 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/board/ethphy/enet/rtos_drivers/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/GCC/ARM_CA53",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_unibase",

    ],
};


const defines = {
    common: [
        'TSNPKGVERSION=\\"1.1.4\\"',
        'PRINT_FORMAT_NO_WARNING',
        'NO_GETOPT_LONG=1'
    ],
};

const defines_a53 = {
    common: [
        'TSNPKGVERSION=\\"1.1.4\\"',
        'PRINT_FORMAT_NO_WARNING',
        'NO_GETOPT_LONG=1'
    ],
};

const soc_defines = {
    am62x : [
    ],
    am243x : [
    ],
    am64x : [
    ],
    am62ax : [
    ],
    am62px : [
    ],
    am62dx : [
        "SOC_AM62DX",
    ],
    am263x : [
    ],
    am263px : [
    ],
    am273x : [
    ],
    awr294x : [
    ],
};

const deviceSpecificIncludes = {
    am243x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am243x/r5f",
    ],
    am64x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am64x/r5f",
    ],
    am62ax : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62ax/r5f",
    ],
    am62px : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62px/r5f",
    ],
    am62dx : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62dx/r5f",
    ],
    am263x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am263x/r5f",
    ],
    am273x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am273x/r5f",
    ],
    awr294x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/awr294x/r5f",
    ],
};

const deviceSpecificIncludes_a53 = {
    am62x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62x/a53",
    ],
};


const cflags = {
    common: [
        "-Wno-extra",
        "-Wvisibility",
        "--include tsn_buildconf/sitara_buildconf.h",
    ],
    release: [
        "-Oz",
        "-flto",
    ],
};

const cflags_a53 = {
    common: [
        "-Wno-extra",
        "--include tsn_buildconf/sitara_buildconf.h",
    ],
    release: [
        "-flto",
    ],
};

const deviceSpecific_cflags = {
     am243x : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am64x : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am62ax : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am62px : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am62dx : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am263x : [
    ],
    am273x : [
        "-fno-strict-aliasing",
    ],
    awr294x : [
        "-fno-strict-aliasing",
    ],
};

const deviceSpecific_cflags_a53 = {
    am62x : [
        "-fno-strict-aliasing",
    ],
};

const buildOptionCombos = [
    { device: "am263x", cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am243x", cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am273x", cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am64x",  cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "awr294x", cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am62ax",  cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am62px",  cpu: "wkup-r5f", cgt: "ti-arm-clang"},
    { device: "am62dx",  cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am62x", cpu: "a53", cgt: "gcc-aarch64"},
];

function getComponentProperty(device) {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "library";
    property.name = "tsn_unibase-freertos";
    property.tag  = "tsn_unibase_freertos";
    if (device === "am62ax")
    {
        property.isInternal = true;
    }
    else
    {
        property.isInternal = false;
    }

    deviceBuildCombos = []
    for (buildCombo of buildOptionCombos)
    {
        if (buildCombo.device === device)
        {
            deviceBuildCombos.push(buildCombo)
        }
    }
    property.buildOptionCombos = deviceBuildCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    build_property.files = files;
    build_property.filedirs = filedirs;

    if(buildOption.cpu.match(/a53*/)){
        includes_a53.common = _.union(includes_a53.common, deviceSpecificIncludes_a53[device]);
        build_property.includes = includes_a53;

        defines_a53.common = _.union(defines_a53.common, soc_defines[device])
        build_property.defines = defines_a53;

        cflags_a53.common = _.union(cflags_a53.common, deviceSpecific_cflags_a53[device]);
        build_property.cflags = cflags_a53;

        return build_property;

    }

    includes.common = _.union(includes.common, deviceSpecificIncludes[device]);
    build_property.includes = includes;

    defines.common = _.union(defines.common, soc_defines[device])
    build_property.defines = defines;

    cflags.common = _.union(cflags.common, deviceSpecific_cflags[device]);
    build_property.cflags = cflags;

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
