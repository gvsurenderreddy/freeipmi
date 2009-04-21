/*
  Copyright (C) 2003-2009 FreeIPMI Core Team

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>

#include "freeipmi/spec/ipmi-entity-ids-spec.h"

#include "freeipmi-portability.h"

/* achu: case preserved from spec.  space before and after slashes removed */
const char *const ipmi_entity_ids[] =
  {
    "unspecified",
    "other",
    "unknown",
    "processor",
    "disk or disk bay",
    "peripheral bay",
    "system management module",
    "system board",
    "memory module",
    "processor module",
    "power supply",
    "add-in card",
    "front panel board",
    "back panel board",
    "power system board",
    "drive backplane",
    "system internal expansion board",
    "other system board",
    "processor board",
    "power unit/power domain",
    "power module/DC-to-DC converter",
    "power management/power distribution board",
    "chassis back panel board",
    "system chassis",
    "sub-chassis",
    "Other chassis board",
    "Disk Drive Bay",
    "Peripheral Bay",
    "Device Bay",
    "fan/cooling device",
    "cooling unit/cooling domain",
    "cable/interconnect",
    "memory device",
    "System Management Software",
    "System Firmware",
    "Operating System",
    "system bus",
    "Group",
    "Remote Management Communication Device",
    "External Environment",
    "battery",
    "Processing blade",
    "Connectivity switch",
    "Processor/memory module",
    "I/O module",
    "Processor/IO module",    /* no slash between IO in spec */
    "Management Controller Firmware",
    "IPMI Channel",
    "PCI Bus",
    "PCI Express Bus",
    "SCSI Bus",
    "SATA/SAS bus",
    "Processor/front-side bus",
    "Real Time Clock",
    NULL
  };

