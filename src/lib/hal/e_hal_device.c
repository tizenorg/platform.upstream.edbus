#include <stdlib.h>

#include <E_Hal.h>
#include "e_hal_private.h"

#define e_hal_device_call_new(udi, member) dbus_message_new_method_call(E_HAL_SENDER, udi, E_HAL_DEVICE_INTERFACE, member)
#define e_hal_device_volume_call_new(udi, member) dbus_message_new_method_call(E_HAL_SENDER, udi, E_HAL_DEVICE_VOLUME_INTERFACE, member)

#if 0
static void cb_device_get_property(void *data, DBusMessage *msg, DBusError *err);
static void cb_device_get_all_properties(void *data, DBusMessage *msg, DBusError *err);
static void cb_device_query_capability(void *data, DBusMessage *msg, DBusError *err);
#endif

/* Device.GetProperty */
static void *
unmarshal_device_get_property(DBusMessage *msg, DBusError *err)
{
  E_Hal_Device_Get_Property_Return *ret = NULL;
  DBusMessageIter iter;
  int type;
  const char *str;

  ret = calloc(1, sizeof(E_Hal_Device_Get_Property_Return));
  if (!ret) 
  {
    dbus_set_error(err, DBUS_ERROR_NO_MEMORY, "");
    return NULL;
  }

  dbus_message_iter_init(msg, &iter);
  type = dbus_message_iter_get_arg_type(&iter);
  switch(type)
  {
    case DBUS_TYPE_STRING:
      ret->type = E_HAL_PROPERTY_TYPE_STRING;
      dbus_message_iter_get_basic(&iter, &str);
      ret->val.s = eina_stringshare_add(str);
      break;
    case DBUS_TYPE_INT32:
      ret->type = E_HAL_PROPERTY_TYPE_INT;
      dbus_message_iter_get_basic(&iter, &(ret->val.i));
      break;
    case DBUS_TYPE_BOOLEAN:
      ret->type = E_HAL_PROPERTY_TYPE_BOOL;
      dbus_message_iter_get_basic(&iter, &(ret->val.b));
      break;
    case DBUS_TYPE_DOUBLE:
      ret->type = E_HAL_PROPERTY_TYPE_DOUBLE;
      dbus_message_iter_get_basic(&iter, &(ret->val.d));
      break;
  }

  return ret;
}

static void 
free_device_get_property(void *data)
{
  E_Hal_Device_Get_Property_Return *ret = data;

  if (!ret) return;
  if (ret->type == E_HAL_PROPERTY_TYPE_STRLIST)
  {
    const char *str;
    EINA_LIST_FREE(ret->val.strlist, str)
      eina_stringshare_del(str);
  }
  else if (ret->type == E_HAL_PROPERTY_TYPE_STRING)
    eina_stringshare_del(ret->val.s);
  free(ret);
}

EAPI DBusPendingCall *
e_hal_device_get_property(E_DBus_Connection *conn, const char *udi, const char *property, E_DBus_Callback_Func cb_func, void *data)
{
  DBusMessage *msg;
  DBusPendingCall *ret;

  msg = e_hal_device_call_new(udi, "GetProperty");
  dbus_message_append_args(msg, DBUS_TYPE_STRING, &property, DBUS_TYPE_INVALID);
  ret = e_dbus_method_call_send(conn, msg, unmarshal_device_get_property, cb_func, free_device_get_property, -1, data);
  dbus_message_unref(msg);
  return ret;
}

/* Device.GetAllProperties */

