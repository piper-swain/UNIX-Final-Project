# Bingo Web Game (C, HTML, JavaScript)
An interactive Bingo game built from scratch using a custom C-based HTTP server and a dynamic HTML/CSS/JavaScript frontend. The game features a playable user board, an automated robot opponent, animations, and is visually appealing.

## 🌼 Features
- **Randomly generated Bingo cards** (one for the player, one for the robot opponent).
- **Bingo cards align with real-life game standards** (ex: free space, 'B' column includes numbers ranging from 1-15, 'I' column from 16-30, etc.)
- **Automated robot opponent** marks its card in real time, accurately, accoording to a number drawn by the opponent. This ensures realistic gameplay.
- **Animated number drawing system.**
- **Functioning restart button** for easy replay.
- **Wins are automatically detected and celebrated** with an animated win banner and confetti explosion.
- **Stylized User Interface** which includes gradients, shadows, and a responsive layout, which makes for a more enjoyable, quality experience.

## 🗃 Structure
**UNIX-Final-Project/**
* server.c        : Backend (Custom C HTTP server)
* index.html      : Frontend (HTML + CSS + JS)
* README.md       : Project documentation & Descriptoon

### Backend (C Server)
* uses UNIX socket programming
* Listens on port 9090
* Handles HTTP requests from a browser
* Reads index.html from a disk and sends it back as an HTTP Response

### Frontend (HTML, JavaScript)
* Dynamically generates Bingo boards, which are different every time the page is reloaded or the game is restarted
* Handles user interaction (clicks)
* Animates drawn numbers
* Automates robot gameplay
* Detects win conditions (full rows, columns, or diagonals)

## How to Use

## 📖 Author
Piper Swain
