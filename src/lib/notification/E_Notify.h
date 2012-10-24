#ifndef E_NOTIFY_H
#define E_NOTIFY_H

#include <Eina.h>
#include <Evas.h>
#include <E_DBus.h>

/**
 * @defgroup ENotify_Group ENotify
 *
 * @{
 */


/* notifications */
typedef struct E_Notification_Image E_Notification_Image;
typedef struct E_Notification E_Notification;
typedef struct E_Notification_Action E_Notification_Action;

typedef enum E_Notification_Urgency E_Notification_Urgency;
typedef enum E_Notification_Hint_Type E_Notification_Hint_Type;
typedef enum E_Notification_Closed_Reason E_Notification_Closed_Reason;

/* method returns */
typedef struct E_Notification_Return_Notify E_Notification_Return_Notify;
typedef struct E_Notification_Return_Get_Capabilities E_Notification_Return_Get_Capabilities;
typedef struct E_Notification_Return_Get_Server_Information E_Notification_Return_Get_Server_Information;


/* signal events */
typedef struct E_Notification_Event_Action_Invoked E_Notification_Event_Action_Invoked;
typedef struct E_Notification_Event_Notification_Closed E_Notification_Event_Notification_Closed;

/* enums */

enum E_Notification_Urgency
{
  E_NOTIFICATION_URGENCY_LOW,
  E_NOTIFICATION_URGENCY_NORMAL,
  E_NOTIFICATION_URGENCY_CRITICAL
};

enum E_Notification_Closed_Reason
{
  E_NOTIFICATION_CLOSED_EXPIRED,
  E_NOTIFICATION_CLOSED_DISMISSED,
  E_NOTIFICATION_CLOSED_REQUESTED,
  E_NOTIFICATION_CLOSED_UNDEFINED
};

enum E_Notification_Hint_Type
{
  E_NOTIFICATION_HINT_URGENCY        = (1 << 0),
  E_NOTIFICATION_HINT_CATEGORY       = (1 << 1),
  E_NOTIFICATION_HINT_DESKTOP        = (1 << 2),
  E_NOTIFICATION_HINT_SOUND_FILE     = (1 << 3),
  E_NOTIFICATION_HINT_TRANSIENT      = (1 << 4),
  E_NOTIFICATION_HINT_RESIDENT       = (1 << 5),
  E_NOTIFICATION_HINT_ACTION_ICONS   = (1 << 6),
  E_NOTIFICATION_HINT_SUPPRESS_SOUND = 0x10,
  E_NOTIFICATION_HINT_XY             = 0x20,
  E_NOTIFICATION_HINT_IMAGE_DATA     = 0x40
};

/* client method returns */
struct E_Notification_Return_Notify
{
  unsigned int notification_id;
  E_Notification *notification;
};

struct E_Notification_Return_Get_Capabilities
{
  Eina_List *capabilities;
};

struct E_Notification_Return_Get_Server_Information
{
  const char *name;
  const char *vendor;
  const char *version;
  const char *spec_version;
};

/* signals */
struct E_Notification_Event_Notification_Closed
{
  unsigned int notification_id;
  E_Notification_Closed_Reason reason;
};

struct E_Notification_Event_Action_Invoked
{
  unsigned int notification_id;
  char *action_id;
};

