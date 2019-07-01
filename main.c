#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdbool.h>
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
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
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
    gFont = TTF_OpenFont( "16_true_type_fonts/lazy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text

        SDL_Color textColor = { 0, 0, 0 };
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
        struct textureStruct gPressTexture;
        struct textureStruct gUpTexture;
        struct textureStruct gDownTexture;
        struct textureStruct gLeftTexture;
        struct textureStruct gRightTexture;
        struct textureStruct currentTexture;
        struct musicStruct gMusicStruct;
        struct soundStruct gScratch;
        struct soundStruct gHigh;
        struct soundStruct gMedium;
        struct soundStruct gLow;
        struct textureStruct gPromptTexture;

        gPromptTexture.imagePath = "21_sound_effects_and_music/prompt.png";
        gMusicStruct.musicPath = "21_sound_effects_and_music/beat.wav";
        loadMusic(&gMusicStruct);
        gScratch.musicPath = "21_sound_effects_and_music/scratch.wav";
        loadSound(&gScratch);
        gHigh.musicPath = "21_sound_effects_and_music/high.wav";
        loadSound(&gHigh);
        gMedium.musicPath = "21_sound_effects_and_music/medium.wav";
        loadSound(&gMedium);
        gLow.musicPath = "21_sound_effects_and_music/low.wav";
        loadSound(&gLow);


        gPressTexture.imagePath = "18_key_states/press.png";
        gUpTexture.imagePath = "18_key_states/up.png";
        gDownTexture.imagePath = "18_key_states/down.png";
        gLeftTexture.imagePath = "18_key_states/left.png";
        gRightTexture.imagePath = "18_key_states/right.png";

        gPromptTexture.xPos = 0;
        gPromptTexture.yPos = 0;

        gPressTexture.xPos = 0;
        gPressTexture.yPos = 0;
        gUpTexture.xPos = 0;
        gUpTexture.yPos = 0;
        gDownTexture.xPos = 0;
        gDownTexture.yPos = 0;
        gLeftTexture.xPos = 0;
        gLeftTexture.yPos = 0;
        gRightTexture.xPos = 0;
        gRightTexture.yPos = 0;

        // struct textureStruct gArrowTexture;
        // gArrowTexture.imagePath = "15_rotation_and_flipping/arrow.png";
       /* struct ttfStruct gTextTexture;
        gTextTexture.textureText = "This is my text duuude \n";

        //Mouse button sprites
        SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
        struct textureStruct gButtonSpriteSheetTexture;
        gButtonSpriteSheetTexture.imagePath = "17_mouse_events/button.png";

        //Buttons objects
        struct buttonStruct gButtons1;
        struct buttonStruct gButtons2;
        struct buttonStruct gButtons3;
        struct buttonStruct gButtons4;

        gButtons1.xPos = 0;
        gButtons1.yPos = 0;

        gButtons2.xPos = SCREEN_WIDTH - BUTTON_WIDTH;
        gButtons2.yPos = 0;

        gButtons3.xPos = 0;
        gButtons3.yPos = SCREEN_HEIGHT - BUTTON_HEIGHT;

        gButtons4.xPos = SCREEN_WIDTH - BUTTON_WIDTH;
        gButtons4.yPos = SCREEN_HEIGHT - BUTTON_HEIGHT;

        for(int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
        {
            gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i * 200;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
        }
        */
        /*
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w =  64;
        gSpriteClips[ 0 ].h = 205;

        gSpriteClips[ 1 ].x =  64;
        gSpriteClips[ 1 ].y =   0;
        gSpriteClips[ 1 ].w =  64;
        gSpriteClips[ 1 ].h = 205;

        gSpriteClips[ 2 ].x = 128;
        gSpriteClips[ 2 ].y =   0;
        gSpriteClips[ 2 ].w =  64;
        gSpriteClips[ 2 ].h = 205;

        gSpriteClips[ 3 ].x = 196;
        gSpriteClips[ 3 ].y =   0;
        gSpriteClips[ 3 ].w =  64;
        gSpriteClips[ 3 ].h = 205;
        */

        if( !LTexture(&gPressTexture) )
        {
            printf( "Failed to load media! \n" );
        }

        if( !LTexture(&gUpTexture) )
        {
            printf( "Failed to load media! \n" );
        }
        if( !LTexture(&gDownTexture) )
        {
            printf( "Failed to load media! \n" );
        }
        if( !LTexture(&gLeftTexture) )
        {
            printf( "Failed to load media! \n" );
        }
        if( !LTexture(&gRightTexture) )
        {
            printf( "Failed to load media! \n" );
        }
        if( !LTexture(&gPromptTexture) )
        {
            printf( "Failed to load media! \n" );
        }

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
                else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            //Play high sound effect
                            case SDLK_1:
                            Mix_PlayChannel( -1, gHigh.gSound, 0 );
                            break;

                            //Play medium sound effect
                            case SDLK_2:
                            Mix_PlayChannel( -1, gMedium.gSound, 0 );
                            break;

                            //Play low sound effect
                            case SDLK_3:
                            Mix_PlayChannel( -1, gLow.gSound, 0 );
                            break;

                            //Play scratch sound effect
                            case SDLK_4:
                            Mix_PlayChannel( -1, gScratch.gSound, 0 );
                            break;
                             case SDLK_9:
                            //If there is no music playing
                            if( Mix_PlayingMusic() == 0 )
                            {
                                //Play the music
                                Mix_PlayMusic( gMusicStruct.gMusic, -1 );
                            }
                            //If music is being played
                            else
                            {
                                //If the music is paused
                                if( Mix_PausedMusic() == 1 )
                                {
                                    //Resume the music
                                    Mix_ResumeMusic();
                                }
                                //If the music is playing
                                else
                                {
                                    //Pause the music
                                    Mix_PauseMusic();
                                }
                            }
                            break;

                            case SDLK_0:
                            //Stop the music
                            Mix_HaltMusic();
                            break;
                        }
                    }

                /*
                handleEvent(&gButtons1, &e);
                handleEvent(&gButtons2, &e);
                handleEvent(&gButtons3, &e);
                handleEvent(&gButtons4, &e);
                */

                /*
                else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_a:
                            degrees -= 60;
                            break;

                            case SDLK_d:
                            degrees += 60;
                            break;

                            case SDLK_q:
                            flipType = SDL_FLIP_HORIZONTAL;
                            break;

                            case SDLK_w:
                            flipType = SDL_FLIP_NONE;
                            break;

                            case SDLK_e:
                            flipType = SDL_FLIP_VERTICAL;
                            break;
                        }
                    }
                    */

            }

                //Set texture based on current keystate
                const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
                if( currentKeyStates[ SDL_SCANCODE_UP ] )
                {
                    currentTexture = gUpTexture;
                }
                else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
                {
                    currentTexture = gDownTexture;
                }
                else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
                {
                    currentTexture = gLeftTexture;
                }
                else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
                {
                    currentTexture = gRightTexture;
                }
                else
                {
                    currentTexture = gPressTexture;
                }


                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                textureRender(&gPromptTexture, NULL, degrees, NULL, flipType);
                //loadMusic("21_sound_effects_and_music/beat.wav");


                /*
                //Render buttons
             //   gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
                gButtonSpriteSheetTexture.xPos = 0;
                gButtonSpriteSheetTexture.yPos = 0 * 200;
                setPosition(&gButtons1, 0, 0);
                //textureRender(&gButtonSpriteSheetTexture, NULL, degrees, NULL, flipType);
                //textureRender(&gButtons1, NULL, degrees, NULL, flipType);

                gButtonSpriteSheetTexture.xPos = gButtons1.xPos;
                gButtonSpriteSheetTexture.yPos = gButtons1.yPos;
                textureRender(&gButtonSpriteSheetTexture, &gSpriteClips[ gButtons1.mCurrentSprite ], 0, NULL, flipType);

                gButtonSpriteSheetTexture.xPos = gButtons2.xPos;
                gButtonSpriteSheetTexture.yPos = gButtons2.yPos;
                textureRender(&gButtonSpriteSheetTexture, &gSpriteClips[ gButtons2.mCurrentSprite ], 0, NULL, flipType);

                gButtonSpriteSheetTexture.xPos = gButtons3.xPos;
                gButtonSpriteSheetTexture.yPos = gButtons3.yPos;
                textureRender(&gButtonSpriteSheetTexture, &gSpriteClips[ gButtons3.mCurrentSprite ], 0, NULL, flipType);

                gButtonSpriteSheetTexture.xPos = gButtons4.xPos;
                gButtonSpriteSheetTexture.yPos = gButtons4.yPos;
                textureRender(&gButtonSpriteSheetTexture, &gSpriteClips[ gButtons4.mCurrentSprite ], 0, NULL, flipType);


                //struct textureStruct *structinput, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip
                //gButtonSpriteSheetTexture.render( gButtons1.xPos, Buttons1.yPos, &gSpriteClips[ mCurrentSprite ] );
                //gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
				//	gButtons[ i ].render();
                */


                //Render current frame
                /*
                gTextTexture.xPos = ( SCREEN_WIDTH - gTextTexture.mWidth ) / 2;
                gTextTexture.yPos = ( SCREEN_HEIGHT - gTextTexture.mHeight ) / 2;
                textureRender(&gTextTexture, NULL, degrees, NULL, flipType);

                SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
                gSpriteSheetTexture.xPos = (SCREEN_WIDTH - currentClip->w ) / 2;
                gSpriteSheetTexture.yPos = ( SCREEN_HEIGHT - currentClip->h ) / 2;
                */


           //     textureRender(&gArrowTexture, NULL, degrees, NULL, flipType);

                //Update screen

                SDL_RenderPresent( gRenderer );

                //Go to next frame
                /*
                ++frame;

                //Cycle animation
                if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                {
                    frame = 0;
                }
                */

        }
        SDL_DestroyTexture( gPressTexture.mTexture );
        SDL_DestroyTexture( gUpTexture.mTexture );
        SDL_DestroyTexture( gDownTexture.mTexture );
        SDL_DestroyTexture( gLeftTexture.mTexture );
        SDL_DestroyTexture( gRightTexture.mTexture );
        Mix_FreeChunk( gScratch.gSound );
        Mix_FreeChunk( gHigh.gSound );
        Mix_FreeChunk( gMedium.gSound );
        Mix_FreeChunk( gLow.gSound );
        Mix_FreeMusic( gMusicStruct.gMusic );
        gScratch.gSound = NULL;
        gHigh.gSound = NULL;
        gMedium.gSound = NULL;
        gLow.gSound = NULL;
        gMusicStruct.gMusic = NULL;


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

