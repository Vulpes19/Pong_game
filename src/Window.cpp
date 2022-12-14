#include "Window.hpp"

Window::Window( unsigned int w, unsigned int h): width(w), height(h)
{
	window = nullptr;
	renderer = nullptr;
	running = false;
	play = true;
	pause = false;
	menu = new Menu;
	player = new Player;
	enemy = new Enemy;
	score = new Score;
	ball = new Ball;
	pauseMenu = new Pause;
}

Window::~Window( void )
{
	delete menu;
	delete player;
	delete enemy;
	delete score;
	delete ball;
	delete pauseMenu;
	SDL_DestroyWindow( window );
	SDL_DestroyRenderer( renderer );
	SDL_Quit();
}

int    Window::createWindow( void )
{
	std::cout << "** CONTROLS **" << std::endl;
	std::cout << "up arrow to go up" << std::endl;
	std::cout << "down arrow to go down" << std::endl;
	std::cout << "P to pause the game" << std::endl;
	std::cout << "ESC to exit game" << std::endl;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Error: SDL failed to initialize: "<< SDL_GetError() << '\n';
		return (EXIT_FAILURE);
	}
	window = SDL_CreateWindow("Ayman's PONG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	if(!window)
	{
		std::cerr << "Error: Failed to open window: "<< SDL_GetError() << '\n';
		return (EXIT_FAILURE);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer)
	{
		std::cerr << "Error: Failed to create renderer: "<< SDL_GetError() << '\n';
		return (EXIT_FAILURE);
	}
	SDL_Surface	*icon = IMG_Load("/Users/asus/Documents/Pong_cpp/assets/pong icon.png");
	SDL_SetWindowIcon( window, icon);
	SDL_FreeSurface( icon );
	return (EXIT_SUCCESS);
}

void	Window::handleEvents( void )
{
	while (SDL_PollEvent(&event))
	{
		if ( event.type == SDL_QUIT )
			running = false;
		else if ( event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
				{
					running = false;
					return ;
				}
				case SDLK_p:
				{
					pause = true;
					return ;
				}
				default:
					break;
			}
		}
		const	Uint8	*state = SDL_GetKeyboardState( NULL );
		if ( state[SDL_SCANCODE_UP] )
			player->setPosY( player->getPosY() - player->getSpeed() );
		if ( state[SDL_SCANCODE_DOWN])
			player->setPosY( player->getPosY() + player->getSpeed() );
	}
}

void    Window::gameLoop( void )
{
	while ( play )
	{
		menu->renderMenu( renderer );
		menu->mouseEvents( running, play);
	}
	 while ( running )
	 {
		while ( pause )
		{
			pauseMenu->renderMenu( renderer );
			pauseMenu->pauseEvents( running, pause );
		}
		if ( running == false )
			break ;
		handleEvents();
		if ( running == false )
			break ;
		render();
		ball->updateBall( *player, *enemy, *score );
		enemy->updateAI( *ball );
	}
}

void	Window::render( void )
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	player->drawRacket( renderer );
	enemy->drawRacket( renderer );
	score->displayScore( renderer );
	ball->drawBall( renderer );
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	SDL_RenderPresent(renderer);
}
