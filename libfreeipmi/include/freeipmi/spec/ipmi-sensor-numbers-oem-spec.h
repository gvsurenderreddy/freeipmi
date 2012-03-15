/*
 * Copyright (C) 2003-2012 FreeIPMI Core Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef _IPMI_SENSOR_NUMBERS_OEM_SPEC_H
#define _IPMI_SENSOR_NUMBERS_OEM_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************
 * Dell                                    *
 *******************************************/

/*
 * Dell Poweredge R610
 * Dell Poweredge R710
 */

#define IPMI_SENSOR_NUMBER_OEM_DELL_ECC_CORRECTABLE_ERRORS                              0x01
#define IPMI_SENSOR_NUMBER_OEM_DELL_ECC_UNCORRECTABLE_ERRORS                            0x02
#define IPMI_SENSOR_NUMBER_OEM_DELL_IO_CHANNEL_CHECK                                    0x03
#define IPMI_SENSOR_NUMBER_OEM_DELL_PCI_PARITY_ERROR                                    0x04
#define IPMI_SENSOR_NUMBER_OEM_DELL_PCI_SYSTEM_ERROR                                    0x05
#define IPMI_SENSOR_NUMBER_OEM_DELL_EVENT_LOGGING_FOR_CORRECTABLE_ECC_EVENTS_DISABLED   0x06
#define IPMI_SENSOR_NUMBER_OEM_DELL_EVENT_LOGGING_DISABLED                              0x07
#define IPMI_SENSOR_NUMBER_OEM_DELL_UNKNOWN_ERROR                                       0x08
#define IPMI_SENSOR_NUMBER_OEM_DELL_CPU_INTERNAL_ERROR                                  0x09
#define IPMI_SENSOR_NUMBER_OEM_DELL_CPU_PROTOCOL_ERROR                                  0x0A
#define IPMI_SENSOR_NUMBER_OEM_DELL_CPU_BUSS_PERR                                       0x0B
#define IPMI_SENSOR_NUMBER_OEM_DELL_CPU_BUS_INITIALIZATION_ERROR                        0x0C
#define IPMI_SENSOR_NUMBER_OEM_DELL_CPU_MACHINE_CHECK_ERROR                             0x0D
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEMORY_SPARE                                        0x11
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEMORY_MIRROR                                       0x12
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEMORY_RAID                                         0x13
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEMORY_HOT_ADD                                      0x14
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEMORY_HOT_REMOVE                                   0x15
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEMORY_HOT_FAILURE                                  0x16
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEMORY_REDUNDANCY_REGAINED                          0x17
#define IPMI_SENSOR_NUMBER_OEM_DELL_FATAL_PCI_EXPRESS_ERRORS                            0x18
#define IPMI_SENSOR_NUMBER_OEM_DELL_CHIPSET_ERROR                                       0x19
#define IPMI_SENSOR_NUMBER_OEM_DELL_ERROR_REGISTER_POINTER                              0x1A
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEMORY_CORRECTABLE_ECC_WARNING_OR_CRITICAL_EXCEEDED 0x1B
#define IPMI_SENSOR_NUMBER_OEM_DELL_CRC_MEMORY_ERROR                                    0x1C
#define IPMI_SENSOR_NUMBER_OEM_DELL_USB_OVER_CURRENT                                    0x1D
#define IPMI_SENSOR_NUMBER_OEM_DELL_POST_FATAL_ERROR                                    0x1E
#define IPMI_SENSOR_NUMBER_OEM_DELL_INCOMPATIBLE_BMC_FIRMWARE                           0x1F
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEM_OVERTEMP                                        0x20
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEM_FATAL_SB_CRC                                    0x21
#define IPMI_SENSOR_NUMBER_OEM_DELL_MEM_FATAL_NB_CRC                                    0x22
#define IPMI_SENSOR_NUMBER_OEM_DELL_OS_WATCHDOG_TIMER                                   0x23
#define IPMI_SENSOR_NUMBER_OEM_DELL_LINK_TUNING_ERROR                                   0x24
#define IPMI_SENSOR_NUMBER_OEM_DELL_LT_FLEXADDR                                         0x25
#define IPMI_SENSOR_NUMBER_OEM_DELL_NON_FATAL_PCI_EXPRESS_ERRORS                        0x26
#define IPMI_SENSOR_NUMBER_OEM_DELL_FATAL_IO_ERROR                                      0x27
#define IPMI_SENSOR_NUMBER_OEM_DELL_MSR_INFO_LOG                                        0x28

