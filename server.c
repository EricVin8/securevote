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
#include <sqlite3.h>
#include <string.h>


#define server_address "192.168.1.16"
#define tcp_backlog 5

int padding = RSA_PKCS1_PADDING;


int decrypt(int encoded_data_length, unsigned char *encoded, unsigned char *id2, char *result) {
    
    FILE * key = fopen(strncat(id2, ".pem", 11),"rb");
    if(key == NULL)
    {
        printf("%s cannot be found!\n", id2);
        return -1;    
    }
    RSA *rsa= RSA_new();
    rsa = PEM_read_RSAPrivateKey(key, &rsa, NULL, NULL);
    int resultstatus = RSA_private_decrypt(encoded_data_length, encoded, result, rsa, padding);
    //todo split into arguments at split


    return resultstatus;

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

    sqlite3 *database;
    int status = sqlite3_open("voters.db", &database);
    sqlite3_stmt *result;
    char *sqlcommand = "SELECT Firstname, Lastname, SSnumber, idnumber, canidate FROM Voters where idnumber = @id;";
    status = sqlite3_prepare_v2(database, sqlcommand, -1, &result, 0);
    
    

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
        int encoded_data_length;
        char *voterfname, *voterlname, *ssnumber, *idnumber, *canidate, *idnumber2;
        char *voterfnameencrypted, *voterlnameencrypted, *ssnumberencrypted, *idnumberencrypted, *canidateencrypted;
        int lenvoterfname, lenvoterlname, lenssnumber, lenidnumber, lencanidate;
        int value;
        int csocket = accept(ssocket, (struct sockaddr*)&client, &length);
        SSL *ssl = SSL_new(context);
        SSL_read(ssl, voterfnameencrypted, sizeof(voterfnameencrypted));
        SSL_read(ssl, &lenvoterfname, sizeof(lenvoterfname));
        SSL_read(ssl, voterlnameencrypted, sizeof(voterlnameencrypted));
        SSL_read(ssl, &lenvoterlname, sizeof(lenvoterlname));
        SSL_read(ssl, ssnumberencrypted, sizeof(ssnumberencrypted));
        SSL_read(ssl, &lenssnumber, sizeof(lenssnumber));
        SSL_read(ssl, idnumberencrypted, sizeof(idnumberencrypted));
        SSL_read(ssl, &lenidnumber, sizeof(lenidnumber));
        SSL_read(ssl, canidateencrypted, sizeof(canidateencrypted));
        SSL_read(ssl, &lencanidate, sizeof(lencanidate));
        SSL_read(ssl, idnumber2, sizeof(idnumber2));

        decrypt(lenvoterfname, voterfnameencrypted, idnumber2, voterfname);
        decrypt(lenvoterlname, voterlnameencrypted, idnumber2, voterlname);
        decrypt(lenssnumber, ssnumberencrypted, idnumber2, ssnumber);
        decrypt(lenidnumber, idnumberencrypted, idnumber2, idnumber);
        decrypt(lencanidate, canidateencrypted, idnumber2, canidate);
        int index = sqlite3_bind_parameter_index(result, "@id");
        sqlite3_bind_int(result, index, value);
        if(strncmp(voterfname, sqlite3_column_text(result, 0), strlen(sqlite3_column_text(result, 0))) && strncmp(voterlname, sqlite3_column_text(result, 1), strlen(sqlite3_column_text(result, 1))) && strncmp(ssnumber, sqlite3_column_text(result, 2), strlen(sqlite3_column_text(result, 2))) && strncmp(idnumber, sqlite3_column_text(result, 3), strlen(sqlite3_column_text(result, 3))) && sqlite3_column_text(result, 4) == NULL) {
         char *safecanidate = sqlite3_mprintf("UPDATE Voters SET canidate = '%q' WHERE idnumber = %q;", canidate, idnumber);
         sqlite3_exec(database, safecanidate, 0, 0, 0);
         sqlite3_free(safecanidate);
        }
        else{
            printf("voter not found");
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(csocket);
         //todo: write code to return failure, and also increment ip suspicion value   
        }
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(csocket);
    }
    close(ssocket);
    SSL_CTX_free(context);




    }
