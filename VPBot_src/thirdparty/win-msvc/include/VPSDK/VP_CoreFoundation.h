//
//  VP_CoreFoundation.h
//  Virtual Paradise
//
//  Created by Edwin Rijkee on 01-01-17.
//
//

#ifndef VPSDK_VP_CoreFoundation_h
#   define VPSDK_VP_CoreFoundation_h
#   ifdef __APPLE__
#       define VPSDK_USE_CoreFoundation
#   endif
#   ifdef VPSDK_USE_CoreFoundation
#       include "VP_net.h"
#       include <CoreFoundation/CoreFoundation.h>

/**
 *  Initialize a network configuration object
 */
VPSDK_API int vp_corefoundation_init(vp_net_config* net_config, CFRunLoopRef runloop);

#   endif

#endif /* VP_CoreFoundation_h */
