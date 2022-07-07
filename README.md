# Block Block 3D
Sokoban-based puzzle game made for OpenGL practice.

WIP playable at this repository's Github Pages page: https://gerard-moledo.github.io/opengl-puzzle-game/

![Block Block](https://user-images.githubusercontent.com/72152123/177729391-5f1cfd8b-782e-4b41-8410-5b0ff7e02b7d.gif)

How To Play
-----------
The goal (thus far) is to control the red block to push all brown blocks onto a yellow space.
You move in one direction until you hit another block or the play boundaries.
Learn the rest of the rules by playing the game!

Controls
--------
Press TAB to toggle modes

In 'Play' mode:
- WASD to move
- Space to proceed (after beating level)

In 'Edit' mode:
- WASD to move
- Scroll wheel to change levels
- Left-click to place block or change its type
- Middle-click or shift-left-click to position player block
- Right-click to remove block
- Arrow keys to increase/decrease size of playing field
- IJKL keys to rotate camera spherically about the origin
- UO keys to zoom camera in/out

Create your own levels
----------------------
Level data is stored in a text file (levels.data), and you can extract the text representation of levels by pressing 'Enter' at any state. It looks something like this:

```
# . . . . . . . . 
. @ . . B . . o # 
. . . . . . . . . 
```
This means you can create your own levels via the 'Edit' mode and extract the data for that level. On the web build, you can also add that level to the game by clicking the 'Add Level' button. Levels added this way won't persist between sessions, however.

Thus far, the only way to "save" your level is to copy the extracted text data and store it somewhere. If you think you've made an interesting level, feel free to share it and perhaps it will be added to the game!
