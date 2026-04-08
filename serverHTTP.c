#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define MY_SERVER_PORT 9090  //The port number on which the server will listen for incoming connections. This is defined as a constant using the #define preprocessor directive
#define buffer_size 8192     //The size of the buffer used to store messages received from the client. This is also defined as a constant using the #define preprocessor directive.
//struct sockaddr_in address;   //The sockaddr_in structure is used to specify the address and port number for the server socket. It is defined in the netinet/in.h header file and contains fields for the address family, port number, and IP address. The address variable is declared as a global variable so that it can be accessed throughout the program.

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
    exit(EXIT_FAILURE);
}

//////////////////////////S////////////////////////
////      Step 2: Identify/Name a Socket      ////
//////////////////////////////////////////////////

    //assigning a transport address to the socket. This is called "binding" the socket to an address. 
    //The address is specified as a sockaddr structure, which includes the IP address and port number.
    //Like assigning a phone number to a phone line, so that it can be reached by others.

    //Binding the Socket:
struct sockaddr_in address;

/*  htonl converts a long integer (e.g. address) to network representation 
    htons converts a short integer (e.g. port) to a network representation
*/

memset((char *)&address, 0, sizeof(address)); //Initialize the structure to zero
address.sin_family = AF_INET; //Set the address family to AF_INET
address.sin_addr.s_addr = INADDR_ANY; //Set the IP address to INADDR_ANY, which means that the socket will bind to all available interfaces on the machine
address.sin_port = htons(MY_SERVER_PORT); //Set the port number to the specified MY_SERVER_PORT

if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0) //Bind the socket to the address and port specified in the sockaddr_in structure
{ 
    perror("bind failed");  
    return 0; 
}

//////////////////////////////////////////////////////////////////////
////   Step 3: Wait for an incoming connection on the server      ////
//////////////////////////////////////////////////////////////////////

//Code to listen and accept incoming connections:
if (listen(server_fd, 3) < 0) 
{ 
    printf("Socket created, bind completed.\n");
    printf("Listening...\n");
    perror("In listen"); 
    exit(EXIT_FAILURE); 
}
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
{
    printf("Client connected.\n");
    perror("In accept");            
    exit(EXIT_FAILURE);        
}


////////////////////////////////////////////////////
////     Step 4: Read and receive messages      ////
////////////////////////////////////////////////////
char buffer[8192] = {0}; //Buffer to store the message received from the client.
read(new_socket, buffer, 8192); //read the message from client and store in buffer
printf("%s\n", buffer); //Print the message received from the client to the console. Shows the browswer's request to the server


char response[8192];
response[0] = '\0';

strcat(response,
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Connection: close\r\n"
"\r\n"
"<html>"
"<head><title>BINGO</title></head>"
"<body>"
"<h1>Welcome to the Bingo Server!</h1>"
"<p>You have successfully connected to the server.</p>"
"<table border='1'><tr>"
);


for (int i = 1; i <= 25; i++) {
    char cell[100];
    sprintf(cell, "<td onclick='mark(this)'>%d</td>", i); //Create a table cell with an onclick event that calls the mark() function when clicked. The cell displays the number i.
    strcat(response, cell); //Append the cell to the response string, building the HTML content dynamically as the loop iterates through the numbers 1 to 25.
    if (i % 5 == 0) {
        strcat(response, "</tr><tr>"); //After every 5 cells, add a new table row to the response string. This creates a 5x5 grid of cells in the HTML table.
    }
}

strcat(response,
"</tr></table>"
"<script>"
"function mark(cell) { cell.style.backgroundColor = 'yellow'; }"
"setInterval(function() {"
" let cells = document.querySelectorAll('td');"
" let random = Math.floor(Math.random() * cells.length);"
" cells[random].style.background = 'red';"
"}, 2000);"
"</script>"
"</body></html>"
);

write (new_socket, response, strlen(response));

////////////////////////////////////////////////////////
////     Step 5: Close the connection and socket    ////
////////////////////////////////////////////////////////
close(new_socket); 
    //The close() system call is used to close the connected socket (new_socket) after we are done communicating with the client. This releases the resources associated with the socket and allows the server to accept new incoming connections. It is important to close the socket when it is no longer needed to free up system resources and avoid potential memory leaks.

}
