# AirCavLOS
Line of Sight tool for West End Games Air Cav

This tool is intended to be used in conjunction with the board game "Air Cav" from West End Games, 1985. It calculates Line of Sight (LOS) and Final Kill Numbers (FKN) for all units in a given scenario. It does not implement "turns" of the game - but it keeps track of all units and their state, in addition to the map terrain. Think of it as a companion for taking care of the tedious parts of the game.

I originally wrote this to handle LOS calculations because these are quite difficult in this game, given the topography of the map. This program will handle all of this for you, letting you play the game quickly. The interactions of all the units can also become difficult - now this is handled automatically. One great thing about Air Cav is line of sight is not mutually exclusive - a unit can see without being seen, and this greatly affects what tactics are used.

Note that it does NOT implement any dice rolling! When a unit fires, the FKN is displayed and a result of Kill or No Kill can be chosen. Again, it is intended as a companion to the boardgame.

One minor tweak to the board game is instead of the FKN being a number from 1-20, it applies a small amount of randomness so that the FKN is instead a percentage (from 1-100%) so that you can resolve the combat using a d100 instead of a d20. This was done simply to make it a bit less deterministic (and also the FKN numbers actually seem quite high in my opinion).

Air Cav LOS includes the following features:
- All unit information for the active unit is displayed
- All units that the active unit can see are displayed, including range and FKN numbers to that unit
- All opposing units that can see the active unit are displayed, including range and FKN numbers to the active unit
- The active unit can Move, can Fire, Evade, enter Defilade, do a Popup or Pop Smoke at the click of a button, expending Operations points
- Operations points are expended as operations are conducted, the program will not allow operations if not enough points are available
- When moved, all Lines of Sight are re-calculated and displayed
- Opposing units can Opportunity Fire on the active unit at any time, once all opportunity firing units are known, these are resolved with a single button click
- Indirect Fire can be applied at any time
- Weather is implemented
- Terrain for the hex currently occupied by the active unit is displayed

Current limitations:
- The map is not complete - if information for a hex is required, the program will ask for it (this is saved on exit, so the map is built up over time)
- Not all scenarios are implemented
- Only the European map is implemented
- See the "Air Cav LOS Features.doc" file for a list of features on the "to do" list
- Runs on Windows only

Please email bjmcho@gmail.com for any questions
