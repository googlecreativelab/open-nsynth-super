// Socket implementation adapted from: https://www.geeksforgeeks.org/socket-programming-cc/
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <mutex>
#include <thread>
#include <iostream>
#define PORT    4242
#define DATA_LEN = 4

class InstrumentServer {
    public:
    InstrumentServer() {
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        
        // Forcefully attaching socket to the port
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                    &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );
        
        // Forcefully attaching socket to the port
        if (bind(server_fd, (struct sockaddr *)&address, 
                                    sizeof(address))<0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        std::thread([this]() {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                (socklen_t*)&addrlen))<0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            while(true) {
                readPos += read( new_socket , dataInProgress, DATA_LEN-readPos);
                if(readPos == 4) {
                    readPos = 0;
                    std::cout << "COMMAND: " << dataInProgress[0] << ", " << dataInProgress[1] << ", " << dataInProgress[2] << ", " << dataInProgress[3] << std::endl;
                    m.lock();
                    currentData[0] = dataInProgress[0];
                    currentData[1] = dataInProgress[1];
                    currentData[2] = dataInProgress[2];
                    currentData[3] = dataInProgress[3];
                    m.unlock();
                }
            }
        }).detach();
    }

    private:
        int server_fd, new_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        int opt = 1;

        int readPos = 0;
        char currentData[DATA_LEN];
        char dataInProgress[DATA_LEN];
        std::mutex m;
};