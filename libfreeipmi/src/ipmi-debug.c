/*
  ipmi-debug.c - IPMI Debugging Functions

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#ifdef STDC_HEADERS
#include <string.h>
#include <stdarg.h>
#endif /* STDC_HEADERS */
#include <stdint.h>
#include <errno.h>

#include "freeipmi-portability.h"
#include "fiid.h"
#include "err-wrappers.h"
#include "fiid-wrappers.h"
#include "ipmi-comp-code-spec.h"

#include "bit-ops.h"
#include "ipmi-common.h"

#define IPMI_DEBUG_MAX_PREFIX_LEN        32
#define IPMI_DEBUG_MAX_BUF_LEN        65536
#define IPMI_DEBUG_MAX_PKT_LEN        65536
#define IPMI_DEBUG_CHAR_PER_LINE          8

#define IPMI_DPRINTF(args) \
        do { \
          if((ipmi_dprintf args) < 0) \
            return -1; \
        } while(0) 

#define IPMI_DPRINTF_CLEANUP(args) \
        do { \
          if((ipmi_dprintf args) < 0) \
            goto cleanup; \
        } while(0) 

#define IPMI_DEBUG_MAX_UNEXPECTED_BYTES 65536
#define IPMI_DEBUG_MAX_UNEXPECTED_BITS  (IPMI_DEBUG_MAX_UNEXPECTED_BYTES*8)

fiid_template_t tmpl_unexpected =
  {
    {IPMI_DEBUG_MAX_UNEXPECTED_BITS, "unexpected_data", FIID_FIELD_REQUIRED | FIID_FIELD_LENGTH_FIXED},
    {0, "", 0}
  };

static int8_t
_set_prefix_str(char *buf, unsigned int buflen, char *prefix)
{
  if (!buf || buflen <= 3)
    return (-1);

  memset(buf, '\0', buflen);
  if (prefix)
    {
      strncpy(buf, prefix, buflen);
      buf[buflen - 1] = '\0'; /* strncpy may not null terminate */
      buf[buflen - 2] = '\0'; /* guaranteed space for ' ' */
      buf[buflen - 3] = '\0'; /* guaranteed space for ':' */
      strcat(buf, ": ");
    }

  return (0);
}

static int8_t
_output_str(int fd, char *prefix, char *str)
{
  /* achu: Yeah, I know this is slow.  Figure out something better
   * later.
   */
  if (str)
    {
      char *ptr = str;

      if (prefix)
        IPMI_DPRINTF((fd, "%s", prefix));
      while (*ptr != '\0')
        {
          if (*ptr == '\n')
            {
              IPMI_DPRINTF((fd, "%c", *ptr++));
              if (prefix)
                IPMI_DPRINTF((fd, "%s", prefix));
            }
          else
            IPMI_DPRINTF((fd, "%c", *ptr++));
        }
      IPMI_DPRINTF((fd, "\n"));
    }

  return 0;
} 

static int8_t
_output_byte_array(int fd, char *prefix, uint8_t *buf, uint32_t buf_len)
{
  uint32_t count = 0;

  if (!buf || !buf_len)
    return 0;

  while (count < buf_len)
    {
      int i = 0;
      if (prefix)
        IPMI_DPRINTF ((fd, "%s", prefix));
      IPMI_DPRINTF ((fd, "[ "));
      while (count < buf_len && i < IPMI_DEBUG_CHAR_PER_LINE)
	{
	  IPMI_DPRINTF ((fd, "%02Xh ", buf[count++]));
	  i++;
	}
      IPMI_DPRINTF ((fd, "]\n"));
    }

  return 0;
}

int8_t
ipmi_dump_setup(int fd, char *prefix, char *hdr, char *prefix_buf, uint32_t prefix_buf_len)
{
  if (_set_prefix_str(prefix_buf, IPMI_DEBUG_MAX_PREFIX_LEN, prefix) < 0)
    return (-1);

  if (_output_str(fd, prefix_buf, hdr) < 0)
    return (-1);

  return (0);
}

