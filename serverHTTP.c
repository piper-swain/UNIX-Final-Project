#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#define MY_SERVER_PORT 9090  //The port number on which the server will listen for incoming connections. This is defined as a constant using the #define pre>
#define buffer_size 200000     //The size of the buffer used to store messages received from the client. This is also defined as a constant using the #define>
//struct sockaddr_in address;   //The sockaddr_in structure is used to specify the address and port number for the server socket. It is defined in the netin>

int main() {
    srand(time(NULL)); //Seed the random number generator with the current time to ensure that the random numbers generated for the Bingo boards are different each time the server is run.

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

///////////////////////////////////////////////////
////      Step 2: Identify/Name a Socket      ////
//////////////////////////////////////////////////

    //assigning a transport address to the socket. This is called "binding" the socket to an address.
    //The address is specified as a sockaddr structure, which includes the IP address and port number.
    //Like assigning a phone number to a phone line, so that it can be reached by others.
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
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
    exit(EXIT_FAILURE);
}
printf("Socket created, bind completed");

//////////////////////////////////////////////////////////////////////
////   Step 3: Wait for an incoming connection on the server      ////
//////////////////////////////////////////////////////////////////////


printf("server_fd before listen: %d\n", server_fd);
//Code to listen and accept incoming connections:
if (listen(server_fd, 3) < 0)
{
       perror ("listen failed");
       exit (EXIT_FAILURE);
}

    printf("Listening...\n");

