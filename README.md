Overview
=======
ToT AI is a Tour of Turns AI module.
It connects to the main program with standard IO stream providing behavior for racers.

-----------

Structure of Artificial intelligence in Tour of Turns
=======
At the start of the game ToT AI must provide an informations about cars and tires selected by computer players, 
as well in every turn program is obligated to send requests for actions and attacks of all AI participants.
There are multiple queries that could be send to retreive information about the race.
Messages must start with a code specified in Constants.h file of the project.

------------

Informations about current module
=======
ToT AI creates 3 different types of AI:
-aggressive: tries to attack player, goes as fast as possible
-moderate: tries to keep perfect balance
-drifter: tries to priorities drifts when deciding on optimal path

Names and behavior is set accordingly to selected type.



 
