#ifndef GAME_H
#define GAME_H

class Camera;
class Input;
class Window;

class Game
{
public:
	Game() {}
	virtual ~Game() {}

	void Init(const Window& window);

	void ProcessInput(const Input& input, float delta);
	void Update(float delta);
	void Render();

protected:
private:
	Game(Game const&) = delete;
	Game& operator=(Game const&) = delete;

	Camera*	m_camera;
};

#endif
