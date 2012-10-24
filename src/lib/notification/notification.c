#include "E_Notify.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "e_notify_private.h"

/* private functions */
static Eina_List             *e_notification_action_list_new(void);
static E_Notification_Action *e_notification_action_new(const char *id,
                                                        const char *name);

int _e_dbus_notify_log_dom = -1;

void
loginit(void)
{
   if (_e_dbus_notify_log_dom == -1)
     _e_dbus_notify_log_dom = eina_log_domain_register("e_dbus_notify", E_DBUS_COLOR_DEFAULT);
}

/* (con|de)structor */

EAPI E_Notification *
e_notification_full_new(const char *app_name, unsigned int replaces_id, const char *app_icon, const char *summary, const char *body, int expire_timeout)
{
   E_Notification *n;

   loginit();
   n = e_notification_new();
   if (!n) return NULL;

   n->app_name = eina_stringshare_add(app_name);
   n->replaces_id = replaces_id;
   n->app_icon = eina_stringshare_add(app_icon);
   n->summary = eina_stringshare_add(summary);
   n->body = eina_stringshare_add(body);
   n->expire_timeout = expire_timeout;
   n->hints.x = n->hints.y = -1;

   return n;
}

EAPI E_Notification *
e_notification_new(void)
{
   E_Notification *n;
   
   loginit();
   n = calloc(1, sizeof(E_Notification));
   if (!n) return NULL;
   n->refcount = 1;

   return n;
}

EAPI void
e_notification_ref(E_Notification *n)
{
   loginit();
   n->refcount++;
}

EAPI void
e_notification_unref(E_Notification *n)
{
   loginit();
   if (--n->refcount == 0) e_notification_free(n);
}

EAPI void
e_notification_free(E_Notification *n)
{
   loginit();
   if (!n) return;

   eina_stringshare_del(n->app_name);
   eina_stringshare_del(n->app_icon);
   eina_stringshare_del(n->summary);
   eina_stringshare_del(n->body);

   eina_list_free(n->actions);

   eina_stringshare_del(n->hints.category);
   eina_stringshare_del(n->hints.desktop);
   eina_stringshare_del(n->hints.sound_file);
   if (n->hints.image_data) e_notification_image_free(n->hints.image_data);
   if (n->hints.icon_data) e_notification_image_free(n->hints.icon_data);
   free(n);
}

/* mutators */
EAPI void
e_notification_id_set(E_Notification *note, unsigned int id)
{
   loginit();
   note->id = id;
}

EAPI void
e_notification_app_name_set(E_Notification *note, const char *app_name)
{
   loginit();
   eina_stringshare_replace(&note->app_name, app_name);
}

EAPI void
e_notification_app_icon_set(E_Notification *note, const char *app_icon)
{
   loginit();
   eina_stringshare_replace(&note->app_icon, app_icon);
}

EAPI void
e_notification_summary_set(E_Notification *note, const char *summary)
{
   loginit();
   eina_stringshare_replace(&note->summary, summary);
}

EAPI void
e_notification_body_set(E_Notification *note, const char *body)
{
   loginit();
   eina_stringshare_replace(&note->body, body);
}

EAPI void
e_notification_action_add(E_Notification *n, const char *action_id, const char *action_name)
{
   E_Notification_Action *a;

   loginit();
   if (!n->actions)
     n->actions = e_notification_action_list_new();

   a = e_notification_action_new(action_id, action_name);
   n->actions = eina_list_append(n->actions, a);
}

EAPI void
e_notification_replaces_id_set(E_Notification *note, int replaces_id)
{
   loginit();
   note->replaces_id = replaces_id;
}

EAPI void
e_notification_timeout_set(E_Notification *note, int timeout)
{
   loginit();
   note->expire_timeout = timeout;
}

EAPI void
e_notification_closed_set(E_Notification *note, unsigned char closed)
{
   loginit();
   note->closed = closed;
}

/* accessors */
EAPI unsigned int
e_notification_id_get(E_Notification *note)
{
   loginit();
   return note->id;
}

EAPI const char *
e_notification_app_name_get(E_Notification *note)
{
   loginit();
   return note->app_name;
}

EAPI const char *
e_notification_app_icon_get(E_Notification *note)
{
   loginit();
   return note->app_icon;
}

EAPI const char *
e_notification_summary_get(E_Notification *note)
{
   loginit();
   return note->summary;
}

EAPI const char *
e_notification_body_get(E_Notification *note)
{
   loginit();
   return note->body;
}

EAPI Eina_List *
e_notification_actions_get(E_Notification *note)
{
   loginit();
   return note->actions;
}

EAPI int
e_notification_replaces_id_get(E_Notification *note)
{
   loginit();
   return note->replaces_id;
}

