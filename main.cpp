/* Surya Sharma : Project 2
** Make a short animation with moving sprites 
** Use at least 2 classes
*/

#include <SDL.h>
#include "SDL_image.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "generateFrames.h" // Used to creates frames for animation
#include <math.h>

bool makeVideo = false;

const unsigned int gameWidth = 854u; // Defined here so that it can be changed easily.
const unsigned int gameHeight = 480u;

const unsigned int DT = 17u; // 60 frames per second. 1000 ticks / 60 frames = 17.

/* Follow rule of 3 for this class
** Must have destructor, copy constructor, copy assignment operator
** What should a sprite have as methods?
** Draw and Update
** Draw will draw the sprite on the screen
** Update will update the sprite (X / Y) [and alpha if used]
*/
class Sprite
{
public:
	Sprite(const char * filename) : image(NULL), x(0.0), y(0.0), t(0.0), alpha(0.0) {
		SDL_Surface *temp = IMG_Load(filename);
		if (temp == NULL) {
			throw std::string("Unable to load bitmap.") + SDL_GetError();
		}
		Uint32 colorkey = SDL_MapRGB(temp->format, 0, 0, 0);
		SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

		image = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);

		x = y = alpha = 0.0; // This should go in the init list
	}

	~Sprite() {
		SDL_FreeSurface(image);
	}

	const SDL_Surface * getImage() const
	{
		return image;
	}

	void drawto(SDL_Surface* screen) const {
		Sint16 xCoord = static_cast<Sint16>(x);
		Sint16 yCoord = static_cast<Sint16>(y);
		
		Uint16 w = image->w;	// Need this or typecasting
		Uint16 h = image->h;

		SDL_Rect src = { 0, 0, w, h };
		SDL_Rect dest = {xCoord, yCoord, 0,0 };
		SDL_BlitSurface(image, &src, screen, &dest);
	}
	
	bool update()
	{
		static unsigned int remainder = 0u;
		static unsigned int currentTicks = 0u;
		static unsigned int prevTicks = SDL_GetTicks();
		static unsigned int entryComplete = 0;
		static bool direction = true;
		static bool whichimage = false;
		currentTicks = SDL_GetTicks();

		unsigned int elapsedTicks = currentTicks - prevTicks + remainder; // Calculates ticks since last update

		if(elapsedTicks < DT) return false; // Do not update if we're less than 17 ticks from last update

		float incr = 200 * DT * 0.001;

		if ((y > 300) && (entryComplete == 0)) {
			if (alpha < 255) { SDL_SetAlpha(image, SDL_SRCALPHA, alpha += 3); y -= incr; }
		}
		else if ( (t < 3.27475) && (entryComplete <= 1)) {
			
			entryComplete = 1;
			
			if (direction == true)	x = (427 - ((image->w) / 2)) + 300 * sin(1 * t);
			else x = (427 - ((image->w)/2)) - 300 * sin(1 * t);

			y = 300 + 300 * sin(2 * t);
			t = 0.03 + 0.991*t;
			
			if ((t > 3.15) && (alpha > 0))	SDL_SetAlpha(image, SDL_SRCALPHA, alpha -= 3);

		}
		else if (entryComplete == 2) {
			SDL_FreeSurface(image);
			SDL_Surface *temp;

			if (whichimage == false)	temp = IMG_Load("images/pizzaz.png");
			else temp = IMG_Load("images/player.png");

			whichimage = !whichimage;
			direction = !direction;

			if (temp == NULL) {
				throw std::string("Unable to load bitmap.") + SDL_GetError();
			}
			Uint32 colorkey = SDL_MapRGB(temp->format, 0, 0, 0);
			SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

			image = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);

			x = y = alpha = t = 0.0; // This should go in the init list
			setXY((854 / 2) - (image->w) / 2, 520);
			entryComplete = 0;
		}
		else if ((t >= 3.27475) && (entryComplete <= 1)) entryComplete = 2;
		

		prevTicks = currentTicks;
		remainder = elapsedTicks - DT;

		return true;

	}

	void setXY(float x, float y) { this->x = x; this->y = y; }

	float getX() const { return x; }
	float getY() const { return y; }

