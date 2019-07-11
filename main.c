#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include "Game1.h"

// int n declaration of variable
// int *p declaration of pointer called p
// * gives data
// & gives address
// p = &n makes the pointer p point to the address of n
// *p gives now the value of that which p points to, i.e. the value of n
// int *p, int* p, int * p should be equivalent
// void myFunction(int *n) {}
// int r = 5;
// myFunction(&r)

// This means, send the address of r to the function. The function uses the value of this address


/*
    Do like this, fool

    int *ptrMyVar;
    int myValue = 5;
    ptrMyVar = &myValue;

    void myFunc(int *input)
{
    int output = *input; // input still is an address here
    printf("Myfunc output: %d \n", output);
}

    myFunc(ptrMyVar);

    Also:

    struct myStruct
{
    char *myChar;
};

void myFunction(struct myStruct *inputStruct);
void myFunction2(struct myStruct *inputStruct);

int main()
{
    struct myStruct myStruct1;
    myStruct1.myChar = "Holy sheeet";
    struct myStruct *ptrMyStruct1;
    ptrMyStruct1 = &myStruct1;

    //printf("%s \n", myStruct1.myChar);
    //printf("%s \n", ptrMyStruct1->myChar);

    void myFunction3(char *inputChar)
    {
        printf("%s \n", inputChar);
    }

    void myFunction2(struct myStruct *inputStruct)
    {
        printf("%s \n", inputStruct->myChar);
    }

    void myFunction(struct myStruct *inputStruct)
    {
        struct myStruct *ptrMyStruct2;
        ptrMyStruct2 = inputStruct;
        myFunction2(ptrMyStruct2);
        myFunction2(inputStruct);
        myFunction3(inputStruct->myChar);
        myFunction3(ptrMyStruct2->myChar);
    }

    // all 4 above give same output



    myFunction(ptrMyStruct1);


*/



//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / 60;



#define TOTAL_DATA 10
Sint32 gData[ TOTAL_DATA ];

