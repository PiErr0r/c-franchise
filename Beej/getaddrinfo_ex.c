#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main(void) {

int status;
struct addrinfo hints;
struct addrinfo *servinfo;        // pointer to results

memset(&hints, 0, sizeof(hints)); // struct is empty
hints.ai_family = AF_UNSPEC;      // can be IPv4 or IPv6 (use AF_INET or AF_INET6 if you want to use one of them specifically)
hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
hints.ai_flags = AI_PASSIVE;      // fill in IP for me - specify to use my localhost address to socket structs so you don't have to do it by hand
                                  // other way is to specify it instead of NULL in the getaddrinfo call

// this doesn't actually connect but sets up the structs
if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
  exit(1);
}

printf("%d\n", hints.ai_socktype);

// do stuff with linked list of strucg addrinfos ...

// free the linked list
freeaddrinfo(servinfo);
  return 0;
}
