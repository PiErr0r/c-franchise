#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "logger.h"

int main(int argc, char* argv[]) {
  struct addrinfo hints, *res, *p;
  int status, sockfd;
  char ipstr[INET6_ADDRSTRLEN];
  char *info_str = (char *)malloc(100 * sizeof(char));
  char *err_str = (char *)malloc(100 * sizeof(char));
  if (info_str == NULL || err_str == NULL) {
    err("Couldn't allocate memory for err or info string");
    return -1;
  }


  if (argc != 3) {
    err("usage: hostname port");
    return 1;
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0) {
    sprintf(err_str, "getaddrinfo: %s", gai_strerror(status));
    err(err_str);
    return 2;
  }

  printf("IP addresses for  %s:\n\n", argv[1]);

  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (p->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in*) p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else if(p->ai_family == AF_INET6) {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*) p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    } else {
      err("Unknown address family!");
    }

    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("\t%s: %s\n", ipver, ipstr);

    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1) {
      // bind
      if ((status = bind(sockfd, p->ai_addr, p->ai_addrlen)) == - 1){
        err("Not able to bind the socket descriptor!");
        return 3;
      }
      sprintf(info_str, "Bound to port %s", argv[2]);
      info(info_str);

      // connect
      if ((status = connect(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
        sprintf(err_str, "Couldn't connect to host: %s", argv[1]);
        err(err_str);
        return 4;
      }
      info("Connected");
      break;
    }
  }

  

  freeaddrinfo(res);
  return 0;
}


