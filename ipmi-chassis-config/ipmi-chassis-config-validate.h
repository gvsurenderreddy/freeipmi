/*
 * Copyright (C) 2008-2013 FreeIPMI Core Team
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

#ifndef IPMI_CHASSIS_CONFIG_VALIDATE_H
#define IPMI_CHASSIS_CONFIG_VALIDATE_H

#include "ipmi-chassis-config.h"
#include "ipmi-chassis-config-sections.h"

config_validate_t power_restore_policy_number_validate (const char *section_name,
                                                        const char *key_name,
                                                        const char *value,
                                                        void *arg);

config_validate_t bios_boot_type_number_validate (const char *section_name,
                                                  const char *key_name,
                                                  const char *value,
                                                  void *arg);

config_validate_t boot_device_number_validate (const char *section_name,
                                               const char *key_name,
                                               const char *value,
                                               void *arg);

config_validate_t device_instance_selector_number_validate (const char *section_name,
                                                            const char *key_name,
                                                            const char *value,
                                                            void *arg);

config_validate_t firmware_bios_verbosity_number_validate (const char *section_name,
                                                           const char *key_name,
                                                           const char *value,
                                                           void *arg);

config_validate_t console_redirection_number_validate (const char *section_name,
                                                       const char *key_name,
                                                       const char *value,
                                                       void *arg);

#endif /* IPMI_CHASSIS_CONFIG_VALIDATE_H */
