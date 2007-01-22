/* 
   $Id: ipmi-sensors-argp.h,v 1.6 2007-01-22 23:39:27 chu11 Exp $ 
   
   ipmi-sensors-argp.h - IPMI Sensors utility.
   
   Copyright (C) 2006 FreeIPMI Core Team
   
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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  
*/

#ifndef _IPMI_SENSORS_ARGP_H
#define _IPMI_SENSORS_ARGP_H

void ipmi_sensors_argp_parse (int argc, char **argv, struct ipmi_sensors_arguments *cmd_args);

#endif
