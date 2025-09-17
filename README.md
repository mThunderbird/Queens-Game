# Queens game

A board game created for a Game Jam contest by my friends and me. Won 2nd place

Player vs Player or Player vs Bot: Take turns to place queens on a checkboard. Each placed queen blocks the places she can reach (according to chess rules). Be the player to put the last queen on the board to win!

---
## Screenshots

| Menu | Gameplay | Endscreen |
|:----------------------:|:--------------------:| :--------------------:
| ![Menu](media/menu.png) | ![Gameplay](media/gameplay.png) | ![Endscreen](media/endscreen.png) |

---
## Tech stack

- C++ SDL2 for gameplay
- Figma for graphics

---
## Features

- Utilising the State Design Pattern to control the Game state
- Properly managing textures so they are stored only once using C++ pointers
- 2 bot difficulties - random and simple algorithmic
- Background music and responsive sound effects

---
## Setup & Run

1. **Clone or download** this repo (ZIP)  
2. **Open** EngineSetUp/Debug  
3. **Run** `EngineSetUp.exe`
