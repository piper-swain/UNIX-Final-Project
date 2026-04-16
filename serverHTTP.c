#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define MY_SERVER_PORT 9090  //The port number on which the server will listen for incoming connections. This is defined as a constant using the #define preprocessor directive
#define buffer_size 65536     //The size of the buffer used to store messages received from the client. This is also defined as a constant using the #define preprocessor directive.
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
char buffer[65536] = {0}; //Buffer to store the message received from the client.
read(new_socket, buffer, 65536); //read the message from client and store in buffer
printf("%s\n", buffer); //Print the message received from the client to the console. Shows the browswer's request to the server


char response[65536];
response[0] = '\0';


strcat(response,
"HTTP/1.1 200 OK\r\n"           //HTTP status line indicating a successful response with status code 200 
"Content-Type: text/html\r\n"   //HTTP header specifying the content type of the response as HTML
"Connection: close\r\n"         //HTTP header indicating that the server will close the connection after sending the response
"\r\n"                          //Blank line to separate the headers from the body of the response

"<!DOCTYPE html>"              //HTML5 doctype declaration indicating that the document is an HTML5 document
"<html>"                       //Opening HTML tag to start the HTML document
"<head><title><Let's play BINGO!></title></head>"   //Head section of the HTML document containing the title of the page, which is displayed in the browser's tab

"<body>"                                                //Opening body tag to start the body of the HTML document, which contains the content that will be displayed on the webpage
"<h1 style='text-align:center;'>Let's play BINGO!</h1>" //displays large title at the top of the screen. 

"<div style='text-align:center; font-size:40px;' id='call'>Draw Next Number</div>"  //Div element with an id of "call" that is used to display the current letter and number drawn in the Bingo game. Initially, it displays "Click Draw" as a prompt for the user to start the game.
"<div style='text-align:center; margin:10px;'>"            //div element that contains the buttons for drawing a number and restarting the game. It is styled to center the content and add some margin around it.
"<button onclick='draw()'>Draw Number</button>"            //Button element that triggers the draw() JavaScript function when clicked. This function is responsible for randomly selecting a letter and number combination for the Bingo game and updating the content of the "call" div to display the drawn number.
"<button onclick='location.reload()'>Restart</button>"     //Button element that triggers a page reload when clicked, effectively restarting the game by resetting all elements to their initial state.
"</div>"                                                   //Closing div tag to end the section containing the buttons

"<div style='display:flex; justify-content:space-around;'>"  //Div element that serves as a container for the player and robot boards. It is styled to use flexbox layout and space the child elements (the boards) evenly across the available width.

//Client/User board
"<div>"                                 //Div element that contains the player's Bingo board. It is styled to be a block-level element, which allows it to take up the full width of its container and stack vertically with other elements.
"<h2>Your Bingo Card</h2>"                          //Heading element that displays the title "You" above the player's Bingo board, indicating that this board belongs to the user.
"<table id='player' border='1' style='font-size:30px;'>"       //Table element that represents the player's Bingo board. It has an id of "player" for reference in JavaScript, a border attribute to create visible borders around the cells, and inline styling to set the font size of the cell content to 30 pixels.
"<tr><th>B</th><th>I</th><th>N</th><th>G</th><th>O</th></tr>" //Header row of the player's Bingo board, containing the letters B, I, N, G, and O as column headers. This row is static and does not change during the game, serving as a reference for the columns of the Bingo board.
);

// Player grid- Randomly chooses a numer 1-99 to fill the player's Bingo board
for (int i = 1; i <= 25; i++) {
    char cell[100];
    int num = rand() % 75 + 1; // Generate a random number between 1 and 75
    sprintf(cell, "<td onclick='mark(this)'>%d</td>", num); // Create a table cell with the random number and an onclick event to mark the cell when clicked
    strcat(response, cell); // Append the cell HTML to the response string

    if (i % 5 == 0 && i != 25) // After every 5 cells, close the current row and start a new one, except after the last cell
        strcat(response, "</tr><tr>");
}

