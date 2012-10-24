#include "e_connman_private.h"

E_Connman_Element *
e_connman_service_get(const char *path)
{
   E_Connman_Element *service;

   EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);

   service = e_connman_element_get(path);
   if (!service)
      return NULL;

   if (!e_connman_element_is_service(service))
     {
        WRN("path '%s' is not a service!", path);
        return NULL;
     }

   return service;
}

/**
 * Connect this service.
 *
 * Connect this service. It will attempt to connect
 * WiFi, WiMAX or Bluetooth services.
 *
 * For Ethernet devices this method can only be used
 * if it has previously been disconnected. Otherwise
 * the plugging of a cable will trigger connecting
 * automatically. If no cable is plugged in this method
 * will fail.
 *
 * @param service path to call method on server.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_connect(E_Connman_Element *service, E_DBus_Method_Return_Cb cb, const void *data)
{
   const char name[] = "Connect";

   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   return e_connman_element_call_full
             (service, name, NULL, &service->_pending.service_connect, cb, data);
}

/**
 * Disconnect this service.
 *
 * Disconnect this service. If the service is not
 * connected an error message will be generated.
 *
 * On Ethernet devices this will disconnect the IP
 * details from the service. It will not magically
 * unplug the cable. When no cable is plugged in this
 * method will fail.
 *
 * This method can also be used to abort a previous
 * connectiong attempt via the Connect method.
 *
 * @param service path to call method on server.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_disconnect(E_Connman_Element *service, E_DBus_Method_Return_Cb cb, const void *data)
{
   const char name[] = "Disconnect";

   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   return e_connman_element_call_full
             (service, name, NULL, &service->_pending.service_disconnect, cb, data);
}

/**
 * Remove this service.
 *
 * A successfully connected service with Favorite=true
 * can be removed this way. If it is connected, it will
 * be automatically disconnected first.
 *
 * If the service requires a passphrase it will be
 * cleared and forgotten when removing.
 *
 * This is similar to setting the Favorite property
 * to false, but that is currently not supported.
 *
 * In the case a connection attempt failed and the
 * service is in the State=failure, this method can
 * also be used to reset the service.
 *
 * Calling this method on Ethernet devices will cause
 * an error message. It is not possible to remove these
 * kind of devices.
 *
 * @param service path to call method on server.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_remove(E_Connman_Element *service, E_DBus_Method_Return_Cb cb, const void *data)
{
   const char name[] = "Remove";

   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   return e_connman_element_call_full
             (service, name, NULL, &service->_pending.service_remove, cb, data);
}

/**
 * Clears the value of the specified property.
 *
 *
 * @param service path to call method on server.
 * @param property to be cleared.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_clear_property(E_Connman_Element *service, const char *property, E_DBus_Method_Return_Cb cb, const void *data)
{
   const char name[] = "ClearProperty";

   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(property, EINA_FALSE);
   return e_connman_element_call_with_string
             (service, name, property, NULL, &service->_pending.service_clear_property,
             cb, data);
}

/**
 * Move service before in favorites list.
 *
 * Call method MoveBefore(object service) at the given service on server.
 *
 * If a service has been used before, this allows a
 * reorder of the favorite services.
 *
 * The target service object must be part of this
 * profile. Moving between profiles is not supported.
 *
 * @param service path to call method on server.
 * @param object_path object service.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_move_before(E_Connman_Element *service, const char *object_path, E_DBus_Method_Return_Cb cb, const void *data)
{
   const char name[] = "MoveBefore";

   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(object_path, EINA_FALSE);
   return e_connman_element_call_with_path
             (service, name, object_path, NULL,
             &service->_pending.service_move_before, cb, data);
}

/**
 * Move service after in favorites list.
 *
 * Call method MoveAfter(object service) at the given service on server.
 *
 * If a service has been used before, this allows a
 * reorder of the favorite services.
 *
 * The target service object must be part of this
 * profile. Moving between profiles is not supported.
 *
 * @param service path to call method on server.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_move_after(E_Connman_Element *service, const char *object_path, E_DBus_Method_Return_Cb cb, const void *data)
{
   const char name[] = "MoveAfter";

   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(object_path, EINA_FALSE);
   return e_connman_element_call_with_path
             (service, name, object_path, NULL,
             &service->_pending.service_move_after, cb, data);
}

/**
 * Get property "State" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The service state information.
 *
 * Valid states are "idle", "failure", "association",
 * "configuration", "ready", "login" and "online".
 *
 * @param service path to get property.
 * @param state where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_state_get(const E_Connman_Element *service, const char **state)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(state, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_state, NULL, state);
}

/**
 * Get property "Error" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The service error status details.
 *
 * When error occur during connection or disconnection
 * the detailed information are represented in this
 * property to help the user interface to present the
 * user with alternate options.
 *
 * This property is only valid when the service is in
 * the "failure" state. Otherwise it might be empty or
 * not present at all.
 *
 * Current defined error code is "dhcp-failed".
 *
 * @param service path to get property.
 * @param error where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_error_get(const E_Connman_Element *service, const char **error)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(error, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_error, NULL, error);
}

/**
 * Get property "Name" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The service name (for example "Wireless" etc.)
 *
 * This name can be used for directly displaying it in
 * the application. It has pure informational purpose.
 *
 * For Ethernet devices and hidden WiFi networks it is
 * not guaranteed that this property is present.
 *
 * @param service path to get property.
 * @param name where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_name_get(const E_Connman_Element *service, const char **name)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(name, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_name, NULL, name);
}

/**
 * Get property "Type" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The service type (for example "ethernet", "wifi" etc.)
 *
 * This information should only be used to determine
 * advanced properties or showing the correct icon
 * to the user.
 *
 * @param service path to get property.
 * @param type where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_type_get(const E_Connman_Element *service, const char **type)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(type, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_type, NULL, type);
}

/**
 * Get property "Security" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * If the service type is WiFi, then this property is
 * present and contains the list of security method or key
 * management setting.
 *
 * Possible values are "none", "wep", "wpa", "rsn", "psk", "ieee8021x" and "wps"
 *
 * This property might be only present for WiFi
 * services.
 *
 * @param service path to get property.
 * @param count where to return the number of elements in @a security
 * @param security where to store the property value, must be a pointer
 *        to array of strings, it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_security_get(const E_Connman_Element *service, unsigned int *count, const char ***security)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(security, EINA_FALSE);

   return e_connman_element_strings_array_get_stringshared
     (service, e_connman_prop_security, count, security);
}

/**
 * Get property "Passphrase" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * If the service type is WiFi, then this property
 * can be used to store a passphrase.
 *
 * No PropertyChanged signals will be send for this
 * property. The PassphraseRequired property should
 * be monitored instead.
 *
 * This property might also not always be included
 * since it is protected by a different security policy.
 *
 * @param service path to get property.
 * @param passphrase where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 * @see e_connman_service_passphrase_set()
 */
