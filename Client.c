#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#define resolver_server_address "192.168.1.16"
int padding = RSA_PKCS1_PADDING;


int encrypt(int length, unsigned char *toencrypt, unsigned char *result, unsigned char *filename) {
    
    FILE * file = fopen(filename,"rb");
 
    if(file == NULL)
    {
        printf("Unable to open file %s \n",filename);
        return -1;    
    }
    RSA *rsa= RSA_new() ;
    rsa = PEM_read_RSA_PUBKEY(file, &rsa,NULL, NULL);
    int resultstat = RSA_public_encrypt(length, toencrypt, result, rsa, padding);
    return resultstat;
    
}


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
SSL_CTX* InitCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = SSLv23_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
int main() {
SSL_CTX *ctx;
SSL *ssl;
SSL_library_init();
ctx = InitCTX();
ssl = SSL_new(ctx);

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
 
SSL_set_fd(ssl, network_socket); 
SSL_get_cipher(ssl);
SSL_free(ssl);
close(network_socket);
SSL_CTX_free(ctx);
return 0;
}
