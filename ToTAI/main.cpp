#include "AI.h"
#include <time.h>

int main()
{
	srand(time(0));
	AI game_instance;
	game_instance.InitializationPhase();
	game_instance.GamePhase();
}