Eina_Bool
e_connman_service_passphrase_get(const E_Connman_Element *service, const char **passphrase)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(passphrase, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_passphrase, NULL, passphrase);
}

/**
 * Set property "Passphrase" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * If the service type is WiFi, then this property
 * can be used to store a passphrase.
 *
 * No PropertyChanged signals will be send for this
 * property. The PassphraseRequired property should
 * be monitored instead.
 *
 * This property might also not always be included
 * since it is protected by a different security policy.
 *
 * @param service path to get property.
 * @param passphrase value to set.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 * @see e_connman_service_passphrase_get()
 */
Eina_Bool
e_connman_service_passphrase_set(E_Connman_Element *service, const char *passphrase, E_DBus_Method_Return_Cb cb, const void *data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   return e_connman_element_property_set_full
             (service, e_connman_prop_passphrase, DBUS_TYPE_STRING,
             passphrase, cb, data);
}

/**
 * Get property "PassphraseRequired" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * If the service type is WiFi, then this property
 * indicates if a passphrase is required.
 *
 * If a passphrase has been set already or if no
 * passphrase is needed, then this property will
 * be set to false.
 *
 * @param service path to get property.
 * @param passphrase_required where to store the property value, must be a
 *        pointer to Eina_Bool (Eina_Bool *).
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_passphrase_required_get(const E_Connman_Element *service, Eina_Bool *passphrase_required)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(passphrase_required, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_passphrase_required, NULL, passphrase_required);
}

/**
 * Get property "LoginRequired" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Indicates that a manual configuration must be done to login the
 * user, likely access an website using a browser.
 *
 * If a login has been set already or if no
 * login is needed, then this property will
 * be set to false.
 *
 * @param service path to get property.
 * @param login_required where to store the property value, must be a
 *        pointer to Eina_Bool (Eina_Bool *).
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 *
 * @since 1.1.0
 */
