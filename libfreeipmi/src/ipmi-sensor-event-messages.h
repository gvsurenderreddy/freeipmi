/* 
   ipmi-sensor-event-messages.h - IPMI Sensor Event Messages

   Copyright (C) 2003, 2004, 2005 FreeIPMI Core Team

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
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  
*/

#ifndef _IPMI_SENSOR_EVENT_MESSAGES_H
#define _IPMI_SENSOR_EVENT_MESSAGES_H

#include <stdint.h>

int ipmi_get_generic_event_message (uint8_t event_reading_type_code,
				    uint16_t offset,
				    char *buf,
				    unsigned int buflen);

int ipmi_get_sensor_type_code_message (int sensor_type_code,
				       int offset,
				       char *buf,
				       unsigned int buflen);

int ipmi_get_event_data2_message (int sensor_type_code,
				  int offset,
				  uint8_t event_data2,
				  char *buf,
				  unsigned int buflen);

int ipmi_get_event_data3_message (int sensor_type_code,
				  int offset,
				  uint8_t event_data2,
				  uint8_t event_data3,
				  char *buf,
				  unsigned int buflen);

#endif
