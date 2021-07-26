/**
 * @file server.h
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021.07.23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SERVER_H
#define SERVER_H

// Comment this out to use the default IP address.
#define AUTODETECT_IPV4

#define MAX_SIZE_CLIENT_MESSAGE 0x800
#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 51924
#define RECV_TIMEOUT 10 // seconds

/**
 * @brief Finds the localhost's IPv4 IP address.
 * 
 * If it fails, the passed ip_buffer is left unchanged,
 * 
 * @param buffer Memory to store the IPv4 as a string.
 * @param buffer_size The size of the buffer.
 * @return int 1 on success, 0 on failure.
 */
int find_ipv4(char *buffer, ssize_t buffer_size);

#endif // SERVER_H