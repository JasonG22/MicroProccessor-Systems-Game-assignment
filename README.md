Link to Project video https://youtu.be/-Aud4VfSPMk?si=5o4y8xpCODXmMtZo

This project is a space shooter game developed as a group assignment for the STM32 microcontroller board.
The goal was to create a complete and functional game that showcased our embedded programming skills while meeting a set of assessment criteria that included gameplay quality, code structure, use of graphics, originality, and interactivity.

As a group, we worked together on most aspects of the game collectively, including the design of the menu system, graphics, scoring, and overall game loop. 
The game begins with a main menu where players can start the game, view credits, or adjust the difficulty between easy and hard. 
During gameplay, the player controls a ship, moves it across the screen, and fires projectiles at descending enemies. Points are awarded when projectiles hit enemies, while collisions with enemies reduce the player’s lives. 
The game tracks both the player’s score and the highest score achieved, and when all lives are lost, a “Game Over” screen appears before returning the player to the main menu.
Within the group, I contributed across the project but took responsibility specifically for the lives system creating a clear visual indicator of remaining chances and integrating it into the gameplay loop. 
I also specifiaclly collaborated together with another member on the collision detection system, which ensured accurate interactions between the player, projectiles, and enemies.

The project was written entirely in C and made use of GPIO inputs for controls, sprite rendering for visuals, SysTick timers for game pacing, and random number generation for enemy spawning. 
Working collaboratively gave us the opportunity to become familiar with working together in a team to combine our strengths, share ideas, and solve problems as, while also allowing me to focus more deeply on systems that were crucial to gameplay.
Completing this project helped me develop both technical skills in programming and practical experience in teamwork. I learned how to structure efficient C code, integrate graphics and hardware inputs into a real-time application, and coordinate with others to produce a complete, polished game.
