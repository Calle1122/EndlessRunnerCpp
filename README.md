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
  The Character extends the third-person template character. All I did was swap out the action mappings to instead move the character based on the lanes.
  
  #### Random Projectile System
  ...
  
 ### Planned improvements for Part 2
 * Functioning Game Over Screen
 * Fix Score Multiplier
 * Add Highscore System
 * Infinite Lane Issue
