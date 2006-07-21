
#include <stdio.h>
#include "bmc-diff.h"
#include "bmc-sections.h"
#include "bmc-config-api.h"

static char *
channel_access_mode_string (uint8_t mode)
{
  switch (mode) {
  case IPMI_MESSAGING_ACCESS_MODE_DISABLED:
    return "Disabled";
  case IPMI_MESSAGING_ACCESS_MODE_PRE_BOOT_ONLY:
    return "Pre_Boot_Only";
  case IPMI_MESSAGING_ACCESS_MODE_ALWAYS_AVAILABLE:
    return "Always_Available";
  case IPMI_MESSAGING_ACCESS_MODE_SHARED:
    return "Shared";
  }
  return "";
}

static int
channel_access_mode (const char *string)
{
  if (same (string, "disabled"))
    return IPMI_MESSAGING_ACCESS_MODE_DISABLED;
  if (same (string, "pre_boot_only"))
    return IPMI_MESSAGING_ACCESS_MODE_PRE_BOOT_ONLY;
  if (same (string, "always_available"))
    return IPMI_MESSAGING_ACCESS_MODE_ALWAYS_AVAILABLE;
  if (same (string, "shared"))
    return IPMI_MESSAGING_ACCESS_MODE_SHARED;
  return -1;
}

static char *
get_privilege_limit_string (uint8_t limit)
{
  switch (limit) {
  case IPMI_PRIVILEGE_LEVEL_CALLBACK:
    return "Callback";
  case IPMI_PRIVILEGE_LEVEL_USER:
    return "User";
  case IPMI_PRIVILEGE_LEVEL_OPERATOR:
    return "Operator";
  case IPMI_PRIVILEGE_LEVEL_ADMIN:
    return "Administrator";
  case IPMI_PRIVILEGE_LEVEL_OEM:
    return "OEM_Proprietary";
  case IPMI_PRIVILEGE_LEVEL_NO_ACCESS:
    return "NO_Access";
  }
  return "";
}

static uint8_t
get_privilege_limit_number (const char *value)
{
  if (same (value, "callback"))
    return IPMI_PRIVILEGE_LEVEL_CALLBACK;
  if (same (value, "user"))
    return IPMI_PRIVILEGE_LEVEL_USER;
  if (same (value, "operator"))
    return IPMI_PRIVILEGE_LEVEL_OPERATOR;
  if (same (value, "administrator"))
    return IPMI_PRIVILEGE_LEVEL_ADMIN;
  if (same (value, "oem_proprietary"))
    return IPMI_PRIVILEGE_LEVEL_OEM;
  if (same (value, "no_access"))
    return IPMI_PRIVILEGE_LEVEL_NO_ACCESS;
  return 0;
}


/* volatile */

static int
lan_channel_volatile_access_set (ipmi_device_t *dev,
				 uint8_t access_mode,
				 uint8_t access_mode_is_set,
				 uint8_t user_level_authentication,
				 uint8_t user_level_authentication_is_set,
				 uint8_t per_message_authentication,
				 uint8_t per_message_authentication_is_set,
				 uint8_t pef_alerting,
				 uint8_t pef_alerting_is_set,
				 uint8_t channel_privilege_limit,
				 uint8_t channel_privilege_limit_is_set)
{
  uint8_t tmp_access_mode;
  uint8_t tmp_user_level_authentication;
  uint8_t tmp_per_message_authentication;
  uint8_t tmp_pef_alerting;
  uint8_t tmp_channel_privilege_limit;
  int ret;
  
  ret = get_bmc_lan_channel_volatile_access (dev,
					     &tmp_access_mode,
					     &tmp_user_level_authentication,
					     &tmp_per_message_authentication,
					     &tmp_pef_alerting,
					     &tmp_channel_privilege_limit);
  if (ret != 0)
    return -1;

  if (access_mode_is_set)
    tmp_access_mode = access_mode;
  if (user_level_authentication_is_set)
    tmp_user_level_authentication = user_level_authentication;
  if (per_message_authentication_is_set)
    tmp_per_message_authentication = per_message_authentication;
  if (pef_alerting_is_set)
    tmp_pef_alerting = pef_alerting;
  if (channel_privilege_limit_is_set)
    tmp_channel_privilege_limit = channel_privilege_limit;

  ret = set_bmc_lan_channel_volatile_access (dev,
					     tmp_access_mode,
					     tmp_user_level_authentication,
					     tmp_per_message_authentication,
					     tmp_pef_alerting,
					     tmp_channel_privilege_limit);

  if (ret != 0)
    return -1;

  return 0;
}


