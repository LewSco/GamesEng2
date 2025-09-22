#include <SFML/Graphics.hpp>
using namespace sf;

const Keyboard::Key controls[4] = 
{
	Keyboard::A,   // Player1 UP
	Keyboard::Z,   // Player1 Down
	Keyboard::Up,  // Player2 UP
	Keyboard::Down // Player2 Down
};

//Parameters
const Vector2f _paddleSize(25.f, 100.f);
const float _ballRadius = 10.f;
const int _gameWidth = 800;
const int _gameHeight = 600;
const float _paddleSpeed = 400.f;
const float _paddleOffsetWall = 10.f;
const float _timeStep = 0.017f; //60 fps
Vector2f _ballVelocity;
bool _player1Serving = true;
const float _initialVelocityX = 100.f; //horizontal velocity
const float _initialVelocityY = 60.f; //vertical velocity
const float _velocityMultiplier = 1.1f; //how much the ball will speed up everytime it hits a paddle. Here, 10% every time.

//Objects of the game
CircleShape _ball;
RectangleShape _paddles[2];

/// <summary>
/// resets the paddles and ball
/// </summary>
void Reset()
{
	// reset paddle position
	_paddles[0].setPosition(_paddleOffsetWall + _paddleSize.x / 2.f, _gameHeight / 2.f);
	_paddles[1].setPosition(_gameWidth - _paddleOffsetWall - _paddleSize.x / 2.f, _gameHeight / 2.f);

	// reset Ball Position
	_ball.setPosition(_gameWidth / 2.f, _gameHeight / 2.f);

	// reset the ball to the initial velocity
	_ballVelocity = { (_player1Serving ? _initialVelocityX : -_initialVelocityX), _initialVelocityY };
}

/// <summary>
/// initialise all the objects needed for the game.
/// </summary>
void Init() 
{ 
	// Set size and origin of paddles
	for (RectangleShape& p : _paddles) {
		p.setSize(_paddleSize);
		p.setOrigin(_paddleSize / 2.f);
	}

	// Set size and origin of ball
	_ball.setRadius(_ballRadius);
	_ball.setOrigin(_ballRadius, _ballRadius); //Should be half the ball width and height

	Reset();
}

/// <summary>
/// // Update Everything
/// </summary>
/// <param name="dt"> deltaTime or the time between frames </param>
void Update(float dt) 
{
	// handle paddle movement
	float direction = 0.0f;

	if (Keyboard::isKeyPressed(controls[0]))
		direction--;
	if (Keyboard::isKeyPressed(controls[1])) 
		direction++;

	_paddles[0].move(Vector2f(0.f, direction * _paddleSpeed * dt));

	_ball.move(_ballVelocity * dt);

	// check ball collision
	if (_ball.getPosition().y > _gameHeight) // bottom wall
	{
		_ballVelocity.x *= _velocityMultiplier;
		_ballVelocity.y *= -_velocityMultiplier;
		_ball.move(Vector2f(0.f, -10.f));
	}
	else if (_ball.getPosition().y < 0) // top wall
	{ 
		_ballVelocity.x *= _velocityMultiplier;
		_ballVelocity.y *= -_velocityMultiplier;
		_ball.move(Vector2f(0.f, 10.f));
	}
	else if (_ball.getPosition().x > _gameWidth) // right wall 
	{
		Reset();
	}
	else if (_ball.getPosition().x < 0) // left wall
	{
		Reset();
	}
	else if (_ball.getPosition().x < _paddleSize.x + _paddleOffsetWall &&                  // ball is inline or behind paddle AND
			_ball.getPosition().y > _paddles[0].getPosition().y - (_paddleSize.y * 0.5) &&  // ball is below top edge of paddle AND
			_ball.getPosition().y < _paddles[0].getPosition().y + (_paddleSize.y * 0.5))     // ball is above bottom edge of paddle 
	{
		// bounce off left paddle
		_ballVelocity.x *= -_velocityMultiplier;
		_ballVelocity.y *= _velocityMultiplier;
		_ball.move(Vector2f(10.f, 0.f));
	}
	else if (_ball.getPosition().x > _gameWidth - (_paddleSize.x + _paddleOffsetWall) &&   // ball is inline or behind paddle AND
			_ball.getPosition().y > _paddles[1].getPosition().y - (_paddleSize.y * 0.5) &&  // ball is below top edge of paddle AND
			_ball.getPosition().y < _paddles[1].getPosition().y + (_paddleSize.y * 0.5))     // ball is above bottom edge of paddle 
	{
		// bounce off right paddle
		_ballVelocity.x *= -_velocityMultiplier;
		_ballVelocity.y *= _velocityMultiplier;
		_ball.move(Vector2f(-10.f, 0.f));
	}

}

void Render(RenderWindow& window) {
	// Draw Everything
	window.draw(_paddles[0]);
	window.draw(_paddles[1]);
	window.draw(_ball);
}

void Clean() 
{
	//free up the memory if necessary.
}

int main() 
{
	//create the window
	RenderWindow window(VideoMode({ _gameWidth, _gameHeight }), "PONG");
	// turn on vsync to limit the frame rate to the refresh rate of the monitor
	window.setVerticalSyncEnabled(true);
	//initialise and load
	Init();

	// Game loop runs while window is open
	while (window.isOpen()) 
	{
		//Calculate dt
		static sf::Clock clock;
		const float dt = clock.restart().asSeconds();

		// clear previous display
		window.clear();

		Update(dt);
		Render(window);
		//wait for the time_step to finish before displaying the next frame.
		sleep(seconds(_timeStep));
		//Wait for Vsync
		window.display();
	}

	//Unload and shutdown
	Clean();
}