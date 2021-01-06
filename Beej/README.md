# Notes

##### Socket data types:
- socket descriptor: `int`
- addrinfo:
```c
struct addrinfo {
  int             ai_flags;           // AI_PASSIVE, AI_CANONNAME, etc.
  int             ai_family;          // AF_INET, AF_INET6, AF_UNSPEC
  int             ai_socktype;        // SOCK_STREAM, SOCK_DGRAM
  int             ai_protocol;        // use 0 for "any"
  size_t          ai_addrlen;         // size of ai_addr in bytes
  struct sockaddr *ai_addr;           // struct sockaddr_in or _in6
  char            *ai_canonname;      // full canonical hostname
  struct          addrinfo *ai_next;  // linked list, next node
};
```
- sockaddr:
```c
struct sockaddr {
  unsigned short  sa_family;    // address family, AF_XXX
  char            sa_data[14];  // 14 bytes of protocol address
};
```
- sockaddr\_in (for IPv4, pointer to `sockaddr` ca be cast to `sockaddr_in`):
```c
struct sockarrd_in {
  short int           sin_family;   // Address family, AF_INET
  unsigned short int  sin_port;     // Port number - in Network Byte Order (Big-endian)
  struct in_addr      sin_addr;     // Internet address
  unsigned char       sin_zero[8];  // Same size as struct sockaddr - use memset to set to zeros
};
```
- in\_addr:
```c
struct in_addr {
  uint32_t s_addr;
};
```
- sockaddr\_in6 (used for IPv6):
```c
// maybe necessary to typedef uint32_t u_int32_t
struct sockaddr_in6 {
  u_int16_t       sin6_family;    // address family, AF_INET6
  u_int16_t       sin6_port;      // port number, NBO
  u_int32_t       sin6_flowinfo;  // IPv6 flow information
  struct in6_addr sin6_addr;      // IPv6 address
  u_int32_t       sin6_scope_id;  // Scope ID
};
```
- in6\_addr:
```c
strucn in6_addr {
  unsigned char s6_addr[16]; // IPv6 address
};
```
- sockaddr\_storage (big enough to hold both IPv4 and IPv6 structures):
```c
struct sockaddr_storage {
  sa_family_t ss_family; // address family

  // padding (implementation specific)
  char        __ss_pad1[_SS_PAD1SIZE];
  int64_t     __ss_align;
  char        __ss_pad2[_SS_PAD2SIZE];
};
```
> TODO: IPv6 Flow information and Scope ID (out of the scope of this guide)

Saving and IP address to a struct with `inet_pton()`:

```c
struct sockaddr_in sin;
struct sockaddr_in6 sin6;
// inet_pton() returns -1 if there was an error, 0 if address is messed up and num greater than 0 otherwise
inet_pton(AF_INET, "192.168.5.10", &(sin.sin_addr)); 
inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sin6.sin6_addr));

// Obsolete as they don't support IPv6:
// inet_addr();
// inet_aton();
```

`inet_ntop()` does the inverse (network to presentation):
```c
// IPv4
char ip4[INET_ADDRSTRLEN];
struct sockaddr_in sa;

inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
printf("The IPv4 address: %s\n", ip4);

// IPv6
char ip6[INET6_ADDRSTRLEN];
struct sockaddr_in6 sa6;

inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);
printf("The IPv6 address: %s\n", ip6);

// Obsolete inet_ntoa();
```

## System calls

### `getaddrinfo`

Declaration of [`getaddrinfo`](getaddrinfo_ex.c)

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(
  const char* node, // e.g. "www.example.com" or IP
  const char* service, // e.g. "http" or port number
  const struct addrinfo* hints,
  struct addrinfo** res
);
```




