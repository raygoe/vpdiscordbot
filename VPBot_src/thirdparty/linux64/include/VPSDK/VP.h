#ifndef VPSDK_VP_h__
#define VPSDK_VP_h__

#if defined(WIN32) || defined(UNDER_CE)
#   ifdef VPSDK_EXPORTS
#       ifdef VPSDK_STATIC
#           define VPSDK_API extern "C"
#       else
#           define VPSDK_API extern "C" __declspec(dllexport)
#       endif
#   else
#       ifdef __cplusplus
#           ifdef VPSDK_STATIC
#               define VPSDK_API extern "C"
#           else
#               define VPSDK_API extern "C" __declspec(dllimport)
#           endif
#       else
#           ifdef VPSDK_STATIC
#               define VPSDK_API extern
#           else
#               define VPSDK_API __declspec(dllimport)
#           endif
#       endif
#   endif
#else
#   if defined(VPSDK_EXPORTS) && (__GNUC__ >= 4)
#       ifdef __cplusplus
#           define VPSDK_API extern "C" __attribute__ ((visibility ("default")))
#       else
#           define VPSDK_API extern __attribute__ ((visibility ("default")))
#       endif
#   else
#       ifdef __cplusplus
#           define VPSDK_API extern "C"
#       else
#           define VPSDK_API extern
#       endif
#   endif
#endif

/* API Version */
#define VPSDK_VERSION 4

/**
 *	Events can be registered using #vp_event_set
 */