/* access_mode */

static int
volatile_access_mode_checkout (const struct arguments *args,
			       const struct section *sect,
			       struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &get_val,
					     &foo,
					     &foo,
					     &foo,
					     &foo);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);
  kv->value = strdup (channel_access_mode_string (get_val));
  return 0;
}

static int
volatile_access_mode_commit (const struct arguments *args,
			     const struct section *sect,
			     const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = channel_access_mode (kv->value);
  return lan_channel_volatile_access_set ((ipmi_device_t *)&args->dev,
					  commit_val, 1,
					  0, 0,
					  0, 0,
					  0, 0,
					  0, 0);
}

static int
volatile_access_mode_diff (const struct arguments *args,
			   const struct section *sect,
			   const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &get_val,
					     &foo,
					     &foo,
					     &foo,
					     &foo);
  if (ret != 0)
    return -1;

  passed_val = channel_access_mode (kv->value);

  if (passed_val == get_val) {
    ret = 0; 
  } else {
    report_diff (sect->section, 
		 kv->key,
		 kv->value,
		 channel_access_mode_string (get_val));
    ret = 1;
  }

  return ret;
}

static int
volatile_access_mode_validate (const struct arguments *args,
			       const struct section *sect,
			       const char *value)
{
  return (channel_access_mode (value) >= 0) ? 0 : 1;
}


/* enable_user_level_auth */

static int
volatile_enable_user_level_auth_checkout (const struct arguments *args,
			       const struct section *sect,
			       struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &get_val,
					     &foo,
					     &foo,
					     &foo);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);
  if (get_val)
    kv->value = strdup ("Yes");
  else
    kv->value = strdup ("No");
  return 0;
}

static int
volatile_enable_user_level_auth_commit (const struct arguments *args,
					const struct section *sect,
					const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = same (kv->value, "yes");
  return lan_channel_volatile_access_set ((ipmi_device_t *)&args->dev,
					  0, 0,
					  commit_val, 1,
					  0, 0,
					  0, 0,
					  0, 0);
}

static int
volatile_enable_user_level_auth_diff (const struct arguments *args,
				      const struct section *sect,
				      const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &get_val,
					     &foo,
					     &foo,
					     &foo);
  if (ret != 0)
    return -1;

  passed_val = same (kv->value, "yes");
  if (passed_val == get_val) {
    ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_val ? "Yes" : "No");
    ret = 1;
  }
  return ret;
}

static int
volatile_enable_user_level_auth_validate (const struct arguments *args,
					  const struct section *sect,
					  const char *value)
{
  return (value && (same (value, "yes") || same (value, "no"))) ? 0 : 1;
}


/* enable_per_message_auth */

static int
volatile_enable_per_msg_auth_checkout (const struct arguments *args,
				       const struct section *sect,
				       struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &foo,
					     &get_val,
					     &foo,
					     &foo);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);
  if (get_val)
    kv->value = strdup ("Yes");
  else
    kv->value = strdup ("No");
  return 0;
}

static int
volatile_enable_per_msg_auth_commit (const struct arguments *args,
				     const struct section *sect,
				     const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = same (kv->value, "yes");
  return lan_channel_volatile_access_set ((ipmi_device_t *)&args->dev,
					  0, 0,
					  0, 0,
					  commit_val, 1,
					  0, 0,
					  0, 0);
}