strcat(response,
    "</tr></table>"   // Closing table tag to end the player's Bingo board
    "</div>"          // Closing div tag to end the section containing the player's board

    // ROBOT BOARD
    "<div>"  //Div element that contains the robot's Bingo board. It is styled to be a block-level element, similar to the player's board, allowing it to stack vertically with other elements.
    "<h2>Robot's Bingo Card</h2>"                                         //Heading element that displays the title "Robot" above the robot's Bingo board, indicating that this board belongs to the computer opponent.
    "<table id='robot' border='1' style='font-size:30px;'>" //Table element that represents the robot's Bingo board. It has an id of "robot" for reference in JavaScript, a border attribute to create visible borders around the cells, and inline styling to set the font size of the cell content to 30 pixels.
    "<tr><th>B</th><th>I</th><th>N</th><th>G</th><th>O</th></tr>"  //Header row of the robot's Bingo board, containing the letters B, I, N, G, and O as column headers. This row is static and does not change during the game, serving as a reference for the columns of the Bingo board.
    );

// Robot grid
for (int i = 1; i <= 25; i++) {
    char cell[100];
    int num = rand() % 75 + 1; // Generate a random number between 1 and 75
    sprintf(cell, "<td onclick='mark(this)'>%d</td>", num); // Create a table cell with the random number and an onclick event to mark the cell when clicked
    strcat(response, cell); // Append the cell HTML to the response string

    if (i % 5 == 0 && i != 25) // After every 5 cells, close the current row and start a new one, except after the last cell
        strcat(response, "</tr><tr>");
}