typedef enum vp_event_t
{
/**
 *	Called when a user in the same world the instance is in sends a message.
 *  Attributes:
 *  - #VP_AVATAR_SESSION
 *  - #VP_AVATAR_NAME
 *  - #VP_CHAT_MESSAGE
 *  \note   You must announce an avatar position (using vp_avatar_change) to be
            able to receive user chat messages.
 */
	VP_EVENT_CHAT,
/**
 *	Called when a user enters the world
 *  Attributes:
 *  - #VP_AVATAR_NAME
 *  - #VP_AVATAR_SESSION
 *  - #VP_AVATAR_X
 *  - #VP_AVATAR_Y
 *  - #VP_AVATAR_Z
 *  - #VP_AVATAR_PITCH
 *  - #VP_AVATAR_YAW
 *  - #VP_AVATAR_TYPE
 *  - #VP_USER_ID
 */
	VP_EVENT_AVATAR_ADD,
/**
 *	Called when a users' avatar changes shape or position
 *  Attributes:
 *  - #VP_AVATAR_NAME
 *  - #VP_AVATAR_SESSION
 *  - #VP_AVATAR_X
 *  - #VP_AVATAR_Y
 *  - #VP_AVATAR_Z
 *  - #VP_AVATAR_PITCH
 *  - #VP_AVATAR_YAW
 *  - #VP_AVATAR_TYPE
 */
	VP_EVENT_AVATAR_CHANGE,
/**
 *	Called when a user leaves the world.
 *  Attributes:
 *
 *  - #VP_AVATAR_NAME
 *  - #VP_AVATAR_SESSION
 */
	VP_EVENT_AVATAR_DELETE,
/**
 *	Called when a new object is created or sent as a result to a query. The
 *	following attributes are set when the event is called:
 *      - #VP_AVATAR_SESSION (set to 0 if this event is in response to a query)
 *		- #VP_OBJECT_ID
 *		- #VP_OBJECT_USER_ID
 *		- #VP_OBJECT_TIME
 *		- #VP_OBJECT_X
 *		- #VP_OBJECT_Y
 *		- #VP_OBJECT_Z
 *		- #VP_OBJECT_ROTATION_X
 *		- #VP_OBJECT_ROTATION_Y
 *		- #VP_OBJECT_ROTATION_Z
 *		- #VP_OBJECT_ROTATION_ANGLE
 *		- #VP_OBJECT_TYPE
 *		- #VP_OBJECT_DATA
 *		- #VP_OBJECT_MODEL
 *		- #VP_OBJECT_DESCRIPTION
 *		- #VP_OBJECT_ACTION
 */
	VP_EVENT_OBJECT,
/**
 *	Called when an object is changed by another user. The following attributes
 *	are set when the event is called:
 *      - #VP_AVATAR_SESSION
 *		- #VP_OBJECT_ID
 *		- #VP_OBJECT_USER_ID
 *		- #VP_OBJECT_TIME
 *		- #VP_OBJECT_X
 *		- #VP_OBJECT_Y
 *		- #VP_OBJECT_Z
 *		- #VP_OBJECT_ROTATION_X
 *		- #VP_OBJECT_ROTATION_Y
 *		- #VP_OBJECT_ROTATION_Z
 *		- #VP_OBJECT_ROTATION_ANGLE
 *		- #VP_OBJECT_TYPE
 *		- #VP_OBJECT_DATA
 *		- #VP_OBJECT_MODEL
 *		- #VP_OBJECT_DESCRIPTION
 *		- #VP_OBJECT_ACTION
 */
	VP_EVENT_OBJECT_CHANGE,

/**
 *	Called when an object is deleted another user. The #VP_OBJECT_ID attribute
 *	is set when the event is called.
 *  Attributes:
 *  - #VP_AVATAR_SESSION
 *  - #VP_OBJECT_ID
 */
	VP_EVENT_OBJECT_DELETE,

/**
 *  Called when an object is clicked by another user. The following attributes
 *  are set when the event is called:
 *  - #VP_AVATAR_SESSION
 *  - #VP_OBJECT_ID
 *  - #VP_CLICK_HIT_X
 *  - #VP_CLICK_HIT_Y
 *  - #VP_CLICK_HIT_Z
 */
	VP_EVENT_OBJECT_CLICK,

	/**
	 *  Attributes:
	 *	Called when a world changes state or when the world list is requested using
	 *	#vp_world_list. 
	 *		- #VP_WORLD_NAME
	 *		- #VP_WORLD_USERS
	 *		- #VP_WORLD_STATE
	 */
	VP_EVENT_WORLD_LIST,

	/**
	 *  Attributes:
	 *  - #VP_WORLD_SETTING_KEY
	 *  - #VP_WORLD_SETTING_VALUE
	 */
	VP_EVENT_WORLD_SETTING,

	/**
	 *  Attributes:
	 *  Called when the server is done sending world settings
	 */
	VP_EVENT_WORLD_SETTINGS_CHANGED,

	VP_EVENT_FRIEND,
    
    /**
     *  Attributes:
     *  - #VP_DISCONNECT_ERROR_CODE
     */
	VP_EVENT_WORLD_DISCONNECT,
    
    /**
     *  Attributes:
     *  - #VP_DISCONNECT_ERROR_CODE
     */
	VP_EVENT_UNIVERSE_DISCONNECT,

	/**
	 *  Attributes:
	 *  - #VP_USER_ID
	 *  - #VP_USER_REGISTRATION_TIME
	 *  - #VP_USER_ONLINE_TIME
	 *  - #VP_USER_LAST_LOGIN
	 *  - #VP_USER_NAME
	 *  - #VP_USER_EMAIL
	 */
	VP_EVENT_USER_ATTRIBUTES,
	
	/**
	 *  Attributes:
	 *  - #VP_CELL_X
	 *  - #VP_CELL_Z
	 */
	VP_EVENT_CELL_END,
    
	/**
	 *  Attributes:
	 *  - #VP_TERRAIN_NODE_DATA
	 *  - #VP_TERRAIN_NODE_X
	 *  - #VP_TERRAIN_NODE_Z
	 *  - #VP_TERRAIN_NODE_REVISION
	 *  - #VP_TERRAIN_TILE_X
	 *  - #VP_TERRAIN_TILE_Z
	 */
    VP_EVENT_TERRAIN_NODE,
    
    /**
     *  Attributes:
     *  - #VP_AVATAR_SESSION
     *  - #VP_CLICKED_SESSION
     *  - #VP_CLICK_HIT_X
     *  - #VP_CLICK_HIT_Y
     *  - #VP_CLICK_HIT_Z
     */
    VP_EVENT_AVATAR_CLICK,
    
    /**
     *  Attributes:
     *  - VP_AVATAR_SESSION
     *  - VP_TELEPORT_X
     *  - VP_TELEPORT_Y
     *  - VP_TELEPORT_Z
     *  - VP_TELEPORT_YAW
     *  - VP_TELEPORT_PITCH
     *  - VP_TELEPORT_WORLD
     */
    VP_EVENT_TELEPORT,
	
    /**
     *  Attributes:
     *  - VP_AVATAR_SESSION
     *  - VP_URL
     *  - VP_URL_TARGET
     */
    VP_EVENT_URL,

    /**
     *  Attributes:
     *  - VP_AVATAR_SESSION
     *  - VP_OBJECT_ID
     */
    VP_EVENT_OBJECT_BUMP_BEGIN,

    /**
    *  Attributes:
    *  - VP_AVATAR_SESSION
    *  - VP_OBJECT_ID
    */
    VP_EVENT_OBJECT_BUMP_END,

    /**
    *   Attributes:
    *   - VP_TERRAIN_TILE_X
    *   - VP_TERRAIN_TILE_Z
    *   - VP_TERRAIN_NODE_X
    *   - VP_TERRAIN_NODE_Z
    */
    VP_EVENT_TERRAIN_NODE_CHANGED,

	/**
	 *  A join request was received
	 *  Attributes:
	 *  - VP_JOIN_ID
	 *  - VP_JOIN_NAME
	 *  - VP_USER_ID
	 */
	VP_EVENT_JOIN,
    
	VP_HIGHEST_EVENT
} vp_event_t;

