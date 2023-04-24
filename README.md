# Endless Runner Game | C++ Course Assignment

 ## Part 1 Features:
 * Infinite Generation
 * Health System
 * Score System
 * Movement (Running, Jumping & Lane Switching)
 * Random Projectile System (Difficulty Based)
 
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
