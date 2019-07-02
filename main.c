#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include "Game1.h"


//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

//Loads individual image as texture
SDL_Texture* loadTexture( char *path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Loads individual image
SDL_Surface* loadSurface( char *path );

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

//Current displayed image
//SDL_Surface* gCurrentSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gImage = NULL;

char imagePathDef[24] = "Hello.bmp";
char imagePathUp[24] = "Up.bmp";
char imagePathDown[24] = "Down.bmp";
char imagePathLeft[24] = "Left.bmp";
char imagePathRight[24] = "Right.bmp";
char imagePathViewport[64] = "09_the_viewport/viewport.png";

SDL_Texture* newTexture = NULL;


bool loadMediaGeometry();

struct timerStruct
{
    Uint32 mStartTicks;
    Uint32 mPausedTicks;
    bool mPaused;
    bool mStarted;
};

struct textureStruct
{
    char *imagePath;
    int mWidth;
    int mHeight;
    int xPos;
    int yPos;
    SDL_Texture* mTexture;
};

struct ttfStruct
{
    char *imagePath;
    int mWidth;
    int mHeight;
    int xPos;
    int yPos;
    SDL_Texture* mTexture;
    char *textureText;
    SDL_Color textColor;
};

struct buttonStruct
{
    char *imagePath;
    int mWidth;
    int mHeight;
    int xPos;
    int yPos;
    SDL_Texture* mTexture;
    char *textureText;
    int mCurrentSprite;
};

struct musicStruct
{
    Mix_Music *gMusic;
    char *musicPath;
};

struct soundStruct
{
    Mix_Chunk *gSound;
    char *musicPath;
};

void setPosition(struct buttonStruct *structInput, int x, int y)
{
    structInput->xPos = x;
    structInput->yPos = y;
}

bool LTexture(struct textureStruct *inputStruct);

bool loadFromFile(char *path);
void free();
void render(int x, int y);


bool loadFromRenderedText(struct ttfStruct *structinput, SDL_Color textColor );

//Globally used font
TTF_Font *gFont = NULL;

SDL_Texture* mTexture;

//Image dimensions
int globalWidth;
int globalHeight;

//Scene sprites
//SDL_Rect gSpriteClips[ 4 ];
struct textureStruct gSpriteSheetTexture;

//Set blending
void setBlendMode( SDL_BlendMode blending );

//Set alpha modulation
void setAlpha( Uint8 alpha );

#define WALKING_ANIMATION_FRAMES 4

SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else
            {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }
    }
        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    return success;
}

bool loadMusic(struct musicStruct *inputStruct)
{
    bool success = true;
    inputStruct->gMusic = Mix_LoadMUS( inputStruct->musicPath );
    if( inputStruct->gMusic == NULL )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    return success;
}

bool loadSound(struct soundStruct *inputStruct)
{
    bool success = true;
    inputStruct->gSound = Mix_LoadWAV( inputStruct->musicPath );
    if( inputStruct->gSound == NULL )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    return success;
}

bool initRenderer()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                else
                {
                    //Get window surface
                    gScreenSurface = SDL_GetWindowSurface( gWindow );
                }
            }
        }
    }
                     //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    return success;
}

bool initVsyncRenderer()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                else
                {
                    //Get window surface
                    gScreenSurface = SDL_GetWindowSurface( gWindow );
                }
            }
        }
    }
    return success;
}