static int
volatile_enable_per_msg_auth_diff (const struct arguments *args,
				   const struct section *sect,
				   const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &foo,
					     &get_val,
					     &foo,
					     &foo);
  if (ret != 0)
    return -1;

  passed_val = same (kv->value, "yes");

  if (passed_val == get_val) {
     ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_val ? "Yes" : "No");
    ret = 1;
  }

  return ret;
}

static int
volatile_enable_per_msg_auth_validate (const struct arguments *args,
					  const struct section *sect,
					  const char *value)
{
  return (value && (same (value, "yes") || same (value, "no"))) ? 0 : 1;
}


/* enable_pef_alerting */


static int
volatile_enable_pef_alerting_checkout (const struct arguments *args,
				       const struct section *sect,
				       struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &foo,
					     &foo,
					     &get_val,
					     &foo);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);
  if (get_val)
    kv->value = strdup ("Yes");
  else
    kv->value = strdup ("No");
  return 0;
}

static int
volatile_enable_pef_alerting_commit (const struct arguments *args,
				     const struct section *sect,
				     const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = same (kv->value, "yes");
  return lan_channel_volatile_access_set ((ipmi_device_t *)&args->dev,
					  0, 0,
					  0, 0,
					  0, 0,
					  commit_val, 1,
					  0, 0);
}

static int
volatile_enable_pef_alerting_diff (const struct arguments *args,
				   const struct section *sect,
				   const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &foo,
					     &foo,
					     &get_val,
					     &foo);
  if (ret != 0)
    return -1;

  passed_val = same (kv->value, "yes");

  if (passed_val == get_val) {
     ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_val ? "Yes" : "No");
    ret = 1;
  }

  return ret;
}

static int
volatile_enable_pef_alerting_validate (const struct arguments *args,
				       const struct section *sect,
				       const char *value)
{
  return (value && (same (value, "yes") || same (value, "no"))) ? 0 : 1;
}

/* channel_privilege_level */


static int
volatile_channel_priv_limit_checkout (const struct arguments *args,
				      const struct section *sect,
				      struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &foo,
					     &foo,
					     &foo,
					     &get_val);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);

  kv->value = strdup (get_privilege_limit_string (get_val));

  return 0;
}

static int
volatile_channel_priv_limit_commit (const struct arguments *args,
				    const struct section *sect,
				    const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = get_privilege_limit_number (kv->value);
  return lan_channel_volatile_access_set ((ipmi_device_t *)&args->dev,
					  0, 0,
					  0, 0,
					  0, 0,
					  0, 0,
					  commit_val, 1);
}

static int
volatile_channel_priv_limit_diff (const struct arguments *args,
				  const struct section *sect,
				  const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &foo,
					     &foo,
					     &foo,
					     &get_val);
  if (ret != 0)
    return -1;

  passed_val = get_privilege_limit_number (kv->value);

  if (passed_val == get_val) {
     ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_privilege_limit_string (get_val));
    ret = 1;
  }

  return ret;
}

static int
volatile_channel_priv_limit_validate (const struct arguments *args,
				      const struct section *sect,
				      const char *value)
{
  int level = get_privilege_limit_number (value);
  return (level > 0) ? 0 : 1;
}


/* non volatile */


static int
lan_channel_non_volatile_access_set (ipmi_device_t *dev,
				 uint8_t access_mode,
				 uint8_t access_mode_is_set,
				 uint8_t user_level_authentication,
				 uint8_t user_level_authentication_is_set,
				 uint8_t per_message_authentication,
				 uint8_t per_message_authentication_is_set,
				 uint8_t pef_alerting,
				 uint8_t pef_alerting_is_set,
				 uint8_t channel_privilege_limit,
				 uint8_t channel_privilege_limit_is_set)
{
  uint8_t tmp_access_mode;
  uint8_t tmp_user_level_authentication;
  uint8_t tmp_per_message_authentication;
  uint8_t tmp_pef_alerting;
  uint8_t tmp_channel_privilege_limit;
  int ret;
  
  ret = get_bmc_lan_channel_non_volatile_access (dev,
					     &tmp_access_mode,
					     &tmp_user_level_authentication,
					     &tmp_per_message_authentication,
					     &tmp_pef_alerting,
					     &tmp_channel_privilege_limit);
  if (ret != 0)
    return -1;

  if (access_mode_is_set)
    tmp_access_mode = access_mode;
  if (user_level_authentication_is_set)
    tmp_user_level_authentication = user_level_authentication;
  if (per_message_authentication_is_set)
    tmp_per_message_authentication = per_message_authentication;
  if (pef_alerting_is_set)
    tmp_pef_alerting = pef_alerting;
  if (channel_privilege_limit_is_set)
    tmp_channel_privilege_limit = channel_privilege_limit;

  ret = set_bmc_lan_channel_non_volatile_access (dev,
					     tmp_access_mode,
					     tmp_user_level_authentication,
					     tmp_per_message_authentication,
					     tmp_pef_alerting,
					     tmp_channel_privilege_limit);

  if (ret != 0)
    return -1;

  return 0;
}