typedef enum vp_callback_t
{
	/**
	 *  The attribute #VP_OBJECT_ID is set to the object ID of the new object
	 */
	VP_CALLBACK_OBJECT_ADD,
    /**
     *  #VP_OBJECT_ID attribute is set to the changed object ID
     */
	VP_CALLBACK_OBJECT_CHANGE,
    /**
     *  #VP_OBJECT_ID attribute is set to the deleted object ID
     */
	VP_CALLBACK_OBJECT_DELETE,
	VP_CALLBACK_GET_FRIENDS,
	VP_CALLBACK_FRIEND_ADD,
	VP_CALLBACK_FRIEND_DELETE,
    VP_CALLBACK_TERRAIN_QUERY,
    VP_CALLBACK_TERRAIN_NODE_SET,
    
    /**
     * Reason codes:
     * - #VP_RC_SUCCESS
     * - #VP_RC_DATABASE_ERROR
     * - #VP_RC_OBJECT_NOT_FOUND
     * - #VP_RC_UNKNOWN_ERROR
     *
     * Attributes:
     * - #VP_OBJECT_ID
     * - #VP_OBJECT_USER_ID
     * - #VP_OBJECT_TIME
     * - #VP_OBJECT_X
     * - #VP_OBJECT_Y
     * - #VP_OBJECT_Z
     * - #VP_OBJECT_ROTATION_X
     * - #VP_OBJECT_ROTATION_Y
     * - #VP_OBJECT_ROTATION_Z
     * - #VP_OBJECT_ROTATION_ANGLE
     * - #VP_OBJECT_TYPE
     * - #VP_OBJECT_DATA
     * - #VP_OBJECT_MODEL
     * - #VP_OBJECT_DESCRIPTION
     * - #VP_OBJECT_ACTION
     */
    VP_CALLBACK_OBJECT_GET,
    
    /**
     * Reason codes
     * - #VP_RC_SUCCESS
     * - #VP_RC_NOT_ALLOWED
     # - #VP_RC_DATABASE_ERROR
     */
    VP_CALLBACK_OBJECT_LOAD,
    
    /**
     *  Attributes:
     *  - VP_MY_USER_ID
     */
    VP_CALLBACK_LOGIN,

    VP_CALLBACK_ENTER,

	VP_CALLBACK_JOIN,
	VP_CALLBACK_CONNECT_UNIVERSE,

    VP_CALLBACK_WORLD_PERMISSION_USER_SET,
    VP_CALLBACK_WORLD_PERMISSION_SESSION_SET,
    VP_CALLBACK_WORLD_SETTING_SET,
    
	VP_HIGHEST_CALLBACK
} vp_callback_t;

/* Ints */
typedef enum vp_int_attribute_t
{
	VP_AVATAR_SESSION,
	VP_AVATAR_TYPE,
	VP_MY_TYPE,
	
	VP_OBJECT_ID,
	VP_OBJECT_TYPE,
	VP_OBJECT_TIME,
	VP_OBJECT_USER_ID,
	
	VP_WORLD_STATE,
	VP_WORLD_USERS,
	
	VP_REFERENCE_NUMBER,
	VP_CALLBACK,
	
	VP_USER_ID,
	VP_USER_REGISTRATION_TIME,
	VP_USER_ONLINE_TIME,
	VP_USER_LAST_LOGIN,
	
    /**
     *  @deprecated
     */
	VP_FRIEND_ID,
	VP_FRIEND_USER_ID,
	VP_FRIEND_ONLINE,
	
	VP_MY_USER_ID,
	VP_PROXY_TYPE,
	VP_PROXY_PORT,
	
	VP_CELL_X,
	VP_CELL_Z,
    
    VP_TERRAIN_TILE_X,
    VP_TERRAIN_TILE_Z,
    VP_TERRAIN_NODE_X,
    VP_TERRAIN_NODE_Z,
    VP_TERRAIN_NODE_REVISION,
    
    VP_CLICKED_SESSION,
    
    VP_CHAT_TYPE,
    VP_CHAT_COLOR_RED,
    VP_CHAT_COLOR_GREEN,
    VP_CHAT_COLOR_BLUE,
    VP_CHAT_EFFECTS,
    
    VP_DISCONNECT_ERROR_CODE,
    
    VP_URL_TARGET,
    VP_CURRENT_EVENT,
    VP_CURRENT_CALLBACK,

    VP_CELL_REVISION,
    VP_CELL_STATUS,

	VP_JOIN_ID,
	
	VP_HIGHEST_INT
} vp_int_attribute_t;