Eina_Bool
e_connman_service_login_required_get(const E_Connman_Element *service, Eina_Bool *login_required)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(login_required, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_login_required, NULL, login_required);
}

/**
 * Get property "Strength" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Indicates the signal strength of the service. This
 * is a normalized value between 0 and 100.
 *
 * This property will not be present for Ethernet
 * devices.
 *
 * @param service path to get property.
 * @param strength where to store the property value, must be a pointer
 *        to byte (unsigned char*).
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_strength_get(const E_Connman_Element *service, unsigned char *strength)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(strength, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_strength, NULL, strength);
}

/**
 * Get property "Favorite" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Will be true if a cable is plugged in or the user
 * selected and successfully connected to this service.
 *
 * This value is automatically changed and to revert
 * it back to false the Remove() method needs to be
 * used.
 *
 * @param service path to get property.
 * @param favorite where to store the property value, must be a
 *        pointer to Eina_Bool (Eina_Bool *).
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_favorite_get(const E_Connman_Element *service, Eina_Bool *favorite)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(favorite, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_favorite, NULL, favorite);
}

/**
 * Get property "Immutable" value.
 *
 * This value will be set to true if the service is configured
 * externally via a configuration file.
 *
 * The only valid operation are e_connman_service_connect() and
 * e_connman_service_disconnect(). The e_connman_service_remove()
 * method will result in an error.

 * @param service path to get property.
 * @param immutable where to store the property value, must be a
 *        pointer to Eina_Bool (Eina_Bool *).
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_immutable_get(const E_Connman_Element *service, Eina_Bool *immutable)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(immutable, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_immutable, NULL, immutable);
}

/**
 * Get property "AutoConnect" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * If set to true, this service will auto-connect
 * when not other connection is available.
 *
 * For favorite services it is possible to change
 * this value to prevent or permit automatic
 * connection attempts.
 *
 * @param service path to get property.
 * @param auto_connect where to store the property value, must be a
 *        pointer to Eina_Bool (Eina_Bool *).
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 * @see e_connman_service_auto_connect_set()
 */
Eina_Bool
e_connman_service_auto_connect_get(const E_Connman_Element *service, Eina_Bool *auto_connect)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(auto_connect, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_auto_connect, NULL, auto_connect);
}

/**
 * Set property "AutoConnect" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * If set to true, this service will auto-connect
 * when not other connection is available.
 *
 * For favorite services it is possible to change
 * this value to prevent or permit automatic
 * connection attempts.
 *
 * @param service path to get property.
 * @param service_favorite where to store the property value, must be a
 *        pointer to Eina_Bool (Eina_Bool *).
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 * @see e_connman_service_auto_connect_get()
 */
Eina_Bool
e_connman_service_auto_connect_set(E_Connman_Element *service, Eina_Bool auto_connect, E_DBus_Method_Return_Cb cb, const void *data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   return e_connman_element_property_set_full
             (service, e_connman_prop_auto_connect, DBUS_TYPE_BOOLEAN, &auto_connect, cb, data);
}