EAPI int
e_notification_timeout_get(E_Notification *note)
{
   loginit();
   return note->expire_timeout;
}

EAPI unsigned char
e_notification_closed_get(E_Notification *note)
{
   loginit();
   return note->closed;
}

/***** actions *****/

static Eina_List *
e_notification_action_list_new(void)
{
   Eina_List *alist;

   loginit();
   alist = NULL;
   return alist;
}

static E_Notification_Action *
e_notification_action_new(const char *id, const char *name)
{
   E_Notification_Action *act;
   
   loginit();
   act = malloc(sizeof(E_Notification_Action));
   act->id = eina_stringshare_add(id);
   act->name = eina_stringshare_add(name);
   return act;
}

EAPI const char *
e_notification_action_id_get(E_Notification_Action *a)
{
   loginit();
   EINA_SAFETY_ON_NULL_RETURN_VAL(a, NULL);
   return a->id;
}

EAPI const char *
e_notification_action_name_get(E_Notification_Action *a)
{
   loginit();
   EINA_SAFETY_ON_NULL_RETURN_VAL(a, NULL);
   return a->name;
}

/********* hints *******/
EAPI void
e_notification_hint_transient_set(E_Notification *n, Eina_Bool transient)
{
   loginit();
   if (transient)
     n->hint_flags |= E_NOTIFICATION_HINT_TRANSIENT;
   else
     n->hint_flags ^= E_NOTIFICATION_HINT_TRANSIENT;
}

EAPI void
e_notification_hint_resident_set(E_Notification *n, Eina_Bool resident)
{
   loginit();
   if (resident)
     n->hint_flags |= E_NOTIFICATION_HINT_RESIDENT;
   else
     n->hint_flags ^= E_NOTIFICATION_HINT_RESIDENT;
}

EAPI void
e_notification_hint_action_icons_set(E_Notification *n, Eina_Bool action_icons)
{
   loginit();
   if (action_icons)
     n->hint_flags |= E_NOTIFICATION_HINT_ACTION_ICONS;
   else
     n->hint_flags ^= E_NOTIFICATION_HINT_ACTION_ICONS;
}

EAPI void
e_notification_hint_urgency_set(E_Notification *n, char urgency)
{
   loginit();
   n->hints.urgency = urgency;
   n->hint_flags |= E_NOTIFICATION_HINT_URGENCY;
}

EAPI void
e_notification_hint_image_path_set(E_Notification *n, const char *path)
{
   loginit();
   eina_stringshare_replace(&n->hints.image_path, path);
}

EAPI void
e_notification_hint_category_set(E_Notification *n, const char *category)
{
   loginit();
   eina_stringshare_replace(&n->hints.category, category);
   n->hint_flags |= E_NOTIFICATION_HINT_CATEGORY;
}

EAPI void
e_notification_hint_desktop_set(E_Notification *n, const char *desktop)
{
   loginit();
   eina_stringshare_replace(&n->hints.desktop, desktop);
   n->hint_flags |= E_NOTIFICATION_HINT_DESKTOP;
}

EAPI void
e_notification_hint_sound_file_set(E_Notification *n, const char *sound_file)
{
   loginit();
   eina_stringshare_replace(&n->hints.sound_file, sound_file);
   n->hint_flags |= E_NOTIFICATION_HINT_SOUND_FILE;
}

EAPI void
e_notification_hint_suppress_sound_set(E_Notification *n, char suppress_sound)
{
   loginit();
   n->hints.suppress_sound = suppress_sound;
   n->hint_flags |= E_NOTIFICATION_HINT_SUPPRESS_SOUND;
}

EAPI void
e_notification_hint_xy_set(E_Notification *n, int x, int y)
{
   loginit();
   n->hints.x = x;
   n->hints.y = y;
   n->hint_flags |= E_NOTIFICATION_HINT_XY;
}

EAPI void
e_notification_hint_image_data_set(E_Notification *n, E_Notification_Image *image)
{
   loginit();
   n->hints.image_data = image;
}

EAPI char
e_notification_hint_urgency_get(E_Notification *n)
{
   loginit();
   return n->hint_flags & E_NOTIFICATION_HINT_URGENCY ? n->hints.urgency : 1;
}

EAPI const char *
e_notification_hint_category_get(E_Notification *n)
{
   loginit();
   return n->hints.category;
}

EAPI const char *
e_notification_hint_desktop_get(E_Notification *n)
{
   loginit();
   return n->hints.desktop;
}

EAPI const char *
e_notification_hint_image_path_get(E_Notification *n)
{
   loginit();
   return n->hints.image_path;
}

EAPI const char *
e_notification_hint_sound_file_get(E_Notification *n)
{
   loginit();
   return n->hints.sound_file;
}

