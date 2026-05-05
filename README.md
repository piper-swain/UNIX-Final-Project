# Bingo Web Game (C, HTML, JavaScript)
An interactive Bingo game built from scratch using a custom C-based HTTP server and a dynamic HTML/CSS/JavaScript frontend. The game features a playable user board, an automated robot opponent, animations, and is visually appealing.

<img src="https://github.com/user-attachments/assets/222f06e0-9a72-4a90-b0a9-db593ff4c17c" width="300">

---
## 🌼 Features
- **Animated number drawing system.**
- **Restart button**
- **Randomly generated Bingo cards**
  - one for the player, one for the robot opponent
  - cards align with real-life game standards (ex: free space, 'B' column includes numbers ranging from 1-15, 'I' column from 16-30, etc.)
- **Automated robot opponent**
  - marks its card in real time, accurately, accoording to a number drawn by the opponent. This ensures realistic gameplay
- **Wins are automatically detected and celebrated** 
  - animated win banner and confetti explosion when a win occurs
- **Stylized User Interface**
  - gradients, shadows, and a responsive layout, which makes for a more enjoyable, quality experience


## 🐞 Structure
* **UNIX-Final-Project/**
  * server.c        : Backend (Custom C HTTP server)
  * index.html      : Frontend (HTML + CSS + JS)
  * README.md       : Project documentation & Descriptoon

### Backend (C Server)
* Uses IPv4 TCP sockets (`AF_INET`, `SOCK_STREAM`)
* Binds to port 9090 and listens for incoming connections
* Handles HTTP requests from a browser
* Reads index.html from a disk into memory
* Sends index.html back as a formatted HTTP response
* Handles connections in a loop

### Frontend (HTML, JavaScript, Browser)
* Dynamically generates Bingo boards, which are different every time the page is reloaded or the game is restarted
* Handles user interaction (clicks)
* Animates drawn numbers
* Automates robot gameplay
* Detects win conditions (full rows, columns, or diagonals)

---
## 🌿 To open the game:
<img src="https://github.com/user-attachments/assets/50803f0b-eb20-4fb7-9659-3159c234b916" width="300">

**1. Compile the server**
* gcc server.c -o server

**2. Run the server**
* ./server
Expected output:
* Socket created and bound to port 9090
* Listening...

**3. Open on the web browser**
* http://localhost:9090

## 🍃 Gameplay Flow
**1. Click “Draw Number”**
  * A random Bingo call (e.g., B-12, G-58) is displayed
  * The number display animates for visual feedback

**2. Match the number on your board:**
  * If you have the number, click the corresponding cell
  * The cell turns green

**3. Robot behavior:**
  * The robot automatically scans its board
  * If it has the number, it marks it (cell turns pink)
---
## 🏵 Limitations
* Possible duplicate numbers within columns
* Single-threaded server (handles one client at a time)
* etc.
---
## 🌷 Demonstrated Concepts & Skills
**Systems Programming**
  * Socket creation and management
  * Memory allocation and file I/O
  * HTTP protocol formatting
* **Web Fundamentals**
  * Client-server architecture
  * Request/response lifecycle
  * Static file serving
* **Frontend Development**
  *  DOM manipulation
  * Event handling
  * CSS animations and transitions
* **Game Logic Design**
  * State tracking via DOM
  * Win condition evaluation
  * Timed event simulation
---
## 📖 Author
Piper Swain
