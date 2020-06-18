#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define server_address "192.168.1.16"
#define tcp_backlog 5

int padding = RSA_PKCS1_PADDING;


char *decrypt(int encoded_data_length, unsigned char *encoded, unsigned char *filename, unsigned char *result) {
    FILE * key = fopen(filename,"rb");
    if(key == NULL)
    {
        printf("%s cannot be found!\n",filename);
        return NULL;    
    }
    RSA *rsa= RSA_new() ;
    rsa = PEM_read_RSAPrivateKey(key, &rsa,NULL, NULL);
    int result = RSA_private_decrypt(encoded_data_length, encoded, result, rsa, padding);
    return result;

}
SSL_CTX *init_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
	perror("Unable to create SSL context");
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    return ctx;
}

int main() {

    
    SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();

    SSL_CTX *context = init_context();

    SSL_CTX_set_ecdh_auto(context, 1);

    if (SSL_CTX_use_certificate_file(context, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
        printf("Error importing cert.pem! Check to make sure file exists!\n\n");
	return -1;
    }

    if (SSL_CTX_use_PrivateKey_file(context, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
        printf("Error importing key.pem! Check to make sure file exists!\n\n");
	return -1;
    }
    int ssocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(6969);
    address.sin_addr.s_addr = inet_addr(server_address);
    //bind the socket
    bind(ssocket, (struct sockaddr*)&address, sizeof(address));

    //listen for clients
    listen(ssocket, tcp_backlog);
    while(1) {
        struct sockaddr_in client;
        int length = sizeof(client);

        int csocket = accept(ssocket, (struct sockaddr*)&client, &length);
        SSL *ssl = SSL_new(context);
        SSL_set_fd(ssl, csocket);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(csocket);
    }
    close(ssocket);
    SSL_CTX_free(context);




    }