bool SDL_RWopsWrapper(char *path)
{
    bool success = false;
    SDL_RWops* file = SDL_RWFromFile( path , "r+b" );
    if( file == NULL )
    {
    printf( "Warning: Unable to open file! SDL Error: %s\n", SDL_GetError() );
    //Create file for writing
    file = SDL_RWFromFile( path , "w+b" );
    if( file != NULL )
        {
            printf( "New file created!\n" );

            //Initialize data
            for( int i = 0; i < TOTAL_DATA; ++i )
            {
                gData[ i ] = 0;
                SDL_RWwrite( file, &gData[ i ], sizeof(Sint32), 1 );
            }

            //Close file handler
            SDL_RWclose( file );
        }
        else
        {
            printf( "Error: Unable to create file! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
    }        //File exists
else
    {
    //Load data
    printf( "Reading file...!\n" );
    for( int i = 0; i < TOTAL_DATA; ++i )
    {
        SDL_RWread( file, &gData[ i ], sizeof(Sint32), 1 );
    }

    //Close file handler
    SDL_RWclose( file );
    }
    return success;
}



SDL_Color textColor = { 0, 0, 0, 0xFF };


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

bool checkCollision( SDL_Rect a, SDL_Rect b );
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

struct dotStruct
{
    int DOT_WIDTH;
    int DOT_HEIGHT;
    int DOT_VEL;
    int mPosX;
    int mPosY;
    int mVelX;
    int mVelY;
    char *imagePath;
    SDL_Texture* mTexture;
    SDL_Color textColor;
    SDL_Rect mCollider;
};

struct Circle
{
    int x, y, r;
    char *imagePath;
    SDL_Texture* mTexture;
    SDL_Color textColor;
    SDL_Rect mCollider[11];
};

struct dotStructArray
{
    int DOT_WIDTH;
    int DOT_HEIGHT;
    int DOT_VEL;
    int mPosX;
    int mPosY;
    int mVelX;
    int mVelY;
    char *imagePath;
    SDL_Texture* mTexture;
    SDL_Color textColor;
    SDL_Rect mCollider[11];
    struct Circle mColliders;
};


bool checkCollisionArray( SDL_Rect *a, SDL_Rect *b );

void shiftColliders(struct dotStructArray *inputStruct);

double distanceSquared( int x1, int y1, int x2, int y2 )
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}


void initDot(struct dotStruct *inputStruct)
{
    inputStruct->DOT_WIDTH = 20;
    inputStruct->DOT_HEIGHT = 20;
    inputStruct->DOT_VEL = 1;
    inputStruct->mPosX = 0;
    inputStruct->mPosY = 0;
    inputStruct->mVelX = 0;
    inputStruct->mVelY = 0;
    inputStruct->mCollider.w = inputStruct->DOT_WIDTH;
    inputStruct->mCollider.h = inputStruct->DOT_HEIGHT;
}

void initDotArray(struct dotStructArray *inputStruct, int posX, int posY)
{
    inputStruct->DOT_WIDTH = 20;
    inputStruct->DOT_HEIGHT = 20;
    inputStruct->DOT_VEL = 10;
    inputStruct->mPosX = posX;
    inputStruct->mPosY = posY;
    inputStruct->mVelX = 0;
    inputStruct->mVelY = 0;

    inputStruct->mCollider[ 0 ].w = 6;
    inputStruct->mCollider[ 0 ].h = 1;

    inputStruct->mCollider[ 1 ].w = 10;
    inputStruct->mCollider[ 1 ].h = 1;

    inputStruct->mCollider[ 2 ].w = 14;
    inputStruct->mCollider[ 2 ].h = 1;

    inputStruct->mCollider[ 3 ].w = 16;
    inputStruct->mCollider[ 3 ].h = 2;

    inputStruct->mCollider[ 4 ].w = 18;
    inputStruct->mCollider[ 4 ].h = 2;

    inputStruct->mCollider[ 5 ].w = 20;
    inputStruct->mCollider[ 5 ].h = 6;

    inputStruct->mCollider[ 6 ].w = 18;
    inputStruct->mCollider[ 6 ].h = 2;

    inputStruct->mCollider[ 7 ].w = 16;
    inputStruct->mCollider[ 7 ].h = 2;

    inputStruct->mCollider[ 8 ].w = 14;
    inputStruct->mCollider[ 8 ].h = 1;

    inputStruct->mCollider[ 9 ].w = 10;
    inputStruct->mCollider[ 9 ].h = 1;

    inputStruct->mCollider[ 10 ].w = 6;
    inputStruct->mCollider[ 10 ].h = 1;

    inputStruct->mColliders.r = inputStruct->DOT_WIDTH / 2;
    shiftColliders(inputStruct);
}




void handleDotEvent(struct dotStruct *inputStruct, SDL_Event *e)
{
//If a key was pressed
    if( e->type == SDL_KEYDOWN && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_UP: inputStruct->mVelY -= inputStruct->DOT_VEL; break;
            case SDLK_DOWN: inputStruct->mVelY += inputStruct->DOT_VEL; break;
            case SDLK_LEFT: inputStruct->mVelX -= inputStruct->DOT_VEL; break;
            case SDLK_RIGHT: inputStruct->mVelX += inputStruct->DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e->type == SDL_KEYUP && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_UP: inputStruct->mVelY += inputStruct->DOT_VEL; break;
            case SDLK_DOWN: inputStruct->mVelY -= inputStruct->DOT_VEL; break;
            case SDLK_LEFT: inputStruct->mVelX += inputStruct->DOT_VEL; break;
            case SDLK_RIGHT: inputStruct->mVelX -= inputStruct->DOT_VEL; break;
        }
    }
}

