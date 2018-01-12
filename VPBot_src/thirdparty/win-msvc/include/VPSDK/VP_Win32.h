#ifndef VPSDK_VP_Win32_h__
#define VPSDK_VP_Win32_h__

#include "VP.h"

/**
 *  Initialize global state for Windows-message based notification of network
 *  events.
 */
VPSDK_API int vp_winsock_async_init();

/**
 *  Release global state for Windows-message based notification of network 
 *  events.
 */
VPSDK_API int vp_winsock_async_term();

/**
 *  Create a network configuration object
 */
VPSDK_API int vp_winsock_async_create(vp_net_config* net_config);

/**
 *  \param net_config pointer to a #vp_net_config that was initialized by #vp_winsock_async_create
 */
VPSDK_API int vp_winsock_async_destroy(vp_net_config* net_config);

#endif