int8_t
ipmi_obj_dump_perror (int fd, char *prefix, char *hdr, char *trlr, fiid_obj_t obj)
{
  char prefix_buf[IPMI_DEBUG_MAX_PREFIX_LEN];
  fiid_iterator_t iter = NULL;

  if (!fiid_obj_valid(obj))
    {
      errno = EINVAL;
      return (-1);
    }
  
  ERR (!(ipmi_dump_setup(fd, prefix, hdr, prefix_buf, IPMI_DEBUG_MAX_PREFIX_LEN) < 0));

  if (!(iter = fiid_iterator_create(obj)))
    goto cleanup;

  while (!fiid_iterator_end(iter))
    {
      int32_t field_len;
      uint8_t *key;

      if (!(key = fiid_iterator_key(iter)))
        goto cleanup;

      if ((field_len = fiid_iterator_field_len(iter)) < 0)
        goto cleanup;
    
      if (!field_len)
        {
          fiid_iterator_next(iter);
          continue;
        }

      if (prefix)
        IPMI_DPRINTF_CLEANUP ((fd, "%s", prefix));

      if (field_len <= 64)
        {
          uint64_t val = 0;

	  if (fiid_iterator_get (iter, &val) < 0)
            goto cleanup;

          IPMI_DPRINTF_CLEANUP ((fd, "[%16LXh] = %s[%2db]\n", (uint64_t) val, key, field_len));
        }
      else
        {
          char buf[IPMI_DEBUG_MAX_BUF_LEN];
          int len;

          IPMI_DPRINTF_CLEANUP ((fd, "[  BYTE ARRAY ... ] = %s[%2dB]\n", key, BITS_ROUND_BYTES(field_len)));
     
          if ((len = fiid_iterator_get_data(iter, buf, IPMI_DEBUG_MAX_BUF_LEN)) < 0)
            goto cleanup;
       
          ERR_CLEANUP (!(_output_byte_array(fd, prefix, buf, len) < 0));
        }

      fiid_iterator_next(iter);
    }

  ERR_CLEANUP (!(_output_str(fd, prefix, trlr) < 0));

  fiid_iterator_destroy(iter);
  return (0);

 cleanup:
  if (iter)
    fiid_iterator_destroy(iter);
  return (-1);
}

int8_t
ipmi_obj_dump (int fd, fiid_obj_t obj)
{
  char *hdr = 
    "================================================================\n"
    "[ VALUE               TAG NAME:LENGTH                          ]\n"
    "================================================================";
  char *trlr = 
    "================================================================";

  return ipmi_obj_dump_perror(fd, NULL, hdr, trlr, obj);
}

