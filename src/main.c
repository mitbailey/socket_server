/**
 * @file main.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021.07.23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// Inspired by: https://www.binarytides.com/server-client-example-c-sockets-linux/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "server.h"

int main(int argc, char *argv[])
{
    int listening_socket, accepted_socket, socket_size;
    int read_size = 0;
    struct sockaddr_in server_address, client_address;
    char client_message[MAX_SIZE_CLIENT_MESSAGE];
    memset(client_message, 0x0, MAX_SIZE_CLIENT_MESSAGE);

    // Create socket.
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1)
    {
        printf("Could not create socket.\n");
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure.
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Set the IP address.
    if (inet_pton(AF_INET, SERVER_IP_ADDRESS, &server_address.sin_addr) <= 0)
    {
        printf("Invalid address; Address not supported.\n");
    }

    // Set the timeout for recv, which will allow us to reconnect to poorly disconnected clients.
    struct timeval timeout;
    timeout.tv_sec = RECV_TIMEOUT;
    timeout.tv_usec = 0;
    setsockopt(listening_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    // Bind.
    if (bind(listening_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("Error: Port binding failed (%s:%d).\n", SERVER_IP_ADDRESS, SERVER_PORT);
        perror("bind");
        return -1;
    }
    printf("Bound to port (%s:%d).\n", SERVER_IP_ADDRESS, SERVER_PORT);
    
    while (1)
    {
        // Listen.
        listen(listening_socket, 3);

        // Accept an incoming connection.
        printf("Waiting for incoming connections...\n");
        socket_size = sizeof(struct sockaddr_in);

        // Accept connection from an incoming client.
        accepted_socket = accept(listening_socket, (struct sockaddr *)&client_address, (socklen_t *)&socket_size);
        if (accepted_socket < 0)
        {
            if (errno == EAGAIN)
            {
                // printf("Waiting for connection timed-out.\n");
                continue;
            }
            else
            {
                perror("accept failed");
                continue;
            }
        }
        printf("Connection accepted.\n");

        // We are now connected.

        while (read_size >= 0)
        {
            read_size = recv(accepted_socket, client_message, 2000, 0);
            if (read_size > 0)
            {
                printf("PRINTING CLIENT MESSAGE: ");
                for (int i = 0; i < read_size; i++)
                {
                    printf("%c", client_message[i]);
                }
                printf("\n");
            }
            else
            {
                break;
            }
        }
        if (read_size == 0)
        {
            printf("Client closed connection.\n");
            continue;
        }
        else if (errno == EAGAIN)
        {
            printf("Active connection timed-out.\n");
            continue;
        }
    }

    return 0;
}