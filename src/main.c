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
    int server_socket, client_socket, socket_size;
    struct sockaddr_in server_address, client_address;
    char client_message[MAX_SIZE_CLIENT_MESSAGE];
    memset(client_message, 0x0, MAX_SIZE_CLIENT_MESSAGE);

    // Create socket.
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
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
    setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    // Bind.
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("Error: Port binding failed.\n");
        perror("bind");
        return 1;
    }
    printf("Bound to port.\n");

    while (1)
    {
        // Listen.
        listen(server_socket, 3);

        // Accept an incoming connection.
        printf("Waiting for incoming connections...\n");
        socket_size = sizeof(struct sockaddr_in);

        // Accept connection from an incoming client.
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&socket_size);
        if (client_socket < 0)
        {
            if (errno == EAGAIN)
            {
                printf("Waiting for connection timed-out.\n");
                continue;
            }
            else
            {
                perror("accept failed");
                return -1;
            }
        }
        printf("Connection accepted.\n");

        // We are now connected.

        int read_size = 0;
        while (read_size >= 0)
        {
            read_size = recv(client_socket, client_message, 2000, 0);
            if (read_size > 0)
            {
                printf("PRINTING CLIENT MESSAGE: ");
                for (int i = 0; i < read_size; i++)
                {
                    printf("%c", client_message[i]);
                }
                printf("\n");
            }
            printf("Read Size: %d\n", read_size);
        }

        printf("Timeout, closing connection.\n");
    }

    return 0;
}