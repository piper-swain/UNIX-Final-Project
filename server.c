#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define MY_SERVER_PORT 8080  //The port number on which the server will listen for incoming connections. This is defined as a constant using the #define preprocessor directive
#define buffer_size 1024     //The size of the buffer used to store messages received from the client. This is also defined as a constant using the #define preprocessor directive.
struct sockaddr_in address;   //The sockaddr_in structure is used to specify the address and port number for the server socket. It is defined in the netinet/in.h header file and contains fields for the address family, port number, and IP address. The address variable is declared as a global variable so that it can be accessed throughout the program.
int addrlen = sizeof(address); //The addrlen variable is used to store the size of the address structure. It is initialized to the size of the address. This variable is passed as a parameter to the accept() system call to specify the size of the address structure that will be filled in with the client's address information when a connection is accepted.

int main() {

int server_fd, new_socket;
int addrlen = sizeof(struct sockaddr_in);
//////////////////////////////////////////////////
////         Step 1: Create a socket          ////
//////////////////////////////////////////////////

    //Domain: communication domain in which the socket should be created.
    //Type: Type of service, selected accoording to properties required by the application
    //Protocol: Protocol to be used with the socket. If 0, the system will choose the most appropriate protocol based on the specified domain and type.

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
    perror("cannot create socket");
    return 0;
}

//////////////////////////////////////////////////
////      Step 2: Identify/Name a Socket      ////
//////////////////////////////////////////////////

    //assigning a transport address to the socket. This is called "binding" the socket to an address. 
    //The address is specified as a sockaddr structure, which includes the IP address and port number.
    //Like assigning a phone number to a phone line, so that it can be reached by others.

int bind(int socket, const struct sockaddr *address, socklen_t address_len); 
    //socket: socket that was created previously
    //sockadddr: generic container that allows the OS to read the first couple of bytes that idenntify the address family
{
struct sockaddr_in {
    __uint8_t       sin_len;        // length of this struct (16)
    sa_family_t     sin_family;     // the address family we used to set up the socket, which isAF_INET
    in_port_t       sin_port;       // port number
    struct in_addr   sin_addr;      // internet address for the socket, AKA my machines IP address
    char            sin_zero[8];    // this line is not used, but is included to make the structure the same size as struct sockaddr
};

    //Binding the Socket:
struct sockaddr_in address;

/*  htonl converts a long integer (e.g. address) to network representation 
    htons converts a short integer (e.g. port) to a network representation
*/

memset((char *)&address, 0, sizeof(address)); //Initialize the structure to zero
address.sin_family = AF_INET; //Set the address family to AF_INET
address.sin_addr.s_addr = htonl(INADDR_ANY); //Set the IP address to INADDR_ANY, which means that the socket will bind to all available interfaces on the machine
address.sin_port = htons(MY_SERVER_PORT); //Set the port number to the specified PORT

if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0) //Bind the socket to the address and port specified in the sockaddr_in structure
{ 
    perror("bind failed");  
    return 0; 
}
}
//////////////////////////////////////////////////////////////////////
////   Step 3: Wait for an incoming connection on the server      ////
//////////////////////////////////////////////////////////////////////

//The server needs to wait for incoming connection requests from clients. This is done using the listen() system call, which puts the server socket into a passive mode where it can accept incoming connection requests.
int listen(int socket, int backlog);
    //socket: the file descriptor of the server socket that was created and bound in the previous steps.
    //backlog: the maximum number of pending connections that can be queued up before the server starts rejecting new connection requests. This parameter is used to control how many clients can be waiting to connect to the server at any given time.

//accept: grabs the first connection request on the queue of pending connections for the listening socket, creates a new connected socket, and returns a new file descriptor referring to that socket. The newly created socket is not in the listening state. The original socket specified in the listen() call remains open and continues to listen for incoming connection requests.
int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
    //socket: the socket that was set for accepting connections with listen()
    //address: gets filed in with the address of the client that is connecting
    //address_len: The length of the address structure, which is passed as a pointer to the accept() system call. The accept() function will fill in this value with the actual length of the client's address.

//Code to listen and accept incoming connections:
if (listen(server_fd, 3) < 0) 
{ 
    perror("In listen"); 
    exit(EXIT_FAILURE); 
}
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
{
    perror("In accept");            
    exit(EXIT_FAILURE);        
}


////////////////////////////////////////////////////
////     Step 4: Read and receive messages      ////
////////////////////////////////////////////////////
char buffer[1024] = {0};                             //Buffer to store the message received from the client. The size of the buffer is 1024 bytes and is initialized to zero using memset() to ensure that it does not contain any garbage values before receiving data from the client.
int valread = read( new_socket , buffer, 1024);      //The read() system call is used to read data from the connected socket. It takes three parameters: the file descriptor of the connected socket (new_socket), a buffer to store the received data (buffer), and the maximum number of bytes to read (1024). The return value of read() is the number of bytes actually read, which is stored in the variable valread. If valread is negative, it indicates an error occurred while reading from the socket.
printf("%s\n",buffer );                              //Print the message received from the client to the console. The buffer is treated as a null-terminated string, and the %s format specifier is used to print it. 
if(valread < 0)
{ 
    printf("Error in communication: no bytes are there to read. \nTry connecting again.");      //If valread is negative, it indicates that an error occurred while reading from the socket. In this case, we print a message to the console indicating that there are no bytes to read. This could happen if the client has closed the connection or if there was an error in the communication.
}
char *hello = "Hello, and welcome to the server!";  //IMPORTANT! 
write(new_socket , hello , strlen(hello));          //The write() system call is used to send a response back to the client. It takes three parameters: the file descriptor of the connected socket (new_socket), a buffer containing the data to be sent (hello), and the number of bytes to write (strlen(hello)). The write() function returns the number of bytes actually written, which can be used to check for errors. In this case, we are sending a simple string "Hello, and welcome to the server!" back to the client as a response.


////////////////////////////////////////////////////////
////     Step 5: Close the connection and socket    ////
////////////////////////////////////////////////////////
close(new_socket); 
    //The close() system call is used to close the connected socket (new_socket) after we are done communicating with the client. This releases the resources associated with the socket and allows the server to accept new incoming connections. It is important to close the socket when it is no longer needed to free up system resources and avoid potential memory leaks.

}