/* Floats */
typedef enum vp_float_attribute_t
{
	VP_AVATAR_X,
	VP_AVATAR_Y,
	VP_AVATAR_Z,
	VP_AVATAR_YAW,
	VP_AVATAR_PITCH,
	
	VP_MY_X,
	VP_MY_Y,
	VP_MY_Z,
	VP_MY_YAW,
	VP_MY_PITCH,
	
	VP_OBJECT_X,
	VP_OBJECT_Y,
	VP_OBJECT_Z,
	VP_OBJECT_ROTATION_X,
	VP_OBJECT_ROTATION_Y,
	VP_OBJECT_ROTATION_Z,
	/**
	 * Only works correctly when #VP_OBJECT_ROTATION_ANGLE is set to infinity
	 * \deprecated Use #VP_OBJECT_ROTATION_X, #VP_OBJECT_ROTATION_Y, #VP_OBJECT_ROTATION_Z and #VP_OBJECT_ROTATION_ANGLE
	 */
	VP_OBJECT_YAW = VP_OBJECT_ROTATION_X,
	/**
	 * Only works correctly when #VP_OBJECT_ROTATION_ANGLE is set to infinity
	 * \deprecated Use #VP_OBJECT_ROTATION_X, #VP_OBJECT_ROTATION_Y, #VP_OBJECT_ROTATION_Z and #VP_OBJECT_ROTATION_ANGLE
	 */
	VP_OBJECT_PITCH = VP_OBJECT_ROTATION_Y,
	/**
	 * Only works correctly when #VP_OBJECT_ROTATION_ANGLE is set to infinity
	 * \deprecated Use #VP_OBJECT_ROTATION_X, #VP_OBJECT_ROTATION_Y, #VP_OBJECT_ROTATION_Z and #VP_OBJECT_ROTATION_ANGLE
	 */
	VP_OBJECT_ROLL = VP_OBJECT_ROTATION_Z,
	VP_OBJECT_ROTATION_ANGLE,
    
    VP_TELEPORT_X,
    VP_TELEPORT_Y,
    VP_TELEPORT_Z,
    VP_TELEPORT_YAW,
    VP_TELEPORT_PITCH,
    
    VP_CLICK_HIT_X,
    VP_CLICK_HIT_Y,
    VP_CLICK_HIT_Z,

	VP_JOIN_X,
	VP_JOIN_Y,
	VP_JOIN_Z,
	VP_JOIN_YAW,
	VP_JOIN_PITCH,
	
	VP_HIGHEST_FLOAT
} vp_float_attribute_t;

/* Strings */
typedef enum vp_string_attribute_t
{
	VP_AVATAR_NAME,
	VP_CHAT_MESSAGE,
	
	VP_OBJECT_MODEL,
	VP_OBJECT_ACTION,
	VP_OBJECT_DESCRIPTION,
	
	VP_WORLD_NAME,
	
	VP_USER_NAME,
	VP_USER_EMAIL,
	
	VP_WORLD_SETTING_KEY,
	VP_WORLD_SETTING_VALUE,
	
	VP_FRIEND_NAME,
	VP_PROXY_HOST,
    
    VP_TELEPORT_WORLD,
    
    VP_URL,
	VP_JOIN_WORLD,
	VP_JOIN_NAME,
    
    VP_START_WORLD,
	
	VP_HIGHEST_STRING
} vp_string_attribute_t;

/* Data */
typedef enum vp_data_attribute_t
{
	VP_OBJECT_DATA,
    VP_TERRAIN_NODE_DATA,
	VP_HIGHEST_DATA
} vp_data_attribute_t;

/**
 *	Proxy types
 */
typedef enum vp_proxy_type_t {
	VP_PROXY_TYPE_NONE,
	VP_PROXY_TYPE_SOCKS4A
} vp_proxy_type_t;

typedef enum vp_url_target_t {
    VP_URL_TARGET_BROWSER,
    VP_URL_TARGET_OVERLAY
} vp_url_target_t;

typedef enum vp_cell_status_t {
    VP_CELL_STATUS_MODIFIED,
    VP_CELL_STATUS_NOT_MODIFIED,
    VP_CELL_STATUS_ERROR
} vp_cell_status_t;