private:
	// explicitly disallow constructors that you don't want compiler to silently write
	Sprite();
	Sprite(const Sprite &);
	Sprite & operator=(const Sprite& rhs);

	SDL_Surface *image;
	float x, y, t, alpha;
};


/* Static Singleton Class for Game Engine (Manager)
** To define a Singleton Class
** 1) Create private constructors and assignment overload
** 2) Declare static instance variable in private section
** 3) Declare static instance variable in the anonymous namespace
** 4) Define static memeber function to access the instance variable
*/
class Manager
{
public:
	static Manager * getInstance()
	{
		if (!instance) instance = new Manager();
		return instance;
	}
	unsigned int getRandom(int highBound, int lowBound = 1) const
	{
		return (rand() % highBound) + lowBound;
	}

	void GameLoop()
	{
		/* Game Loop */
		Sprite Background("images/Background.png");
		Sprite B("images/player.png");
		B.setXY((854 / 2) - (B.getImage()->w)/2, 520);
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
		SDL_Rect dest = { 0, 0, 0, 0 };
		SDL_BlitSurface(screen, NULL, screen, &dest);

		SDL_Event event;
		GenerateFrames genFrames(screen);

		bool done = false;
		unsigned int framecount = 0;

		while (!done) {
			while (!done && SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) done = true;
				if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_ESCAPE) done = true;
				}
			}

			B.update();
			if( (framecount++ < 968) && makeVideo) genFrames.makeFrame();
					
			Background.drawto(screen);
			B.drawto(screen);
			SDL_Flip(screen);

		}
	}
private:
	/* Static Class */
	static Manager * instance; // (2)
	Manager(const Manager&); //(1)
	Manager & operator=(const Manager&);


	/* Members */
	SDL_Surface * screen; // QUESTION 1: Are all members of a singleton class static??
	int TotalFrames; // Keeps track of frames from spawn

	/* Constructor for Static Class
	** This is when the game engine (Manager loads)
	** It should do the follow
	** 1) Initialize the random number generator
	** 2) Init SDL
	** 3) Execute Game Loop
	**   a) Draw Background (sky)
	**	 b) Draw Ship at Random location (Old Idea)
	**	 c) Flip Screen
	*/
	Manager() : screen (NULL), TotalFrames (0)
	{
		try{
			/* Initialize the random number generator */
			srand((int) time(0)); // Seed the random number generator
			TotalFrames = 0; 

			/* Init SDL */
			init();

			/* Setup the Background Screen */
			screen = SDL_SetVideoMode(gameWidth, gameHeight, 0, SDL_DOUBLEBUF);
			if(screen == NULL) {
				throw std::string("Unable to SetVideoMode : ") + SDL_GetError();
			}

			GameLoop();

		}
		catch(const std::string & msg) {std::cout<< msg << std::endl; }
		catch( ... ) {std::cout<<"Unknown Error"<<std::endl;}
	}

	void init() const
	{
		if(SDL_Init(SDL_INIT_VIDEO) != 0) {
			throw(std::string("Unable to Initialize SDL : ") + SDL_GetError());
		}
		atexit(SDL_Quit);
	}

};

Manager* Manager::instance = NULL;

int main()
{
	Manager * engine = Manager::getInstance();
	engine->getRandom(100, 5);
	delete Manager::getInstance();
	
}

/* BASE CHARACTERISTICS FOR SDL
== 26591 == HEAP SUMMARY :
== 26591 == in use at exit : 57, 630 bytes in 869 blocks
== 26591 == total heap usage : 2, 414 allocs, 1, 545 frees, 1, 449, 339 bytes allocated
== 26591 ==
== 26591 == LEAK SUMMARY :
== 26591 == definitely lost : 40 bytes in 4 blocks
== 26591 == indirectly lost : 352 bytes in 8 blocks
== 26591 == possibly lost : 0 bytes in 0 blocks
== 26591 == still reachable : 57, 238 bytes in 857 blocks
== 26591 == suppressed : 0 bytes in 0 blocks
== 26591 == Rerun with --leak - check = full to see details of leaked memory
== 26591 ==
== 26591 == For counts of detected and suppressed errors, rerun with : -v
== 26591 == Use --track - origins = yes to see where uninitialised values come from
== 26591 == ERROR SUMMARY : 2 errors from 1 contexts(suppressed : 0 from 0)
*/