while (1) {

if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
{
    perror("In accept");
    exit(EXIT_FAILURE);
}
printf("Client connected.");

////////////////////////////////////////////////////
////     Step 4: Read and receive messages      ////
////////////////////////////////////////////////////
char buffer[200000] = {0}; //Buffer to store the message received from the client.
read(new_socket, buffer, 200000); //read the message from client and store in buffer
printf("%s\n", buffer); //Print the message received from the client to the console. Shows the browser's request to the server


char response[200000];
response[0] = '\0';


strcat(response,
"HTTP/1.1 200 OK\r\n"           //HTTP status line indicating a successful response with status code 200 
"Content-Type: text/html\r\n"   //HTTP header specifying the content type of the response as HTML
"Connection: close\r\n"         //HTTP header indicating that the server will close the connection after sending the response
"\r\n"                          //Blank line to separate the headers from the body of the response

"<!DOCTYPE html>"              //HTML5 doctype declaration indicating that the document is an HTML5 document
"<html><head>"                       //Opening HTML tag to start the HTML document
"<h1 style='text-align:center; font-size:50px;'>Let's play BINGO!</h1>"   //Head section of the HTML document containing the title of the page, which is displayed in the browser's tab

"<style>"   //Opening style tag to include CSS styles that will define the appearance of the elements on the webpage, such as the Bingo boards, cells, and win banner.
"td { width:50px; height:50px; text-align:center; vertical-align:middle; position:relative; }" //CSS style definition for the table cells (td elements) in the Bingo boards. It sets the width and height of each cell to 50 pixels, centers the text both horizontally and vertically, and sets the position to relative to allow for absolute positioning of the ::after pseudo-element used for marking selected cells.

".win-banner {"   //CSS class definition for the "win-banner" class, which is used to style the banner that is displayed when a player wins the Bingo game. This banner will be positioned in the center of the screen and will have a pop animation to draw attention to the win.
"  position: fixed;"
"  top: 40%; left: 50%;"
"  transform: translate(-50%, -50%);" //The position property is set to fixed, which means that the win banner will be positioned relative to the viewport and will stay in the same position even when the page is scrolled. The top and left properties are set to 40% and 50% respectively, which positions the banner in the center of the screen. The transform property is used to adjust the position of the banner by translating it -50% horizontally and vertically, effectively centering it on the screen.
"  background: gold;"
"  color: DarkGoldenrod;"
"  padding: 20px;"   //The padding property is set to 20 pixels, which adds space around the content of the win banner, making it larger and more visually appealing.
"  font-size: 40px;"
"  border-radius: 10px;"
"  animation: pop 0.5s ease-in-out infinite alternate;"   //The animation property is set to "pop 0.5s ease-in-out infinite alternate", which applies the "pop" animation defined in the @keyframes rule below. This animation will cause the win banner to scale up and down continuously, creating a popping effect to draw attention to the win.
"}"

"@keyframes pop {"  //CSS keyframes rule that defines the "pop" animation used for the win banner. This animation will cause the banner to scale up and down continuously, creating a popping effect to draw attention to the win.
"  from { transform: translate(-50%, -50%) scale(1); }"  //The "from" keyframe defines the starting state of the animation, where the win banner is at its normal size (scale(1)).
"  to { transform: translate(-50%, -50%) scale(1.2); }" //The "to" keyframe defines the ending state of the animation, where the win banner is scaled up to 1.2 times its normal size (scale(1.2)). The animation will alternate between these two states, creating a continuous popping effect.
"}"
"</style>" //Closing style tag to end the CSS styles section of the HTML document
"</head>"  //Closing head tag to end the head section of the HTML document

"<body>"                                                //Opening body tag to start the body of the HTML document, which contains the content that will be displayed on the webpage

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
for (int i = 0; i < 25; i++) {    //Loop through each cell in the player's Bingo board (5 rows x 5 columns = 25 cells) and fill it with a random number based on the column it belongs to. The numbers are generated according to the standard Bingo rules, where each column has a specific range of numbers:
    char cell[100];

    int col = i % 5;  // 0=B, 1=I, 2=N, 3=G, 4=O
    int min, max;

    if (col == 0) { min = 1; max = 15; }        // If the column index is 0 (corresponding to the 'B' column), set the minimum value to 1 and the maximum value to 15 for random number generation. This means that the numbers in the 'B' column of the player's Bingo board will be randomly selected from the range 1 to 15 (inclusive).
    else if (col == 1) { min = 16; max = 30; }  // If the column index is 1 (corresponding to the 'I' column), set the minimum value to 16 and the maximum value to 30 for random number generation. This means that the numbers in the 'I' column of the player's Bingo board will be randomly selected from the range 16 to 30 (inclusive).
    else if (col == 2) { min = 31; max = 45; }
    else if (col == 3) { min = 46; max = 60; }
    else { min = 61; max = 75; }

    if (i == 12) {
        sprintf(cell, "<td onclick='mark(this)' style='background-color:lightgreen;'>FREE</td>");
    } else {
        int num = rand() % (max - min + 1) + min;
        sprintf(cell, "<td onclick='mark(this)'>%d</td>", num);
    }

    strcat(response, cell);

    if ((i + 1) % 5 == 0 && i != 24)
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
for (int i = 0; i < 25; i++) {   
    char cell[100];   

    int col = i % 5;  // Calculate the column index (0 to 4) based on the current cell index (i). This is done using the modulus operator (%) to determine which column the current cell belongs to in the 5x5 grid of the Bingo board.
    int min, max;     // Declare variables to store the minimum and maximum values for the random number generation based on the column index. The Bingo board is divided into 5 columns (B, I, N, G, O), each with a specific range of numbers:

    if (col == 0) { min = 1; max = 15; }   // If the column index is 0 (corresponding to the 'B' column), set the minimum value to 1 and the maximum value to 15 for random number generation.
    else if (col == 1) { min = 16; max = 30; }
    else if (col == 2) { min = 31; max = 45; }
    else if (col == 3) { min = 46; max = 60; }
    else { min = 61; max = 75; }

    if (i == 12) {
        sprintf(cell, "<td style='background-color:lightcoral;'>FREE</td>");
    } else {
        int num = rand() % (max - min + 1) + min;
        sprintf(cell, "<td>%d</td>", num);
    }

    strcat(response, cell);

    if ((i + 1) % 5 == 0 && i != 24)
        strcat(response, "</tr><tr>");
}

strcat(response,      
    "</tr></table>"   // Closing table tag to end the robot's Bingo board
    "</div>"          // Closing div tag to end the section containing the robot's board
    "</div>"          // Closing div tag to end the container that holds both the player's and robot's boards


    "<script>"   //Opening script tag to include JavaScript code that will handle the game logic and interactivity for the Bingo game. This script will define functions for drawing numbers, marking cells, checking for wins, and automating the robot's moves.

    " window.onload = function() { "
    " initCalls(); "
    " };"

    "let calls = [];"
    "let index = 0;"

    "function initCalls() {"   //Function definition for the initCalls() function, which initializes the calls array with all possible letter and number combinations for the Bingo game. This function generates the standard Bingo calls based on the letters B, I, N, G, O and their corresponding number ranges, and then shuffles the calls to randomize the order in which they will be drawn during the game.
    "  let letters = ['B','I','N','G','O'];"
    "  let ranges = ["
    "    [1,15],"
    "    [16,30],"
    "    [31,45],"
    "    [46,60],"
    "    [61,75]"
    "  ];"

    "  for (let i = 0; i < 5; i++) {"    // Loop through each letter and its corresponding number range to generate the calls for the Bingo game. For each letter (B, I, N, G, O), we loop through the numbers in the specified range and create a call by concatenating the letter with the number (e.g., "B1", "I16", "N31", etc.) and push it into the calls array.
    "    for (let n = ranges[i][0]; n <= ranges[i][1]; n++) {"  
    "      calls.push(letters[i] + n);"
    "    }"
    "  }"

    "  // shuffle (Fisher-Yates)"  // After generating the calls, we shuffle the calls array using the Fisher-Yates algorithm to ensure that the order of the calls is random for each game. This algorithm works by iterating through the array from the last element to the first, randomly selecting an index from the remaining unshuffled portion of the array, and swapping the current element with the randomly selected element.
    "  for (let i = calls.length - 1; i > 0; i--) {"
    "    let j = Math.floor(Math.random() * (i + 1));"
    "    let temp = calls[i];"
    "    calls[i] = calls[j];"
    "    calls[j] = temp;"
    "  }"
    "}"

"function draw() {" //Function definition for the draw() function, which is called when the "Draw Number" button is clicked. This function is responsible for drawing the next letter and number combination from the calls array and updating the content of the "call" div to display the drawn number. It also checks if there are no more calls left to draw and updates the "call" div accordingly.
"  if (index >= calls.length) {"
"    document.getElementById('call').innerText = 'No more numbers';"
"    return;"
"  }"

"  document.getElementById('call').innerText = calls[index][0] + \"-\" + calls[index].slice(1);"
"  index++;"
"}"

    "function mark(cell) {"     //Function definition for the mark() function, which is called when a cell in the player's Bingo board is clicked. This function is responsible for marking the cell as selected and checking if the player has won the game.
    "  cell.style.backgroundColor = 'lightGreen';"     // Change the background color of the clicked cell to green, visually indicating that the cell has been marked as selected by the player.
    "  if (checkWin('player')) showWin('You won!');" // After marking the cell, call the checkWin() function with the argument 'player' to check if the player has achieved a winning combination on their Bingo board. If the function returns true, display an alert message indicating that the player has won the game.
    "}"

    "function showWin(message) {" //Function definition for the showWin() function, which is called when a player wins the game. This function is responsible for displaying a banner on the screen with a message indicating the winner of the game.
    "  let banner = document.createElement('div');" // Create a new div element using document.createElement('div') and store it in a variable called banner. This div will be used to display the win message on the screen.
    "  banner.className = 'win-banner';"  // Set the class name of the banner div to "win-banner", which will apply the CSS styles defined for the win banner (such as positioning, background color, font size, and animation) to visually indicate that a player has won the game.
    "  banner.innerText = message;" // Set the inner text of the banner div to the message passed as an argument to the showWin() function. This message will indicate whether the player or the robot has won the game.
    "  document.body.appendChild(banner);" // Append the banner div to the body of the HTML document using document.body.appendChild(banner), which will display the win banner on the screen when a player wins the game.
    "}"

    "function checkWin(boardId) {"     //Function definition for the checkWin() function, which is called to check if a player has achieved a winning combination on their Bingo board. This function takes a boardId as an argument, which specifies whether to check the player's board or the robot's board for a win.
    "  let cells = document.querySelectorAll('#' + boardId + ' td');"  //Select all the table cells (td elements) within the specified board (either 'player' or 'robot') and store them in a variable called cells. This allows the function to access and evaluate each cell on the specified board to check for winning combinations.
    "  let grid = [];"  
    "  for (let i = 0; i < 25; i++) {"
    "    if (cells[i].innerText === 'FREE' || cells[i].style.backgroundColor !== '') {"
    "      grid.push(1);"
    "    } else {"
    "      grid.push(0);"
    "    }"
    "  }"

    // diagonals
    "if (grid[0] && grid[6] && grid[12] && grid[18] && grid[24]) return true; "
    "if (grid[4] && grid[8] && grid[12] && grid[16] && grid[20]) return true; "
    // rows
    "for (let i = 0; i < 5; i++) { "
    "   if (grid[i*5] && grid[i*5+1] && grid[i*5+2] && grid[i*5+3] && grid[i*5+4]) return true; "
    "} "

    // columns
    "for (let i = 0; i < 5; i++) { "
        "if (grid[i] && grid[i+5] && grid[i+10] && grid[i+15] && grid[i+20]) return true; "
    "}  "

    "  return false;"
    "   }"

    // Robot auto play: robot checks cells accoording to the next randomly drawn number and marks it if it exists on the robot's board. This simulates the robot's turn in the Bingo game, allowing it to automatically mark cells based on the drawn numbers and check for wins.
    "setInterval(function() {" 
    "  let call = document.getElementById('call').innerText;"
    "  if (!call.includes('-')) return;"
    "  let cells = document.querySelectorAll('#robot td');"
    "  let number = call.split('-')[1];"
    "  for (let i = 0; i < cells.length; i++) {"       // Loop through each cell in the robot's Bingo board and check if the inner text of the cell matches the current call (the drawn letter and number). If a match is found, it indicates that the robot has that number on its board and should mark it.
    "    if (cells[i].innerText === number) {"     // If the inner text of the current cell (cells[i]) matches the call (the drawn letter and number), it means that the robot has that number on its Bingo board and should mark it as selected.
    "      cells[i].style.backgroundColor = 'lightcoral';"       // Change the background color of the matching cell to lightcoral, visually indicating that the robot has marked this cell as selected based on the drawn number.
    "      if (checkWin('robot')) showWin('Robot wins!');" // After marking the cell, call the checkWin() function with the argument 'robot' to check if the robot has achieved a winning combination on its Bingo board. If the function returns true, display an alert message indicating that the robot has won the game.
    "      break;" // Break out of the loop after marking the cell and checking for a win, since the robot only needs to mark one cell per drawn number and check for a win after marking.
    "    }"
    "  }"
    "}, 2000);"

    "window.onload = function() { initCalls(); };"
    "</script>"
    "</body></html>"
    );

write (new_socket, response, strlen(response));   //The write() system call is used to send the HTTP response back to the client through the connected socket (new_socket). The response string is sent as a byte stream, and the length of the response is determined using strlen(response) to ensure that the entire response is transmitted to the client.

////////////////////////////////////////////////////////
////     Step 5: Close the connection and socket    ////
////////////////////////////////////////////////////////
close(new_socket);  //The close() system call is used to close the connected socket (new_socket) after we are done communicating with the client. This relea>
}
close(server_fd);

exit(EXIT_FAILURE);

}