#ifdef __cplusplus
extern "C" {
#endif

   EAPI int e_notification_init(void);
   EAPI int e_notification_shutdown(void);

/* client */
   EAPI void e_notification_send(E_Notification *n, E_DBus_Callback_Func func, void *data);
   EAPI void e_notification_get_capabilities(E_DBus_Callback_Func func, void *data);
   EAPI void e_notification_get_server_information(E_DBus_Callback_Func func, void *data);


/* Notifications */

   EAPI E_Notification *e_notification_new(void);
   EAPI void e_notification_ref(E_Notification *n);
   EAPI void e_notification_unref(E_Notification *n);
   EAPI void e_notification_free(E_Notification *n);

   EAPI E_Notification *e_notification_full_new(const char *app_name,
                                                unsigned int replaces_id,
                                                const char *app_icon,
                                                const char *summary,
                                                const char *body,
                                                int expire_timeout);

/* notification mutators */
   EAPI void e_notification_id_set(E_Notification *note, unsigned int id);
   EAPI void e_notification_app_name_set(E_Notification *n, const char *app_name);
   EAPI void e_notification_app_icon_set(E_Notification *n, const char *app_icon);
   EAPI void e_notification_summary_set(E_Notification *n, const char *summary);
   EAPI void e_notification_body_set(E_Notification *n, const char *body);
   EAPI void e_notification_replaces_id_set(E_Notification *n, int replaces_id);
   EAPI void e_notification_timeout_set(E_Notification *n, int timeout);
   EAPI void e_notification_closed_set(E_Notification *note, unsigned char closed);

/* notification accessors */
   EAPI unsigned int e_notification_id_get(E_Notification *note);
   EAPI const char *e_notification_app_name_get(E_Notification *n);
   EAPI const char *e_notification_app_icon_get(E_Notification *n);
   EAPI const char *e_notification_summary_get(E_Notification *n);
   EAPI const char *e_notification_body_get(E_Notification *n);
   EAPI int e_notification_replaces_id_get(E_Notification *note);
   EAPI int e_notification_timeout_get(E_Notification *note);
   EAPI unsigned char e_notification_closed_get(E_Notification *note);

/* actions */
   EAPI void e_notification_action_add(E_Notification *n, const char *action_id, const char *action_name);
   EAPI Eina_List *e_notification_actions_get(E_Notification *n);
   EAPI const char *e_notification_action_id_get(E_Notification_Action *a);
   EAPI const char *e_notification_action_name_get(E_Notification_Action *a);

/* hint mutators */
   EAPI void e_notification_hint_transient_set(E_Notification *n, Eina_Bool transient);
   EAPI void e_notification_hint_resident_set(E_Notification *n, Eina_Bool resident);
   EAPI void e_notification_hint_action_icons_set(E_Notification *n, Eina_Bool action_icons);
   EAPI void e_notification_hint_image_path_set(E_Notification *n, const char *path);
   EAPI void e_notification_hint_urgency_set(E_Notification *n, char urgency);
   EAPI void e_notification_hint_category_set(E_Notification *n, const char *category);
   EAPI void e_notification_hint_desktop_set(E_Notification *n, const char *desktop);
   EAPI void e_notification_hint_sound_file_set(E_Notification *n, const char *sound_file);
   EAPI void e_notification_hint_suppress_sound_set(E_Notification *n, char suppress_sound);
   EAPI void e_notification_hint_xy_set(E_Notification *n, int x, int y);
   EAPI void e_notification_hint_image_data_set(E_Notification *n, E_Notification_Image *image);

/* hint accessors */
   EAPI char  e_notification_hint_urgency_get(E_Notification *n);
   EAPI const char *e_notification_hint_category_get(E_Notification *n);
   EAPI const char *e_notification_hint_desktop_get(E_Notification *n);
   EAPI const char *e_notification_hint_sound_file_get(E_Notification *n);
   EAPI const char *e_notification_hint_image_path_get(E_Notification *n);
   EAPI char  e_notification_hint_suppress_sound_get(E_Notification *n);
   EAPI int   e_notification_hint_xy_get(E_Notification *n, int *x, int *y);
   EAPI E_Notification_Image *e_notification_hint_image_data_get(E_Notification *n);
   /* icon_data is deprecated, we do not support setting it */
   EAPI E_Notification_Image *e_notification_hint_icon_data_get(E_Notification *n);

/* image hint */
   EAPI E_Notification_Image *e_notification_image_new(void);
   EAPI void e_notification_image_free(E_Notification_Image *img);
   EAPI Eina_Bool e_notification_image_init(E_Notification_Image *img, Evas_Object *obj) EINA_WARN_UNUSED_RESULT;
   EAPI Evas_Object *e_notification_image_evas_object_add(Evas *evas, E_Notification_Image *img);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
