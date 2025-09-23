#include <SFML/Graphics.hpp>
using namespace sf;

// Controls
const Keyboard::Key _controls[4] = 
{
	Keyboard::W,   // Player1 UP
	Keyboard::S,   // Player1 Down
	Keyboard::Up,  // Player2 UP
	Keyboard::Down // Player2 Down
};

// Parameters
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

// Objects of the game
CircleShape _ball;
RectangleShape _paddles[2];

// Score Params
Font _font;
Text _text;
int _p1Score = 0, _p2Score = 0;

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

	// Update Score Text
	_text.setString(std::to_string(_p1Score) + " : " + std::to_string(_p2Score));
	// Keep Score Text Centered
	_text.setPosition((_gameWidth * .5f) - (_text.getLocalBounds().width * .5f), 0);
}

void MovePaddle(RectangleShape &paddle, Keyboard::Key up, Keyboard::Key down, float deltaTime)
{
	// handle paddle movement
	float direction = 0.0f;

	if (Keyboard::isKeyPressed(up))
		direction--;
	if (Keyboard::isKeyPressed(down))
		direction++;

	paddle.move(Vector2f(0.f, direction * _paddleSpeed * deltaTime));
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
	_ball.setOrigin(_ballRadius, _ballRadius); // Should be half the ball width and height

	// Load font-face from res dir
	_font.loadFromFile("res/Fonts/RobotoMono-Regular.ttf");
	// Set text element to use font
	_text.setFont(_font);
	// set the character size to 24 pixels
	_text.setCharacterSize(24);

	Reset();
}

/// <summary>
/// // Update Everything
/// </summary>
/// <param name="dt"> deltaTime or the time between frames </param>
void Update(float deltaTime) 
{
	// move paddles
	MovePaddle(_paddles[0], _controls[0], _controls[1], deltaTime);
	MovePaddle(_paddles[1], _controls[2], _controls[3], deltaTime);

	_ball.move(_ballVelocity * deltaTime);

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
		_player1Serving = false;
		_p1Score += 1; // increase score

		Reset(); // reset ball, paddles and score text
	}
	else if (_ball.getPosition().x < 0) // left wall
	{
		_player1Serving = true;
		_p2Score += 1; // increase score

		Reset(); // reset ball, paddles and score text
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

	window.draw(_text);
}

void Clean() 
{
	// free up the memory if necessary.
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