/**
 * Get property "Roaming" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * This property indicates if this service is roaming.
 *
 * In the case of Cellular services this normally
 * indicates connections to a foreign provider when
 * traveling abroad.
 *
 * @param service path to get property.
 * @param roaming where to store the property value, must be a
 *        pointer to Eina_Bool (Eina_Bool *).
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_roaming_get(const E_Connman_Element *service, Eina_Bool *roaming)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(roaming, EINA_FALSE);
   return e_connman_element_property_get_stringshared
             (service, e_connman_prop_roaming, NULL, roaming);
}

/**
 * Get property "Nameservers" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 * The list of currently active nameservers for this service. If the server is
 * not in READY or ONLINE state than this list will be empty.
 *
 * Global nameservers are automatically added to this list.
 *
 * The array represents a sorted list of the current nameservers. The first one
 * has the highest priority and is used by default.
 *
 * When using DHCP this array represents the nameservers provided by the
 * network. In case of manual settings, the ones from Nameservers.Configuration
 * are used.
 *
 * @param service path to get property.
 * @param count return the number of elements in array.
 * @param nameservers array with pointers to internal strings. These
 *        strings are not copied in any way, and they are granted to
 *        be eina_stringshare instances, so one can use
 *        eina_stringshare_ref() if he wants to save memory and cpu to
 *        get an extra reference. The array itself is also NOT
 *        allocated or copied, do not modify it. This pointer is just
 *        set if return is @c EINA_TRUE.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_nameservers_get(const E_Connman_Element *service, unsigned int *count, const char ***nameservers)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(nameservers, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   return e_connman_element_strings_array_get_stringshared
             (service, e_connman_prop_nameservers, count, nameservers);
}

/**
 * Get property "Nameservers.Configuration" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 * The list of currently active nameservers for this service. If the server is
 * not in READY or ONLINE state than this list will be empty.
 *
 * Unlike Nameservers, this is the user-set value, rather than the
 * actual value.
 *
 * @param service path to get property.
 * @param count return the number of elements in array.
 * @param nameservers array with pointers to internal strings. These
 *        strings are not copied in any way, and they are granted to
 *        be eina_stringshare instances, so one can use
 *        eina_stringshare_ref() if he wants to save memory and cpu to
 *        get an extra reference. The array itself is also NOT
 *        allocated or copied, do not modify it. This pointer is just
 *        set if return is @c EINA_TRUE.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_nameservers_configuration_get(const E_Connman_Element *service, unsigned int *count, const char ***nameservers)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(nameservers, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   return e_connman_element_strings_array_get_stringshared
             (service, e_connman_prop_nameservers_configuration,
              count, nameservers);
}

/**
 * Set property "Nameservers.Configuration" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Unlike Nameservers, this is the user-set value, rather than the actual value.
 * It allows user to override the default setting.  When using manual
 * configuration and no global nameservers are configured, then it is useful to
 * configure this setting as well.
 *
 * This list is sorted by priority and the first entry represents the nameserver
 * with the highest priority.
 *
 * Changes to the domain name servers can be done at any time. It will not cause
 * a disconnect of the service. However there might be small window where name
 * resolution might fail.
 *
 * @param service path to set property.
 * @param nameservers sorted list of the current nameservers. The first one has
 * the highest priority and is used by default.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 * @see e_connman_service_nameservers_configuration_get()
 */
Eina_Bool
e_connman_service_nameservers_configuration_set(E_Connman_Element *service, unsigned int count, const char **nameservers, E_DBus_Method_Return_Cb cb, const void *data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(nameservers, EINA_FALSE);
   return e_connman_element_property_array_set_full
             (service, e_connman_prop_nameservers_configuration,
              DBUS_TYPE_STRING, count,
              (const void * const *)nameservers, cb, data);
}

/**
 * Get property "Domains" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 * The list of currently active domains for this service. If the server is
 * not in READY or ONLINE state than this list will be empty.
 *
 * The list of currently used search domains.
 *
 * @param service path to get property.
 * @param count return the number of elements in array.
 * @param domains array with pointers to internal strings. These
 *        strings are not copied in any way, and they are granted to
 *        be eina_stringshare instances, so one can use
 *        eina_stringshare_ref() if he wants to save memory and cpu to
 *        get an extra reference. The array itself is also NOT
 *        allocated or copied, do not modify it. This pointer is just
 *        set if return is @c EINA_TRUE.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_domains_get(const E_Connman_Element *service, unsigned int *count, const char ***domains)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(domains, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   return e_connman_element_strings_array_get_stringshared
             (service, e_connman_prop_domains, count, domains);
}

/**
 * Get property "Domains.Configuration" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 * The list of currently active domains for this service. If the server is
 * not in READY or ONLINE state than this list will be empty.
 *
 * Unlike Domains, this is the user-set value, rather than the
 * actual value.
 *
 * @param service path to get property.
 * @param count return the number of elements in array.
 * @param domains array with pointers to internal strings. These
 *        strings are not copied in any way, and they are granted to
 *        be eina_stringshare instances, so one can use
 *        eina_stringshare_ref() if he wants to save memory and cpu to
 *        get an extra reference. The array itself is also NOT
 *        allocated or copied, do not modify it. This pointer is just
 *        set if return is @c EINA_TRUE.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_domains_configuration_get(const E_Connman_Element *service, unsigned int *count, const char ***domains)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(domains, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   return e_connman_element_strings_array_get_stringshared
             (service, e_connman_prop_domains_configuration, count, domains);
}

/**
 * Set property "Domains.Configuration" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Unlike Domains, this is the user-set value, rather than the actual value.
 * It allows user to override the default setting.  When using manual
 * configuration and no global domains are configured, then it is useful to
 * configure this setting as well.
 *
 * This list is sorted by priority and the first entry represents the nameserver
 * with the highest priority.
 *
 * Changes to the domain name servers can be done at any time. It will not cause
 * a disconnect of the service. However there might be small window where name
 * resolution might fail.
 *
 * @param service path to set property.
 * @param count number of elements in @a domain.
 * @param domains sorted list of the current domains. The first one has
 * the highest priority and is used by default.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 * @see e_connman_service_domains_configuration_get()
 */
