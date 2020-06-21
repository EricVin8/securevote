#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#define resolver_server_address "192.168.1.4"
int padding = RSA_PKCS1_PADDING;


void displayerror(char *error) {
//todo have gui show error msg



}
int encrypt(int length, unsigned char *toencrypt, unsigned char *result, unsigned char *filename) {
    
    FILE * file = fopen(filename,"rb");
 
    if(file == NULL)
    {
        displayerror("There was an error opening the voter pass");
        return -1;    
    }
    RSA *rsa= RSA_new();
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
    printf("failed to connect");
    }
    else {
    write(nsocket, &state, sizeof(state));
    read(nsocket, newip, 1024);
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
const char *ip = resolve(1);
int network_socket;
network_socket = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in ordinary_address;
ordinary_address.sin_family = AF_INET;
ordinary_address.sin_port = htons(6969);
ordinary_address.sin_addr.s_addr = inet_addr(ip);
printf(ip);
fflush(stdout);

char *voterpass = "voterpass.pem";

char *voterfname = "Tyler", *voterlname = "Chow", *ssnumber = "696969", *id = "42069", *canidate = "shrek", *id2 = "10101010";
unsigned char evoterfname[4098] = {}, evoterlname[4098] = {}, essnumber[4098] = {}, eid[4098] = {}, ecanidate[4098] = {};
int connection_status = connect(network_socket , (struct sockaddr *) &ordinary_address, sizeof(ordinary_address));

if (connection_status == -1) {
printf("error connecting to remote socket");
//27return -1;
 }
SSL_set_fd(ssl, network_socket); 
SSL_get_cipher(ssl);
int voterfnamelen, voterlnamelen, ssnumberlen, idlen, canidatelen;
voterfnamelen = encrypt(strlen(voterfname),voterfname, evoterfname ,voterpass);

voterlnamelen = encrypt(strlen(voterlname), voterlname, evoterlname, voterpass);
ssnumberlen = encrypt(strlen(ssnumber), ssnumber, essnumber, voterpass);
idlen = encrypt(strlen(id), id, eid, voterpass);
canidatelen = encrypt(strlen(canidate), canidate, ecanidate, voterpass);

SSL_write(ssl, evoterfname, strlen(evoterfname) + 1);
SSL_write(ssl, &voterfnamelen, sizeof(voterfnamelen));
SSL_write(ssl, evoterlname, strlen(evoterlname) + 1);
SSL_write(ssl, &voterlnamelen, sizeof(voterlname));
SSL_write(ssl, essnumber, strlen(essnumber) + 1);
SSL_write(ssl, &ssnumberlen, sizeof(ssnumberlen));
SSL_write(ssl, eid, strlen(eid) + 1);
SSL_write(ssl, &idlen, sizeof(idlen));
SSL_write(ssl, ecanidate, strlen(ecanidate) + 1);
SSL_write(ssl, &canidatelen, sizeof(canidatelen));
printf("%d", canidatelen);
SSL_write(ssl, id2, strlen(id2) + 1);



close(network_socket);
SSL_CTX_free(ctx);
printf("finished");

return 0;
}