/*
 * Dell Poweredge C410x
 */

#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FB_TEMP      0x17
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_BOARD_TEMP_1 0x01
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_BOARD_TEMP_2 0x02
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_BOARD_TEMP_3 0x03
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_BOARD_TEMP_4 0x04
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_BOARD_TEMP_5 0x05
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_BOARD_TEMP_6 0x06
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_1_TEMP  0x07
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_2_TEMP  0x08
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_3_TEMP  0x09
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_4_TEMP  0x0A
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_5_TEMP  0x0B
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_6_TEMP  0x0C
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_7_TEMP  0x0D
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_8_TEMP  0x0E
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_9_TEMP  0x0F
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_10_TEMP 0x10
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_11_TEMP 0x11
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_12_TEMP 0x12
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_13_TEMP 0x13
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_14_TEMP 0x14
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_15_TEMP 0x15
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_16_TEMP 0x16
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_1_WATT  0x50
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_2_WATT  0x51
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_3_WATT  0x52
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_4_WATT  0x53
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_5_WATT  0x54
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_6_WATT  0x55
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_7_WATT  0x56
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_8_WATT  0x57
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_9_WATT  0x58
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_10_WATT 0x59
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_11_WATT 0x5A
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_12_WATT 0x5B
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_13_WATT 0x5C
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_14_WATT 0x5D
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_15_WATT 0x5E
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PCIE_16_WATT 0x5F
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PSU_1_WATT   0x60
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PSU_2_WATT   0x61
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PSU_3_WATT   0x62
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_PSU_4_WATT   0x63
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FAN_1        0x80
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FAN_2        0x81
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FAN_3        0x82
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FAN_4        0x83
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FAN_5        0x84
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FAN_6        0x85
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FAN_7        0x86
#define IPMI_SENSOR_NUMBER_OEM_DELL_C410X_FAN_8        0x87

/*******************************************
 * Intel                                   *
 *******************************************/

/*
 * Intel S5500WB/Penguin Computing Relion 700
 * Quanta QSSC-S4R/Appro GB812X-CN
 * (Quanta motherboard maintains Intel manufacturer ID)
 */

#define IPMI_SENSOR_NUMBER_OEM_INTEL_PCI_SENSOR              0x03
#define IPMI_SENSOR_NUMBER_OEM_INTEL_PCIE_FATAL_SENSOR       0x04
#define IPMI_SENSOR_NUMBER_OEM_INTEL_PCIE_CORRECTABLE_SENSOR 0x05

#define IPMI_SENSOR_NUMBER_OEM_INTEL_QPI_CORRECTABLE_SENSOR  0x06
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QPI_NON_FATAL_SENSOR    0x07
/* QPI_FATAL_SENSOR_A and QPI_FATAL_SENSOR_B are identical, they are
 * logical extensions to provide additional offset values
 */
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QPI_FATAL_SENSOR_A      0x17
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QPI_FATAL_SENSOR_B      0x18

#define IPMI_SENSOR_NUMBER_OEM_INTEL_BIOS_POST_ERROR         0x06

/*
 * Intel S5500WB/Penguin Computing Relion 700
 */

#define IPMI_SENSOR_NUMBER_OEM_INTEL_RAS_STATUS_INFORMATION_FOR_MEMORY_MIRRORING_MIRRORING_MODE  0x01
#define IPMI_SENSOR_NUMBER_OEM_INTEL_MEMORY_ECC_ERROR                                            0x02
#define IPMI_SENSOR_NUMBER_OEM_INTEL_RAS_STATUS_INFORMATION_FOR_MEMORY_MIRRORING_SPARING_MODE    0x11
#define IPMI_SENSOR_NUMBER_OEM_INTEL_MEMORY_MIRRORING_RAS_CONFIGURATION_INFORMATION              0x12
#define IPMI_SENSOR_NUMBER_OEM_INTEL_MEMORY_SPARING_RAS_CONFIGURATION_INFORMATION                0x13
#define IPMI_SENSOR_NUMBER_OEM_INTEL_MEMORY_PARITY_ERROR                                         0x14