Eina_Bool
e_connman_service_domains_configuration_set(E_Connman_Element *service, unsigned int count, const char **domains, E_DBus_Method_Return_Cb cb, const void *data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(domains, EINA_FALSE);
   return e_connman_element_property_array_set_full
             (service, e_connman_prop_domains_configuration,
              DBUS_TYPE_STRING, count,
              (const void * const *)domains, cb, data);
}

/**
 * Get property "IPv4.Method" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The IPv4 method in use. Possible values here are "dhcp" and
 * "static".
 *
 * @param service path to get property.
 * @param method where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_method_get(const E_Connman_Element *service, const char **method)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(method, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ipv4, e_connman_prop_method, NULL, method);
}

/**
 * Get property "IPv4.Address" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The current IPv4 address.
 *
 * @param service path to get property.
 * @param address where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_address_get(const E_Connman_Element *service, const char **address)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(address, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ipv4, e_connman_prop_address, NULL, address);
}

/**
 * Get property "IPv4.Gateway" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The current IPv4 gateway.
 *
 * @param service path to get property.
 * @param gateway where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_gateway_get(const E_Connman_Element *service, const char **gateway)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(gateway, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ipv4, e_connman_prop_gateway, NULL, gateway);
}

/**
 * Get property "IPv4.Netmask" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The current IPv4 netmask.
 *
 * @param service path to get property.
 * @param netmask where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_netmask_get(const E_Connman_Element *service, const char **netmask)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(netmask, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ipv4, e_connman_prop_netmask, NULL, netmask);
}

/**
 * Get property "IPv4.Configuration.Method" value.
 *
 * Unlike IPv4.Method, this is the user-set value, rather than the
 * actual value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The IPv4 configured method. Possible values here are "dhcp" and
 * "static".
 *
 * @param service path to get property.
 * @param method where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_configuration_method_get(const E_Connman_Element *service, const char **method)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(method, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ipv4_configuration, e_connman_prop_method,
             NULL, method);
}

/**
 * Get property "IPv4.Configuration.Address" value.
 *
 * Unlike IPv4.Address, this is the user-set value, rather than the
 * actual value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The current configured IPv4 address.
 *
 * @param service path to get property.
 * @param address where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_configuration_address_get(const E_Connman_Element *service, const char **address)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(address, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ipv4_configuration, e_connman_prop_address,
             NULL, address);
}

/**
 * Get property "IPv4.Configuration.Gateway" value.
 *
 * Unlike IPv4.Gateway, this is the user-set value, rather than the
 * actual value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The current configured IPv4 gateway.
 *
 * @param service path to get property.
 * @param gateway where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_configuration_gateway_get(const E_Connman_Element *service, const char **gateway)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(gateway, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ipv4_configuration, e_connman_prop_gateway,
             NULL, gateway);
}

/**
 * Get property "IPv4.Configuration.Netmask" value.
 *
 * Unlike IPv4.Netmask, this is the user-set value, rather than the
 * actual value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The current configured IPv4 netmask.
 *
 * @param service path to get property.
 * @param netmask where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_configuration_netmask_get(const E_Connman_Element *service, const char **netmask)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(netmask, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ipv4_configuration, e_connman_prop_netmask,
             NULL, netmask);
}

/**
 * Set IPv4 to connect automatically using DHCP.
 *
 * @param service path to set.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_configure_dhcp(E_Connman_Element *service, E_DBus_Method_Return_Cb cb, const void *data)
{
   const char method[] = "dhcp";
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   return e_connman_element_property_dict_set_full
             (service, e_connman_prop_ipv4_configuration, e_connman_prop_method,
             DBUS_TYPE_STRING, method, cb, data);
}

/**
 * Set IPv4 to connect using manually set parameters.
 *
 * @param service path to set.
 * @param address IPv4 address.
 * @param netmask IPv4 netmask, or @c NULL for "/32".
 * @param gateway IPv4 gateway address.
 * @param cb function to call when server replies or some error happens.
 * @param data data to give to cb when it is called.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ipv4_configure_manual(E_Connman_Element *service, const char *address, const char *netmask, const char *gateway, E_DBus_Method_Return_Cb cb, const void *data)
{
   const char name[] = "SetProperty";
   const char *method = "manual"; /* not method[] as gcc screws it with dbus */
   DBusMessage *msg;
   DBusMessageIter itr, variant, dict, entry;

   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(address, EINA_FALSE);

   msg = dbus_message_new_method_call
         (e_connman_system_bus_name_get(), service->path, service->interface, name);

   if (!msg)
      return 0;

   dbus_message_iter_init_append(msg, &itr);
   dbus_message_iter_append_basic
      (&itr, DBUS_TYPE_STRING, &e_connman_prop_ipv4_configuration);

   if (dbus_message_iter_open_container
       (&itr, DBUS_TYPE_VARIANT,
           (DBUS_TYPE_ARRAY_AS_STRING
               DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
               DBUS_TYPE_STRING_AS_STRING
               DBUS_TYPE_STRING_AS_STRING
               DBUS_DICT_ENTRY_END_CHAR_AS_STRING),
           &variant))
     {
        if (dbus_message_iter_open_container
            (&variant, DBUS_TYPE_ARRAY,
                (DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
                    DBUS_TYPE_STRING_AS_STRING
                    DBUS_TYPE_STRING_AS_STRING
                    DBUS_DICT_ENTRY_END_CHAR_AS_STRING),
                &dict))
          {
             if (dbus_message_iter_open_container(&dict, DBUS_TYPE_DICT_ENTRY, NULL, &entry))
               {
                  dbus_message_iter_append_basic
                    (&entry, DBUS_TYPE_STRING, &e_connman_prop_method);
                  dbus_message_iter_append_basic(&entry, DBUS_TYPE_STRING, &method);
                  dbus_message_iter_close_container(&dict, &entry);
               }
             if (dbus_message_iter_open_container(&dict, DBUS_TYPE_DICT_ENTRY, NULL, &entry))
               {
                  dbus_message_iter_append_basic
                    (&entry, DBUS_TYPE_STRING, &e_connman_prop_address);
                  dbus_message_iter_append_basic(&entry, DBUS_TYPE_STRING, &address);
                  dbus_message_iter_close_container(&dict, &entry);
               }
             if (netmask)
               {
                  if (dbus_message_iter_open_container
                      (&dict, DBUS_TYPE_DICT_ENTRY, NULL, &entry))
                    {
                       dbus_message_iter_append_basic
                         (&entry, DBUS_TYPE_STRING, &e_connman_prop_netmask);
                       dbus_message_iter_append_basic(&entry, DBUS_TYPE_STRING, &netmask);
                       dbus_message_iter_close_container(&dict, &entry);
                    }
                  else
                    {
                       ERR("dbus_message_iter_open_container() failed");
                    }
               }
             
             if (gateway)
               {
                  if (dbus_message_iter_open_container
                      (&dict, DBUS_TYPE_DICT_ENTRY, NULL, &entry))
                    {
                       dbus_message_iter_append_basic
                         (&entry, DBUS_TYPE_STRING, &e_connman_prop_gateway);
                       dbus_message_iter_append_basic(&entry, DBUS_TYPE_STRING, &gateway);
                       dbus_message_iter_close_container(&dict, &entry);
                    }
                  else
                    {
                       ERR("dbus_message_iter_open_container() failed");
                    }
               }
             dbus_message_iter_close_container(&variant, &dict);
          }
        else
          {
             ERR("dbus_message_iter_open_container() failed");
          }
        dbus_message_iter_close_container(&itr, &variant);
     }
   else
     {
        ERR("dbus_message_iter_open_container() failed");
     }

   return e_connman_element_message_send
             (service, name, NULL, msg, &service->_pending.property_set, cb, data);
}

