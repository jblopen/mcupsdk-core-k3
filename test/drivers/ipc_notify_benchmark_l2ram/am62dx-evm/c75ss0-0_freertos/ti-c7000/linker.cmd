
--ram_model
-heap  0x20000
-stack 0x20000
--args 0x1000
--diag_suppress=10068 /* to suppress no matching section error */
--cinit_compression=off
-e _c_int00_secure

#define DDR0_ALLOCATED_START  0x80000000
#define C7X_ALLOCATED_START DDR0_ALLOCATED_START + 0x19800000

#define C7X_RESOURCE_TABLE_BASE (C7X_ALLOCATED_START + 0x00100000)
#define C7X_BOOT_BASE           (C7X_ALLOCATED_START + 0x00200000)
#define C7X_VECTOR_BASE         (C7X_ALLOCATED_START + 0x00400000)
#define C7X_DDR_SPACE_BASE      (C7X_ALLOCATED_START + 0x00410000)

MEMORY
{
    L2SRAM_ENTRY: org = 0x7E000000,          len = 0X048
    L2SRAM:       org = 0x7E000000+0x48      len = 0x100000-0x48
    L2SRAM_AUX:   org = 0x7F000000           len = 0x40000
    DDR0_RESERVED: org = 0x80000000,          len = 0x19800000   /*  Reserved for A53 OS */
    C7X_IPC_D:     org = C7X_ALLOCATED_START, len = 0x00100000   /*  1MB DDR */
    C7X_BOOT_D:    org = C7X_BOOT_BASE,       len = 0x400        /*  1024B DDR */
    C7X_VECS_D:    org = C7X_VECTOR_BASE,     len = 0x4000       /*  16KB DDR */
    C7X_CIO_MEM:   org = C7X_DDR_SPACE_BASE,  len = 0x1000       /*  4KB DDR */
    C7X_DDR_SPACE: org = C7X_DDR_SPACE_BASE+0x1000, len = 0x01BF0000-0x1000  /*  27.9MB - 4KB DDR  */
    /* For resource table */
    DDR_IPC_RESOURCE_TABLE_LINUX:      org = C7X_RESOURCE_TABLE_BASE, len = 0x400    /*  1024B DDR */
    /* IPC trace buffer */
    DDR_IPC_TRACE_LINUX: org = 0x99900400, len = 0xFFC00      /* 1023KB DDR */
    DDR_LOG_SHM_MEM             : ORIGIN = 0xA1000000, LENGTH = 0x40000
}

SECTIONS
{
    .vecs       >       L2SRAM_AUX ALIGN(0x200000)
    .text:_c_int00_secure > L2SRAM_ENTRY ALIGN(0x200000)
    .text       >       L2SRAM

    .bss        >       L2SRAM  /* Zero-initialized data */
    RUN_START(__BSS_START)
    RUN_END(__BSS_END)

    .data       >       L2SRAM  /* Initialized data */

    .cinit      >       L2SRAM  /* could be part of const */
    .init_array >       L2SRAM  /* C++ initializations */
    .stack      >       L2SRAM ALIGN(0x2000)
    .args       >       L2SRAM
    .cio        >       C7X_CIO_MEM
    .const      >       L2SRAM
    .switch     >       L2SRAM /* For exception handling. */
    .sysmem     >       L2SRAM /* heap */

    GROUP:              >  L2SRAM
    {
        .data.Mmu_tableArray          : type=NOINIT
        .data.Mmu_tableArraySlot      : type=NOINIT
        .data.Mmu_level1Table         : type=NOINIT
        .data.gMmu_tableArray_NS       : type=NOINIT
        .data.Mmu_tableArraySlot_NS   : type=NOINIT
        .data.Mmu_level1Table_NS      : type=NOINIT
    }

    .benchmark_buffer:     > L2SRAM ALIGN (32)

    /* This is the resource table used by linux to know where the IPC "VRINGs" are located */
    .resource_table: { __RESOURCE_TABLE = .;} > DDR_IPC_RESOURCE_TABLE_LINUX
    /* This IPC log can be viewed via ROV in CCS and when linux is enabled, this log can also be viewed via linux debugfs */
    .bss.debug_mem_trace_buf    : {} palign(128)    > DDR_IPC_TRACE_LINUX
    /* this is used when Debug log's to shared memory is enabled, else this is not used */
    .bss.log_shared_mem  (NOLOAD) : {} > DDR_LOG_SHM_MEM
}
