/*
 * Copyright (C) 2003-2013 FreeIPMI Core Team
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

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <assert.h>

#include "bmc-config.h"
#include "bmc-config-map.h"
#include "bmc-config-validate.h"
#include "bmc-config-utils.h"

#include "freeipmi-portability.h"
#include "pstdout.h"

/* convenience struct */
struct channel_access
{
  uint8_t access_mode;
  uint8_t user_level_authentication;
  uint8_t per_message_authentication;
  uint8_t pef_alerting;
  uint8_t channel_privilege_limit;
};

static config_err_t
_channel_info (bmc_config_state_data_t *state_data,
               const char *section_name,
               uint8_t *channel_number)
{
  config_err_t ret;

  assert (state_data);
  assert (section_name);
  assert (channel_number);

  if (stristr (section_name, "Lan"))
    {
      if ((ret = get_lan_channel_number (state_data,
					 section_name,
					 channel_number)) != CONFIG_ERR_SUCCESS)
	return (ret);
    }
  else
    {
      if ((ret = get_serial_channel_number (state_data,
					    section_name,
					    channel_number)) != CONFIG_ERR_SUCCESS)
	return (ret);
    }
  
  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_get_key_info (bmc_config_state_data_t *state_data,
               const char *section_name,
               const char *key_name,
               uint8_t *channel_number,
               uint8_t *access_type)
{
  config_err_t ret;

  assert (state_data);
  assert (section_name);
  assert (key_name);
  assert (channel_number);
  assert (access_type);

  if ((ret = _channel_info (state_data,
                            section_name,
                            channel_number)) != CONFIG_ERR_SUCCESS)
    return (ret);

  /* Must check for Non_Volatile b/c Volatile is a substring of the former */
  if (stristr (key_name, "Non_Volatile"))
    *access_type = IPMI_CHANNEL_ACCESS_GET_NON_VOLATILE;
  else
    *access_type = IPMI_CHANNEL_ACCESS_GET_VOLATILE;

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_set_key_info (bmc_config_state_data_t *state_data,
               const char *section_name,
               const char *key_name,
               uint8_t *channel_number,
               uint8_t *access_type)
{
  config_err_t ret;

  assert (state_data);
  assert (section_name);
  assert (key_name);
  assert (channel_number);
  assert (access_type);

  if ((ret = _channel_info (state_data,
                            section_name,
                            channel_number)) != CONFIG_ERR_SUCCESS)
    return (ret);

  /* Must check for Non_Volatile b/c Volatile is a substring of the former */
  if (stristr (key_name, "Non_Volatile"))
    *access_type = IPMI_CHANNEL_ACCESS_SET_NON_VOLATILE;
  else
    *access_type = IPMI_CHANNEL_ACCESS_SET_VOLATILE;

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_get_channel_access (bmc_config_state_data_t *state_data,
                     const char *section_name,
                     const char *key_name,
                     struct channel_access *ch)
{
  fiid_obj_t obj_cmd_rs = NULL;
  config_err_t rv = CONFIG_ERR_FATAL_ERROR;
  config_err_t ret;
  uint8_t channel_number;
  uint8_t access_type;
  uint64_t val;

  assert (state_data);
  assert (section_name);
  assert (key_name);
  assert (ch);

  if ((ret = _get_key_info (state_data,
                            section_name,
                            key_name,
                            &channel_number,
                            &access_type)) != CONFIG_ERR_SUCCESS)
    {
      rv = ret;
      goto cleanup;
    }

  if (!(obj_cmd_rs = fiid_obj_create (tmpl_cmd_get_channel_access_rs)))
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_create: %s\n",
                       strerror (errno));
      goto cleanup;
    }

  if (ipmi_cmd_get_channel_access (state_data->ipmi_ctx,
                                   channel_number,
                                   access_type,
                                   obj_cmd_rs) < 0)
    {
      if (state_data->prog_data->args->config_args.common_args.debug)
        pstdout_fprintf (state_data->pstate,
                         stderr,
                         "ipmi_cmd_get_channel_access: %s\n",
                         ipmi_ctx_errormsg (state_data->ipmi_ctx));

      if (config_is_non_fatal_error (state_data->ipmi_ctx,
                                     obj_cmd_rs,
                                     &ret))
        rv = ret;

      goto cleanup;
    }

  if (FIID_OBJ_GET (obj_cmd_rs, "ipmi_messaging_access_mode", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'ipmi_messaging_access_mode': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  ch->access_mode = val;

  /* yes/no is backwards here, see ipmi spec */
  if (FIID_OBJ_GET (obj_cmd_rs, "user_level_authentication", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'user_level_authentication': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  ch->user_level_authentication = (val ? 0 : 1);

  /* yes/no is backwards here, see ipmi spec */
  if (FIID_OBJ_GET (obj_cmd_rs, "per_message_authentication", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'per_message_authentication': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  ch->per_message_authentication = (val ? 0 : 1);

  /* yes/no is backwards here, see ipmi spec */
  if (FIID_OBJ_GET (obj_cmd_rs, "pef_alerting", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'pef_alerting': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  ch->pef_alerting = (val ? 0 : 1);

  if (FIID_OBJ_GET (obj_cmd_rs, "channel_privilege_level_limit", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'channel_privilege_level_limit': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  ch->channel_privilege_limit = val;

  rv = CONFIG_ERR_SUCCESS;
 cleanup:
  fiid_obj_destroy (obj_cmd_rs);
  return (rv);
}

static config_err_t
_set_channel_access (bmc_config_state_data_t *state_data,
                     const char *section_name,
                     const char *key_name,
                     struct channel_access *ch,
                     uint8_t *comp_code)
{
  fiid_obj_t obj_cmd_rs = NULL;
  config_err_t rv = CONFIG_ERR_FATAL_ERROR;
  config_err_t ret;
  uint8_t channel_number;
  uint8_t set_type;
  uint64_t val;

  assert (state_data);
  assert (section_name);
  assert (key_name);
  assert (ch);

  if ((ret = _set_key_info (state_data,
                            section_name,
                            key_name,
                            &channel_number,
                            &set_type)) != CONFIG_ERR_SUCCESS)
    {
      rv = ret;
      goto cleanup;
    }

  if (!(obj_cmd_rs = fiid_obj_create (tmpl_cmd_set_channel_access_rs)))
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_create: %s\n",
                       strerror (errno));
      goto cleanup;
    }

  /* yes/no is backwards several places, see ipmi spec */

  if (ipmi_cmd_set_channel_access (state_data->ipmi_ctx,
                                   channel_number,
                                   ch->access_mode,
                                   (ch->user_level_authentication ? 0 : 1),
                                   (ch->per_message_authentication ? 0 : 1),
                                   (ch->pef_alerting ? 0 : 1),
                                   set_type,
                                   ch->channel_privilege_limit,
                                   (set_type == IPMI_CHANNEL_ACCESS_SET_VOLATILE
                                    ? IPMI_PRIVILEGE_LEVEL_LIMIT_SET_VOLATILE
                                    : IPMI_PRIVILEGE_LEVEL_LIMIT_SET_NON_VOLATILE),
                                   obj_cmd_rs) < 0)
    {
      if (state_data->prog_data->args->config_args.common_args.debug)
        pstdout_fprintf (state_data->pstate,
                         stderr,
                         "ipmi_cmd_set_channel_access: %s\n",
                         ipmi_ctx_errormsg (state_data->ipmi_ctx));

      if (comp_code
          && ipmi_ctx_errnum (state_data->ipmi_ctx) == IPMI_ERR_BAD_COMPLETION_CODE)
        {
          (*comp_code) = 0;
          if (FIID_OBJ_GET (obj_cmd_rs, "comp_code", &val) < 0)
            {
              pstdout_fprintf (state_data->pstate,
                               stderr,
                               "fiid_obj_get: 'comp_code': %s\n",
                               fiid_obj_errormsg (obj_cmd_rs));
              goto cleanup;
            }
          (*comp_code) = val;
        }

      if (config_is_non_fatal_error (state_data->ipmi_ctx,
                                     obj_cmd_rs,
                                     &ret))
        rv = ret;

      goto cleanup;
    }

  rv = CONFIG_ERR_SUCCESS;
 cleanup:
  fiid_obj_destroy (obj_cmd_rs);
  return (rv);
}

static config_err_t
_access_mode_checkout (const char *section_name,
                       struct config_keyvalue *kv,
                       void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output (state_data->pstate,
                                             kv,
                                             channel_access_mode_string (ch.access_mode)) < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_access_mode_commit (const char *section_name,
                     const struct config_keyvalue *kv,
                     void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  ch.access_mode = channel_access_mode (kv->value_input);

  if ((ret = _set_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch,
                                  NULL)) != CONFIG_ERR_SUCCESS)
    return (ret);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_enable_user_level_authentication_checkout (const char *section_name,
                                            struct config_keyvalue *kv,
                                            void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  /* achu: Backwards values in this command are handled above */
  if (config_section_update_keyvalue_output (state_data->pstate,
                                             kv,
                                             ch.user_level_authentication ? "Yes" : "No") < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_enable_user_level_authentication_commit (const char *section_name,
                                          const struct config_keyvalue *kv,
                                          void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;
  uint8_t comp_code = 0;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  ch.user_level_authentication = same (kv->value_input, "yes");

  /* IPMI_COMP_CODE_REQUEST_INVALID_DATA_FIELD is special case for
   * this field, see IPMI spec.  "Return CCh 'invalid data field'
   * error completion code if an attempt is made to set this bit, but
   * hte option is not supported."
   */
  if ((ret = _set_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch,
                                  &comp_code)) != CONFIG_ERR_SUCCESS)
    {
      if (ret == CONFIG_ERR_NON_FATAL_ERROR_INVALID_UNSUPPORTED_CONFIG
          && comp_code == IPMI_COMP_CODE_INVALID_DATA_FIELD_IN_REQUEST)
        ret = CONFIG_ERR_NON_FATAL_ERROR_NOT_SUPPORTED;
      return (ret);
    }

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_enable_per_message_authentication_checkout (const char *section_name,
                                             struct config_keyvalue *kv,
                                             void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  /* achu: Backwards values in this command are handled above */
  if (config_section_update_keyvalue_output (state_data->pstate,
                                             kv,
                                             ch.per_message_authentication ? "Yes" : "No") < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_enable_per_message_authentication_commit (const char *section_name,
                                           const struct config_keyvalue *kv,
                                           void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  ch.per_message_authentication = same (kv->value_input, "yes");

  if ((ret = _set_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch,
                                  NULL)) != CONFIG_ERR_SUCCESS)
    return (ret);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_enable_pef_alerting_checkout (const char *section_name,
                               struct config_keyvalue *kv,
                               void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  /* achu: Backwards values in this command are handled above */
  if (config_section_update_keyvalue_output (state_data->pstate,
                                             kv,
                                             ch.pef_alerting ? "Yes" : "No") < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_enable_pef_alerting_commit (const char *section_name,
                             const struct config_keyvalue *kv,
                             void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  ch.pef_alerting = same (kv->value_input, "yes");

  if ((ret = _set_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch,
                                  NULL)) != CONFIG_ERR_SUCCESS)
    return (ret);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_channel_privilege_limit_checkout (const char *section_name,
                                   struct config_keyvalue *kv,
                                   void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output (state_data->pstate,
                                             kv,
                                             privilege_level_string (ch.channel_privilege_limit)) < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
_channel_privilege_limit_commit (const char *section_name,
                                 const struct config_keyvalue *kv,
                                 void *arg)
{
  bmc_config_state_data_t *state_data;
  struct channel_access ch;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);

  state_data = (bmc_config_state_data_t *)arg;

  if ((ret = _get_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch)) != CONFIG_ERR_SUCCESS)
    return (ret);

  ch.channel_privilege_limit = privilege_level_number (kv->value_input);

  if ((ret = _set_channel_access (state_data,
                                  section_name,
                                  kv->key->key_name,
                                  &ch,
                                  NULL)) != CONFIG_ERR_SUCCESS)
    return (ret);

  return (CONFIG_ERR_SUCCESS);
}

int
bmc_config_channel_common_section_get (bmc_config_state_data_t *state_data,
                                       struct config_section *channel_section)
{
  assert (state_data);
  assert (channel_section);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Volatile_Access_Mode",
                              "Possible values: Disabled/Pre_Boot_Only/Always_Available/Shared",
                              0,
                              _access_mode_checkout,
                              _access_mode_commit,
                              channel_access_mode_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Volatile_Enable_User_Level_Auth",
                              "Possible values: Yes/No",
                              0,
                              _enable_user_level_authentication_checkout,
                              _enable_user_level_authentication_commit,
                              config_yes_no_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Volatile_Enable_Per_Message_Auth",
                              "Possible values: Yes/No",
                              0,
                              _enable_per_message_authentication_checkout,
                              _enable_per_message_authentication_commit,
                              config_yes_no_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Volatile_Enable_Pef_Alerting",
                              "Possible values: Yes/No",
                              0,
                              _enable_pef_alerting_checkout,
                              _enable_pef_alerting_commit,
                              config_yes_no_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Volatile_Channel_Privilege_Limit",
                              "Possible values: Callback/User/Operator/Administrator/OEM_Proprietary",
                              0,
                              _channel_privilege_limit_checkout,
                              _channel_privilege_limit_commit,
                              privilege_level_number_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Non_Volatile_Access_Mode",
                              "Possible values: Disabled/Pre_Boot_Only/Always_Available/Shared",
                              0,
                              _access_mode_checkout,
                              _access_mode_commit,
                              channel_access_mode_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Non_Volatile_Enable_User_Level_Auth",
                              "Possible values: Yes/No",
                              0,
                              _enable_user_level_authentication_checkout,
                              _enable_user_level_authentication_commit,
                              config_yes_no_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Non_Volatile_Enable_Per_Message_Auth",
                              "Possible values: Yes/No",
                              0,
                              _enable_per_message_authentication_checkout,
                              _enable_per_message_authentication_commit,
                              config_yes_no_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Non_Volatile_Enable_Pef_Alerting",
                              "Possible values: Yes/No",
                              0,
                              _enable_pef_alerting_checkout,
                              _enable_pef_alerting_commit,
                              config_yes_no_validate) < 0)
    return (-1);

  if (config_section_add_key (state_data->pstate,
                              channel_section,
                              "Non_Volatile_Channel_Privilege_Limit",
                              "Possible values: Callback/User/Operator/Administrator/OEM_Proprietary",
                              0,
                              _channel_privilege_limit_checkout,
                              _channel_privilege_limit_commit,
                              privilege_level_number_validate) < 0)
    return (-1);

  return (0);
}
