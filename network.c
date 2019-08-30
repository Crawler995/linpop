#include "network.h"

#include <gtk/gtk.h>
#include <stdio.h>
#include <ifaddrs.h>
#include <sys/ioctl.h> 
#include <net/if.h> 
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

char* get_self_ip_address() {
    char hname[128];
    struct hostent *hent;
    int i;

    gethostname(hname, sizeof(hname));
    hent = gethostbyname(hname);

    struct ifaddrs *ifap0 = NULL, *ifap = NULL;
    void *tmpAddrPtr = NULL;

    getifaddrs(&ifap0);
    ifap = ifap0;

    char *self_ip_address = (char*)malloc(sizeof(char) * INET_ADDRSTRLEN);

    while (ifap != NULL) {
        if (ifap->ifa_addr->sa_family == AF_INET) {
            tmpAddrPtr = &((struct sockaddr_in *)ifap->ifa_addr)->sin_addr;
            
            inet_ntop(AF_INET, tmpAddrPtr, self_ip_address, INET_ADDRSTRLEN);

            if(strcmp(self_ip_address, "127.0.0.1") != 0) {
                printf("IPv4: %s\n", self_ip_address);
                break;
            }
        }
        ifap = ifap->ifa_next;
    }

    if (ifap0) { freeifaddrs(ifap0); ifap0 = NULL; }

    return self_ip_address;
}