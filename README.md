# AirCavLOS
Line of Sight tool for West End Games "Air Cav"

This tool is intended to be used in conjunction with the board game "Air Cav" from West End Games, 1985. It calculates Line of Sight (LOS) and Final Kill Numbers (FKN) for all units in a given scenario. It does not implement "turns" of the game - but it keeps track of all units and their state, in addition to the map terrain. Think of it as a companion for taking care of the tedious parts of the game.

I originally wrote (almost 20 years ago!) this to handle LOS calculations because these are quite difficult in this game, given the topography of the map. This program will handle all of this for you, letting you play the game quickly. The interactions of all the units can also become difficult - now this is handled automatically. One great thing about Air Cav is line of sight is not mutually exclusive - a unit can see without being seen, and this greatly affects what tactics are used.

Note that it does NOT implement any dice rolling! When a unit fires, the FKN is displayed and a result of Suppressed, Kill or Miss can be chosen. Again, it is intended as a companion to the boardgame and only dice are truly random.

One minor tweak to the board game is instead of the FKN being a number from 1-10, it applies a small amount of randomness so that the FKN is instead a percentage (from 1-100%) so that you can resolve the combat using a d100 instead of a d10. This was done simply to make it a bit less deterministic.

"Air Cav LOS" includes the following features:
- All unit information for the active unit is displayed
- All units that the active unit can see are displayed, including range and FKN numbers to that unit
- All opposing units that can see the active unit are displayed, including range and FKN numbers to the active unit
- If a unit could only see an enemy if it were to do a popup (meaning another friendly unit can see it), this is noted
- The active unit can Move, can Fire, Evade, enter Defilade, do a Popup or Pop Smoke at the click of a button, expending Operations points
- Operations points are expended as operations are conducted, the program will not allow operations if not enough points are available
- When a unit is moved, all Lines of Sight are re-calculated and displayed
- Opposing units can Opportunity Fire on the active unit at any time, once all opportunity firing units are known, these are resolved with a single button click
- Indirect Fire can be applied at any time, including Smoke - smoke can be randomly dissipated with the push of a button
- Weather, Time of Day, Advanced Observation (Special Vision Equipment, Weather, Skylining), Suppression, Main and Secondary Weapons are all implemented
- Radar and Laser Designation is implemented - a target can be laser designation by one unit and fired on by another unit
- Terrain for the hex currently occupied by the active unit is displayed, including Smoke and Wrecks
- Stacking limits and effects are enforced
- In progress scenarios can be saved at any time and recalled later
- Detailed LOS and FKN information is displayed if desired (in case you want to verify the algorithm results)
- Head to Head networking is supported (this way two players can play remotely against each other)

Current limitations:
- Only the European map is implemented
- Only Scenarios 1-5 are implemented, however the scenario format is a simple text file, so you can easily generate your own
- Close Assault and Overrun, ECM and Random Events are not implemented at this time
- Runs on Windows only (there are no plans to port to any other platform, please don't ask)


Please email aircavlos@gmail.com for any questions or comments
