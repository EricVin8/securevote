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
#define CHK_NULL(x) if ((x)==NULL) exit (1)


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


 char *resolve(int state, char *newip) {
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
    X509* server_cert;
    char *str;
char ip[4098] = {};
resolve(1, ip);
int network_socket;
network_socket = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in ordinary_address;
ordinary_address.sin_family = AF_INET;
ordinary_address.sin_port = htons(6969);
ordinary_address.sin_addr.s_addr = inet_addr(ip);


char *voterpass = "voterpass.pem";

char *voterfname = "Tyler", *voterlname = "Chow", *ssnumber = "696969", *id = "42069", *canidate = "shrek";
unsigned char evoterfname[4098] = {}, evoterlname[4098] = {}, essnumber[4098] = {}, eid[4098] = {}, ecanidate[4098] = {}, id2[4098] = "10101010";
int connection_status = connect(network_socket , (struct sockaddr *) &ordinary_address, sizeof(ordinary_address));

if (connection_status == -1) {
printf("error connecting to remote socket");
//27return -1;
 }
SSL_CTX *ctx;
SSL *ssl;
ctx = InitCTX();
ssl = SSL_new(ctx);
SSL_set_fd(ssl, network_socket); 
SSL_connect(ssl);
printf("Got here");
fflush(stdout);
int voterfnamelen, voterlnamelen, ssnumberlen, idlen, canidatelen;
voterfnamelen = encrypt(strlen(voterfname),voterfname, evoterfname ,voterpass);

voterlnamelen = encrypt(strlen(voterlname), voterlname, evoterlname, voterpass);
ssnumberlen = encrypt(strlen(ssnumber), ssnumber, essnumber, voterpass);
idlen = encrypt(strlen(id), id, eid, voterpass);
canidatelen = encrypt(strlen(canidate), canidate, ecanidate, voterpass);


  printf ("SSL connection using %s\n", SSL_get_cipher (ssl));
  
  /* Get server's certificate (note: beware of dynamic allocation) - opt */

  server_cert = SSL_get_peer_certificate (ssl);       CHK_NULL(server_cert);
  printf ("Server certificate:\n");
  str = X509_NAME_oneline (X509_get_subject_name (server_cert),0,0);
  CHK_NULL(str);
  printf ("\t subject: %s\n", str);
  OPENSSL_free (str);

  str = X509_NAME_oneline (X509_get_issuer_name  (server_cert),0,0);
  CHK_NULL(str);
  printf ("\t issuer: %s\n", str);
  OPENSSL_free (str);

  X509_free (server_cert);

SSL_write(ssl, evoterfname, sizeof(evoterfname));
SSL_write(ssl, &voterfnamelen, sizeof(voterfnamelen));
SSL_write(ssl, evoterlname, sizeof(evoterlname));
SSL_write(ssl, &voterlnamelen, sizeof(voterlname));
SSL_write(ssl, essnumber, sizeof(essnumber));
SSL_write(ssl, &ssnumberlen, sizeof(ssnumberlen));
SSL_write(ssl, eid, sizeof(eid));
SSL_write(ssl, &idlen, sizeof(idlen));
SSL_write(ssl, ecanidate, sizeof(ecanidate));
SSL_write(ssl, &canidatelen, sizeof(canidatelen));
SSL_write(ssl, id2, strlen(id2));



close(network_socket);
SSL_CTX_free(ctx);
printf("finished");

return 0;
}
