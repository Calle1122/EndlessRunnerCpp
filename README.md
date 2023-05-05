# Endless Runner Game | C++ Course Assignment

## Contents:
### [Part 1](#part-1-features)
### [Part 2](#part-2-features)

 ## Part 1 Features:
 * [Infinite Generation](#infinite-generation)
 * [Health System](#health-system)
 * [Score System](#score-system)
 * [Movement (Running, Jumping & Lane Switching)](#movement)
 * [Random Projectile System (Difficulty Based)](#random-projectile-system)
 
 Also see: [Planned Improvements for Part 2](#planned-improvements-for-part-2)
 
 ### C++ use | Part 1
  #### Infinite Generation
  System is divided into two classes:
  * RunTile
  * RunnerGameModeBase
  
  Run Tile:
  
  The actual tile that generates and gets destroyed (what the player sees). Every tile is a blueprint that derrives from the RunTile class. Each tile consists of a TArray with 'ProjectilePositions', a seampoint and a 'DestroyTriggerBox'. The projectile positions are tied into the [Random Projectile System](#random-projectile-system) and decides where projectiles can spawn. The seampoint is what decides where the next tile should connect to the current one and the DestroyTriggerBox is what decides when the current tile should be destroyed and a new one generated (based on when the player has collided).
  
  Runner Game Mode:
  
  The Game Mode that handles the logic of the game (a lot of systems go through this class to make the game function). This class handles the spawning of tiles, however the destruction of them is handled on the tiles themselves. The Game Mode contains a TArray of all possible tiles to spawn as well as a variable for how many tiles to have spawned at once. The Game Mode also keeps track of the next relevant seam point for spawning new tiles.
  
  #### Health System
  The Player has 3 lives each run. Whenever the player collides with an obstacle or a projectile, they lose health. After losing health an FTimerHandle delays further damage taking for 1 second (I-Frames). The Health System is tied to the UI displaying the current Health Status. After losing all health the game ends. This is all handled through the Runner Game Mode, except UI which has its own class ('RunGameHUD').
  
  NOTE: Losing the game currently brings up the Game Over Screen, but the buttons to 'Restart' or 'Exit To Main Menu' are not currently functioning. To replay you must exit the application and re-run it.
  
  #### Score System
  The score determines how far the player got during a run. There is a multiplier which increases throughout the run, making the score gain exponential. When the player dies their final score is displayed. The score counting is also handles through the Runner Game Mode.
  
  #### Movement
  The Character extends the third-person template character. All I did was swap out the action mappings to instead move the character based on the lanes. You use SPACE to jump and A&D to move left and right.
  
  Currently the sideway movement is done by adding a local offset to the player, essentialy teleporting it to the destination.
  
  #### Random Projectile System
  To make the game ramp up in difficutly as the run progresses, a projectile system based was created. Projectiles randomly fall from the sky (on set possible positions) with the chance of them spawning increasing over the course of the run. When the run starts the chance is 1% which turns all the way up to 100% after a few minutes. The game then becomes (almost) impossible by design, ending the run.
  
  The actual timing of making the projectiles hit the player was solved with a shader, so the only C++ used here was the spawning. When a tile is generated it loops over each Projectile Position in the Tile and decides (based on the Run Game Modes current projectile percent chance variable) if a projectile should spawn there.
  
 ### Planned improvements for Part 2
 * Functioning Game Over Screen

I will add functionality to the Restart- and Main Menu button so that the game can be replayed without having to close and re-open it. Basically finishing the game loop.

 * Fix Score Multiplier

I want to have the score multiplier reset whenever the player takes damage. This will make it so that you earn more points the longer you can go without taking damage.

 * Add Highscore System

The players final score will be written to a save file on the computer which will also be used to display previous scores and the High-Score.

 * Infinite Lane Issue

Currently the player can press A&D and go however far left and right they want. At first I wanted to limit movement to the three lanes like in most endless runners. However, after getting some feedback from friends, I will keep the players ability to jump outside the lanes, but making it so that if the player fails to get back to a lane before landing, they instantly die. (Currently they fall through the map, forcing the player to quit the game). Everyone who tested the game thought it made for a fun mechanic when the game got almost impossible due to the projectiles, they could still dodge some if they jumped around them by going outside the lanes.

 * Lerp Movement

Currently the player gets teleported side to side. I want to add a linear interpolation to smooth this out.

## Part 2 Features:
 * [Dual Lanes](#dual-lanes)
 * [Two Players: 1 Keyboard](#two-players)
 * [Dodge Mechanic (Lucky Dodge)](#dodge-mechanic)
 * [Highscore Save & Load](#highscore)
 * [Respawn Mechanic](#respawn)
 
 Also see: [Adressed issues from Part 1](#adressed-issues-from-part-1)
 
 ### C++ use | Part 2
  #### Dual Lanes
  The tile generation functions remains the same, but with the added functionality of copying over generated tiles to another set of lanes. This means that Player1 and Player2 get the same set of tiles to make them play with the same chances. However the players are cooperating for a good score, so having different lanes could have been fine as well, since it is not competetive.  
  
  #### Two Players
  The game now spawns in a 2nd player upon launch. Player1 is now controlled with AD & Space, whilst Player2 uses the Left-Right & Up arrow keys. To make the input from 1 input device (the keyboard) control two characters, I use the first player as a form of input manager. The first player controller then has one set of methods binded to the AD & Space keys and another set for the Left-Right & Up keys. The second set targets the Player2 reference and executes the movement methods for that character. 
  
  #### Dodge Mechanic
  Obstacles now know when they are dodged. If the player successfully dodges an obstacle there is a 25% chance of getting a "Lucky Dodge". A message will then be displayed on screen so the player knows they got one. Upon getting a Lucky Dodge, a random obstacle is selected and destroyed. Since the players are cooperating, the obstacles are not always selected from the set of lanes connected to the player that got the Lucky Dodge, but instead selected from all active obstacles. 
  
  #### Highscore
  The score system works just like in Part 1, but when the run ends, the score is now saved along with the player names. The saving utilizes the SaveGame class in Unreal which allows saving and loading data using a .sav file. I decided to save and load a TArray of my own LeaderboardItem struct. The struct contains 2 strings: Name1 & Name2 (Entered in the Main Menu by the players), as well as an int for the Score. There is also an override for the < operator which makes it return (Score < anotherLeaderboardItem->Score) so that the structs can be sorted in order. The file is then loaded when the run ends and the previous highscores displayed. The run that just ended is then saved into that file and sorted.
  
  #### Respawn
  When one of the players die, the game continues. Three seconds after dying, a player respawns with full health. This means that the run only ends if both players are dead at the same time. This makes for some interseting tactics where the players should try taking turns dying to refresh their health. For this mechanic i mostly used the FTimerHandles and different states for the players.
  
 ### Adressed issues from Part 1
 * GameOver Screen
 
 Made the buttons work to finish the game-loop.
 
 * Highscore
 
 As mentioned above there is now a working highscore system.
 
 * Infinite Lane issue
 
 I decided to limit the players movement to just the three lanes they have. Like mentioned in Part 1 I wanted to make use of the air beside the lanes but ended up scrapping this idea.
 
 I did not Lerp movement or change the Score Multiplier like originally planned.
