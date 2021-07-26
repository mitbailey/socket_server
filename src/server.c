/**
 * @file server.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021.07.23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include "server.h"

int find_ipv4(char *ip_buffer, ssize_t buffer_size)
{
    struct ifaddrs *addr, *temp_addr;
    char buffer[32];
    getifaddrs(&addr);
    for (temp_addr = addr; temp_addr != NULL; temp_addr = temp_addr->ifa_next)
    {
        struct sockaddr_in *addr_in = (struct sockaddr_in *) temp_addr->ifa_addr;
        inet_ntop(AF_INET, &addr_in->sin_addr, buffer, buffer_size);

        // If the IP address is the IPv4...
        if (buffer[0] == '1' && buffer[1] == '7' && buffer[2] == '2' && buffer[3] == '.')
        {
            printf("Detected IPv4: %s\n", buffer);
            memcpy(ip_buffer, buffer, sizeof(buffer));
            return 1;
        }
    }

    return 0;
}