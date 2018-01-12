/**
 *  \file
 *  \brief Functions and callbacks for network IO
 */
#ifndef VPSDK_VP_net_h
#define VPSDK_VP_net_h

#include "VP.h"

/**
*  values to return from the vp_net_config callbacks
*/
enum VP_NET_RC {
    VP_NET_RC_SUCCESS = 0,
    VP_NET_RC_CONNECTION_ERROR = -1,
    VP_NET_RC_WOULD_BLOCK = -2
};

enum VP_NET_NOTIFY {
    /**
     * A connect call finished.
     * \param status a #VP_NET_RC code
     */
    VP_NET_NOTIFY_CONNECT,
    
    /**
     * The connection was lost.
     */
    VP_NET_NOTIFY_DISCONNECT,

    /**
     * Data is ready to be received from this connection.
     */
    VP_NET_NOTIFY_READ_READY,

    /**
     * The connection is ready to send data.
     */
    VP_NET_NOTIFY_WRITE_READY,

    /**
     * The timeout for this connection has expired.
     */
    VP_NET_NOTIFY_TIMEOUT
};

typedef void* vp_net_connection_t;

/**
 * Create a new connection object.
 * \param connection pointer to the internal vpsdk connection object, this 
 *                   should be used to send notifications using #vp_net_notify.
 * \param context the pointer that was set in #vp_net_config.context when #vp_create was called
 */
typedef void*(*vp_net_create_t)(vp_net_connection_t connection, void* context);

/**
 * Destroy a connection.
 * \param obj the pointer that was returned by the #vp_net_config.create callback
 */
typedef void(*vp_net_destroy_t)(void* obj);

/**
 * Initiate a TCP connection. This call should result in a call to 
 * #vp_net_notify with #VP_NET_NOTIFY_CONNECT.
 *
 * \param obj the pointer that was returned by the #vp_net_config.create callback
 * \param host host name of the server
 * \param port TCP port number
 * \return a #VP_NET_RC error code
 */
typedef int(*vp_net_connect_t)(void* obj, const char* host, unsigned short port);

/**
 * \param obj the pointer that was returned by the #vp_net_config.create callback
 * \param data pointer to the data to send
 * \param length the number of bytes data points to
 * \return the number of bytes that were sent or a #VP_NET_RC error code
 */
typedef int(*vp_net_send_t)(void* obj, const char* data, unsigned int length);

/**
 * Attempt to receive data from the connection
 *
 * \param obj the pointer that was returned by the #vp_net_config.create callback
 * \param data pointer to a buffer to write received date to
 * \param length the length of the buffer pointed to by data
 * \return the number of bytes that were received or a #VP_NET_RC error code
 */
typedef int(*vp_net_recv_t)(void* obj, char* data, unsigned int length);

/**
 * Set a timeout for this connection. When the timeout expires #vp_net_notify 
 * should be called with #VP_NET_NOTIFY_TIMEOUT
 *
 * \param obj the pointer that was returned by the #vp_net_config.create callback
 * \param timeout the number of seconds in the future the timeout should be set to (or -1 to cancel the previous timeout) 
 */
typedef int(*vp_net_timeout_t)(void* obj, int timeout);

typedef struct vp_net_config {
    vp_net_create_t create;
    vp_net_destroy_t destroy;
    vp_net_connect_t connect;
    vp_net_send_t send;
    vp_net_recv_t recv;
    vp_net_timeout_t timeout;
    void* context;
} vp_net_config;

/**
 * \param connection the connection
 * \param notification_type a notification code from #VP_NET_NOTIFY
 * \param status result of an asynchronous call or 0 if it does not apply to the notification type
 */
VPSDK_API int vp_net_notify(vp_net_connection_t connection, int notification_type, int status);

#endif // !VPSDK_VP_net_h