/**
 * Get property "Proxy.Method" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Possible values are "direct", "auto" and "manual".
 *
 * @param service path to get property.
 * @param method where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_proxy_method_get(const E_Connman_Element *service, const char **method)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(method, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_proxy, e_connman_prop_method, NULL, method);
}

/**
 * Get property "Proxy.URL" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Automatic proxy configuration URL. Used by "auto" method.
 *
 * @param service path to get property.
 * @param url where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_proxy_url_get(const E_Connman_Element *service, const char **url)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(url, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_proxy, e_connman_prop_url, NULL, url);
}

/**
 * Get property "Proxy.Servers" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * List of proxy URIs. The URI without a protocol will be interpreted as the
 * generic proxy URI. All others will target a specific protocol and only once.
 * Example of generic proxy server entry would be like this:
 * "server.example.com:911".
 *
 * Used when "manual" method is set.
 *
 * @param service path to get property.
 * @param count return the number of elements in array.
 * @param servers array with pointers to internal strings. These
 *        strings are not copied in any way, and they are granted to
 *        be eina_stringshare instances, so one can use
 *        eina_stringshare_ref() if he wants to save memory and cpu to
 *        get an extra reference. The array itself is also NOT
 *        allocated or copied, do not modify it. This pointer is just
 *        set if return is @c EINA_TRUE.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_proxy_servers_get(const E_Connman_Element *service, unsigned int *count, const char ***servers)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(servers, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   return e_connman_element_property_dict_strings_array_get_stringshared
             (service, e_connman_prop_proxy, e_connman_prop_servers, count, servers);
}

/**
 * Get property "Proxy.Excludes" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * List of hosts which can be accessed directly.
 *
 * Used when "manual" method is set.
 *
 * @param service path to get property.
 * @param count return the number of elements in array.
 * @param excludes array with pointers to internal strings. These
 *        strings are not copied in any way, and they are granted to
 *        be eina_stringshare instances, so one can use
 *        eina_stringshare_ref() if he wants to save memory and cpu to
 *        get an extra reference. The array itself is also NOT
 *        allocated or copied, do not modify it. This pointer is just
 *        set if return is @c EINA_TRUE.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_proxy_excludes_get(const E_Connman_Element *service, unsigned int *count, const char ***excludes)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(excludes, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   return e_connman_element_property_dict_strings_array_get_stringshared
             (service, e_connman_prop_proxy, e_connman_prop_excludes, count, excludes);
}

/**
 * Get property "Proxy.Configuration.Method" value.
 *
 * Unlike Proxy.Configuration.Method, this is the user-set value, rather than
 * the actual value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Possible values are "direct", "auto" and "manual".
 *
 * @param service path to get property.
 * @param method where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_proxy_configuration_method_get(const E_Connman_Element *service, const char **method)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(method, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_proxy_configuration, e_connman_prop_method, NULL, method);
}

/**
 * Get property "Proxy.Configuration.URL" value.
 *
 * Unlike Proxy.URL, this is the user-set value, rather than the
 * actual value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Automatic proxy configuration URL. Used by "auto" method.
 *
 * @param service path to get property.
 * @param url where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_proxy_configuration_url_get(const E_Connman_Element *service, const char **url)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(url, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_proxy_configuration, e_connman_prop_url, NULL, url);
}

/**
 * Get property "Proxy.Configuration.Servers" value.
 *
 * Unlike Proxy.Servers, this is the user-set value, rather than the
 * actual value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * List of proxy URIs. The URI without a protocol will be interpreted as the
 * generic proxy URI. All others will target a specific protocol and only once.
 * Example of generic proxy server entry would be like this:
 * "server.example.com:911".
 *
 * Used when "manual" method is set.
 *
 * @param service path to get property.
 * @param count return the number of elements in array.
 * @param servers array with pointers to internal strings. These
 *        strings are not copied in any way, and they are granted to
 *        be eina_stringshare instances, so one can use
 *        eina_stringshare_ref() if he wants to save memory and cpu to
 *        get an extra reference. The array itself is also NOT
 *        allocated or copied, do not modify it. This pointer is just
 *        set if return is @c EINA_TRUE.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_proxy_configuration_servers_get(const E_Connman_Element *service, unsigned int *count, const char ***servers)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(servers, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   return e_connman_element_property_dict_strings_array_get_stringshared
             (service, e_connman_prop_proxy_configuration, e_connman_prop_servers, count, servers);
}

/**
 * Get property "Proxy.Configuration.Excludes" value.
 *
 * Unlike Proxy.Excludes, this is the user-set value, rather than the
 * actual value.

 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * List of hosts which can be accessed directly.
 *
 * Used when "manual" method is set.
 *
 * @param service path to get property.
 * @param count return the number of elements in array.
 * @param excludes array with pointers to internal strings. These
 *        strings are not copied in any way, and they are granted to
 *        be eina_stringshare instances, so one can use
 *        eina_stringshare_ref() if he wants to save memory and cpu to
 *        get an extra reference. The array itself is also NOT
 *        allocated or copied, do not modify it. This pointer is just
 *        set if return is @c EINA_TRUE.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_proxy_configuration_excludes_get(const E_Connman_Element *service, unsigned int *count, const char ***excludes)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(excludes, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(count, EINA_FALSE);
   return e_connman_element_property_dict_strings_array_get_stringshared
             (service, e_connman_prop_proxy_configuration, e_connman_prop_excludes, count, excludes);
}

/**
 * Get property "Ethernet.Interface" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Interface name (for example eth0).
 *
 * @param service path to get property.
 * @param iface where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ethernet_interface_get(const E_Connman_Element *service, const char **iface)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(iface, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ethernet, e_connman_prop_interface, NULL, iface);
}

/**
 * Get property "Ethernet.Method" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The Ethernet configuration method. Possible values here "auto" and "manual".
 *
 * @param service path to get property.
 * @param method where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ethernet_method_get(const E_Connman_Element *service, const char **method)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(method, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ethernet, e_connman_prop_method, NULL, method);
}

/**
 * Get property "Ethernet.Speed" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Selected speed of the line. This information might not always be available.
 *
 * @param service path to get property.
 * @param speed where to store the property value.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ethernet_speed_get(const E_Connman_Element *service, unsigned short *speed)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(speed, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ethernet, e_connman_prop_speed, NULL, speed);
}

/**
 * Get property "Ethernet.Address" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The current configured Ethernet address (mac-address).
 *
 * @param service path to get property.
 * @param address where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ethernet_address_get(const E_Connman_Element *service, const char **address)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(address, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ethernet, e_connman_prop_address, NULL, address);
}

/**
 * Get property "Ethernet.Duplex" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * Selected duplex settings of the line. Possible values are "half" and "full".
 * This information might not always be available.
 *
 * @param service path to get property.
 * @param duplex where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ethernet_duplex_get(const E_Connman_Element *service, const char **duplex)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(duplex, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ethernet, e_connman_prop_duplex, NULL, duplex);
}

/**
 * Get property "Ethernet.MTU" value.
 *
 * If this property isn't found then @c EINA_FALSE is returned.
 * If @c EINA_FALSE is returned, then this call failed and parameter-returned
 * values shall be considered invalid.
 *
 * The current configured Ethernet MTU.
 *
 * @param service path to get property.
 * @param gateway where to store the property value, must be a pointer
 *        to string (const char **), it will not be allocated or
 *        copied and references will be valid until element changes,
 *        so copy it if you want to use it later.
 *
 * @return @c EINA_TRUE on success, @c EINA_FALSE otherwise.
 */
Eina_Bool
e_connman_service_ethernet_mtu_get(const E_Connman_Element *service, unsigned short *mtu)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(service, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(mtu, EINA_FALSE);
   return e_connman_element_property_dict_get_stringshared
             (service, e_connman_prop_ethernet, e_connman_prop_mtu, NULL, mtu);
}