#ifdef __cplusplus
namespace vpsdk {
    struct VPInstance_;
}

typedef vpsdk::VPInstance_ *vp_instance_t;
#else
typedef void* vp_instance_t;
#endif

typedef vp_instance_t VPInstance;

typedef void(*VPEventHandler)(VPInstance);
typedef void(*VPCallbackHandler)(VPInstance, int, int);

typedef struct vp_terrain_cell_t {
    float height;
    
    /* 1 bit visibility, 2 bits rotation, 1 bit padding, 12 bits texture */
    unsigned short attributes;
} vp_terrain_cell_t;

#define VP_PACK_TERRAIN_ATTRIBUTES(tex, rotation, visible) ((tex & 0x1FFF) |   \
                                                            (visible << 15) |  \
                                                            (rotation << 13))

#define VP_UNPACK_TERRAIN_VISIBILITY(x) ((x & 0x8000) >> 15)
#define VP_UNPACK_TERRAIN_ROTATION(x) ((x & 0x6000) >> 13)
#define VP_UNPACK_TERRAIN_TEXTURE(x) (x & 0x0FFF)

/**
 *  Chat message types. 
 */
enum vp_chat_type {
    VP_CHAT_NORMAL,
    VP_CHAT_CONSOLE_MESSAGE,
    VP_CHAT_PRIVATE
};

/**
 *  Text effect flags. Can be combined with bitwise OR operator.
 */
enum vp_text_effect {
    VP_TEXT_EFFECT_BOLD = 1,
    VP_TEXT_EFFECT_ITALIC = 2
};

/** \fn VPSDK_API int vp_init(int version=VPSDK_VERSION)
 *  Check if the version of the VPSDK used to build the application matches the
 *  version that is currently loaded.
 *  \param      version
 *  \returns    #VP_RC_SUCCESS              if successful
 *  \returns    #VP_RC_VERSION_MISMATCH     if the versions don't match
 */
#ifdef __cplusplus
VPSDK_API int vp_init(int version=VPSDK_VERSION);
#else
VPSDK_API int vp_init(int version);
#endif

typedef struct vp_net_config vp_net_config;
/**
 *  Create a new instance.
 *  \param net_config network configuration for the instance or NULL to use 
 *                    the default select-based implementation. The contents of 
 *                    the structure will be copied.
 *  \return New instance or NULL on failure.
 */
VPSDK_API vp_instance_t vp_create(const vp_net_config* net_config);

/**
 *  Destroy a Virtual Paradise SDK instance.
 */
VPSDK_API int vp_destroy(vp_instance_t instance);

/**
 *  Connect to a universe server. If a #VP_CALLBACK_CONNECT_UNIVERSE callback
 *  is set this function will return immediately and return the result using 
 *  the callback, otherwise it will call #vp_wait in a loop until done.
 *
 *  \param instance
 *  \param host Host address of server to connect to.
 *  \param port TCP port of remote server.
 *  \returns #VP_RC_SUCCESS
 *  \returns #VP_RC_CONNECTION_ERROR
 */
VPSDK_API int vp_connect_universe(vp_instance_t instance, const char * host, int port);

/**
 *  Login to the universe server. If a #VP_CALLBACK_LOGIN callback is set this 
 *  function will return immediately and return the result using the callback, 
 *  otherwise it will call #vp_wait in a loop until done.
 *
 *  \param instance
 *  \param username
 *  \param password
 *  \param botname
 *  \returns #VP_RC_SUCCESS
 *  \returns #VP_RC_STRING_TOO_LONG
 *  \returns #VP_RC_INVALID_LOGIN
 *  \returns #VP_RC_TIMEOUT
 *  \returns #VP_RC_NOT_IN_UNIVERSE
 */
VPSDK_API int vp_login(vp_instance_t instance, const char * username, const char * password, const char * botname);

/**
 *  Process incoming and outgoing data. Waits for connection to be ready for 
 *  sending or receiving. This function needs to be called for events to fire.
 *
 *  \param instance
 *  \param milliseconds The maximum time to wait in milliseconds.
 *  \warning Not reentrant when used with the same instance, may not be called from event handlers unless it is for a different instance.
 *  \return #VP_RC_SUCCESS success
 *  \return #VP_RC_NOT_SUPPORTED #vp_wait is not supported by this instance, for example a custom vp_net_config was passed to #vp_create
 *  \return #VP_RC_RECURSIVE_WAIT #vp_wait was called recursively on the same instance
 *  \return #VP_RC_CONNECTION_ERROR waiting for data failed or there are no connections to wait for events on
 */
VPSDK_API int vp_wait(vp_instance_t instance, int milliseconds);