int8_t
ipmi_dump_lan_packet (int fd, char *prefix, char *hdr, uint8_t *pkt, uint32_t pkt_len, fiid_template_t tmpl_msg_hdr, fiid_template_t tmpl_cmd)
{
  uint32_t indx = 0;
  int32_t obj_cmd_len, obj_lan_msg_trlr_len;
  char prefix_buf[IPMI_DEBUG_MAX_PREFIX_LEN];
  char *rmcp_hdr = 
    "RMCP Header:\n"
    "------------";
  char *session_hdr =
    "IPMI Session Header:\n"
    "--------------------";
  char *msg_hdr =
    "IPMI Message Header:\n"
    "--------------------";
  char *cmd_hdr =
    "IPMI Command Data:\n"
    "------------------";
  char *trlr_hdr =
    "IPMI Trailer:\n"
    "--------------";
  char *unexpected_hdr =
    "Unexpected Data:\n"
    "----------------";
  fiid_obj_t obj_rmcp_hdr = NULL;
  fiid_obj_t obj_session_hdr = NULL;
  fiid_obj_t obj_lan_msg_hdr = NULL;
  fiid_obj_t obj_cmd = NULL;
  fiid_obj_t obj_lan_msg_trlr = NULL;
  fiid_obj_t obj_unexpected = NULL;
  int32_t len;
  int8_t rv = -1;
  uint64_t authentication_type;

  if (!(pkt && tmpl_msg_hdr && tmpl_cmd))
    {
      errno = EINVAL;
      return (-1);
    }

  ERR (!(ipmi_dump_setup(fd, prefix, hdr, prefix_buf, IPMI_DEBUG_MAX_PREFIX_LEN) < 0));

  /* Dump rmcp header */
  
  FIID_OBJ_CREATE_CLEANUP (obj_rmcp_hdr, tmpl_rmcp_hdr);
  
  FIID_OBJ_SET_ALL_LEN_CLEANUP (len, obj_rmcp_hdr, pkt + indx, pkt_len - indx);
  indx += len;
  
  ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, prefix_buf, rmcp_hdr, NULL, obj_rmcp_hdr) < 0));
  
  if (pkt_len <= indx)
    {
      rv = 0;
      goto cleanup;
    }

  /* Dump session header */
  /* Output of session header depends on the auth code */

  FIID_OBJ_CREATE_CLEANUP (obj_session_hdr, tmpl_lan_session_hdr);
  
  FIID_OBJ_SET_BLOCK_LEN_CLEANUP(len, 
				 obj_session_hdr, 
				 (uint8_t *)"authentication_type", 
				 (uint8_t *)"session_id", 
				 pkt + indx, 
				 pkt_len - indx);
  indx += len;
  
  if (pkt_len <= indx)
    {
      ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, 
					  prefix_buf, 
					  session_hdr, 
					  NULL, 
					  obj_session_hdr) < 0));

      rv = 0;
      goto cleanup;
    }

  FIID_OBJ_GET_CLEANUP (obj_session_hdr, "authentication_type", &authentication_type);

  if (authentication_type != IPMI_AUTHENTICATION_TYPE_NONE)
    {
      FIID_OBJ_SET_DATA_LEN_CLEANUP(len, 
				    obj_session_hdr,
				    (uint8_t *)"authentication_code",
				    pkt + indx,
				    pkt_len - indx);
      indx += len;
    }

  if (pkt_len <= indx)
    {
      ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, 
					  prefix_buf, 
					  session_hdr, 
					  NULL, 
					  obj_session_hdr) < 0));

      rv = 0;
      goto cleanup;
    }

  FIID_OBJ_SET_DATA_LEN_CLEANUP(len,
				obj_session_hdr,
				"ipmi_msg_len",
				pkt + indx,
				pkt_len - indx);
  indx += len;

  ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, 
				      prefix_buf, 
				      session_hdr, 
				      NULL, 
				      obj_session_hdr) < 0));

  if (pkt_len <= indx)
    {
      rv = 0;
      goto cleanup;
    }

  /* Dump message header */

  FIID_OBJ_CREATE_CLEANUP (obj_lan_msg_hdr, tmpl_msg_hdr);
  
  FIID_OBJ_SET_ALL_LEN_CLEANUP (len, obj_lan_msg_hdr, pkt + indx, pkt_len - indx);
  indx += len;
  
  ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, prefix_buf, msg_hdr, NULL, obj_lan_msg_hdr) < 0));
  
  if (pkt_len <= indx)
    {
      rv = 0;
      goto cleanup;
    }

  /* Dump command data */

  FIID_OBJ_CREATE_CLEANUP (obj_cmd, tmpl_cmd);
  FIID_OBJ_CREATE_CLEANUP (obj_lan_msg_trlr, tmpl_lan_msg_trlr);
  
  FIID_TEMPLATE_LEN_BYTES (obj_lan_msg_trlr_len, tmpl_lan_msg_trlr);

  if ((pkt_len - indx) >= obj_lan_msg_trlr_len)
    obj_cmd_len = (pkt_len - indx) - obj_lan_msg_trlr_len;
  else if ((pkt_len - indx) < obj_lan_msg_trlr_len)
    obj_cmd_len = 0;

  if (obj_cmd_len)
    {
      
      FIID_OBJ_SET_ALL_LEN_CLEANUP (len, 
				    obj_cmd,
				    pkt + indx, 
				    obj_cmd_len);
      indx += len;
      
      ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, 
					  prefix_buf, 
					  cmd_hdr, 
					  NULL, 
					  obj_cmd) < 0));

      if (pkt_len <= indx)
	{
	  rv = 0;
	  goto cleanup;
	}
    }

  /* Dump trailer */

  FIID_OBJ_SET_ALL_LEN_CLEANUP (len, obj_lan_msg_trlr, pkt + indx, pkt_len - indx);
  indx += len;
  
  ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, prefix_buf, trlr_hdr, NULL, obj_lan_msg_trlr) < 0));
  
  if (pkt_len <= indx)
    {
      rv = 0;
      goto cleanup;
    }

  /* Dump unexpected stuff */
  
  FIID_OBJ_CREATE_CLEANUP (obj_unexpected, tmpl_unexpected);
  
  FIID_OBJ_SET_ALL_LEN_CLEANUP (len, obj_unexpected, pkt + indx, pkt_len - indx);
  indx += len;
  
  ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, prefix_buf, unexpected_hdr, NULL, obj_unexpected) < 0));
  
  rv = 0;
 cleanup:
  FIID_OBJ_DESTROY_NO_RETURN(obj_rmcp_hdr);
  FIID_OBJ_DESTROY_NO_RETURN(obj_session_hdr);
  FIID_OBJ_DESTROY_NO_RETURN(obj_lan_msg_hdr);
  FIID_OBJ_DESTROY_NO_RETURN(obj_cmd);
  FIID_OBJ_DESTROY_NO_RETURN(obj_lan_msg_trlr);
  FIID_OBJ_DESTROY_NO_RETURN(obj_unexpected);
  return (rv);
}

