 #include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define transtype SOCK_STREAM
#define server_address 
#define backlog
#define Alabama
#define Alaska
#define Arizona
#define Arkansas
#define California
#define Colorado
#define Connecticut
#define Deleware
#define Florida
#define Georgia
#define Hawaii
#define Idaho
#define Illinois
#define Indiana
#define Iowa
#define Kansas
#define Kentucky
#define Louisiana
#define Maine
#define Maryland
#define Massachusetts
#define Michigan
#define Minnesota
#define Mississippi
#define Missouri
#define Montana
#define Nebraska
#define Nevada
#define New_Hampshire
#define New_Jersey
#define New_Mexico
#define New_York
#define North_Carolina
#define North_Dakota
#define Ohio
#define Oklahoma
#define Oregon
#define Pennsylvania
#define Rhode_Island
#define South_Carolina
#define South_Dakota
#define Tennessee
#define Texas
#define Utah
#define Vermont
#define Virginia
#define Washington
#define West_Virginia
#define Wisconsin
#define Wyoming

int main() {
  //socket creation
    int socket = socket(AF_INET, transtype, 0);
    //define connection address
    struct sockaddr_in address;
    addr.sin_family = AF_INET
    addr.sin_port = htons(6969);
    addr.sin_addr.s_addr = htonl(server_address);
    
    //bind the socket
    bind(socket, (struct sockaddr*) &address, sizeof(address));
    
    //accept client
    listen(socket, backlog);
    
    
    int client_socket;
    int client_reply;
    char *redirect_ip;
    //client handler
    while(1) {
    client_socket = accept(socket, NULL, NULL);
    recv(client_socket, &client_reply, sizeof(client_reply), 0);
    switch(client_reply) {
     case(1) {
      redirect_ip = Alabama;   
     }
     case(2) {
      redirect_ip = Alaska;
     }
     case(3) {
      redirect_ip = Arizona;
     }
     case(4) {
    redirect_ip =  Arkansas;
     }
     case(5) {
     redirect_ip = California;
    }
    case(6) {
     redirect_ip =  Colorado;   
    }
    case(7) {
     redirect_ip =  Connecticut;   
    }
    case(8) {
     redirect_ip = Delaware;
    }
    case(9) {
    redirect_ip = Florida;
    }
    case(10) {
     redirect_ip = Georgia;   
    }
    case(11) {
     redirect_ip = Hawaii;   
    }
    case(12) {
     redirect_ip = Idaho;   
    }
    case(13) {
     redirect_ip = Illinois;
    }
    case(14) {
     redirect_ip = Indiana;
    }
    case(15) {
     redirect_ip = Iowa;
    }
    case(16) {
     redirect_ip = Kansas;   
    }
    case(17) {
    redirect_ip = Kentucky;
    }
    case(18) {
    redirect_ip = Lousiana;
    }
    case(19) {
    redirect_ip = Maine;
    }
    case(20) {
    redirect_ip = Maryland;
    }
    case(21) {
     redirect_ip = Massachusetts;   
    }
    case(22) {
     redirect_ip = Michigan;
    }
    case(23) {
     redirect_ip = Minnesota;   
    }
    case(24) {
     redirect_ip = Mississippi;   
    }
    case(25) {
     redirect_ip = Missouri;   
    }
    case(26) {
     redirect_ip = Montana;   
    }
    case(27) {
     redirect_ip = Nebraska;   
    }
    case(28) {
     redirect_ip = Nevada;   
    }
    case(29) {
     redirect_ip = New_Hampshire; 
    }
    case(30) {
     redirect_ip = New_Jersey;
    }
    case(31) {
     redirect_ip = New_Mexico;
    }
    case(32) {
     redirect_ip = New_York;   
    }
    case(33) {
     redirect_ip = North_Carolina;   
    }
    case(34) {
     redirect_ip = North_Dakota;
    }
    case(35) {
     redirect_ip = Ohio;   
    }
    case(36) {
     redirect_ip = Oklahoma;   
    }
    case(37) {
     redirect_ip = Oregon;   
    }
    case(38) {
     redirect_ip = Pennsylvania;   
    }
    case(39) {
     redirect_ip = Rhode_Island;   
    }
    case(40) {
     redirect_ip = South_Carolina;   
    }
    case(41) {
     redirect_ip = South_Dakota;
    }
    case(42) {
     redirect_ip = Tennessee;   
    }
    case(43) {
     redirect_ip = Texas;   
    }
    case(44) {
     redirect_ip = Utah;   
    }
    case(45) {
     redirect_ip = Vermont;   
    }
    case(46) {
     redirect_ip = Virginia;   
    }
    case(47) {
     redirect_ip = Washington;
    }
    case(48) {
     redirect_ip = West_Virginia;   
    }
    case(49) {
     redirect_ip = Wisconsin;   
    }
    case(50) {
     redirect_ip = Wyoming;   
    }
    
    }
    send(client_socket, &redirect_ip, sizeof(redirect_ip), 0);
    close(client_socket);
    
    }
    return 0;
}