/**
 *  Enter a world, the current world will be left. If a #VP_CALLBACK_ENTER 
 *  callback is set this function will return immediately and return the result
 *  using the callback, otherwise it will call #vp_wait in a loop until done.
 *
 *  \param instance
 *  \param world_name the name of the world to enter
 *  \warning This function uses #vp_wait internally, the same warnings apply.
 *  \returns #VP_RC_SUCCESS success
 *  \returns #VP_RC_STRING_TOO_LONG world name was longer than 255 bytes
 *  \returns #VP_RC_CONNECTION_ERROR disconnected from universe while sending or connecting to the world failed
 *  \returns #VP_RC_WORLD_NOT_FOUND world not found
 *  \returns #VP_RC_WORLD_LOGIN_ERROR failed to log in on the world server
 *  \returns #VP_RC_TIMEOUT
 *  \returns #VP_RC_NOT_IN_UNIVERSE
 */
VPSDK_API int vp_enter(vp_instance_t instance, const char * world_name);

/**
 *	Leave the current world.
 *  \param instance
 *  \returns #VP_RC_SUCCESS
 *  \returns #VP_RC_NOT_IN_WORLD
 */
VPSDK_API int vp_leave(vp_instance_t instance);

/**
 *  Send a simple message to everyone in the current world.
 *  \param instance
 *  \param message The message to send.
 *  \returns #VP_RC_SUCCESS
 *  \returns #VP_RC_NOT_IN_WORLD
 *  \returns #VP_RC_STRING_TOO_LONG
 */
VPSDK_API int vp_say(vp_instance_t instance, const char * message);

/**
 *  Send a console message.
 *  \param instance
 *  \param session The session ID to send the message to. Zero to send to everyone
 *  \param name The name to use for the chat message. Empty string to hide name.
 *  \param message Chat message contents
 *  \param effects Text effects (combination of #vp_text_effect flags)
 *  \param red Red component of the text color(0-255)
 *  \param green Green component of the text color(0-255)
 *  \param blue Blue component of the text color(0-255)
 *  \returns #VP_RC_SUCCESS
 *  \returns #VP_RC_NOT_IN_WORLD
 *  \returns #VP_RC_STRING_TOO_LONG
 */
VPSDK_API int vp_console_message(vp_instance_t instance,
                                 int session,
                                 const char* name,
                                 const char* message,
                                 int effects,
                                 unsigned char red,
                                 unsigned char green,
                                 unsigned char blue);

/**
 *  Register an event handler.
 *  \param instance
 *  \param event_name
 *  \param event function pointer for the event handler
 *  \return Zero when successful, otherwise nonzero. See RC.h
 */
VPSDK_API int vp_event_set(vp_instance_t instance, vp_event_t event_name, VPEventHandler event);

/**
 *  Register a callback function.
 *  \param instance
 *  \param callbackname
 *  \param callback
 *  \return Zero when successful, otherwise nonzero. See RC.h
 */
VPSDK_API int vp_callback_set(vp_instance_t instance, vp_callback_t callbackname, VPCallbackHandler callback);

/**
 *  Retrieve the pointer to user-defined data for this instance.
 *  \param instance
 *  \return Pointer to user-defined data.
 */
VPSDK_API void * vp_user_data(vp_instance_t instance);

/**
 *  Sets a pointer to user-defined data for this instance.
 *  This pointer is not accessed in any way, allocating and freeing it is the responsibility of the application programmer.
 *  \param instance
 *  \param data The pointer to your user-defined data.
 */
VPSDK_API void vp_user_data_set(vp_instance_t instance, void * data);

/**
 *  Update avatar
 *
 *  Attributes:
 *  - #VP_MY_X
 *  - #VP_MY_Y
 *  - #VP_MY_Z
 *  - #VP_MY_YAW
 *  - #VP_MY_PITCH
 *  - #VP_MY_TYPE
 *  \param instance
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NOT_IN_WORLD
 */
VPSDK_API int vp_state_change(vp_instance_t instance);

/**
 *  \param instance
 *  \param attr attribute key
 *  \return the value of the attribute
 */
VPSDK_API int vp_int(vp_instance_t instance, vp_int_attribute_t attr);

/**
 *  \param instance
 *  \param attr attribute key
 *  \return the value of the attribute
 */
VPSDK_API float vp_float(vp_instance_t instance, vp_float_attribute_t attr);

/**
 *  \param instance
 *  \param attr attribute key
 *  \return the value of the attribute
 */
VPSDK_API double vp_double(vp_instance_t instance, vp_float_attribute_t attr);

/**
 *  \param instance
 *  \param attr attribute key
 *  \return the value of the attribute
 */