static void *
unmarshal_device_get_all_properties(DBusMessage *msg, DBusError *err)
{
  E_Hal_Device_Get_All_Properties_Return *ret = NULL;
  DBusMessageIter iter, a_iter, s_iter, v_iter;

  /* a{sv} = array of string+variant */
  if (!dbus_message_has_signature(msg, "a{sv}")) 
  {
    dbus_set_error(err, DBUS_ERROR_INVALID_SIGNATURE, "");
    return NULL;
  }

  ret = calloc(1, sizeof(E_Hal_Device_Get_All_Properties_Return));
  if (!ret) 
  {
    dbus_set_error(err, DBUS_ERROR_NO_MEMORY, "");
    return NULL;
  }

  ret->properties = eina_hash_string_small_new(EINA_FREE_CB(e_hal_property_free));

  dbus_message_iter_init(msg, &iter);
  dbus_message_iter_recurse(&iter, &a_iter);
  while (dbus_message_iter_get_arg_type(&a_iter) != DBUS_TYPE_INVALID)
  {
    const char *name;
    const char *str;
    E_Hal_Property *prop = calloc(1, sizeof(E_Hal_Property));
    dbus_message_iter_recurse(&a_iter, &s_iter);
    dbus_message_iter_get_basic(&s_iter, &name);
    dbus_message_iter_next(&s_iter);
    dbus_message_iter_recurse(&s_iter, &v_iter);
    
    switch(dbus_message_iter_get_arg_type(&v_iter))
    {
      case DBUS_TYPE_STRING:
        prop->type = E_HAL_PROPERTY_TYPE_STRING;
        dbus_message_iter_get_basic(&v_iter, &str);
        prop->val.s = eina_stringshare_add(str);
        break;
      case DBUS_TYPE_INT32:
        prop->type = E_HAL_PROPERTY_TYPE_INT;
        dbus_message_iter_get_basic(&v_iter, &(prop->val.i));
        break;
      case DBUS_TYPE_UINT64:
        prop->type = E_HAL_PROPERTY_TYPE_UINT64;
        dbus_message_iter_get_basic(&v_iter, &(prop->val.u64));
        break;
      case DBUS_TYPE_BOOLEAN:
        prop->type = E_HAL_PROPERTY_TYPE_BOOL;
        dbus_message_iter_get_basic(&v_iter, &(prop->val.b));
        break;
      case DBUS_TYPE_DOUBLE:
        prop->type = E_HAL_PROPERTY_TYPE_DOUBLE;
        dbus_message_iter_get_basic(&v_iter, &(prop->val.d));
        break;
      case DBUS_TYPE_ARRAY:
        prop->type = E_HAL_PROPERTY_TYPE_STRLIST;
        {
          DBusMessageIter list_iter;
          prop->val.strlist = NULL;
          dbus_message_iter_recurse(&v_iter, &list_iter);
          while (dbus_message_iter_get_arg_type(&list_iter) != DBUS_TYPE_INVALID)
          {
            dbus_message_iter_get_basic(&list_iter, &str);
            prop->val.strlist = eina_list_append(prop->val.strlist, eina_stringshare_add(str));
            dbus_message_iter_next(&list_iter);
          }
        }
        break;
      default:
        WARN("Ehal Error: unexpected property type (%s): %c", name, dbus_message_iter_get_arg_type(&v_iter));
        break;
    }
    eina_hash_add(ret->properties, name, prop);

    dbus_message_iter_next(&a_iter);
  }

  return ret;
}

static void
free_device_get_all_properties(void *data)
{
  E_Hal_Device_Get_All_Properties_Return *ret = data;

  if (!ret) return;
  eina_hash_free(ret->properties);
  free(ret);
}

EAPI DBusPendingCall *
e_hal_device_get_all_properties(E_DBus_Connection *conn, const char *udi, E_DBus_Callback_Func cb_func, void *data)
{
  DBusMessage *msg;
  DBusPendingCall *ret;

  msg = e_hal_device_call_new(udi, "GetAllProperties");
  ret = e_dbus_method_call_send(conn, msg, unmarshal_device_get_all_properties, cb_func, free_device_get_all_properties, -1, data);
  dbus_message_unref(msg);
  return ret;
}



/* bool Device.QueryCapability(string udi) */

// XXX this is same as Device_Exists in manager.
static void *
unmarshal_device_query_capability(DBusMessage *msg, DBusError *err)
{
  E_Hal_Device_Query_Capability_Return *ret = NULL;
  dbus_bool_t val;

  ret = calloc(1, sizeof(E_Hal_Manager_Device_Exists_Return));
  if (!ret) 
  {
    dbus_set_error(err, DBUS_ERROR_NO_MEMORY, "");
    return NULL;
  }

  dbus_message_get_args(msg, err, DBUS_TYPE_BOOLEAN, &val, DBUS_TYPE_INVALID);

  if (dbus_error_is_set(err))
  {
    free(ret);
    return NULL;
  }

  ret->boolean = val;
  return ret;
}

static void
free_device_query_capability(void *data)
{
  E_Hal_Device_Query_Capability_Return *ret = data;

  if (!ret) return;
  free(ret);
}

EAPI DBusPendingCall *
e_hal_device_query_capability(E_DBus_Connection *conn, const char *udi, const char *capability, E_DBus_Callback_Func cb_func, void *data)
{
  DBusMessage *msg;
  DBusPendingCall *ret;

  msg = e_hal_device_call_new(udi, "QueryCapability");
  dbus_message_append_args(msg, DBUS_TYPE_STRING, &capability, DBUS_TYPE_INVALID);
  ret = e_dbus_method_call_send(conn, msg, unmarshal_device_query_capability, cb_func, free_device_query_capability, -1, data);
  dbus_message_unref(msg);
  return ret;
}



