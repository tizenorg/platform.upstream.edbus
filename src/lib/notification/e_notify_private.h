#ifndef E_NOTIFY_PRIVATE_H
#define E_NOTIFY_PRIVATE_H

#define E_NOTIFICATION_BUS_NAME "org.freedesktop.Notifications"
#define E_NOTIFICATION_INTERFACE "org.freedesktop.Notifications"
#define E_NOTIFICATION_PATH "/org/freedesktop/Notifications"

#define e_notification_call_new(member) dbus_message_new_method_call(E_NOTIFICATION_BUS_NAME, E_NOTIFICATION_PATH, E_NOTIFICATION_INTERFACE, member)

typedef void (*E_DBus_Variant_Marshaller) (DBusMessageIter *iter, void *data);
#define E_DBUS_VARIANT_MARSHALLER(x) ((E_DBus_Variant_Marshaller)(x))

void e_notify_marshal_dict_variant(DBusMessageIter *iter, const char *key, char *type_str, E_DBus_Variant_Marshaller func, void *data);
void e_notify_marshal_dict_string(DBusMessageIter *iter, const char *key, const char *value);
void e_notify_marshal_dict_byte(DBusMessageIter *iter, const char *key, char value);
void e_notify_marshal_dict_int(DBusMessageIter *iter, const char *key, int value);

void e_notify_marshal_string_array(DBusMessageIter *iter, const char **strings);
void e_notify_marshal_string_list_as_array(DBusMessageIter *iter, Eina_List *strings);
Eina_List * e_notify_unmarshal_string_array_as_list(DBusMessageIter *iter, DBusError *err);
DBusMessage * e_notify_marshal_get_capabilities();
DBusMessage * e_notify_marshal_get_capabilities_return(DBusMessage *method_call, const char **capabilities);
void * e_notify_unmarshal_get_capabilities_return(DBusMessage *msg, DBusError *err);
void   e_notify_free_get_capabilities_return(void *data);
DBusMessage * e_notify_marshal_get_server_information();
DBusMessage * e_notify_marshal_get_server_information_return(DBusMessage *method_call, const char *name, const char *vendor, const char *version, const char *spec_version);
void * e_notify_unmarshal_get_server_information_return(DBusMessage *msg, DBusError *err);
void   e_notify_free_get_server_information_return(void *data);
DBusMessage * e_notify_marshal_close_notification(dbus_uint32_t id);
dbus_uint32_t e_notify_unmarshal_close_notification(DBusMessage *msg, DBusError *err);
DBusMessage * e_notify_marshal_notification_closed_signal(dbus_uint32_t id, dbus_uint32_t reason);
E_Notification_Event_Notification_Closed * e_notify_unmarshal_notification_closed_signal(DBusMessage *msg, DBusError *err);
DBusMessage * e_notify_marshal_action_invoked_signal(dbus_uint32_t id, const char *action_id);
E_Notification_Event_Action_Invoked * e_notify_unmarshal_action_invoked_signal(DBusMessage *msg, DBusError *err);
DBusMessage * e_notify_marshal_notify(E_Notification *n);
E_Notification * e_notify_unmarshal_notify(DBusMessage *msg, DBusError *err);
DBusMessage * e_notify_marshal_notify_return(DBusMessage *method_call, dbus_uint32_t notification_id);
void * e_notify_unmarshal_notify_return(DBusMessage *msg, DBusError *err);
void   e_notify_free_notify_return(void *data);
void e_notify_unmarshal_notify_actions(E_Notification *n, DBusMessageIter *iter);
void e_notify_unmarshal_notify_hints(E_Notification *n, DBusMessageIter *iter);
void e_notify_marshal_hint_image(DBusMessageIter *iter, E_Notification_Image *img);
E_Notification_Image * e_notify_unmarshal_hint_image(DBusMessageIter *iter);
void loginit(void);
extern int _e_dbus_notify_log_dom;

#ifndef E_DBUS_COLOR_DEFAULT
#define E_DBUS_COLOR_DEFAULT EINA_COLOR_CYAN
#endif

#undef DBG
#undef INF
#undef WRN
#undef ERR

#define DBG(...) EINA_LOG_DOM_DBG(_e_dbus_notify_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_e_dbus_notify_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_e_dbus_notify_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_e_dbus_notify_log_dom, __VA_ARGS__)

struct E_Notification_Image
{
  int   width;
  int   height;
  int   rowstride;
  char  has_alpha;
  int   bits_per_sample;
  int   channels;
  unsigned char *data;
};

struct E_Notification
{
  int id;
  const char *app_name;
  unsigned int replaces_id;
  const char *app_icon;
  const char *summary;
  const char *body;
  int expire_timeout;

  Eina_List *actions;

  struct
  {
    char urgency;
    const char *category;
    const char *desktop;
    const char *sound_file;
    const char *image_path;
    char suppress_sound;
    int x, y;
    E_Notification_Image *image_data;
    E_Notification_Image *icon_data;
  } hints;

  int hint_flags;
  unsigned char closed;

  int refcount;
};

struct E_Notification_Action 
{
  const char *id;
  const char *name;
};

#endif
