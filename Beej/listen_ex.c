#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "logger.h"

#define MYPORT "8080"
#define BACKLOG 10

int main(void) {
  struct addrinfo hints, *res, *p;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  int status, sockfd, newfd;
  char ipstr[INET6_ADDRSTRLEN];
  char *info_str = (char *)malloc(200 * sizeof(char));
  char *err_str = (char *)malloc(200 * sizeof(char));
  if (info_str == NULL || err_str == NULL) {
    err("Couldn't allocate memory for err or info string");
    return 1;
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;

  // AI_PASSIVE set my local address so we can pass NULL
  if ((status = getaddrinfo(NULL, MYPORT, &hints, &res)) != 0) {
    sprintf(err_str, "getaddrinfo: %s", gai_strerror(status));
    err(err_str);
    return 2;
  }

  if (res == NULL) {
    err("No addresses found!");
    return 3;
  }

  inet_ntop(res->ai_family, res->ai_addr, ipstr, sizeof ipstr);
  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
    err("Not able to create socket!");
    return 4;
  }

  if ((status = bind(sockfd, res->ai_addr, res->ai_addrlen)) == -1) {
    err("Not able to bind socket descriptor");
    return 5;
  }

  if ((status = listen(sockfd, BACKLOG)) == -1) {
    sprintf(err_str, "Not able to listen on port: %s", MYPORT);
    err(err_str);
    return 7;
  }

  struct sockaddr_in *ipv4 = (struct sockaddr_in*) res->ai_addr;
  inet_ntop(res->ai_family, &(ipv4->sin_addr), ipstr, sizeof ipstr);
  sprintf(info_str, "Listening on %s:%s", ipstr, MYPORT);
  info(info_str);
  gethostname(info_str, 200);
  info(info_str);

  addr_size = sizeof their_addr;
  if ((newfd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size)) == -1) {
    err("Can't accept a connection!");
    return 8;
  }

  close(sockfd);
  freeaddrinfo(res);
  return 0;
}


