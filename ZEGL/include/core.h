#ifndef CORE_H
#define CORE_H

#include <string>

class Game;
class Window;

//This is the central part of the game engine. It's purpose is to manage interaction 
//between the various sub-engines (such as the rendering and physics engines) and the game itself.
class Core
{
public:
	Core(double frameRate, Window* window, Game* game);
	virtual ~Core() {};

	void Start(); //Starts running the game; contains central game loop.
	void Stop();  //Stops running the game, and disables all subsystems.

protected:
private:
	Core(Core const&) = delete;
	Core& operator=(Core const&) = delete;
	
	Window*		m_window;          //Used to display the game
	Game*		m_game;
	bool		m_isRunning;
	double		m_frameTime;       //How long, in seconds, one frame should take
	
};

#endif