VPSDK_API const char* vp_string(vp_instance_t instance, vp_string_attribute_t attr);

/**
 *  \param instance
 *  \param attr attribute key
 *  \param length pointer to the int where the length of the data will be written
 *  \return the value of the attribute
 */
VPSDK_API const char* vp_data(vp_instance_t instance, vp_data_attribute_t attr, int* length);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API int vp_int_get(vp_instance_t instance, vp_int_attribute_t attr, int* value);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API int vp_float_get(vp_instance_t instance, vp_float_attribute_t attr, float* value);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API int vp_double_get(vp_instance_t instance, vp_float_attribute_t attr, double* value);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API int vp_string_get(vp_instance_t instance, vp_string_attribute_t attr, char** value);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API int vp_int_set(vp_instance_t instance, vp_int_attribute_t name, int value);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API int vp_float_set(vp_instance_t instance, vp_float_attribute_t name, float value);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API int vp_double_set(vp_instance_t instance, vp_float_attribute_t attr, double value);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API void vp_string_set(vp_instance_t instance, vp_string_attribute_t name, const char * str);

/**
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NO_SUCH_ATTRIBUTE
 */
VPSDK_API int vp_data_set(vp_instance_t instance, vp_data_attribute_t name, int length, const char* data);

/**
 *	Query the objects in a single cell
 *	Each object will be sent in a #VP_EVENT_OBJECT event. After all the objects
 *  for the cell have been sent, a #VP_EVENT_CELL_END event will be raised.
 */
VPSDK_API int vp_query_cell(vp_instance_t instance, int x, int z);
VPSDK_API int vp_query_cell_revision(vp_instance_t instance, int x, int z, int revision);

/**
 *  Builds a new object
 *
 *  Uses attributes:
 *  - #VP_OBJECT_TYPE
 *  - #VP_OBJECT_X
 *  - #VP_OBJECT_Y
 *  - #VP_OBJECT_Z
 *  - #VP_OBJECT_ROTATION_X
 *  - #VP_OBJECT_ROTATION_Y
 *  - #VP_OBJECT_ROTATION_Z
 *  - #VP_OBJECT_ROTATION_ANGLE
 *  - #VP_OBJECT_MODEL
 *  - #VP_OBJECT_ACTION
 *  - #VP_OBJECT_DESCRIPTION
 *  - #VP_OBJECT_DATA
 */
VPSDK_API int vp_object_add(vp_instance_t instance);

VPSDK_API int vp_object_load(vp_instance_t instance);

/**
 *  Send a object contact begin event to other users in the world
 *  \param instance
 *  \param object_id
 *  \param session_to Session ID to send a bump event to, or 0 to send to everyone
 */
VPSDK_API int vp_object_bump_begin(vp_instance_t instance,
                                   int object_id, int session_to);

/**
*  Send a object contact end event to other users in the world
*  \param instance
*  \param object_id
*  \param session_to Session ID to send a bump event to, or 0 to send to everyone
*/
VPSDK_API int vp_object_bump_end(vp_instance_t instance,
                                 int object_id, int session_to);

/**
 *  Changes an existing object
 *
 *  Uses attributes:
 *  - #VP_OBJECT_ID
 *  - #VP_OBJECT_TYPE
 *  - #VP_OBJECT_X
 *  - #VP_OBJECT_Y
 *  - #VP_OBJECT_Z
 *  - #VP_OBJECT_ROTATION_X
 *  - #VP_OBJECT_ROTATION_Y
 *  - #VP_OBJECT_ROTATION_Z
 *  - #VP_OBJECT_ROTATION_ANGLE
 *  - #VP_OBJECT_MODEL
 *  - #VP_OBJECT_ACTION
 *  - #VP_OBJECT_DESCRIPTION
 *  - #VP_OBJECT_DATA
 */
VPSDK_API int vp_object_change(vp_instance_t instance);

/**
 *  Sends an object click event to other users in the world.
 *  \param instance
 *  \param object_id            Object ID of the clicked object
 *  \param session_to           Target session, or 0 to send to everyone
 *  \param hit_x,hit_y,hit_z    Position where the object was hit
 *  \returns #VP_RC_SUCCESS
 *  \returns #VP_RC_NOT_IN_WORLD
 */
VPSDK_API int vp_object_click(vp_instance_t instance, int object_id, 
                              int session_to, float hit_x, 
                              float hit_y, float hit_z);

/**
 *  Delete an object
 *  \param instance
 *  \param object_id ID of the object to be deleted
 */
VPSDK_API int vp_object_delete(vp_instance_t instance, int object_id);