/* access_mode */

static int
non_volatile_access_mode_checkout (const struct arguments *args,
			       const struct section *sect,
			       struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
					     &get_val,
					     &foo,
					     &foo,
					     &foo,
					     &foo);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);
  kv->value = strdup (channel_access_mode_string (get_val));
  return 0;
}

static int
non_volatile_access_mode_commit (const struct arguments *args,
				 const struct section *sect,
				 const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = channel_access_mode (kv->value);
  return lan_channel_non_volatile_access_set ((ipmi_device_t *)&args->dev,
					  commit_val, 1,
					  0, 0,
					  0, 0,
					  0, 0,
					  0, 0);
}

static int
non_volatile_access_mode_diff (const struct arguments *args,
			       const struct section *sect,
			       const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
						 &get_val,
						 &foo,
						 &foo,
						 &foo,
						 &foo);
  if (ret != 0)
    return -1;

  passed_val = channel_access_mode (kv->value);
  if (passed_val == get_val) {
     ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_val ? "Yes" : "No");
    ret = 1;
  }

  return ret;
}

static int
non_volatile_access_mode_validate (const struct arguments *args,
				   const struct section *sect,
				   const char *value)
{
  return (channel_access_mode (value) >= 0) ? 0 : 1;
}


/* enable_user_level_auth */

static int
non_volatile_enable_user_level_auth_checkout (const struct arguments *args,
					      const struct section *sect,
					      struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
						 &foo,
						 &get_val,
						 &foo,
						 &foo,
						 &foo);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);
  if (get_val)
    kv->value = strdup ("Yes");
  else
    kv->value = strdup ("No");
  return 0;
}

static int
non_volatile_enable_user_level_auth_commit (const struct arguments *args,
					    const struct section *sect,
					    const struct keyvalue *kv)
{
  uint8_t commit_val;
  
  commit_val = same (kv->value, "yes");
  return lan_channel_non_volatile_access_set ((ipmi_device_t *)&args->dev,
					      0, 0,
					      commit_val, 1,
					      0, 0,
					      0, 0,
					      0, 0);
}

static int
non_volatile_enable_user_level_auth_diff (const struct arguments *args,
				      const struct section *sect,
				      const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
						 &foo,
						 &get_val,
						 &foo,
						 &foo,
						 &foo);
  if (ret != 0)
    return -1;

  passed_val = same (kv->value, "yes");
  if (passed_val == get_val) {
     ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_val ? "Yes" : "No");
    ret = 1;
  }

  return ret;
}

static int
non_volatile_enable_user_level_auth_validate (const struct arguments *args,
					      const struct section *sect,
					      const char *value)
{
  return (value && (same (value, "yes") || same (value, "no"))) ? 0 : 1;
}


/* enable_per_message_auth */

static int
non_volatile_enable_per_msg_auth_checkout (const struct arguments *args,
					   const struct section *sect,
					   struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
						 &foo,
						 &foo,
						 &get_val,
						 &foo,
						 &foo);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);
  if (get_val)
    kv->value = strdup ("Yes");
  else
    kv->value = strdup ("No");
  return 0;
}

