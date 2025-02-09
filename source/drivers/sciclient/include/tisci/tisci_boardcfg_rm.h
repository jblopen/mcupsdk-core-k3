/*
 *  Copyright (C) 2018-2024 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/**
 * \ingroup TISCI
 * \defgroup tisci_boardcfg_rm tisci_boardcfg_rm
 *
 * DMSC controls the power management, security and resource management
 * of the device.
 *
 *
 * @{
 */
/**
 *
 *  \brief  This file contains:
 *
 *          WARNING!!: Autogenerated file from SYSFW. DO NOT MODIFY!!
 * System Firmware Source File
 *
 * Board Configuration Data Structures
 *
 */

#ifndef TISCI_BOARD_CFG_RM_H
#define TISCI_BOARD_CFG_RM_H

#ifdef __cplusplus
extern "C"
{
#endif



#define TISCI_BOARDCFG_RM_HOST_CFG_MAGIC_NUM          (0x4C41U)
#define TISCI_BOARDCFG_RM_RESASG_MAGIC_NUM            (0x7B25U)

#define TISCI_BOARDCFG_RM_ABI_MAJ_VALUE               (0x00)
#define TISCI_BOARDCFG_RM_ABI_MIN_VALUE               (0x01)

/**
 * \brief Contains a unique magic number for each substructure and the size
 *      of the associated superstructure for data validation/API
 *      compatibility checks.
 *
 * \param magic Unique magic number for data integrity check.
 * \param size `sizeof(superstructure containing this header)` for data
 *           integrity check.
 */
struct tisci_boardcfg_substructure_rm_header {
    uint16_t    magic;
    uint16_t    size;
} __attribute__((__packed__));

/**
 * \brief Board Config data ABI version.
 *
 * \param tisci_boardcfg_abi_maj Major Board Config data ABI version.
 * \param tisci_boardcfg_abi_min Minor Board Config data ABI version.
 */
struct tisci_boardcfg_abi_rm_rev {
    uint8_t    tisci_boardcfg_abi_maj;
    uint8_t    tisci_boardcfg_abi_min;
} __attribute__((__packed__));

/**
 * \brief Resource Management host ID configuration entry describing a
 *        host's allowed system priorities as well as a host's supervisor
 *
 * \param host_id Processing entity host ID
 *
 * \param allowed_qos Allowed QoS level configuration for host ID:
 *
 *                    The host ID gets assigned a list of QoS levels which
 *                    are allowed. As QoS level is a 3-bit field, there are
 *                    8 possible order-IDs. Thus in one 16-bit word, flags
 *                    are set specifying whether or not the QoS level is
 *                    allowed for the host ID. For each QoS level, the value
 *                    of 01b means not allowed, 10b means allowed, and 11b
 *                    and 00b are invalid/errors. These are encoded in a
 *                    bitfield because there is one set of allowed QoS levels
 *                    for every host ID.
 *
 * \param allowed_orderid Allowed order-ID configuration for the host ID:
 *
 *                        The host ID gets assigned a list of order-IDs which
 *                        are allowed. As order-ID is a 4-bit field, there are
 *                        16 possible order-IDs. Thus in one 32-bit word, flags
 *                        are set specifying whether or not the order-ID is
 *                        allowed for the host ID. For each order-ID, the value
 *                        of 01b means not allowed, 10b means allowed, and 11b
 *                        and 00b are invalid/errors. These are encoded in a
 *                        bitfield because there is one set of allowed
 *                        order-IDs for every host ID.
 *
 * \param allowed_atype Allowed atype configuration for the host ID:
 *
 *                      The host ID gets assigned a list of atypes which
 *                      are allowed. atype is a 2-bit field with 3 possible
 *                      values. Thus in one 8-bit word, flags are set
 *                      specifying whether or not an atype value is
 *                      allowed for the host ID. For each atype, the value
 *                      of 01b means not allowed, 10b means allowed, and 11b
 *                      and 00b are invalid/errors. These are encoded in a
 *                      bitfield because there is one set of allowed atypes
 *                      for every host ID.
 *
 * \param allowed_priority Allowed bus priority configuration for host ID:
 *
 *                         The host ID gets assigned a list of bus priorities
 *                         which are allowed. As bus priority is a 3-bit field,
 *                         there are 8 possible bus priorities. Thus in one
 *                         16-bit word, flags are set specifying whether or not
 *                         the bus priority is allowed for the host ID. For each
 *                         bus priority, the value of 01b means not allowed,
 *                         10b means allowed, and 11b and 00b are
 *                         invalid/errors. These are encoded in a bitfield
 *                         because there is one set of allowed bus priorities
 *                         for every host ID.
 *
 * \param allowed sched_priority Allowed UDMAP channel scheduling priority
 *                               configuration for host ID:
 *
 *                               The host ID gets assigned a list of UDMAP
 *                               channel scheduling priorities which are
 *                               allowed. As UDMAP channel scheduling priority
 *                               is a 2-bit field, there are 4 possible UDMAP
 *                               channel scheduling priorities. Thus in one
 *                               8-bit word, flags are set specifying whether or
 *                               not UDMAP channel scheduling priority is
 *                               allowed for the host ID. For each priority, the
 *                               value of 01b means not allowed, 10b means
 *                               allowed, and 11b and 00b are invalid/errors.
 *                               These are encoded in a bitfield because there
 *                               is one set of allowed UDMAP channel scheduling
 *                               priorities for every host ID.
 */
struct tisci_boardcfg_rm_host_cfg_entry {
    uint8_t    host_id;
    uint8_t    allowed_atype;
    uint16_t    allowed_qos;
    uint32_t    allowed_orderid;
    uint16_t    allowed_priority;
    uint8_t    allowed_sched_priority;
} __attribute__((__packed__));

/* \brief How many host configuration list entries */
#define TISCI_BOARDCFG_RM_HOST_CFG_ENTRIES (32U)

/**
 * \brief Resource Management host ID configuration describing a host's allowed
 *        system priorities
 *
 * \param subhdr Magic and size for integrity check.
 * \param host_cfg_entries @ref tisci_boardcfg_rm_host_cfg_entry entries up to
 *                         @ref TISCI_BOARDCFG_RM_HOST_CFG_ENTRIES
 */
struct tisci_boardcfg_rm_host_cfg {
    struct tisci_boardcfg_substructure_rm_header    subhdr;
    struct tisci_boardcfg_rm_host_cfg_entry    host_cfg_entries[
        TISCI_BOARDCFG_RM_HOST_CFG_ENTRIES];
};

/**
 * \brief One contiguous block of any resource to be assigned to one
 *      processing entity.
 *
 * \param start_resource First resource number (inclusive) for a host_id
 *             and resource.
 * \param num_resource Number of continuous resources allocated to a
 *               host_id and resource. 0 means unused.
 * \param type Unique type value identifying the specific resource to which the
 *             range assignment applies
 * \param host_id Processing entity host ID to which the resource range is
 *                assigned
 */
struct tisci_boardcfg_rm_resasg_entry {
    uint16_t    start_resource;
    uint16_t    num_resource;
    uint16_t    type;
    uint8_t    host_id;
    uint8_t    reserved;
};

/**
 * \brief RM tisci_boardcfg resource assignment configuration.
 *
 * \param subhdr Magic and size for integrity check.
 * \param resasg_entries_size Size of resasg_entries array in bytes
 * \param resasg_entries Array of @ref tisci_boardcfg_rm_resasg_entry containing
 *                       resource range assignments to hosts.
 */
struct tisci_boardcfg_rm_resasg {
    struct tisci_boardcfg_substructure_rm_header    subhdr;
    uint16_t                    resasg_entries_size;
    uint16_t                    reserved;
} __attribute__((__packed__));

/**
 * \brief Resource Management configuration.
 *
 * \param rev Resource Management board config ABI version (separate from Board
 *            Config ABI version)
 * \param host_cfg Array of host configuration structures.
 * \param resasg Resource assignment configuration structure.
 */
struct tisci_boardcfg_rm {
    struct tisci_boardcfg_abi_rm_rev    rev;
    struct tisci_boardcfg_rm_host_cfg    host_cfg;
    struct tisci_boardcfg_rm_resasg    resasg;
} __attribute__((__packed__));

/**
 * \brief Get a pointer to the local copy of the tisci_boardcfg_rm_host_cfg data.
 *
 * \return NULL if the local RM board data has not yet been received or is
 *         invalid, pointer otherwise.
 */
const struct tisci_boardcfg_rm_host_cfg *tisci_boardcfg_get_rm_host_cfg(void);

/**
 * \brief Get a the size of the local copy of the tisci_boardcfg_rm_resasg_entry data.
 *
 * \return Zero if the local RM board data has not yet been received or is
 *         invalid, greater than zero otherwise.
 */
uint16_t tisci_boardcfg_get_rm_resasg_size(void);

/**
 * \brief Get a pointer to the local copy of the tisci_boardcfg_rm_resasg data.
 *
 * \return NULL if the local RM board data has not yet been received or is
 *         invalid, pointer otherwise.
 */
const struct tisci_boardcfg_rm_resasg_entry *tisci_boardcfg_get_rm_resasg(void);

/**
 * \brief Get the RM device group from the local tisci_boardcfg_rm data.
 *
 * \param devgrp Pointer to a device group variable in which the device group
 *        is returned on SUCCESS
 *
 * \return EINIT if the local RM board data has not yet been received or is
 *         invalid, SUCCESS if in the board data has been received and is valid.
 */
int32_t tisci_boardcfg_get_rm_devgrp(devgrp_t *devgrp);


#ifdef __cplusplus
}
#endif

#endif          /* TISCI_BOARD_CFG_RM_H */

/** @} */
