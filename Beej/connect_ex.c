#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void err() {
  fprintf(stderr, "\n[ERROR]\t");
}

void info(char* msg) {
  fprintf(stdout, "\n[INFO]\t%s", msg);
}

int main(int argc, char* argv[]) {
  struct addrinfo hints, *res, *p;
  int status, sockfd;
  char ipstr[INET6_ADDRSTRLEN];

  if (argc != 3) {
    err();
    fprintf(stderr, "usage: hostname port\n");
    return 1;
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0) {
    err();
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(status));
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
      err();
      fprintf(stderr, "Unknown address family!\n");
    }

    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("\t%s: %s\n", ipver, ipstr);

    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1) {
      /* bind 
      if ((status = bind(sockfd, p->ai_addr, p->ai_addrlen)) == - 1){
        err();
        fprintf(stderr, "Not able to bind the socket descriptor!\n");
        return 3;
      }
      break;
      */

      if ((status = connect(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
        err();
        fprintf(stderr, "Couldn't connect to host: %s\n", argv[1]);
        return 4;
      }
      info("Connected");
      break;
    }
  }

  

  freeaddrinfo(res);
  return 0;
}