void handleDotEventArray(struct dotStructArray *inputStruct, SDL_Event *e)
{
//If a key was pressed
    if( e->type == SDL_KEYDOWN && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_UP: inputStruct->mVelY -= inputStruct->DOT_VEL; break;
            case SDLK_DOWN: inputStruct->mVelY += inputStruct->DOT_VEL; break;
            case SDLK_LEFT: inputStruct->mVelX -= inputStruct->DOT_VEL; break;
            case SDLK_RIGHT: inputStruct->mVelX += inputStruct->DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e->type == SDL_KEYUP && e->key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e->key.keysym.sym )
        {
            case SDLK_UP: inputStruct->mVelY += inputStruct->DOT_VEL; break;
            case SDLK_DOWN: inputStruct->mVelY -= inputStruct->DOT_VEL; break;
            case SDLK_LEFT: inputStruct->mVelX += inputStruct->DOT_VEL; break;
            case SDLK_RIGHT: inputStruct->mVelX -= inputStruct->DOT_VEL; break;
        }
    }
}

void moveDot(struct dotStruct *inputStruct, SDL_Rect *wall)
{
    inputStruct->mPosX += inputStruct->mVelX;
    inputStruct->mCollider.x = inputStruct->mPosX;
    //If the dot went too far to the left or right
    if( ( inputStruct->mPosX < 0 ) || ( inputStruct->mPosX + inputStruct->DOT_WIDTH > SCREEN_WIDTH ) || checkCollision(inputStruct->mCollider, *wall) )
    {
        //Move back
        inputStruct->mPosX -= inputStruct->mVelX;
        inputStruct->mCollider.x = inputStruct->mPosX;
    }
        //Move the dot up or down
    inputStruct->mPosY += inputStruct->mVelY;
    inputStruct->mCollider.y = inputStruct->mPosY;

    //If the dot went too far up or down
    if( ( inputStruct->mPosY < 0 ) || ( inputStruct->mPosY + inputStruct->DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision(inputStruct->mCollider, *wall) )
    {
        //Move back
        inputStruct->mPosY -= inputStruct->mVelY;
        inputStruct->mCollider.y = inputStruct->mPosY;
    }
}

void moveDotArray(struct dotStructArray *inputStruct, SDL_Rect *wall)
{
    inputStruct->mPosX += inputStruct->mVelX;

    shiftColliders(inputStruct);

    //inputStruct->mCollider.x = inputStruct->mPosX;
    //If the dot went too far to the left or right
    if( ( inputStruct->mPosX < 0 ) || ( inputStruct->mPosX + inputStruct->DOT_WIDTH > SCREEN_WIDTH ) || checkCollisionArray(inputStruct->mCollider, wall) )
    {
        //Move back
        inputStruct->mPosX -= inputStruct->mVelX;
      //  inputStruct->mCollider.x = inputStruct->mPosX;
        shiftColliders(inputStruct);
    }
        //Move the dot up or down
    inputStruct->mPosY += inputStruct->mVelY;
    //inputStruct->mCollider.y = inputStruct->mPosY;
    shiftColliders(inputStruct);

    //If the dot went too far up or down
    if( ( inputStruct->mPosY < 0 ) || ( inputStruct->mPosY + inputStruct->DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollisionArray(inputStruct->mCollider, wall) )
    {
        inputStruct->mPosY -= inputStruct->mVelY;
        //inputStruct->mCollider.y = inputStruct->mPosY;
        shiftColliders(inputStruct);
    }

}
bool checkCircleCircleCollision( struct Circle *a, struct Circle *b )
{
    //Calculate total radius squared
    int totalRadiusSquared = a->r + b->r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a->x, a->y, b->x, b->y ) < ( totalRadiusSquared ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

bool checkCollisionCircleRect( struct Circle *a, SDL_Rect *b )
{
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( a->x < b->x )
    {
        cX = b->x;
    }
    else if( a->x > b->x + b->w )
    {
        cX = b->x + b->w;
    }
    else
    {
        cX = a->x;
    }
       //Find closest y offset
    if( a->y < b->y )
    {
        cY = b->y;
    }
    else if( a->y > b->y + b->h )
    {
        cY = b->y + b->h;
    }
    else
    {
        cY = a->y;
    }

    //If the closest point is inside the circle
    if( distanceSquared( a->x, a->y, cX, cY ) < a->r * a->r )
    {
        //This box and the circle have collided
        return true;
    }

    //If the shapes have not collided
    return false;
}

void moveDotArraySquareCircle(struct dotStructArray *inputStruct, struct Circle *circle, SDL_Rect *wall)
{
    inputStruct->mPosX += inputStruct->mVelX;

    shiftColliders(inputStruct);
    //inputStruct->mCollider.x = inputStruct->mPosX;
    //If the dot went too far to the left or right
    if( ( inputStruct->mPosX - inputStruct->mColliders.r < 0 ) || ( inputStruct->mPosX + inputStruct->mColliders.r > SCREEN_WIDTH ) || checkCollisionCircleRect(&inputStruct->mColliders, wall) || checkCircleCircleCollision(&inputStruct->mColliders, circle) )
    {
        //Move back
        inputStruct->mPosX -= inputStruct->mVelX;
        shiftColliders(inputStruct);
    }
        //Move the dot up or down
    inputStruct->mPosY += inputStruct->mVelY;

    //inputStruct->mCollider.y = inputStruct->mPosY;
    shiftColliders(inputStruct);

    //If the dot went too far up or down
    if( ( inputStruct->mPosY - inputStruct->mColliders.r < 0 ) || ( inputStruct->mPosY + inputStruct->mColliders.r > SCREEN_HEIGHT ) || checkCollisionCircleRect(&inputStruct->mColliders, wall) || checkCircleCircleCollision(&inputStruct->mColliders, circle)  )
    {
        inputStruct->mPosY -= inputStruct->mVelY;
        //inputStruct->mCollider.y = inputStruct->mPosY;
        shiftColliders(inputStruct);
    }

}

void shiftColliders(struct dotStructArray *inputStruct)
{
    //int lengthOfArray = sizeof(*inputStruct->mCollider)/sizeof(inputStruct->mCollider[0]);
    //The row offset
	inputStruct->mColliders.x = inputStruct->mPosX;
	inputStruct->mColliders.y = inputStruct->mPosY;
}


bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
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

    //If none of the sides from A are outside B
    return true;
}


bool checkCollisionArray( SDL_Rect a[], SDL_Rect b[] )
{
    int lengthOfA;
    int lengthOfB;
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    lengthOfA = 11;
    lengthOfB = 11;

    for( int Abox = 0; Abox < lengthOfA; Abox++)
    {
        leftA = a[Abox].x;
        rightA = a[Abox].x + a[Abox].w;
        topA = a[Abox].y;
        bottomA = a[Abox].y + a[Abox].h;
        for( int Bbox = 0; Bbox < lengthOfB; Bbox++)
        {
            leftB = b[Bbox].x;
            rightB = b[Bbox].x + b[Bbox].w;
            topB = b[Bbox].y;
            bottomB = b[Bbox].y + b[Bbox].h;

            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
            {
                //A collision is detected
                return true;
            }
        }
    }
    //If neither set of collision boxes touched


    return false;
}

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

void closeRWopsWrapper(char *path)
{
    //Open data for writing
    SDL_RWops* file = SDL_RWFromFile( path, "w+b" );
    if( file != NULL )
    {
        //Save data
        for( int i = 0; i < TOTAL_DATA; ++i )
        {
            SDL_RWwrite( file, &gData[ i ], sizeof(Sint32), 1 );
        }

        //Close file handler
        SDL_RWclose( file );
    }
    else
    {
        printf( "Error: Unable to save file! %s\n", SDL_GetError() );
    }
    	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

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
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
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

bool loadMedia(struct ttfStruct *structinput, SDL_Color textColor)
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

bool LDotTexture(struct dotStruct *structinput)
{

    structinput->mTexture = NULL;
    newTexture = NULL;
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
            structinput->DOT_WIDTH = loadedSurface->w;
            structinput->DOT_HEIGHT = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    loadedSurface = NULL;
    //Return success
    structinput->mTexture = newTexture;
    return newTexture != NULL;
}

bool LDotTextureArray(struct dotStructArray *structinput)
{

    structinput->mTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load( structinput->imagePath ); // crashes here if loading two different structs


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
            structinput->DOT_WIDTH = loadedSurface->w;
            structinput->DOT_HEIGHT = loadedSurface->h;
        }


        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
        loadedSurface = NULL;

    }

    //Return success
    structinput->mTexture = newTexture;
    newTexture = NULL;
    return structinput->mTexture != NULL;
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

void textureRender(struct textureStruct *structinput, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, int posX, int posY)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { structinput->xPos+ posX, structinput->yPos+ posY, structinput->mWidth, structinput->mHeight };

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

void textureDotRender(struct dotStruct *structinput, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { structinput->mPosX, structinput->mPosY, structinput->DOT_WIDTH, structinput->DOT_HEIGHT };

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

void textureDotRenderArray(struct dotStructArray *structinput, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, int posX, int posY)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { structinput->mPosX + posX, structinput->mPosY + posY, structinput->DOT_WIDTH, structinput->DOT_HEIGHT };

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

void moveStruct(struct dotStructArray *inputStruct)
{
    //Move the dot left or right
    inputStruct->mPosX += inputStruct->mVelX;

    //If the dot went too far to the left or right
    if( ( inputStruct->mPosX < 0 ) || ( inputStruct->mPosX + inputStruct->DOT_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        inputStruct->mPosX -= inputStruct->mVelX;
    }

    //Move the dot up or down
    inputStruct->mPosY += inputStruct->mVelY;

    //If the dot went too far up or down
    if( ( inputStruct->mPosY < 0 ) || ( inputStruct->mPosY + inputStruct->DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        inputStruct->mPosY -= inputStruct->mVelY;
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
    SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };

    //Current input point
    int currentData = 0;
    bool quit = false;
    SDL_Event e;
    double degrees = 0;
    SDL_RendererFlip flipType = SDL_FLIP_NONE;
    SDL_RWopsWrapper("33_file_reading_and_writing/nums.bin");
    //Start up SDL and create window
    if( !initRenderer() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        struct ttfStruct gTimeTexture;
        struct timerStruct fpsTimer;
        struct timerStruct capTimer;
        struct ttfStruct gfpsTexture;
        struct ttfStruct gInputTextTexture;
        struct ttfStruct gPromptTextTexture;
        struct ttfStruct gDataTextures[ TOTAL_DATA ];

        struct dotStructArray gDotTextureArray;
        struct dotStructArray gDotTextureArray2;
        struct textureStruct gBGTexture;

        SDL_Color textColor = { 0, 0, 0, 255 };

        gBGTexture.imagePath = "31_scrolling_backgrounds/bg.png";
        gDotTextureArray.imagePath = "31_scrolling_backgrounds/dot.bmp";
      //  gDotTextureArray2.imagePath = "30_scrolling/dot.bmp";

        initDotArray(&gDotTextureArray, 50, 0);
        gDotTextureArray.mPosX = gDotTextureArray.DOT_WIDTH / 2;
        gDotTextureArray.mPosX = gDotTextureArray.DOT_HEIGHT / 2;
      //  initDotArray(&gDotTextureArray2, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
        int countedFrames = 0;
        char *inputText = "lmao";

       // printf("Length of String is %d\n", strlen(inputText));

        gfpsTexture.imagePath = "22_timing/lazy.ttf";
        gPromptTextTexture.imagePath = "22_timing/lazy.ttf";

        for(int i = 0; i < TOTAL_DATA; i++)
        {
            gDataTextures[i].imagePath = "22_timing/lazy.ttf";
        }

        gfpsTexture.xPos = 0;
        gfpsTexture.yPos = 0;
        gInputTextTexture.xPos = 0;
        gInputTextTexture.yPos = 0;
        gfpsTexture.textureText = "Whatup";
        gPromptTextTexture.xPos = 0;
        gPromptTextTexture.yPos = 0;
        gPromptTextTexture.textureText = "Whatup";


        gTimeTexture.imagePath = "22_timing/lazy.ttf";
        gTimeTexture.textureText = "LMAO";
        gInputTextTexture.imagePath = "22_timing/lazy.ttf";
        gInputTextTexture.textureText = inputText;
        if( !loadMedia(&gfpsTexture, textColor) )
        {
            printf( "Failed to load media! \n" );
        }


        char textBuffer1[100];
        for(int i = 0; i < TOTAL_DATA; i++)
        {
            sprintf(textBuffer1, "%u", gData[ i ]  );
            gDataTextures[i].textureText = textBuffer1;
            loadMedia(&gDataTextures[i], highlightColor);
        }
        /*
        for(int i = 0; i < TOTAL_DATA; i++)
        {
            if( !loadMedia(&gDataTextures[i], highlightColor)  )
            {
                printf( "Failed to load media! \n" );
            }
        }*/

        if( !loadMedia(&gInputTextTexture, textColor) )
        {
            printf( "Failed to load media! \n" );
        }
        if( !loadMedia(&gPromptTextTexture, textColor) )
        {
            printf( "Failed to load media! \n" );
        }

        if( !loadMedia(&gTimeTexture, textColor) )
        {
            printf( "Failed to load media! \n" );
        }
        if( !LDotTextureArray(&gDotTextureArray) )
        {
            printf( "Failed to load media! \n" );
        }
        if( !LTexture(&gBGTexture) )
        {
            printf( "Failed to load media! \n" );
        }
    //    gDotTextureArray2.mTexture = gDotTextureArray.mTexture;
    //    gDotTextureArray2.DOT_WIDTH = gDotTextureArray.DOT_WIDTH;
    //    gDotTextureArray2.DOT_HEIGHT = gDotTextureArray.DOT_HEIGHT;
        gBGTexture.xPos = 0;
        gBGTexture.yPos = 0;
    /*   if( !LDotTextureArray(&gDotTextureArray2) )
        {
            printf( "Failed to load media! \n" );
        }
        */

        timerInit(&fpsTimer);
        timerStart(&fpsTimer);

        //Set the wall
     /*   SDL_Rect wall;
        wall.x = 300;
        wall.y = 40;
        wall.w = 40;
        wall.h = 400;
        */

        SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        int scrollingOffset = 0;
        //While application is running
     //   SDL_StartTextInput();
        while( !quit )
        {
            timerStart(&capTimer);
        //    bool renderText = false;
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
						    char textBuffer[100] = "";
							//Previous data entry
							case SDLK_UP:
							//Rerender previous entry input point
							sprintf(textBuffer, "%u", gData[ currentData ]  );
							gDataTextures[ currentData ].textureText = textBuffer;
							loadMedia(&gDataTextures[ currentData ], textColor);
							--currentData;
							if( currentData < 0 )
							{
								currentData = TOTAL_DATA - 1;
							}

							//Rerender current entry input point
							sprintf(textBuffer, "%u", gData[ currentData ]  );
							gDataTextures[ currentData ].textureText = textBuffer;
							loadMedia(&gDataTextures[ currentData ], highlightColor);
							break;

							//Next data entry
							case SDLK_DOWN:
							//Rerender previous entry input point
							sprintf(textBuffer, "%u", gData[ currentData ]  );
							gDataTextures[ currentData ].textureText = textBuffer;
							loadMedia(&gDataTextures[ currentData ], textColor);
							++currentData;
							if( currentData == TOTAL_DATA )
							{
								currentData = 0;
							}

							//Rerender current entry input point
							sprintf(textBuffer, "%u", gData[ currentData ]  );
							gDataTextures[ currentData ].textureText = textBuffer;
							loadMedia(&gDataTextures[ currentData ], highlightColor);
							break;

							//Decrement input point
							case SDLK_LEFT:
							--gData[ currentData ];
							sprintf(textBuffer, "%u", gData[ currentData ]  );
							gDataTextures[ currentData ].textureText = textBuffer;
							loadMedia(&gDataTextures[ currentData ], highlightColor);
							break;

							//Increment input point
							case SDLK_RIGHT:
							++gData[ currentData ];
							sprintf(textBuffer, "%u", gData[ currentData ]  );
							gDataTextures[ currentData ].textureText = textBuffer;
							loadMedia(&gDataTextures[ currentData ], highlightColor);
							break;
						}
					}
                }

                float avgFPS = countedFrames / ( getTicks(&fpsTimer) / 1000.f );
                if( avgFPS > 2000000 )
                {
                    avgFPS = 0;
                }

                //Scroll background
                --scrollingOffset;
                if( scrollingOffset < -gBGTexture.mWidth )
                {
                    scrollingOffset = 0;
                }

                //Set text to be rendered h
                char timeText[100] = "";
                char timeBuffer[100] = "";

                camera.x = ( gDotTextureArray.mPosX + gDotTextureArray.DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
                camera.y = ( gDotTextureArray.mPosY + gDotTextureArray.DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

                //Keep the camera in bounds
                if( camera.x < 0 )
                {
                    camera.x = 0;
                }
                if( camera.y < 0 )
                {
                    camera.y = 0;
                }
                if( camera.x > LEVEL_WIDTH - camera.w )
                {
                    camera.x = LEVEL_WIDTH - camera.w;
                }
                if( camera.y > LEVEL_HEIGHT - camera.h )
                {
                    camera.y = LEVEL_HEIGHT - camera.h;
                }

                strcpy(timeText, "FPS: ");
                sprintf(timeBuffer, "%.0f", avgFPS  );
                strcat(timeText, timeBuffer);

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                gfpsTexture.xPos = (SCREEN_WIDTH - 1.5*gfpsTexture.mWidth ) ;
                gfpsTexture.yPos = ( gfpsTexture.mHeight) ;
                gfpsTexture.textureText = timeText;

                if( !loadMedia(&gfpsTexture, textColor) )
                {
                    printf( "Failed to load media! \n" );
                }
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
              //  SDL_RenderDrawRect( gRenderer, &wall );
                //textureRender(&gBGTexture, &camera, degrees, NULL, flipType, 0 , 0);
            //    textureRender(&gBGTexture, NULL, degrees, NULL, flipType, scrollingOffset, 0);
            //    textureRender(&gBGTexture, NULL, degrees, NULL, flipType, scrollingOffset + gBGTexture.mWidth, 0);
             //   textureDotRenderArray(&gDotTextureArray, NULL, degrees, NULL, flipType, 0, 0);
                textureRenderttf(&gfpsTexture, NULL, degrees, NULL, flipType);

                gPromptTextTexture.xPos = (SCREEN_WIDTH - gPromptTextTexture.mWidth) / 2;
                gPromptTextTexture.yPos = 0;
                textureRenderttf(&gPromptTextTexture, NULL, degrees, NULL, flipType);

                for( int i = 0; i < TOTAL_DATA; ++i )
                {
                    gDataTextures[i].xPos = (SCREEN_WIDTH - gDataTextures[ i ].mWidth ) / 2;
                    gDataTextures[i].yPos = (gPromptTextTexture.mWidth + gDataTextures[ 0 ].mHeight * i);
                    textureRenderttf(&gDataTextures[i], NULL, degrees, NULL, flipType);
                }




             /*   gInputTextTexture.xPos = (SCREEN_WIDTH - gInputTextTexture.mWidth) / 2;
                gInputTextTexture.yPos = gPromptTextTexture.mHeight;
                textureRenderttf(&gInputTextTexture, NULL, degrees, NULL, flipType);
                */
               // textureDotRenderArray(&gDotTextureArray2, NULL, degrees, NULL, flipType, -gDotTextureArray2.mColliders.r, -gDotTextureArray2.mColliders.r);


                SDL_RenderPresent( gRenderer );
                ++countedFrames;
                int frameTicks = getTicks(&capTimer);
                if( frameTicks < SCREEN_TICKS_PER_FRAME )
                {
                    //Wait remaining time
                    SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
                }
            }
        closeRWopsWrapper("33_file_reading_and_writing/nums.bin");
        SDL_StopTextInput();
        gTexture = gDotTextureArray.mTexture;
        closeTexture();
        close();
        gTexture = gDotTextureArray2.mTexture;
        closeTexture();
        close();
        }


        //Free resources and close SDL

        return 0;
}