SDL_Surface* loadSurface( char *path )
{
    //The final optimized image
	SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path, IMG_GetError() );
    }
    else
    {
       //Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}

SDL_Texture* loadTexture( char *path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

bool loadMedia(struct ttfStruct *structinput)
{
    //Loading success flag
    bool success = true;

    //Open the font
    gFont = TTF_OpenFont( structinput->imagePath, 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text

        SDL_Color textColor = { 0, 0, 0, 255 };
        if( !loadFromRenderedText(structinput, textColor) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }
    return success;
}

bool loadMediaTexture()
{
    //Loading success flag
    bool success = true;

    //Load PNG texture
    gTexture = loadTexture( imagePathViewport );
    if( gTexture == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = false;
    }

    return success;
}

bool loadMediaGeometry()
{
    //Loading success flag
    bool success = true;

    //Nothing to load
    return success;
}

void close()
{
    //Deallocate surface
    SDL_FreeSurface( gImage );
    gImage= NULL;

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Free the music

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
}

void closeTexture()
{
    //Free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool LTexture(struct textureStruct *structinput)
{

    mTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( structinput->imagePath );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", structinput->imagePath, IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", structinput->imagePath, SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            structinput->mWidth = loadedSurface->w;
            structinput->mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    structinput->mTexture = newTexture;
    return newTexture != NULL;
}

void textureRenderttf(struct ttfStruct *structinput, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { structinput->xPos, structinput->yPos, structinput->mWidth, structinput->mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    //SDL_RenderCopy( gRenderer, structinput->mTexture, clip, &renderQuad );
    SDL_RenderCopyEx( gRenderer, structinput->mTexture, clip, &renderQuad, angle, center, flip );
}

void textureRender(struct textureStruct *structinput, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { structinput->xPos, structinput->yPos, structinput->mWidth, structinput->mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    //Render to screen
    //SDL_RenderCopy( gRenderer, structinput->mTexture, clip, &renderQuad );
    SDL_RenderCopyEx( gRenderer, structinput->mTexture, clip, &renderQuad, angle, center, flip );
}

bool loadFromRenderedText(struct ttfStruct *structinput, SDL_Color textColor )
{
    //Get rid of preexisting texture
    //closeTexture();
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, structinput->textureText, textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            structinput->mWidth = textSurface->w;
            structinput->mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    structinput->mTexture = mTexture;
    return mTexture != NULL;
}

void handleEvent(struct buttonStruct *inputStruct, SDL_Event* e)
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < inputStruct->xPos )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > inputStruct->xPos + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < inputStruct->yPos )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > inputStruct->yPos + BUTTON_HEIGHT )
        {
            inside = false;
        }
                //Mouse is outside button
        if( !inside )
        {
            inputStruct->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                inputStruct->mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                inputStruct->mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                inputStruct->mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

void timerInit(struct timerStruct *inputStruct)
{
    inputStruct->mStarted = false;
    inputStruct->mPaused = false;
    inputStruct->mStartTicks = 0;
    inputStruct->mPausedTicks = 0;
}

void timerStart(struct timerStruct *inputStruct)
{
    inputStruct->mStarted = true;
    inputStruct->mPaused = false;
    inputStruct->mStartTicks = SDL_GetTicks();
    inputStruct->mPausedTicks = 0;
}

void timerStop(struct timerStruct *inputStruct)
{
    inputStruct->mStarted = false;
    inputStruct->mPaused = false;
    inputStruct->mStartTicks = 0;
    inputStruct->mPausedTicks = 0;
}

void timerPause(struct timerStruct *inputStruct)
{
    if( inputStruct->mStarted && !inputStruct->mPaused )
    {
        inputStruct->mPaused = true;
        inputStruct->mPausedTicks = SDL_GetTicks() - inputStruct->mStartTicks;
        inputStruct->mStartTicks = 0;
    }
}

void timerUnpause(struct timerStruct *inputStruct)
{
    if( inputStruct->mStarted && inputStruct->mPaused )
    {
        inputStruct->mPaused = false;
        inputStruct->mPausedTicks = SDL_GetTicks() - inputStruct->mPausedTicks;
        inputStruct->mPausedTicks = 0;
    }
}

Uint32 getTicks(struct timerStruct *inputStruct)
{
    Uint32 time = 0;
    if( inputStruct->mStarted )
    {
        if( inputStruct->mPaused)
        {
            time = inputStruct->mPausedTicks;
        }
        else
            {
                time = SDL_GetTicks() - inputStruct->mStartTicks;
            }
    }
    return time;
}





int main( int argc, char* args[] )
{
    //Main loop flag
    bool quit = false;

    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;

    //Event handler
    SDL_Event e;

    //Modulation component
    Uint8 a = 255;

    int frame = 0;

    //Set text color as black
    SDL_Color textColor = { 0, 0, 0, 255 };

    //Current time start time
    Uint32 startTime = 0;

    //Angle of rotation
    double degrees = 0;

    //Flip type
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    //Start up SDL and create window
    if( !initRenderer() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        struct ttfStruct gStartPromptTexture;
        struct ttfStruct gTimeTexture;
        struct ttfStruct gPausePromptTexture;
        struct timerStruct gTimer;
        struct timerStruct fpsTimer;
        struct ttfStruct gfpsTexture;

        int countedFrames = 0;


        gTimer.mStartTicks = 0;
        gTimer.mPausedTicks = 0;
        gTimer.mPaused = false;
        gTimer.mStarted = false;


        gStartPromptTexture.imagePath = "22_timing/lazy.ttf";
        gStartPromptTexture.xPos = 0;
        gStartPromptTexture.yPos = 0;
        gStartPromptTexture.textureText = "Press S to Start or Stop the Timer";

        gfpsTexture.imagePath = "22_timing/lazy.ttf";
        gfpsTexture.xPos = 0;
        gfpsTexture.yPos = 0;
        gfpsTexture.textureText = "Whatup";

        gPausePromptTexture.imagePath = "22_timing/lazy.ttf";
        gPausePromptTexture.xPos = 0;
        gPausePromptTexture.yPos = 0;
        gPausePromptTexture.textureText = "Press P to Pause or Unpause the Timer";

        gTimeTexture.imagePath = "22_timing/lazy.ttf";
        gTimeTexture.textureText = "LMAO";
        if( !loadMedia(&gfpsTexture) )
        {
            printf( "Failed to load media! \n" );
        }
        if( !loadMedia(&gTimeTexture) )
        {
            printf( "Failed to load media! \n" );
        }
        timerInit(&fpsTimer);
        timerStart(&fpsTimer);
/*
        gArrowTexture.xPos = ( SCREEN_WIDTH - gArrowTexture.mWidth ) / 2;
        gArrowTexture.yPos = ( SCREEN_HEIGHT - gArrowTexture.mHeight ) / 2;
*/

        //While application is running
        while( !quit )
        {

            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                //Reset start time on return keypress
            }
                float avgFPS = countedFrames / ( getTicks(&fpsTimer) / 1000.f );
                if( avgFPS > 2000000 )
                {
                    avgFPS = 0;
                }

                //Set text to be rendered
                char timeText[100] = "";
                char timeBuffer[100] = "";

                // printf("LMffao \n");


                strcpy(timeText, "FPS: ");
                // printf(timeText);
                //int convertedTime = ( getTicks(&gTimer) / 1000.0);
                sprintf(timeBuffer, "%.0f", avgFPS  );
                strcat(timeText, timeBuffer);
                //printf(timeText);


                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                gfpsTexture.xPos = (SCREEN_WIDTH - 1.5*gfpsTexture.mWidth ) ;
                gfpsTexture.yPos = ( gfpsTexture.mHeight) ;
                gfpsTexture.textureText = timeText;

                if( !loadMedia(&gfpsTexture) )
                {
                    printf( "Failed to load media! \n" );
                }
                textureRenderttf(&gfpsTexture, NULL, degrees, NULL, flipType);

           //     textureRender(&gPromptTexture, NULL, degrees, NULL, flipType);


                SDL_RenderPresent( gRenderer );
                ++countedFrames;


        }


   }

        //Free resources and close SDL
        closeTexture();
        close();
        return 0;
}




































/* Old


//Screen dimension constants
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

//Main loop flag
bool quit = false;

//Event handler
SDL_Event e;

void createPic();

//Picture to be placed in the window
SDL_Surface* screenPicture = NULL;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

int main( int argc, char* args[] )
{


    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );

            //Fill the surface white
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

            SDL_BlitSurface( screenPicture, NULL, screenSurface, NULL );
            //Update the surface
            SDL_UpdateWindowSurface( window );

                     //While application is running
            while( !quit )
            {
                     //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }
           }
         }

     //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();
}

    return 0;
}

void createPic()
{
    //Load splash image
    screenPicture = SDL_LoadBMP( "Hello.bmp" );

    if( screenPicture == NULL )
    {
    printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
    }
}
*/

