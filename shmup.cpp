///
//Shmup Game with SDL
//13.11.2013
///

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int FRAMES_PER_SECOND = 30;

const int SHIP_WIDTH = 32;
const int SHIP_HEIGHT = 32;

SDL_Surface *ship = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;

SDL_Event event;

//Test wall
SDL_Rect wall;

//The player
class Ship
{
	private:
	SDL_Rect box;
	int xVel, yVel;

	public:
	Ship();
	void handle_input();
	void move();
	void show();
};

//Collision
bool check_collision( SDL_Rect A, SDL_Rect B )
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;

	if( bottomA <= topB )
	{
		return false;
	}

	if( topA >= bottomB )
	{
		return false;
	}

	if( rightA <= leftB )
	{
		return false;
	}

	if( leftA >= rightB )
	{
		return false;
	}

	return true;
}

//Timer for ingame calc
class Timer
{
	private:
	int startTicks;
	int pausedTicks;
	bool paused;
	bool started;

	public:
	Timer();
	void start();
	void stop();
	void pause();
	void unpause();
	int get_ticks();
	bool is_started();
	bool is_paused();
};

//Image load
SDL_Surface *load_image( std::string filename )
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = IMG_Load( filename.c_str() );
	if( loadedImage != NULL )
	{
		optimizedImage = SDL_DisplayFormat( loadedImage );
		SDL_FreeSurface( loadedImage );
		if( optimizedImage != NULL )
		{
			Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 100, 100, 100 );
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
		}
	}
	return optimizedImage;
}

//Blit image
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface( source, clip, destination, &offset );
}

//Init
bool init()
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;
	}

	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	if( screen == NULL )
	{
		return false;
	}

	SDL_WM_SetCaption( "Shmup", NULL );
	return true;
}

bool load_files()
{
	ship = load_image( "shmupShip.png" );
	if( ship == NULL )
	{
		return false;
	}

	background = load_image( "background.png" );
	if( background == NULL )
	{
		return false;
	}
	return true;
}	

void clean_up()
{
	SDL_FreeSurface( ship );
	SDL_FreeSurface( background );
	SDL_Quit();
}

Ship::Ship()
{
	box.x = 0;
	box.y = 0;
	box.w = SHIP_WIDTH;
	box.h = SHIP_HEIGHT;
	xVel = 0;
	yVel = 0;
}

void Ship::handle_input()
{
	if( event.type == SDL_KEYDOWN )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_UP: yVel -= SHIP_HEIGHT / 2; break;
			case SDLK_DOWN: yVel += SHIP_HEIGHT / 2; break;
			case SDLK_LEFT: xVel -= SHIP_WIDTH / 2; break;
			case SDLK_RIGHT: xVel += SHIP_WIDTH / 2; break;
		}
	}
	else if( event.type == SDL_KEYUP )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_UP: yVel += SHIP_HEIGHT / 2; break;
			case SDLK_DOWN: yVel -= SHIP_HEIGHT / 2; break;
			case SDLK_LEFT: xVel += SHIP_WIDTH / 2; break;
			case SDLK_RIGHT: xVel -= SHIP_WIDTH / 2; break;
		}
	}
}

void Ship::move()
{
	box.x += xVel;
	if( ( box.x < 0 ) || ( box.x + SHIP_WIDTH > SCREEN_WIDTH ) || ( check_collision( box, wall ) ) )
	{
		box.x -= xVel;
	}

	box.y += yVel;
	if( ( box.y < 0 ) || ( box.y + SHIP_HEIGHT > SCREEN_HEIGHT ) || ( check_collision( box, wall ) ) )
	{
		box.y -= yVel;
	}
}

void Ship::show()
{
	apply_surface( box.x, box.y, ship, screen );
}

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

void Timer::start()
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}

void Timer::stop()
{
	started = false;
	paused = false;
}

void Timer::pause()
{
	if( ( started == true ) && ( paused == false ) )
	{
		paused = true;
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause()
{
	if( paused == true )
	{
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

int Timer::get_ticks()
{
	if( started == true )
	{
		if( paused == true )
		{
			return pausedTicks;
		}
		else
		{
			return SDL_GetTicks() -startTicks;
		}
	}
	return 0;
}

bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
}

int main( int argc, char* args[] )
{
	bool quit = false;
	Ship myShip;
	Timer fps;

	if( init() == false )
	{
		return 1;
	}

	if( load_files() == false )
	{
		return 1;
	}

	//Test wall
	wall.x = 300;
	wall.y = 40;
	wall.w = 40;
	wall.h = 400;

	while( quit == false )
	{
		fps.start();
		//While theres events
		while( SDL_PollEvent( &event ) )
		{
			myShip.handle_input();
			if( event.type == SDL_QUIT )
			{
				quit = true;
			}
		}
		
		myShip.move();
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
		apply_surface(0, 0, background, screen );
		
		//Test wall
		SDL_FillRect( screen, &wall, SDL_MapRGB( screen->format, 0x77, 0x77, 0x77 ) );

		myShip.show();

		if( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}

		if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
		{
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
		}
	}
	clean_up();
	return 0;
}


















































