strcat(response,
    "</tr></table>"   // Closing table tag to end the robot's Bingo board
    "</div>"          // Closing div tag to end the section containing the robot's board
    "</div>"          // Closing div tag to end the container that holds both the player's and robot's boards

    // JAVASCRIPT
    "<script>"   //Opening script tag to include JavaScript code that will handle the game logic and interactivity for the Bingo game. This script will define functions for drawing numbers, marking cells, checking for wins, and automating the robot's moves.

    "function draw() {"     //Function definition for the draw() function, which is called when the "Draw Number" button is clicked. This function is responsible for randomly selecting a letter and number combination for the Bingo game and updating the content of the "call" div to display the drawn number.
    "  let letters = ['B','I','N','G','O'];" 
    "  let letter = letters[Math.floor(Math.random()*5)];" // Generate a random letter from the array of letters (B, I, N, G, O) by selecting a random index between 0 and 4.
    "  let number = Math.floor(Math.random()*75) + 1;"  // Generate a random number between 1 and 75, which is the typical range of numbers used in a Bingo game.
    "  document.getElementById('call').innerText = letter + number;" // Update the inner text of the "call" div to display the drawn letter and number combination, providing feedback to the player about the current call in the Bingo game.
    "}"

    "function mark(cell) {"     //Function definition for the mark() function, which is called when a cell in the player's Bingo board is clicked. This function is responsible for marking the cell as selected and checking if the player has won the game.
    "  cell.style.backgroundColor = 'green';"     // Change the background color of the clicked cell to green, visually indicating that the cell has been marked as selected by the player.
    "  if (checkWin('player')) alert('You won!');" // After marking the cell, call the checkWin() function with the argument 'player' to check if the player has achieved a winning combination on their Bingo board. If the function returns true, display an alert message indicating that the player has won the game.
    "}"

    "function checkWin(boardId) {"    //Function definition for the checkWin() function, which takes a boardId as an argument to identify which Bingo board (player or robot) to check for a winning combination. This function checks for winning conditions (five in a row) in both rows and columns of the specified board. 
    "  let cells = document.querySelectorAll('#' + boardId + ' td');"  // Select all the table cells (td elements) within the specified board (identified by boardId) and store them in a variable called cells. This allows the function to access and evaluate the state of each cell on the board to determine if there is a winning combination.
    "  let grid = [];"   // Initialize an empty array called grid, which will be used to store the background color of each cell on the specified Bingo board. This array will be used to check for winning combinations by evaluating the colors of the cells in rows and columns.
    "  for (let i = 0; i < 25; i++) {"    // Loop through each of the 25 cells on the Bingo board (5 rows x 5 columns) and push the background color of each cell into the grid array. This allows the function to create a representation of the board's state based on the colors of the cells, which can then be used to check for winning combinations.
    "    grid.push(cells[i].style.backgroundColor);" // Push the background color of the current cell (cells[i]) into the grid array. This captures the state of each cell in terms of whether it has been marked (e.g., green for the player) or not, which is essential for determining if there is a winning combination on the board.
    "  }"
    
    //Check diagonals
    "  if (grid[0] && grid[0] === grid[6] && grid[0] === grid[12] && grid[0] === grid[18] && grid[0] === grid[24]) return true;"  // Check the first diagonal (top-left to bottom-right) by comparing the background colors of the cells at indices 0, 6, 12, 18, and 24 in the grid array. If all these cells have the same non-empty background color, it indicates a winning combination along this diagonal, and the function returns true.
    "  if (grid[4] && grid[4] === grid[8] && grid[4] === grid[12] && grid[4] === grid[16] && grid[4] === grid[20]) return true;"
    "  }"

    // rows
    "  for (let i = 0; i < 5; i++) {"
    "    if (grid[i*5] && grid[i*5] === grid[i*5+1] && grid[i*5] === grid[i*5+2] && grid[i*5] === grid[i*5+3] && grid[i*5] === grid[i*5+4]) return true;"
    "  }"

    // columns
    "  for (let i = 0; i < 5; i++) {"
    "    if (grid[i] && grid[i] === grid[i+5] && grid[i] === grid[i+10] && grid[i] === grid[i+15] && grid[i] === grid[i+20]) return true;"
    "  }"

    "  return false;"
    "}"

    // Robot auto play: robot checks cells accoording to the next randomly drawn number and marks it if it exists on the robot's board. This simulates the robot's turn in the Bingo game, allowing it to automatically mark cells based on the drawn numbers and check for wins.
    "setInterval(function() {" 
    "  let call = document.getElementById('call').innerText;" // Get the current call (letter and number) from the "call" div and store it in a variable called call. This allows the robot to know which number has been drawn and should be marked on its Bingo board if it exists.
    "  let cells = document.querySelectorAll('#robot td');" // Select all the table cells (td elements) within the robot's Bingo board and store them in a variable called cells. This allows the robot to access and evaluate each cell on its board to check if it contains the drawn number that needs to be marked.
    "  for (let i = 0; i < cells.length; i++) {"       // Loop through each cell in the robot's Bingo board and check if the inner text of the cell matches the current call (the drawn letter and number). If a match is found, it indicates that the robot has that number on its board and should mark it.
    "    if (cells[i].innerText === call) {"     // If the inner text of the current cell (cells[i]) matches the call (the drawn letter and number), it means that the robot has that number on its Bingo board and should mark it as selected.
    "      cells[i].style.backgroundColor = 'red';"       // Change the background color of the matching cell to red, visually indicating that the robot has marked this cell as selected based on the drawn number.
    "      if (checkWin('robot')) alert('Robot wins!');"
    "    }"
    "  }"
    "}, 2000);"


    "</script>"

    "</body></html>"
    );


write (new_socket, response, strlen(response));

////////////////////////////////////////////////////////
////     Step 5: Close the connection and socket    ////
////////////////////////////////////////////////////////
close(new_socket);  //The close() system call is used to close the connected socket (new_socket) after we are done communicating with the client. This releases the resources associated with the socket and allows the server to accept new incoming connections. It is important to close the socket when it is no longer needed to free up system resources and avoid potential memory leaks.
close(server_fd);

return 0;

}