static int
non_volatile_enable_per_msg_auth_commit (const struct arguments *args,
					 const struct section *sect,
					 const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = same (kv->value, "yes");
  return lan_channel_non_volatile_access_set ((ipmi_device_t *)&args->dev,
					      0, 0,
					      0, 0,
					      commit_val, 1,
					      0, 0,
					      0, 0);
}

static int
non_volatile_enable_per_msg_auth_diff (const struct arguments *args,
				       const struct section *sect,
				       const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &foo,
					     &get_val,
					     &foo,
					     &foo);
  if (ret != 0)
    return -1;

  passed_val = same (kv->value, "yes");

  if (passed_val == get_val) {
     ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_val ? "Yes" : "No");
    ret = 1;
  }

  return ret;
}

static int
non_volatile_enable_per_msg_auth_validate (const struct arguments *args,
					  const struct section *sect,
					  const char *value)
{
  return (value && (same (value, "yes") || same (value, "no"))) ? 0 : 1;
}


/* enable_pef_alerting */


static int
non_volatile_enable_pef_alerting_checkout (const struct arguments *args,
					   const struct section *sect,
					   struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
						 &foo,
						 &foo,
						 &foo,
						 &get_val,
						 &foo);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);
  if (get_val)
    kv->value = strdup ("Yes");
  else
    kv->value = strdup ("No");
  return 0;
}

static int
non_volatile_enable_pef_alerting_commit (const struct arguments *args,
					 const struct section *sect,
					 const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = same (kv->value, "yes");
  return lan_channel_non_volatile_access_set ((ipmi_device_t *)&args->dev,
					      0, 0,
					      0, 0,
					      0, 0,
					      commit_val, 1,
					      0, 0);
}

static int
non_volatile_enable_pef_alerting_diff (const struct arguments *args,
				       const struct section *sect,
				       const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
						 &foo,
						 &foo,
						 &foo,
						 &get_val,
						 &foo);
  if (ret != 0)
    return -1;

  passed_val = same (kv->value, "yes");

  if (passed_val == get_val) {
     ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_val ? "Yes" : "No");
    ret = 1;
  }

  return ret;
}

static int
non_volatile_enable_pef_alerting_validate (const struct arguments *args,
					   const struct section *sect,
					   const char *value)
{
  return (value && (same (value, "yes") || same (value, "no"))) ? 0 : 1;
}

/* channel_privilege_level */


static int
non_volatile_channel_priv_limit_checkout (const struct arguments *args,
					  const struct section *sect,
					  struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  int ret;
  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
					     &foo,
					     &foo,
					     &foo,
					     &foo,
					     &get_val);
  if (ret != 0)
    return ret;

  if (kv->value)
    free (kv->value);

  kv->value = strdup (get_privilege_limit_string (get_val));

  return 0;
}

static int
non_volatile_channel_priv_limit_commit (const struct arguments *args,
					const struct section *sect,
					const struct keyvalue *kv)
{
  uint8_t commit_val;

  commit_val = get_privilege_limit_number (kv->value);
  return lan_channel_non_volatile_access_set ((ipmi_device_t *)&args->dev,
					      0, 0,
					      0, 0,
					      0, 0,
					      0, 0,
					      commit_val, 1);
}

static int
non_volatile_channel_priv_limit_diff (const struct arguments *args,
				      const struct section *sect,
				      const struct keyvalue *kv)
{
  uint8_t get_val;
  uint8_t foo;
  uint8_t passed_val;
  int ret;

  ret = get_bmc_lan_channel_non_volatile_access ((ipmi_device_t *)&args->dev,
						 &foo,
						 &foo,
						 &foo,
						 &foo,
						 &get_val);
  if (ret != 0)
    return -1;

  passed_val = get_privilege_limit_number (kv->value);
  
  if (passed_val == get_val) {
     ret = 0;
  } else {
    report_diff (sect->section,
		 kv->key,
		 kv->value,
		 get_privilege_limit_string (get_val));
    ret = 1;
  }

  return ret;
}

