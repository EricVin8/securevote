#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define server_address

int main() {

int network_socket;
network_socket = socket(AF_INET , SOCK_STREAM, 0 );

struct sockaddr_in ordinary_address;
ordinary_address.sin_family = AF_INET;
ordinary_address.sin_port = htons(6969);
ordinary_address.sin_addr.s_addr = htonl(server_address);

int connection_status = connect(network_socket , (struct sockaddr *) &ordinary_address, sizeof(ordinary_address));

if (connection_status == -1) {
printf("Shrek whispers in your ear, There was an error making a connection to the remote socket");
 }

close(network_socket);

return 0;
}