int8_t 
ipmi_dump_rmcp_packet (int fd, char *prefix, char *hdr, uint8_t *pkt, uint32_t pkt_len, fiid_template_t tmpl_cmd)
{
  uint32_t indx = 0;
  char prefix_buf[IPMI_DEBUG_MAX_PREFIX_LEN];
  char *rmcp_hdr =
    "RMCP Header:\n"
    "------------";
  char *rmcp_cmd =
    "RMCP Command Data:\n"
    "------------------";
  char *unexpected_hdr =
    "Unexpected Data:\n"
    "----------------";
  fiid_obj_t obj_rmcp_hdr = NULL;
  fiid_obj_t obj_cmd = NULL;
  fiid_obj_t obj_unexpected = NULL;
  int32_t len;
  int8_t rv = -1;

  if (!(pkt && tmpl_cmd))
    {
      errno = EINVAL;
      return (-1);
    }

  if (ipmi_dump_setup(fd, prefix, hdr, prefix_buf, IPMI_DEBUG_MAX_PREFIX_LEN) < 0)
    return (-1);

  /* Dump rmcp header */
  
  FIID_OBJ_CREATE_CLEANUP (obj_rmcp_hdr, tmpl_rmcp_hdr);
  
  FIID_OBJ_SET_ALL_LEN_CLEANUP (len, obj_rmcp_hdr, pkt + indx, pkt_len - indx);
  indx += len;
  
  ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, prefix_buf, rmcp_hdr, NULL, obj_rmcp_hdr) < 0));
  
  if (pkt_len <= indx)
    {
      rv = 0;
      goto cleanup;
    }
  
  /* Dump command data */
  
  FIID_OBJ_CREATE_CLEANUP (obj_cmd, tmpl_cmd);
  
  FIID_OBJ_SET_ALL_LEN_CLEANUP (len, obj_cmd, pkt + indx, pkt_len - indx);
  indx += len;
  
  ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, prefix_buf, rmcp_cmd, NULL, obj_cmd) < 0));
  
  if (pkt_len <= indx)
    {
      rv = 0;
      goto cleanup;
    }
  
  /* Dump unexpected stuff */
  
  FIID_OBJ_CREATE_CLEANUP (obj_unexpected, tmpl_unexpected);
  
  FIID_OBJ_SET_ALL_LEN_CLEANUP (len, obj_unexpected, pkt + indx, pkt_len - indx);
  indx += len;
  
  ERR_CLEANUP (!(ipmi_obj_dump_perror(fd, prefix_buf, unexpected_hdr, NULL, obj_unexpected) < 0));
  
  rv = 0;
 cleanup:
  FIID_OBJ_DESTROY_NO_RETURN(obj_rmcp_hdr);
  FIID_OBJ_DESTROY_NO_RETURN(obj_cmd);
  FIID_OBJ_DESTROY_NO_RETURN(obj_unexpected);
  return (rv);
}

void
ipmi_debug(const char *fmt, ...)
{
#if defined (IPMI_SYSLOG) || defined (IPMI_TRACE)
  va_list ap;
#endif
#if defined (IPMI_SYSLOG)
  char dbgstr[IPMI_ERR_STR_MAX_LEN];
  char errstr[IPMI_ERR_STR_MAX_LEN];
  int dbglen, errlen;
#endif

#if defined (IPMI_SYSLOG)
  va_start(ap, fmt);
  dbglen = snprintf (dbgstr, IPMI_ERR_STR_MAX_LEN, 
                     "%s: %d: %s: ", __FILE__, __LINE__, __PRETTY_FUNCTION__);
  errlen = vsnprintf (errstr, IPMI_ERR_STR_MAX_LEN, fmt, ap);
  strncat(dbgstr, errstr, IPMI_ERR_STR_MAX_LEN - dbglen - 1);
  syslog (LOG_MAKEPRI (LOG_FAC (LOG_LOCAL1), LOG_ERR), dbgstr);
#endif /* !IPMI_SYSLOG */

#if defined (IPMI_TRACE)
  fprintf (stderr, 
           "%s: %d: %s: ", __FILE__, __LINE__, __PRETTY_FUNCTION__);
  vfprintf (stderr, fmt, ap);
  fprintf (stderr, "\n");
  fflush (stderr);
#endif /* !IPMI_TRACE */ 
}