static int
non_volatile_channel_priv_limit_validate (const struct arguments *args,
					  const struct section *sect,
					  const char *value)
{
  int level = get_privilege_limit_number (value);
  return (level > 0) ? 0 : 1;
}


struct section *
bmc_lan_channel_section_get (struct arguments *args)
{
  struct section * lan_channel_section = NULL;

  lan_channel_section = (void *) calloc (1, sizeof (struct section));
  lan_channel_section->section = strdup ("LAN_Channel");

  add_keyvalue (lan_channel_section,
		"Volatile_Access_Mode",
		"Possible values: Disabled/Pre_Boot_Only/Always_Available/Shared",
                0,
		volatile_access_mode_checkout,
		volatile_access_mode_commit,
		volatile_access_mode_diff,
		volatile_access_mode_validate);

  add_keyvalue (lan_channel_section,
		"Volatile_Enable_User_Level_Auth",
		"Possible values: Yes/No",
                0,
		volatile_enable_user_level_auth_checkout,
		volatile_enable_user_level_auth_commit,
		volatile_enable_user_level_auth_diff,
		volatile_enable_user_level_auth_validate);

  add_keyvalue (lan_channel_section,
		"Volatile_Enable_Per_Message_Auth",
		"Possible values: Yes/No",
                0,
		volatile_enable_per_msg_auth_checkout,
		volatile_enable_per_msg_auth_commit,
		volatile_enable_per_msg_auth_diff,
		volatile_enable_per_msg_auth_validate);

  add_keyvalue (lan_channel_section,
		"Volatile_Enable_Pef_Alerting",
		"Possible values: Yes/No",
                0,
		volatile_enable_pef_alerting_checkout,
		volatile_enable_pef_alerting_commit,
		volatile_enable_pef_alerting_diff,
		volatile_enable_pef_alerting_validate);

  add_keyvalue (lan_channel_section,
		"Volatile_Channel_Privilege_Limit",
		"Possible values: Callback/User/Operator/Administrator/OEM_Proprietary/NO_Access",
                0,
		volatile_channel_priv_limit_checkout,
		volatile_channel_priv_limit_commit,
		volatile_channel_priv_limit_diff,
		volatile_channel_priv_limit_validate);

  add_keyvalue (lan_channel_section,
		"Non_Volatile_Access_Mode",
		"Possible values: Disabled/Pre_Boot_Only/Always_Available/Shared",
                0,
		non_volatile_access_mode_checkout,
		non_volatile_access_mode_commit,
		non_volatile_access_mode_diff,
		non_volatile_access_mode_validate);

  add_keyvalue (lan_channel_section,
		"Non_Volatile_Enable_User_Level_Auth",
		"Possible values: Yes/No",
                0,
		non_volatile_enable_user_level_auth_checkout,
		non_volatile_enable_user_level_auth_commit,
		non_volatile_enable_user_level_auth_diff,
		non_volatile_enable_user_level_auth_validate);

  add_keyvalue (lan_channel_section,
		"Non_Volatile_Enable_Per_Message_Auth",
		"Possible values: Yes/No",
                0,
		non_volatile_enable_per_msg_auth_checkout,
		non_volatile_enable_per_msg_auth_commit,
		non_volatile_enable_per_msg_auth_diff,
		non_volatile_enable_per_msg_auth_validate);

  add_keyvalue (lan_channel_section,
		"Non_Volatile_Enable_Pef_Alerting",
		"Possible values: Yes/No",
                0,
		non_volatile_enable_pef_alerting_checkout,
		non_volatile_enable_pef_alerting_commit,
		non_volatile_enable_pef_alerting_diff,
		non_volatile_enable_pef_alerting_validate);

  add_keyvalue (lan_channel_section,
		"Non_Volatile_Channel_Privilege_Limit",
		"Possible values: Callback/User/Operator/Administrator/OEM_Proprietary/NO_Access",
                0,
		non_volatile_channel_priv_limit_checkout,
		non_volatile_channel_priv_limit_commit,
		non_volatile_channel_priv_limit_diff,
		non_volatile_channel_priv_limit_validate);

  return lan_channel_section;
}