/* void Device.Mount(string mount_point, string fstype, array{string}options) */

/**
 * @brief Mount a Volume
 *
 * @param conn the E_DBus_Connection
 * @param udi the udi of the device object
 * @param mount_point the path to mount to, or null for default
 * @param fstype the fstype of the device (e.g. volume.fstype property)
 * @param options a list of additional options (not sure... fstype dependent?)
 * @param cb_func an optional callback to call when the mount is done
 * @param data custom data pointer for the callback function
 */
EAPI DBusPendingCall *
e_hal_device_volume_mount(E_DBus_Connection *conn, const char *udi, const char *mount_point, const char *fstype, Eina_List *options, E_DBus_Callback_Func cb_func, void *data)
{
  DBusMessage *msg;
  DBusMessageIter iter, subiter;
  Eina_List *l;
  DBusPendingCall *ret;

  msg = e_hal_device_volume_call_new(udi, "Mount");

  dbus_message_iter_init_append(msg, &iter);
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &mount_point);
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &fstype);
  if (dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &subiter))
  {
    if (options)
    {
      const char *opt;
       
      EINA_LIST_FOREACH(options, l, opt)
      {
        dbus_message_iter_append_basic(&subiter, DBUS_TYPE_STRING, &opt);
      }
    }
    dbus_message_iter_close_container(&iter, &subiter);
  }
  else
  {
    ERR("dbus_message_iter_open_container() failed");
  }

  ret = e_dbus_method_call_send(conn, msg, NULL, cb_func, NULL, -1, data);
  dbus_message_unref(msg);
  return ret;
}

/* void Unmount(array{string} options) */

/**
 * @brief Unmount a Volume
 *
 * @param conn the E_DBus_Connection
 * @param udi the udi of the device object
 * @param options a list of additional options (not sure... fstype dependent?)
 * @param cb_func an optional callback to call when the unmount is done
 * @param data cuatom data pointer for the callback function
 */
EAPI DBusPendingCall *
e_hal_device_volume_unmount(E_DBus_Connection *conn, const char *udi, Eina_List *options, E_DBus_Callback_Func cb_func, void *data)
{
  DBusMessage *msg;
  DBusMessageIter iter, subiter;
  Eina_List *l;
  DBusPendingCall *ret;

  msg = e_hal_device_volume_call_new(udi, "Unmount");

  dbus_message_iter_init_append(msg, &iter);
  if (dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &subiter))
  {
    if (options)
    {
      const char *opt;
       
      EINA_LIST_FOREACH(options, l, opt)
      {
        dbus_message_iter_append_basic(&subiter, DBUS_TYPE_STRING, &opt);
      }
    }
    dbus_message_iter_close_container(&iter, &subiter);
  }
  else
  {
    ERR("dbus_message_iter_open_container() failed");
  }

  ret = e_dbus_method_call_send(conn, msg, NULL, cb_func, NULL, -1, data);
  dbus_message_unref(msg);
  return ret;
}

/**
 * @brief Eject a Volume
 *
 * @param conn the E_DBus_Connection
 * @param udi the udi of the device object
 * @param options a list of additional options (not sure... fstype dependent?)
 * @param cb_func an optional callback to call when the eject is done
 * @param data cuatom data pointer for the callback function
 */
EAPI DBusPendingCall *
e_hal_device_volume_eject(E_DBus_Connection *conn, const char *udi, Eina_List *options, E_DBus_Callback_Func cb_func, void *data)
{
  DBusMessage *msg;
  DBusMessageIter iter, subiter;
  Eina_List *l;
  DBusPendingCall *ret;

  msg = e_hal_device_volume_call_new(udi, "Eject");

  dbus_message_iter_init_append(msg, &iter);
  if (dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &subiter))
  {
    if (options)
    {
      const char *opt;
       
      EINA_LIST_FOREACH(options, l, opt)
      {
        dbus_message_iter_append_basic(&subiter, DBUS_TYPE_STRING, &opt);
      }
    }
    dbus_message_iter_close_container(&iter, &subiter);
  }
  else
  {
    ERR("dbus_message_iter_open_container() failed");
  }

  ret = e_dbus_method_call_send(conn, msg, NULL, cb_func, NULL, -1, data);
  dbus_message_unref(msg);
  return ret;
}