/**
 *  Request the attributes of a single object. The result will be returned in
 *  the #VP_CALLBACK_OBJECT_GET callback.
 */
VPSDK_API int vp_object_get(vp_instance_t instance, int object_id);

/**
 *  Request the world list.
 *  The worlds will be listed in the #VP_EVENT_WORLD_LIST event. See vp_event_set().
 *  \param instance
 *  \param time Time since your last update. This is not used yet, the whole list will be requested.
 */
VPSDK_API int vp_world_list(vp_instance_t instance, int time);

/* VPSDK_API void* vp_callback_pointer(vp_instance_t instance); */
/* VPSDK_API void vp_callback_pointer_set(vp_instance_t instance, void* ptr); */

/**
 *  Request user attributes by user ID.
 *  The user attributes will be returned in the #VP_EVENT_USER_ATTRIBUTES event.
 *  \return Zero when successful, otherwise nonzero
 */
VPSDK_API int vp_user_attributes_by_id(vp_instance_t instance, int user_id);

/**
 *  Get user attributes by user name. Not implemented.
 *  \returns #VP_RC_NOT_IMPLEMENTED
 */
VPSDK_API int vp_user_attributes_by_name(vp_instance_t instance, const char * name);

VPSDK_API int vp_friends_get(vp_instance_t instance);
VPSDK_API int vp_friend_add_by_name(vp_instance_t instance, const char* name);
VPSDK_API int vp_friend_delete(vp_instance_t instance, int friend_user_id);

/**
 *  Query a terrain tile
 *  \param instance
 *  \param  tile_x
 *  \param  tile_z
 *  \param  revision 16 node revision numbers (4x4: revision[z][x])
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NOT_IN_WORLD
 */
VPSDK_API int vp_terrain_query(vp_instance_t instance, int tile_x, int tile_z, int revision[][4]);

/**
 *  Replace terrain node data
 *  \param  instance
 *  \param  tile_x
 *  \param  tile_z
 *  \param  node_x
 *  \param  node_z
 *  \param  cells Pointer to 64 cells (8x8: cells[z*8+x])
 *  \return #VP_RC_SUCCESS
 *  \return #VP_RC_NOT_IN_WORLD
 */
VPSDK_API int vp_terrain_node_set(vp_instance_t instance, 
                                  int tile_x, int tile_z, 
                                  int node_x, int node_z, 
                                  struct vp_terrain_cell_t* cells);

/**
 *  Send an avatar click event to other users in the world.
 *  Uses the following attributes:
 *  - #VP_CLICK_HIT_X
 *  - #VP_CLICK_HIT_Y
 *  - #VP_CLICK_HIT_Z
 *
 *  \param      instance
 *  \param      avatar_session The session id of the clicked avatar
 *  \returns    #VP_RC_SUCCESS
 *  \returns    #VP_RC_NOT_IN_WORLD
 */
VPSDK_API int vp_avatar_click(vp_instance_t instance, int avatar_session);

/**
 *  Request that another avatar teleports to a new location.
 *  \note this is only a request and receiving client can choose to ignore it
 *  \param instance
 *  \param target_session   session ID of the avatar to teleport
 *  \param world            destination world, empty string to teleport avatar in current world
 *  \param x,y,z            avatar position
 *  \param yaw,pitch        avatar rotation
 *  \returns                #VP_RC_SUCCESS
 *  \returns                #VP_RC_NOT_IN_WORLD (this is about the bot instance, not the avatar to be teleported)
 */
VPSDK_API int vp_teleport_avatar(vp_instance_t instance,
                                 int target_session,
                                 const char* world,
                                 float x, float y, float z,
                                 float yaw, float pitch);

/**
 *  Send a URL to a user
 *  \param instance
 *  \param session_id   target session id
 *  \param url          the URL to send to the target session ID
 *  \param url_target   Where to open the URL, see #vp_url_target_t
 */
VPSDK_API int vp_url_send(vp_instance_t instance,
                          int session_id,
                          const char* url,
                          vp_url_target_t url_target);

VPSDK_API int vp_join(vp_instance_t instance, int user_id);
VPSDK_API int vp_join_accept(vp_instance_t instance, int requestId, 
                             const char* world, double x, double y, double z,
                             float yaw, float pitch);
VPSDK_API int vp_join_decline(vp_instance_t instance, int requestId);

VPSDK_API int vp_world_permission_user_set(vp_instance_t instance, const char* permission, int user_id, int enable);
VPSDK_API int vp_world_permission_session_set(vp_instance_t instance, const char* permission, int session_id, int enable);

VPSDK_API int vp_world_setting_set(vp_instance_t instance, const char* setting, const char* value, int session_to);

#endif
