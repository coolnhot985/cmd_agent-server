#ifndef _UTILS_H_
#define _UTILS_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

// Dies (exits with a failure status) after printing the given printf-like
// message to stdout.
void die(char* fmt, ...);

// Wraps malloc with error checking: dies if malloc fails.
void* xmalloc(size_t size);

// Dies (exits with a failure status) after printing the current perror status
// prefixed with msg.
void perror_die(char* msg);

// Reports a peer connection to stdout. sa is the data populated by a successful
// accept() call.
void report_peer_connected(const struct sockaddr_in* sa, socklen_t salen);

// Creates a bound and listening INET socket on the given port number. Returns
// the socket fd when successful; dies in case of errors.
int listen_inet_socket(int portnum);

// Sets the given socket into non-blocking mode.
void make_socket_non_blocking(int sockfd);

// MJ 
void mj_free(void *);

#endif /* UTILS_H */