EAPI char
e_notification_hint_suppress_sound_get(E_Notification *n)
{
   loginit();
   return n->hints.suppress_sound;
}

EAPI int
e_notification_hint_xy_get(E_Notification *n, int *x, int *y)
{
   loginit();
   if (x) *x = n->hints.x;
   if (y) *y = n->hints.y;

   return n->hint_flags & E_NOTIFICATION_HINT_XY ? 1 : 0;
}

EAPI E_Notification_Image *
e_notification_hint_image_data_get(E_Notification *n)
{
   loginit();
   return n->hints.image_data;
}

EAPI E_Notification_Image *
e_notification_hint_icon_data_get(E_Notification *n)
{
   loginit();
   return n->hints.icon_data;
}

EAPI E_Notification_Image *
e_notification_image_new(void)
{
   E_Notification_Image *img;

   loginit();
   img = calloc(1, sizeof(E_Notification_Image));
   img->bits_per_sample = 8;
   return img;
}

EAPI Eina_Bool
e_notification_image_init(E_Notification_Image *img, Evas_Object *obj)
{
   int x, y, w = 0, h = 0;
   unsigned char *d, *imgdata;
   int rowstride;
   int *s;
   
   loginit();
   EINA_SAFETY_ON_NULL_RETURN_VAL(img, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);

   evas_object_image_size_get(obj, &w, &h);
   if ((w <= 0) || (h <= 0)) return EINA_FALSE;
   imgdata = evas_object_image_data_get(obj, EINA_FALSE);
   if (!imgdata) return EINA_FALSE;
   
   img->data = malloc(4 * w * h);
   if (!img->data)
     {
        evas_object_image_data_set(obj, imgdata);
        return EINA_FALSE;
     }
   img->channels = 4;
   img->rowstride = 4 * w;
   img->width = w;
   img->height = h;
   img->bits_per_sample = 8;
   img->has_alpha = EINA_TRUE;
   
   rowstride = evas_object_image_stride_get(obj);
   for (y = 0; y < img->height; y++)
     {
        s = (int *)(imgdata + (y * rowstride));
        d = img->data + (y * img->rowstride);
        
        for (x = 0; x < img->width; x++, s++)
          {
             *d++ = (*s >> 16) & 0xff;
             *d++ = (*s >> 8) & 0xff;
             *d++ = (*s) & 0xff;
             *d++ = (*s >> 24) & 0xff;
          }
     }
   
   evas_object_image_data_set(obj, imgdata);
   return EINA_TRUE;
}

EAPI void
e_notification_image_free(E_Notification_Image *img)
{
   loginit();
   if (!img) return;
   free(img->data);
   free(img);
}

static Eina_Bool
_e_notification_image_evas_object_fill(E_Notification_Image *img, Evas_Object *obj)
{
   unsigned char *imgdata;
   
   EINA_SAFETY_ON_NULL_RETURN_VAL(img, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, EINA_FALSE);

   evas_object_image_colorspace_set(obj, EVAS_COLORSPACE_ARGB8888);
   evas_object_image_alpha_set(obj, img->has_alpha);
   evas_object_image_size_set(obj, img->width, img->height);
   
   imgdata = evas_object_image_data_get(obj, EINA_TRUE);
   if (!imgdata) return EINA_FALSE;
   
   if (img->bits_per_sample == 8)
     {
        /* Although not specified.
         * The data are very likely to come from a GdkPixbuf
         * which align each row on a 4-bytes boundary when using RGB.
         * And is RGBA otherwise. */
        int x, y, *d;
        unsigned char *s;
        int rowstride;
        
        rowstride = evas_object_image_stride_get(obj);
        for (y = 0; y < img->height; y++)
          {
             s = img->data + (y * img->rowstride);
             d = (int *)(imgdata + (y * rowstride));

             for (x = 0; x < img->width; x++, s += img->channels, d++)
               {
                  if (img->has_alpha)
                    *d = (s[3] << 24) | (s[0] << 16) | (s[1] << 8) | (s[2]);
                  else
                    *d = (0xff << 24) | (s[0] << 16) | (s[1] << 8) | (s[2]);
               }
          }
     }
   evas_object_image_data_update_add(obj, 0, 0, img->width, img->height);
   evas_object_image_data_set(obj, imgdata);
   
   return EINA_TRUE;
}

EAPI Evas_Object *
e_notification_image_evas_object_add(Evas *evas, E_Notification_Image *img)
{
   Evas_Object *o = NULL;

   loginit();
   if ((!evas) || (!img)) return NULL;
   o = evas_object_image_filled_add(evas);
   evas_object_resize(o, img->width, img->height);
   if (!_e_notification_image_evas_object_fill(img, o))
     {
        evas_object_del(o);
        return NULL;
     }
   return o;
}

