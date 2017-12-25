#ifndef _ERRORS_H
#define _ERRORS_H
#include <errno.h>
#include "errors.hpp"
#include <string.h>

#define EMSG(n,v) const char* n = v;
#define CASEMSG(n,v) case n:return v;

 // EAGAIN or EWOULDBLOCK
 //              The socket is marked nonblocking and the receive operation
 //              would block, or a receive timeout had been set and the timeout
 //              expired before data was received.  POSIX.1 allows either error
 //              to be returned for this case, and does not require these
 //              constants to have the same value, so a portable application
 //              should check for both possibilities.

// const char* eagain = "EAGAIN";
// const char* ewouldblock = "EWOULDBLOCK";
EMSG(eagain,"EAGAIN")
EMSG(ewouldblock,"EWOULDBLOCK")


 //       EBADF  The argument sockfd is an invalid file descriptor.
// const char* ebadf = "EBADF";
EMSG(ebadf,"EBADF")

 //       ECONNREFUSED
 //              A remote host refused to allow the network connection
 //              (typically because it is not running the requested service).
// const char* econnrefused = "ECONNREFUSED";
EMSG(econnrefused,"ECONNREFUSED")

 //       EFAULT The receive buffer pointer(s) point outside the process's
 //              address space.
// const char* efault = "EFAULT";
EMSG(efault,"EFAULT");


 //       EINTR  The receive was interrupted by delivery of a signal before any
 //              data were available; see signal(7).
// const char* eintr = "EINTR";
EMSG(eintr,"EINTR")

 //       EINVAL Invalid argument passed.
EMSG(einval,"EINVAL")

 //       ENOMEM Could not allocate memory for recvmsg().
EMSG(enomen,"ENOMEM")

 //       ENOTCONN
 //              The socket is associated with a connection-oriented protocol
 //              and has not been connected (see connect(2) and accept(2)).
EMSG(enotconn,"ENOTCONN")

 //       ENOTSOCK
 //              The file descriptor sockfd does not refer to a socket.
EMSG(enotsock,"ENOTSOCK")


// EALREADY
//          The socket is nonblocking and a previous connection attempt
//          has not yet been completed.
EMSG(ealready,"EALREADY")

// EINPROGRESS
//        The socket is nonblocking and the connection cannot be
//        completed immediately.  It is possible to select(2) or poll(2)
//        for completion by selecting the socket for writing.  After
//        select(2) indicates writability, use getsockopt(2) to read the
//        SO_ERROR option at level SOL_SOCKET to determine whether
//        connect() completed successfully (SO_ERROR is zero) or
//        unsuccessfully (SO_ERROR is one of the usual error codes
//        listed here, explaining the reason for the failure).

EMSG(einprogress,"EINPROGRESS")

// ENETUNREACH
//               Network is unreachable.
EMSG(enetunreach,"ENETUNREACH")


EMSG(default_error,"DEFAULT ERROR")

const char* get_error_msg(int eno)
{
	switch(eno)
	{
		CASEMSG(EAGAIN,eagain)
		// CASEMSG(EWOULDBLOCK,ewouldblock) //same value
		CASEMSG(EBADF,ebadf)
		CASEMSG(ECONNREFUSED,econnrefused)
		CASEMSG(EFAULT,efault);
		CASEMSG(EINTR,eintr)
		CASEMSG(EINVAL,einval)
		CASEMSG(ENOMEM,enomen)
		CASEMSG(ENOTCONN,enotconn)
		CASEMSG(ENOTSOCK,enotsock)
		CASEMSG(EALREADY,ealready)
		CASEMSG(EINPROGRESS,einprogress)
		CASEMSG(ENETUNREACH,enetunreach)
		default:
			//return default_error;
			return strerror(eno);
	}
}

#endif