/*
 * Quanta QSSC-S4R/Appro GB812X-CN
 * (Quanta motherboard maintains Intel manufacturer ID)
 */
/* achu: Few same as above, but to clearly differentiate motherboards, duplicate them */
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_RAS_STATE_INFORMATION_FOR_MEMORY_MIRRORING_MIRRORING_MODE 0x01
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_MEMORY_ECC_ERROR                                          0x02
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_MEMORY_MISMATCH_CONFIGURATION_ERROR                       0x03
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_SMI_LINK_CRC_ERROR_PERSISTENT                             0x05
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_PATROL_SCRUB_ERROR                                        0x0B
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_SMI_LINK_CRC_ERROR_UNCORRECTABLE                          0x0C
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_RAS_STATE_INFORMATION_FOR_MEMORY_MIRRORING_SPARING_MODE   0x11
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_MEMORY_MIRRORING_RAS_CONFIGURATION_INFORMATION            0x12
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_MEMORY_SPARING_RAS_CONFIGURATION_INFORMATION              0x13
#define IPMI_SENSOR_NUMBER_OEM_INTEL_QUANTA_QSSC_S4R_MEMORY_BOARD_STATE                                        0x20

/*
 * Intel S2600JF/Appro 512X
 */
/* achu: Few same as above, but to clearly differentiate motherboards, duplicate them */
/* achu: note a typo, memory ras configuration status & memory ecc error both 2, not sure why in doc */
#define IPMI_SENSOR_NUMBER_OEM_INTEL_S2600JF_MIRRORING_REDUNDANCY_STATE      0x01
#define IPMI_SENSOR_NUMBER_OEM_INTEL_S2600JF_MEMORY_RAS_CONFIGURATION_STATUS 0x02
#define IPMI_SENSOR_NUMBER_OEM_INTEL_S2600JF_MEMORY_ECC_ERROR                0x02
#define IPMI_SENSOR_NUMBER_OEM_INTEL_S2600JF_LEGACY_PCI_ERROR                0x03

/*
 * Intel Node Manager
 *
 * For Intel Chips, not just Intel Motherboards.  Confirmed for:
 *
 * Intel S5500WB/Penguin Computing Relion 700
 * Intel S2600JF/Appro 512X
 * Inventec 5441/Dell Xanadu II
 * Inventec 5442/Dell Xanadu III
 * Quanta S99Q/Dell FS12-TY
 * Quanta QSSC-S4R/Appro GB812X-CN
 */

#define IPMI_SENSOR_NUMBER_OEM_INTEL_SERVER_PLATFORM_SERVICES_FIRMWARE_HEALTH 0x17

/*******************************************
 * Inventec                                *
 *******************************************/

/*
 * Inventec 5441/Dell Xanadu II
 * Inventec 5442/Dell Xanadu III
 */
/* achu: not official names, named based on use context */
#define IPMI_SENSOR_NUMBER_OEM_INVENTEC_POST_START        0x81
#define IPMI_SENSOR_NUMBER_OEM_INVENTEC_POST_OK           0x85
#define IPMI_SENSOR_NUMBER_OEM_INVENTEC_POST_ERROR_CODE   0x06
#define IPMI_SENSOR_NUMBER_OEM_INVENTEC_PORT80_CODE_EVENT 0x55
#define IPMI_SENSOR_NUMBER_OEM_INVENTEC_MEMORY            0x60

/*******************************************
 * Quanta                                  *
 *******************************************/

/*
 * Quanta S99Q/Dell FS12-TY
 */
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_MEMORY              0x60
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_PCI_SENSORID        0x81
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_QPI_SENSORID        0x82
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_INT_SENSORID        0x83
#define IPMI_SENSOR_NUMBER_OEM_QUANTA_SOFTWARE_NMI        0xA5

#ifdef __cplusplus
}
#endif

#endif
