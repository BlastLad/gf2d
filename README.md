# Super BeWitched!
You're a little witch with a aptitude at casting spells (usually). But this time what was suspossed to be a simple cleaning cantrip for your dorm turned into a nightmare that might end up 
getting your fellow students eaten! All the furniture in the school has begun to run amok and if you don't want to get expelled you
better make sure the other students live to see their next class!

# Controls:
WSAD: Move Piper

Space: Fire Sleep Spell in current movement direction

Q: Use Lockdown in current movement direction

E: Use Emerald Pool at current position

Left-Shift: Use Shadow Effigy at current position

TAB: Use Ultimate Clean
 

# Gameplay

Every Level a number of remaining students will be displayed, every few seconds a new student will spawn and begin to move to the exit.
In the way of the student reaching the exit are pieces of enchanted furniture. If a student encounted a piece of enchancted furniture they will die and the player will lose 1 HP.
Once all students have died or gotten through the level, The level will end and the player can pick an upgrade. After the upgrade is picked an new level will start 


# Student Behaviours

Your students travel to their next class by following the red carpet tiles that are on the game map. All students will appear from the top

1: Girl Student: Will move straight down

2: Boy Student: These students will turn when they reach the a branch instead of travelling straight

3: Purple Students: While moving these students will stop moving for a few seconds, then move really fast, then slow down again

4: Red Variants: After moving for a few seconds, these students will realize they are going to the wrong class and turn around

5: Nervous Student: Moves straight down, but will only move while Piper is near by 

# Piper's Abilities

1: Sleep Spell: (Bound to space): Your primary way of disabling the enemy furniture, The Sleep Spell will temporarily stop the furniture from eating students.

2: Lockdown Spell: (Bound to Q): A special spell that can be used on the Cauldron Funriture (Named Bubbles). This spell will completly stop Bubbles from eating studets, and also prevent him from reawaking.
If you use this spell again on another Bubbles enemy, the first Bubbles will reactivate.

3: Emerald Pool Portal: (Bound to E): When used it places 
one emerald pool on the ground at the players current carpet location.
If a student enters this pool they will glow green and be immune to getting eaten for a 
peroid of time.

4: Shadow Effigy: When used a Shadow clone of your self will spawn at your location and mirror your movements. When you cast Sleep or Lockdown the clone will as well. 

5: Ultima Clean: When used 1 Spell Book Item will be consumed and ALL Furntiure will be stunned as though they were hit by the sleep spell. Only 1 spell book can be held at a time and a new one spawns when you clear a level with full HP.  

# Piper's Upgrades

1: Health Potion: A small red potion that restores 1 HP, If Piper collects 3 health potions when she has full HP, the player will gain +1 Max HP

2: Dual-Cast Sleep: A Permenant Upgrade that will also cast the Sleep Spell behind piper when ever cast

3: Broom of Speed: A Temporary Upgrade that will increase Piper's base speed for a few seconds

4: Transformative Lockdown: A Permenant upgrade that makes it so that while lock down is active on Bubbles, every 5 students that pass will cause bubbles to spawn a Health Potion, Broom, or (very rarely a Spell Book)
Counter restarts if a new Bubbles is targeted

5: Piper's Bookbag: A Permenant upgrade that allows Piper to hold 2 Spell books at a time

# Book of Hazards

1: Bubbles (Cauldron): A enemy that appears on the sides of the carpet tiles, Blocks pipers movement and if a student walks in front of bubbles, bubbles will eat that student

2: Rugby (Rug enemy): A enemy that appears on the carpet that students walk on, If a student walks on Rugby while its active, they will be eaten

3: Karter (Cart Enemy): A enemy that moves between two points on the game map, If a student comes in contact with rugby while its awake, they will be eaten. While disabled Karter will not move.

4: Spill Prank: Magic teleporting water spills that will cause Piper to be unable to act for a short time if she touches one.

5: Bookish: Enchanted books that will fly out from book shelves, if one hits piper, she will lose 1 HP and be unable to act for a short time

# gameframework2d (GF2D)
a collection of utlitity functions designed to facilitate creating 2D games with SDL2
This project is specifically intended to function as an educational tool for my students taking 2D Game Programming.

FOR FULL FEATURES CHECKOUT THE MASTER BRANCH
Students: Don't do that.  You are not ready.... yet.

The main branch is kept deliberately minimal to allow students to build out systems as they are learning.
Other branches have more complete system: Collisions, Windows, Armatures, Entities, Particles...

# Build Process

Before you can build the example code we are providing for you, you will need to obtain the libraries required
by the source code
 - SDL2
 - SDL2_image
 - SDL2_mixer
 - SDL2_ttf
There are additional sub modules that are needed for this project to work as well, but they can be pulled right from within the project.
Performable from the following steps from the root of the cloned git repository within a terminal. 

Make sure you fetch submodules: `git submodule update --init --recursive`
Go into each submodule's src directory and type:
`make`
`make static`

Once each submodule has been made you can go into the base project src folder anre simply type:
`make`

You should now have a `gf2d` binary within the root of your git repository. Executing this will start your game.
