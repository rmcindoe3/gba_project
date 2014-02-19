Personal GBA Project
====================

Phoenix inspired GBA game project.

Playing the game:  Open up the cs2261.gba Gameboy Advance ROM
in an emulator.  Visual Boy Advance is one of the better GBA
emulators and can be found easily with a google search.

File Structure Overview:

 - main.c/.h: Contains the main loop of the program, drawing
    functions specifically created for this project, and most
    of the game flow logic.
 - myLib.c/.h: Contains functions and definitions that apply
    to all GBA projects.
 - shop.c/.h: Contains and keeps track of all in-game shop
    related functions.
 - enemy llist.c/.h: Contains everything related to enemies.
    Specifically, it maintains our enemy linked list and has 
    functions that appropriately add enemies and manipulate 
    the attributes of enemies according to in game events.
 - bullet llist.c/.h: Contains everything related to bullets.
    Specifically, it maintains our ship's bullets and enemy's
    bullets in separate linked lists and has functions that 
    appropriately manipulate the attributes of the bullets
    according to in game events.
 - levels.c/.h: Contains all the different levels of the game
    and keeps track of where the user is in the game at any
    given time.  
 - text.c/.h: Contains functions that will draw text to the
    GBA display.
 - pictures.c/.h: Contains all the bitmap pictures I used
    for drawing the sprites to the screen.
