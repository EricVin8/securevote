#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#define resolver_server_address "192.168.1.16"
char *resolve(int state) {
    char *newip;
    int nsocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(6969);
    address.sin_addr.s_addr = inet_addr(resolver_server_address);
    if(connect(nsocket, (struct sockaddr *) &address, sizeof(address)) == -1) {
    printf("Failed to connect!");
    }
    else {
    send(nsocket, &state, sizeof(state), 0);
    recv(nsocket, &newip, sizeof(newip), 0);
    }
    close(nsocket);
    return newip;
    
}
int main() {
char *ip = resolve(1);
int network_socket;
network_socket = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in ordinary_address;
ordinary_address.sin_family = AF_INET;
ordinary_address.sin_port = htons(6969);
ordinary_address.sin_addr.s_addr = inet_addr(ip);

int connection_status = connect(network_socket , (struct sockaddr *) &ordinary_address, sizeof(ordinary_address));

if (connection_status == -1) {
printf("Shrek whispers in your ear, There was an error making a connection to the remote socket");
return -1;
 }

close(network_socket);

return 0;
}
