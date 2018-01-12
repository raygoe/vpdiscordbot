#ifndef VPSDK_RC_H
#define VPSDK_RC_H

enum VPReturnCode
{
	VP_RC_SUCCESS,
	VP_RC_VERSION_MISMATCH,
	VP_RC_NOT_INITIALIZED,     /**< No longer used */
    VP_RC_ALREADY_INITIALIZED, /**< No longer used */
	VP_RC_STRING_TOO_LONG,
	VP_RC_INVALID_LOGIN /**< incorrect username or password */,
	VP_RC_WORLD_NOT_FOUND,
	VP_RC_WORLD_LOGIN_ERROR,
	VP_RC_NOT_IN_WORLD /**< world request made while not connected to a world server */,
	VP_RC_CONNECTION_ERROR,
	VP_RC_NO_INSTANCE /**< No longer used */,
	VP_RC_NOT_IMPLEMENTED,
	VP_RC_NO_SUCH_ATTRIBUTE,
	VP_RC_NOT_ALLOWED,
	VP_RC_DATABASE_ERROR,
	VP_RC_NO_SUCH_USER,
	VP_RC_TIMEOUT /**< it took too long to receive a response from the server */,
    VP_RC_NOT_IN_UNIVERSE /**< universe request made while not connected to a universe server */,
    VP_RC_INVALID_ARGUMENTS,
    VP_RC_OBJECT_NOT_FOUND,
    VP_RC_UNKNOWN_ERROR,
    VP_RC_RECURSIVE_WAIT /**< #vp_wait was called recursively */,
    VP_RC_JOIN_DECLINED,
    VP_RC_SECURE_CONNECTION_REQUIRED,
    VP_RC_HANDSHAKE_FAILED,
    VP_RC_VERIFICATION_FAILED,
    VP_RC_NO_SUCH_SESSION,
    VP_RC_NOT_SUPPORTED,
    VP_RC_INVITE_DECLINED

};

#endif

