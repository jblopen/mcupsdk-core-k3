let path = require('path');

let device = "am62x";

const files = {
    common: [
        "test_mmcsd.c",
        "main.c",
    ],
};

const defines_dm_r5 = {
    common: [
        "ENABLE_SCICLIENT_DIRECT",
    ],
}

/* Relative to where the makefile will be generated
 * Typically at <example_folder>/<BOARD>/<core_os_combo>/<compiler>
 */
const filedirs = {
    common: [
        "..",       /* core_os_combo base */
        "../..",    /* Board base */
        "../../..", /* Example base */
    ],
};

const libdirs = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/lib",
        "${MCU_PLUS_SDK_PATH}/test/unity/lib",
    ],
};

const libdirs_dm_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/lib",
        "${MCU_PLUS_SDK_PATH}/test/unity/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/sciserver/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/sciclient_direct/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/self_reset/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/dm_stub/lib",
    ],
};

const libdirs_freertos_a53 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/lib",
        "${MCU_PLUS_SDK_PATH}/test/unity/lib",
    ],
};

const includes_r5f = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62x/r5f",
        "${MCU_PLUS_SDK_PATH}/test/unity/",
    ],
};

const includes_freertos_a53 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/GCC/ARM_CA53",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62x/a53",
        "${MCU_PLUS_SDK_PATH}/test/unity",
    ],
};

const libs_dm_r5f = {
    common: [
        "rm_pm_hal.am62x.r5f.ti-arm-clang.${ConfigName}.lib",
        "sciclient_direct.am62x.r5f.ti-arm-clang.${ConfigName}.lib",
        "self_reset.am62x.r5f.ti-arm-clang.${ConfigName}.lib",
        "sciserver.am62x.r5f.ti-arm-clang.${ConfigName}.lib",
        "dm_stub.am62x.r5f.ti-arm-clang.${ConfigName}.lib",
        "freertos.am62x.r5f.ti-arm-clang.${ConfigName}.lib",
        "drivers.am62x.r5f.ti-arm-clang.${ConfigName}.lib",
        "unity.am62x.r5f.ti-arm-clang.${ConfigName}.lib",
    ],
};

const libs_freertos_a53 = {
    common: [
        "freertos.am62x.a53.gcc-aarch64.${ConfigName}.lib",
        "drivers.am62x.a53.gcc-aarch64.${ConfigName}.lib",
        "unity.am62x.a53.gcc-aarch64.${ConfigName}.lib",
    ],
};

const cflags_a53 = {
    common: [
        "-Wno-unused-function",
    ]
}

const lnkfiles = {
    common: [
        "linker.cmd",
    ]
};

const syscfgfile = "../example.syscfg";

const templates_dm_r5f =
[
    {
        input: ".project/templates/am62x/common/linker_dm_r5f.cmd.xdt",
        output: "linker.cmd",
        options: {
            heapSize: 0x8000,
            stackSize: 0x4000,
            irqStackSize: 0x1000,
            svcStackSize: 0x0100,
            fiqStackSize: 0x0100,
            abortStackSize: 0x0100,
            undefinedStackSize: 0x0100,
            dmStubstacksize: 0x0400,
            globalScratchBuf: "true",
        },
    },
    {
        input: ".project/templates/am62x/freertos/main_freertos_dm.c.xdt",
        output: "../main.c",
        options: {
            entryFunction: "test_main",
        },
    }
];

const templates_freertos_a53 =
[
    {
        input: ".project/templates/am62x/common/linker_a53.cmd.xdt",
        output: "linker.cmd",
    },
    {
        input: ".project/templates/am62x/freertos/main_freertos.c.xdt",
        output: "../main.c",
        options: {
            entryFunction: "test_main",
        },
    }
];

const buildOptionCombos = [
    { device: device, cpu: "r5fss0-0", cgt: "ti-arm-clang", board: "am62x-sk", os: "freertos"},
    { device: device, cpu: "r5fss0-0", cgt: "ti-arm-clang", board: "am62x-sip-sk", os: "freertos"},
    { device: device, cpu: "r5fss0-0", cgt: "ti-arm-clang", board: "am62x-sk-lp", os: "freertos"},
    { device: device, cpu: "a53ss0-0", cgt: "gcc-aarch64",  board: "am62x-sk", os: "freertos"},
    { device: device, cpu: "a53ss0-0", cgt: "gcc-aarch64",  board: "am62x-sip-sk", os: "freertos"},
    { device: device, cpu: "a53ss0-0", cgt: "gcc-aarch64",  board: "am62x-sk-lp", os: "freertos"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "executable";
    property.name = "test_mmcsd";
    property.isInternal = true;
    property.skipProjectSpec = true;
    property.buildOptionCombos = buildOptionCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    build_property.filedirs = filedirs;
    build_property.libdirs = libdirs;
    build_property.lnkfiles = lnkfiles;
    build_property.syscfgfile = syscfgfile;
    build_property.files = files;

    if(buildOption.cpu.match(/r5f*/)) {
        build_property.includes = includes_r5f;
        build_property.libs = libs_dm_r5f;
        build_property.libdirs = libdirs_dm_r5f;
        build_property.templates = templates_dm_r5f;
        build_property.defines = defines_dm_r5;
    }

    else if(buildOption.cpu.match(/a53*/)) {
        build_property.includes = includes_freertos_a53;
        build_property.libdirs = libdirs_freertos_a53;
        build_property.libs = libs_freertos_a53;
        build_property.templates = templates_freertos_a53;
        build_property.cflags = cflags_a53;
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
