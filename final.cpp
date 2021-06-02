//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include<bits/stdc++.h>
using namespace std;
 
//Screen dimension constants
const int LEVEL_WIDTH = 1920;
const int LEVEL_HEIGHT = 960;
const int HELP = 40;
const int MENU = 50;
const int OVER1 = 60;
const int OVER2 = 70;
const int SCOREMENU = 80;
const int CREDITS = 90;
const int EXIT = 100;
const int START = 110;
const int NAME = 120;
const int PAUSE = 130;
const int RETURN = 140;
const int ENDING = 150;
const int SAVEMENU = 160;
const int LOADMENU = 170;

const int LEVEL1START = 0;
const int LEVEL1 = 1;
const int LEVEL1END = 4;

const int LEVEL2START = 7;
const int LEVEL2 = 2;
const int LEVEL2END = 5;

const int LEVEL3START = 6;
const int LEVEL3 = 3;
const int LEVEL3END = 8;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
const int FPS = 30;
const int frameDelay = 500/FPS;
 
//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();
 
        //Deallocates memory
        ~LTexture();
 
        //Loads image at specified path
        bool loadFromFile( std::string path );

        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

        void setColor( Uint8 red, Uint8 green, Uint8 blue );
       
        //Deallocates texture
        void free();
 
       
        //Renders texture at given point
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        int mWidth;
        int mHeight;
 
 
    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
 
        //Image dimensions
        
};
 

 
//Starts up SDL and creates window
bool init();
 
//Loads media
bool loadMedia();
bool checkCollision( SDL_Rect a, SDL_Rect b );
 
//Frees media and shuts down SDL
void close();

 
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
 
//The window renderer
SDL_Renderer* gRenderer = NULL;
 
//Scene textures

int highest;
int currentframe;
int playerlife=10;
int partnerlife=10;
int currentscore=100;


TTF_Font *gFont = NULL;
Mix_Chunk *JumpSound = NULL;
Mix_Chunk *SwordSound = NULL;
Mix_Music *BGSound = NULL;

SDL_Rect gSpriteClips[ 7 ];
SDL_Rect gSpriteWalk[ 2 ];
SDL_Rect gSpriteJump[ 2 ];
SDL_Rect gSpriteCouple[ 2 ];
SDL_Rect gSpriteEnemy[3];
SDL_Rect gSpriteLover;
SDL_Rect gSpriteCoupleJump[2];
SDL_Rect gSpriteBird[2];
SDL_Rect gSpriteHeart;
SDL_Rect gSpriteSpike;

LTexture gSpriteSheetTexture;
LTexture gSpriteWalkTexture;
LTexture gSpriteJumpTexture;
LTexture gSpriteCoupleTexture;
LTexture gSpriteCoupleJumpTexture;
LTexture gSpriteEnemyTexture;
LTexture gBackground0;
LTexture gBackground1;
LTexture gBackground2;
LTexture gBackground3;
LTexture gBackground4;
LTexture gBackground5;
LTexture gameover1;
LTexture gameover2;
LTexture gSpriteLoverTexture;
LTexture header_text_input_texture;

LTexture gSpriteBirdTexture;
LTexture gSpriteHeartTexture;
LTexture gSpriteHeart2Texture;
LTexture gSpriteSpikeTexture;
LTexture gMenuTexture;
LTexture gTextTexture;
LTexture gScoreTexture;
LTexture gHighestTexture;
LTexture gHelpTexture;
LTexture gMenuScore;
LTexture gCreditTexture;
LTexture gNameTexture;
LTexture gPauseTexture;
LTexture gName1Texture;
LTexture gName2Texture;
LTexture gName3Texture;

LTexture gReturnTexture;
LTexture gExitTexture;
LTexture gLoadTexture;
LTexture gSaveTexture;
LTexture level1Texture;
LTexture levelendTexture;
LTexture level2StartTexture;
LTexture level1EndTexture;
LTexture level3StartTexture;

bool JumpCount=0;

SDL_RendererFlip flipType = SDL_FLIP_NONE;

 
LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
 
LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
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
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != NULL;
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}
 
bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();
 
    //The final texture
    SDL_Texture* newTexture = NULL;
 
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
 
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
 
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
 
    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}
 
 
void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
 
 
void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
 
    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
 
    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

class Player
{
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 32;
        static const int DOT_HEIGHT = 48;
 
        //Maximum axis velocity of the dot
        static const int DOT_VEL = 4;
 
        //Initializes the variables
        void init();
 
        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );
 
        //Moves the dot
 
        //Shows the dot on the screen

        void setCamera( SDL_Rect& camera);

        void render(int currentframe, SDL_Rect& camera );

        void destroy();

        void attack();

        void interact(SDL_Rect& object);

        int isAttacking=0;
        int attackDone =0;
        int isAlone=1;
        int isInteracting=0;
        int leftat;

        int flipflag =0;

        SDL_Rect attackbox;
        SDL_Rect interactbox;
        SDL_Rect pCollider;
        int xpos, ypos;
        int initY;
 
    private:
        double velX, velY;

        
};

 
 
void Player::init()
{
    //Initialize the offsets
    isAlone=1;
    xpos = 0;
    ypos = 850;
    initY = 850;
    pCollider.w = 16;
    pCollider.h = 40;
    pCollider.x = xpos+5;
    pCollider.y = ypos-4;
    //Initialize the velocity
    velX = 0;
    velY = 0;
}
 
void Player::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:   
            {
                //JumpCount=1;
                Mix_PlayChannel(-1,JumpSound,0);
                if(ypos == initY)
                velY = -7.0; break;
            }
            //case SDLK_DOWN: velY += DOT_VEL; break;
            case SDLK_LEFT: velX = -3.0; break;
            case SDLK_RIGHT: velX = 3.0; break;
        }
 
        if(e.key.keysym.sym==SDLK_SPACE)
        {
            Mix_PlayChannel(-1,SwordSound,0);

            isInteracting=0;

            isAttacking = 1;
        }

        else if(e.key.keysym.sym == SDLK_e && e.key.repeat == 0)
        {
            isInteracting =1;
            isAttacking=0;
            attackDone=0;
            if(!isAlone)
            {
                isAlone=1;
                leftat = xpos;
                //std::cout<<leftat;
                //printf("%d\n", leftat);
            }
            //printf("isInteracting\n");
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        isInteracting =0;
        isAttacking = 0;
        attackDone=0;
        //Adjust the velocity
        switch( e.key.keysym.sym)
        {
            case SDLK_LEFT:  velX = 0; break;
            case SDLK_RIGHT: velX = 0; break;
        }
    }
}
 
 void Player::attack()
{
    if(attackDone && isAttacking)
    {
        
        if(flipflag==0)
        {   attackbox.x = xpos+16; }
        else attackbox.x = xpos;
        attackbox.y = ypos;
        attackbox.w = 20;
        attackbox.h = 48;
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderDrawRect( gRenderer, &attackbox );
    }
    else
    {
        attackbox.y= -100;
    }

}


void Player::interact(SDL_Rect& object)
{
    if(isInteracting)
    {
        if(flipflag==0)
        {   interactbox.x = xpos+16; }
        else interactbox.x = xpos;
        interactbox.y = ypos;
        interactbox.w = 16;
        interactbox.h = 48;
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderDrawRect( gRenderer, &interactbox );

        if(checkCollision(interactbox, object))
        {
            isAlone=0;
            //printf("NOT ALONE\n");
        }    
    }
}
 
void Player::setCamera( SDL_Rect& camera )
{
    //Center the camera over the dot
    camera.x = ( xpos + 16 ) - SCREEN_WIDTH / 2;
    camera.y = ( ypos + 24 ) - SCREEN_HEIGHT / 2;

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
}
 
void Player::render(int currentframe, SDL_Rect& camera)
{
    int frame = 0;
    //While application is running
    pCollider.x=xpos;
    if(isAlone)
    {
        pCollider.w =16;
        if( velX > 0 && velY == 0)
        {
            isAttacking = 0;
            attackDone=0;
            isInteracting=0;
            if(flipflag == 1)
            {
                flipType = SDL_FLIP_HORIZONTAL;
                flipflag = 0;
            }
            else
                flipType = SDL_FLIP_NONE;

             // for(int frame=0;frame<10;frame++)
             // {
                //Render current frame
                SDL_Rect* currentClip = &gSpriteWalk[ (currentframe/5)%2 ];
                gSpriteWalkTexture.render( xpos-camera.x , ypos -camera.y , currentClip , 0, NULL, flipType );
                
                //xpos++;
                
                xpos+=velX;
                pCollider.x = xpos+5;
                if( ( xpos < 0 ) || ( xpos + DOT_WIDTH > LEVEL_WIDTH ) )
                {
                    //Move back
                    xpos -= velX;
                    pCollider.x = xpos+5;
                }
            //}
        }

            else if( velX < 0  && velY == 0)
            {
                isAttacking = 0;
                attackDone=0;
                isInteracting=0;
               
                if(flipflag==0)
                {
                    flipType = SDL_FLIP_HORIZONTAL;
                    flipflag = 1;
                }

                    SDL_Rect* currentClip = &gSpriteWalk[ (currentframe/5)% 2   ];
                    gSpriteWalkTexture.render( xpos - camera.x, ypos - camera.y, currentClip, 0, NULL, flipType  );
                    //xpos--;
                    xpos+=velX;
                    pCollider.x = xpos+12;
                    if( ( xpos < 0 ) || ( xpos + DOT_WIDTH > LEVEL_WIDTH ))
                    {
                        //Move back
                        xpos -= velX;
                        pCollider.x = xpos+12;
                    }
                //}
            }

            else if( velY < 0)
            {
                // if(JumpCount==1)
                // {
                //  Mix_PlayMusic(JumpSound,-1);
                //  JumpCount=0;
                // }
                isAttacking = 0;
                attackDone=0;
                isInteracting=0;
               
                    if(flipflag ==0)
                    {
                        flipType = SDL_FLIP_NONE;
                    }
                    else flipType = SDL_FLIP_HORIZONTAL;
                   
                     //Render current frame
                    SDL_Rect* currentClip = &gSpriteJump[ 0 ];
                    gSpriteJumpTexture.render( xpos - camera.x, ypos -camera.y, currentClip , 0, NULL, flipType );
                    //ypos -= 5;
                       
                       
                    pCollider.x = xpos+5;
                    if( ( xpos < 0 ) || ( xpos + 32 > LEVEL_WIDTH ) )
                    {
                        //Move back
                        xpos -= velX;
                        pCollider.x = xpos+5;
                    }
                    velY+=0.4;
                    ypos+=velY;
                    xpos+=velX;
                    pCollider.y = ypos+4;
                      
            }
 
       
        else if( velY > 0 )
        {
            isInteracting=0;
            isAttacking = 0;
            attackDone=0;
           
                if(flipflag ==0)
                {
                    flipType = SDL_FLIP_NONE;
                }
                else flipType = SDL_FLIP_HORIZONTAL;
               
                 //Render current frame
                SDL_Rect* currentClip = &gSpriteJump[ 1 ];
                gSpriteJumpTexture.render( xpos-camera.x, ypos-camera.y, currentClip , 0, NULL, flipType );
                //ypos -= 5;
                   
                   
                pCollider.x = xpos;
                if( ( xpos < 0 ) || ( xpos + 32 > LEVEL_WIDTH ))
                {
                    //Move back
                    xpos -= velX;
                    pCollider.x = xpos+5;
                }
                velY+=0.4;
                ypos+=velY;
                xpos+=velX;
                pCollider.y = ypos+4;
                if(ypos>851)
                {
                    ypos=850;
                    velY=0.0;
                }
        }
            

            else if(isAttacking == 1)
            {
                isInteracting=0;
                if(flipflag == 1)
                {
                    flipType = SDL_FLIP_HORIZONTAL;
                    //flipflag = 0;
                }
                else
                    flipType = SDL_FLIP_NONE;
                // for(int frame=0;frame<35;frame++)
                // {
                    //Render current frame
                    SDL_Rect* currentClip = &gSpriteClips[ (currentframe/5)%5  ];
                    if((currentframe/5)%5 >2)
                    {
                        attackDone=1;
                    }

                    gSpriteSheetTexture.render( xpos -camera.x,ypos -camera.y, currentClip , 0, NULL, flipType );
                    

                    
             //   }
            }
       


        else if(velX==0.0 && ypos==initY )
        {
            isAttacking=0;
            isInteracting=0;
            attackDone=0;

            if(flipflag==0)
            {
                flipType = SDL_FLIP_NONE;
            }
            else flipType = SDL_FLIP_HORIZONTAL;
            SDL_Rect* currentClip = &gSpriteWalk[ 0 ];
            gSpriteWalkTexture.render( xpos -camera.x, ypos- camera.y, currentClip , 0, NULL, flipType );        
        }


    }

    else if(isAlone==0)
    {
        pCollider.w = 40;

        if( velY < 0)
            {
            isAttacking = 0;
            attackDone=0;
           
                if(flipflag ==0)
                {
                    flipType = SDL_FLIP_NONE;
                }
                else flipType = SDL_FLIP_HORIZONTAL;
               
                 //Render current frame
                SDL_Rect* currentClip = &gSpriteCoupleJump[ 0 ];
                gSpriteCoupleJumpTexture.render( xpos - camera.x, ypos -camera.y, currentClip , 0, NULL, flipType );
                //ypos -= 5;
                   
                   
                pCollider.x = xpos;
                if( ( xpos < 0 ) || ( xpos + 32 > LEVEL_WIDTH ) )
                {
                    //Move back
                    xpos -= velX;
                    pCollider.x = xpos;
                }
                velY+=0.4;
                ypos+=velY;
                xpos+=velX;
                pCollider.y = ypos+4;
                      
        }
 
       
        else if( velY > 0 )
        {
            isAttacking = 0;
            attackDone=0;
           
                if(flipflag ==0)
                {
                    flipType = SDL_FLIP_NONE;
                }
                else flipType = SDL_FLIP_HORIZONTAL;
               
                 //Render current frame
                SDL_Rect* currentClip = &gSpriteCoupleJump[ 1 ];
                gSpriteCoupleJumpTexture.render( xpos-camera.x, ypos-camera.y, currentClip , 0, NULL, flipType );
                //ypos -= 5;
                   
                   
                pCollider.x = xpos;
                if( ( xpos < 0 ) || ( xpos + 32 > LEVEL_WIDTH ))
                {
                    //Move back
                    xpos -= velX;
                    pCollider.x = xpos;
                }
                velY+=0.4;
                ypos+=velY;
                xpos+=velX;
                pCollider.y = ypos+4;
                if(ypos>851)
                {
                    ypos=850;
                    velY=0.0;
                }
        }
        else if(velX < 0 && ypos==initY)
        {
                isAttacking = 0;
                attackDone=0;
               
                if(flipflag==0)
                {
                    flipType = SDL_FLIP_HORIZONTAL;
                    flipflag = 1;
                }

                // for(int frame=0;frame<10;frame++)
                // {
                    //Render current frame
                    SDL_Rect* currentClip = &gSpriteCouple[ (currentframe/5)% 2   ];
                    gSpriteCoupleTexture.render( xpos - camera.x, ypos - camera.y, currentClip, 0, NULL, flipType  );
                    //xpos--;
                    xpos+=velX;
                    pCollider.x = xpos;
                    if( ( xpos < 0 ) || ( xpos + DOT_WIDTH > LEVEL_WIDTH ))
                    {
                        //Move back
                        xpos -= velX;
                        pCollider.x = xpos;
                    }
                //}
            }

        else if(velX > 0 && ypos==initY)
        {
            isAttacking = 0;
            attackDone=0;
            if(flipflag == 1)
            {
                flipType = SDL_FLIP_HORIZONTAL;
                flipflag = 0;
            }
            else
                flipType = SDL_FLIP_NONE;

             // for(int frame=0;frame<10;frame++)
             // {
                //Render current frame
                SDL_Rect* currentClip = &gSpriteCouple[ (currentframe/5)%2 ];
                gSpriteCoupleTexture.render( xpos-camera.x , ypos -camera.y , currentClip , 0, NULL, flipType );
                
                //xpos++;
                
                xpos+=velX;
                pCollider.x = xpos;
                if( ( xpos < 0 ) || ( xpos + DOT_WIDTH > LEVEL_WIDTH ) )
                {
                    //Move back
                    xpos -= velX;
                    pCollider.x = xpos;
                }
            //}
        }

        

        

        else
        {
                if(flipflag==0)
                {
                    flipType = SDL_FLIP_NONE;
                }
                else flipType = SDL_FLIP_HORIZONTAL;
                SDL_Rect* currentClip = &gSpriteCouple[ 0 ];
                gSpriteCoupleTexture.render( xpos -camera.x, ypos- camera.y, currentClip , 0, NULL, flipType );       
        }
    }

           
}


class Enemy
{
    public:

    
        void init(int number);

        //Moves the dot
        //void move(SDL_Rect& playerattack );
        
        void render(int number, int f, SDL_Rect& camera);
        void destroy(int number, SDL_Rect& playerattack);
        SDL_Rect enemy[100];
        int destroyed[100]={0};
        int speed[100] ={0};
        int number;


    private:
        int xpos , ypos;

        
        int initialpos[100]={0};

        int dir[100]= {0} ;
        int range[100]= {0} ;
        
        SDL_RendererFlip flip = SDL_FLIP_NONE;
};

void Enemy::init(int number)
{

    for(int i=0;i<number;i++)
    {
        enemy[i].x      = rand() % 1280 + 200; 
        initialpos[i]   = enemy[i].x;
        enemy[i].y      = 850;
        dir[i]          = rand() %2 ;
        range[i]        = rand() %200 + 0;
        speed[i]        = rand() % 3 +1;
        enemy[i].w = 24;
        enemy[i].h = 48;
        destroyed[i]=0;
    }
}

void Enemy::render(int number, int f, SDL_Rect& camera)
{
    for(int i=0;i<number;i++)
    {
        if(dir[i]==0)
        {
            enemy[i].x += speed[i];
            if(enemy[i].x == initialpos[i]+range[i]-32 || enemy[i].x==LEVEL_WIDTH-32)    dir[i]=1;
        }
        else if(dir[i]==1)
         {
             enemy[i].x -= speed[i];
             if(enemy[i].x == initialpos[i]-range[i] - 32 || enemy[i].x==0)    dir[i]=0;
         }
        //enemy[i].x=xpos;

        if(!destroyed[i])
        {
            SDL_Rect* currentClip;
            if(dir[i]==0)
            {
                flip=SDL_FLIP_HORIZONTAL;
            }
            else flip=SDL_FLIP_NONE;
            if(speed[i]==0)
            {   
                currentClip = &gSpriteEnemy[ 0 ];
            }
            else currentClip = &gSpriteEnemy[ (f/5)%3 ];
            currentClip->h=48;
            currentClip->w=32;
            gSpriteEnemyTexture.render( enemy[i].x - camera.x , enemy[i].y -camera.y, currentClip , 0, NULL, flip );
        }
    }
}

void Enemy::destroy(int number, SDL_Rect& playerattack)
{
    for(int i=0;i<number;i++)
    {
        if(!destroyed[i])
        {
            if(checkCollision( enemy[i], playerattack ))
            {
                destroyed[i]=1;
                currentscore -= 5;
                //printf("killed\n");
            }
        }   
    }
}

class Bird
{
    public:

    
        void init(int number);

        //Moves the dot
        //void move(SDL_Rect& playerattack );
        
        void render(int number, int f, SDL_Rect& camera);
        void destroy(int number, SDL_Rect& playerattack);
        SDL_Rect bird[100];
        int destroyed[100]={0};
        int speed[100] ={0};
        int number;


    private:
        int xpos , ypos;

        
        int initialpos[100]={0};

        int dir[100]= {0} ;
        int up[100]={0} ;
        int range[100]= {0} ;
        
        SDL_RendererFlip flip = SDL_FLIP_NONE;
};

void Bird::init(int number)
{

    for(int i=0;i<number;i++)
    {
        bird[i].x      = rand() % 1280 + 100; 
        initialpos[i]   = bird[i].x;
        bird[i].y       = rand() % 50 + 820;
        dir[i]          = rand() %2 ;
        up[i]           = rand() %2 ;
        range[i]        = rand() %200 + 0;
        speed[i]        = rand() % 2 +1;
        bird[i].w = 23;
        bird[i].h = 15;
        destroyed[i]=0;
    }
}

void Bird::render(int number, int f, SDL_Rect& camera)
{
    for(int i=0;i<number;i++)
    {
        if(dir[i]==0)
        {
            bird[i].x += speed[i];
            if(bird[i].x == initialpos[i]+range[i]-bird[i].w || bird[i].x==LEVEL_WIDTH-bird[i].w-1)    dir[i]=1;
        }
        else if(dir[i]==1)
         {
             bird[i].x -= speed[i];
             if(bird[i].x == initialpos[i]-range[i] - bird[i].w || bird[i].x==0)    dir[i]=0;
         }

         if(up[i]==0)
         {
            bird[i].y += 1;
            if(bird[i].y >= 880) up[i]=1;
         }
         else if(up[i]==1)
         {
            bird[i].y -= 1;
            if(bird[i].y <= 800) up[i]=0;
         }



        //enemy[i].x=xpos;
        if(!destroyed[i])
        {
            SDL_Rect* currentClip;
            if(dir[i]==0)
            {
                flip=SDL_FLIP_HORIZONTAL;
            }
            else flip=SDL_FLIP_NONE;
            if(speed[i]==0)
            {   
                currentClip = &gSpriteBird[ 0 ];
            }
            else currentClip = &gSpriteBird[ (f/15)%2 ];

            currentClip->h=15;
            currentClip->w=23;
            gSpriteBirdTexture.render( bird[i].x - camera.x , bird[i].y -camera.y, currentClip , 0, NULL, flip );

        }
    }
}

void Bird::destroy(int number, SDL_Rect& playerattack)
{
    for(int i=0;i<number;i++)
    {
        if(!destroyed[i])
        {
            if(checkCollision( bird[i], playerattack ))
            {
                destroyed[i]=1;
                currentscore -= 5;
                //printf("killed\n");
            }
        }
    }   
}






class Fire
{
public:

    void init(int number, int difference);   

    void render(int number, SDL_Rect& camera);

    SDL_Rect fire[100];

    int number;
    int difference, start = 100;

private:
    int xpos , ypos;
};

void Fire::init(int number, int difference)
{

    for(int i=0 ;i<number;i++)
    {

        //difference = difference*(i+1);
        //printf("%d %d \n", i , difference);
        fire[i].x = ((rand())) % (1920 - difference) + difference;
        fire[0].x = 200;
        //printf("%d\n", fire[i].x);
        //start = difference;
        fire[i].y = 880;
        fire[i].h = 15;
        fire[i].w = 13;
       
    }
}

void Fire::render(int number, SDL_Rect& camera)
{

    SDL_Rect* currentClip;
    currentClip = &gSpriteSpike;
    currentClip->h=15;
    currentClip->w=13;  


     for(int i=0;i<number;i++)
     {
        gSpriteSpikeTexture.render( fire[i].x - camera.x , fire[i].y -camera.y, currentClip , 0, NULL);
     }
}


class Partner
{
public:
    
    void init();
    void render(SDL_Rect& camera , int isAlone, int px, int flipflag);
    void dead();
    SDL_Rect position;
    int isAlone;
    int withplayer;
    int xpos, ypos;

private:
    


};

void Partner::init()
{
    isAlone = 0;
    position.x = 50;
    position.y = 850;
    withplayer = 0;
    position.h = 48;
    position.w = 32;
}

void Partner::render(SDL_Rect& camera , int isAlone, int px, int flipflag)
{

    
    if(withplayer==1 && isAlone==1)
    {
        //printf("new 1\n");
        if(px-24<0)
        {
            position.x = px;
        }
        else
        position.x = px-24;
        
        SDL_Rect* currentClip = &gSpriteLover;   
        currentClip->h=48;
        currentClip->w=24;
        gSpriteCoupleTexture.render( position.x - camera.x , position.y -camera.y, currentClip , 0, NULL );
    }

    else if(isAlone)
    {
        //printf("new 2\n");
        SDL_Rect* currentClip = &gSpriteLover;   
        currentClip->h=48;
        currentClip->w=24;
        gSpriteCoupleTexture.render( position.x - camera.x , position.y -camera.y, currentClip , 0, NULL );
    }
    else 
        {
           // printf("new 3\n");
            if(flipflag==1)
            {
                position.x = px+24;
            }
            else position.x = px-24;
            withplayer =1;
            //position.x = px-32;
        }


}

class Heart
{
    public:

    
        void init(int number); 
        void render(int number, SDL_Rect& camera, int alone);
        void destroy(int number, SDL_Rect& player, int alone);
        SDL_Rect heart[100];
        int destroyed[100]={0};
        int number;


    private:
        int xpos , ypos;
};

void Heart::init(int number)
{

    for(int i=0;i<number;i++)
    {
        heart[i].x = rand() % 1280 + 200; 
        heart[i].y = 865;
        heart[i].w = 14;
        heart[i].h = 12;
        destroyed[i]=0;
    }
}

void Heart::render(int number, SDL_Rect& camera, int alone)
{
    for(int i=0;i<number;i++)
    {
        if(!destroyed[i])
        {
            SDL_Rect* currentClip;
            currentClip = &gSpriteHeart;
            currentClip->h=12;
            currentClip->w=14;
            if(alone)
            gSpriteHeartTexture.render( heart[i].x - camera.x , heart[i].y -camera.y, currentClip , 0, NULL);
            else
            gSpriteHeart2Texture.render( heart[i].x - camera.x , heart[i].y -camera.y, currentClip , 0, NULL);

        }
    }
}

void Heart::destroy(int number, SDL_Rect& player, int alone)
{
    for(int i=0;i<number;i++)
    {
        if(destroyed[i]!=1)
        {
            if(checkCollision( heart[i], player ))
            {
                destroyed[i]=1;
                if(alone==1)    currentscore += 10;
                else    currentscore += 15;

            }

            
        }   
    }   

}



 
bool init()
{
    //Initialization flag
    bool success = true;
 
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
 
        //Create window
        gWindow = SDL_CreateWindow( "Will You Go With Me??", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
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

                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
            }
        }
    }
 
    return success;
}
 
bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    if( !gMenuTexture.loadFromFile( "menu.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gLoadTexture.loadFromFile( "load.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gSaveTexture.loadFromFile( "save.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    //Load sprite sheet texture
    if( !gSpriteSheetTexture.loadFromFile( "Sword.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        for(int i=0;i<7;i++)
        {
            gSpriteClips[ i ].x =   32*i;
            gSpriteClips[ i ].y =   0;
            gSpriteClips[ i ].w =  32;
            gSpriteClips[ i ].h =  48;
        }  
    }
    if( !gSpriteWalkTexture.loadFromFile( "walk.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        for(int i=0;i<2;i++)
        {
            gSpriteWalk[ i ].x =   32*i;
            gSpriteWalk[ i ].y =   0;
            gSpriteWalk[ i ].w =  32;
            gSpriteWalk[ i ].h =  48;
        }  
    }
 
 
    if( !gSpriteJumpTexture.loadFromFile( "jump.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        for(int i=0;i<2;i++)
        {
            gSpriteJump[ i ].x =   32*i;
            gSpriteJump[ i ].y =   0;
            gSpriteJump[ i ].w =  32;
            gSpriteJump[ i ].h =  48;
        }  
    }

    if( !gSpriteSheetTexture.loadFromFile( "Sword.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        for(int i=0;i<7;i++)
        {
            gSpriteClips[ i ].x =   32*i;
            gSpriteClips[ i ].y =   0;
            gSpriteClips[ i ].w =  32;
            gSpriteClips[ i ].h =  48;
        }  
    }
    if( !gSpriteEnemyTexture.loadFromFile( "Enemy.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        for(int i=0;i<3;i++)
        {
            gSpriteEnemy[ i ].x =   32*i;
            gSpriteEnemy[ i ].y =   0;
            gSpriteEnemy[ i ].w =  32;
            gSpriteEnemy[ i ].h =  48;
        }  
    }
    if( !gameover1.loadFromFile( "gameover1.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gameover2.loadFromFile( "gameover2.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gBackground0.loadFromFile( "background0.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gBackground1.loadFromFile( "background1.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gBackground2.loadFromFile( "background2.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gBackground3.loadFromFile( "background3.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gBackground4.loadFromFile( "background4.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !level1Texture.loadFromFile( "lvl1.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }

    if( !gSpriteCoupleTexture.loadFromFile( "couple.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        for(int i=0;i<2;i++)
        {
            gSpriteCouple[ i ].x =   48*i;
            gSpriteCouple[ i ].y =   0;
            gSpriteCouple[ i ].w =  48;
            gSpriteCouple[ i ].h =  48;
        }  

        gSpriteLover.x =   0;
        gSpriteLover.y =   0;
        gSpriteLover.w =  24;
        gSpriteLover.h =  48;
    }

    if( !gSpriteCoupleJumpTexture.loadFromFile( "couplejump.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        for(int i=0;i<2;i++)
        {
            gSpriteCoupleJump[ i ].x =   48*i;
            gSpriteCoupleJump[ i ].y =   0;
            gSpriteCoupleJump[ i ].w =  48;
            gSpriteCoupleJump[ i ].h =  48;
        }  
    }
    if( !gSpriteHeartTexture.loadFromFile( "heart.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gSpriteHeart2Texture.loadFromFile( "heart2.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gSpriteSpikeTexture.loadFromFile( "spike.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gSpriteBirdTexture.loadFromFile( "birds.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else 
    {
        for(int i=0;i<2;i++)
        {
            gSpriteBird[ i ].x =   23*i;
            gSpriteBird[ i ].y =   0;
            gSpriteBird[ i ].w =  23;
            gSpriteBird[ i ].h =  15;
        }   
    }

    if( !gHelpTexture.loadFromFile( "help.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gCreditTexture.loadFromFile( "credits.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gMenuScore.loadFromFile( "score.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gNameTexture.loadFromFile( "name.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gPauseTexture.loadFromFile( "pause.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gExitTexture.loadFromFile( "exit.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !gReturnTexture.loadFromFile( "return.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !levelendTexture.loadFromFile( "end.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !level1EndTexture.loadFromFile( "level1end.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !level2StartTexture.loadFromFile( "level2start.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    if( !level3StartTexture.loadFromFile( "level3start.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    
    gFont = TTF_OpenFont( "Font3.ttf", 25 );

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    JumpSound = Mix_LoadWAV("jump.wav");
    SwordSound = Mix_LoadWAV("sword.wav");
    BGSound = Mix_LoadMUS("background.mp3");

    //gBackground
 
    return success;
}

void high()
{
     
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        //success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 255,255, 255 };
        if( !gTextTexture.loadFromRenderedText( "Score : ", textColor ) )
        {
            printf( "Failed to render text texture!\n" );
          //  success = false;
        }

        char s[100],g[100];
        int temp=currentscore,i=0;
        while(temp>0)
        {
            s[i]=temp%10+'0';
            temp/=10;
            i++;
        }
        s[i]=0;
        int l=strlen(s);
        for(i=0;i<l;i++)
            g[i]=s[l-1-i];
        g[l]=0;


        if( !gScoreTexture.loadFromRenderedText( g, textColor ) )
        {
            printf( "Failed to render text texture!\n" );
          //  success = false;
        }

    }
}

void HallOfFame()
{
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        //success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 255, 000, 000 };
        FILE *rrp;
        rrp = fopen("HighScore.txt" , "r");

        char scoretemp[100];
        int showpos = 140;
        while( fgets( scoretemp, sizeof( scoretemp ) ,rrp ) != NULL )
        {
            scoretemp[strlen(scoretemp)-1]= 0;
            gHighestTexture.loadFromRenderedText( scoretemp , textColor );
           //printf("%s", scoretemp);
            gHighestTexture.render( 150 ,  showpos );
            showpos += 50;
        }

        fclose(rrp);
        
    }
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

int savedalone, savedplayerxpos, savedpartnerxpos, savedpartneralone, savedenemydest[100], savedheartdest[100], savedbirddest[100]; 


void close()
{
    //Destroy window   
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    Mix_FreeMusic(BGSound);
    Mix_FreeChunk(JumpSound);
    Mix_FreeChunk(SwordSound);
 
    //Quit SDL subsystems
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

char PlayerName[100];
FILE *rp,*wp,*ap;
int init1=0;
int init2=0;
int init3=0;
int LOADED1 =0;
int LOADED2 =0;
int LOADED3 =0;
int isloaded=0;



int main( int argc, char* args[] )
{
    int HELPFROM;
    int PAUSEFROM;
    int EXITFROM;


    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {  
            int startnow;
            //Main loop flag
            bool quit = false;
            int level=MENU;
            SDL_Event e;

            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            Uint32 frameStart;
            int frameTime;

            Player player1;
            Partner P1;
            Fire fire1;
            Heart h1;
            Enemy Enemy1;

            Player player2;
            Partner P2;
            Fire fire2;
            Bird bird2;
            Heart h2;

            Player player3;
            Enemy Enemy3; 
            Bird bird3;
            Fire fire3;
            Partner P3;
            Heart h3;
            //Mix_PlayMusic(BGSound,-1);
            //Enemy.init();
            while(!quit)
            {
                //Mix_HaltMusic();
                startnow=0;
                int var =0;
                while(level==MENU)
                {   
                    Mix_HaltMusic();
                    
                    currentscore =100;
                    SDL_RenderClear(gRenderer);

                    gMenuTexture.render( 0, 0);
                    SDL_Rect opt1;
                    opt1.x = 250;
                    opt1.y = 140;
                    opt1.h = 26;
                    opt1.w = 150;
                    if(var==1)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &opt1 ); 
                    }
                    
                    SDL_Rect load;
                    load.x = 250;
                    load.y = 190-2;
                    load.h = 26;
                    load.w = 150;
                    if(var==6)
                    {
                        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                        SDL_RenderDrawRect( gRenderer,  &load );
                    }



                    SDL_Rect opt2;
                    opt2.x = 250;
                    opt2.y = 234;
                    opt2.h = 26;
                    opt2.w = 150;
                    if(var==2)
                    {
                        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                        SDL_RenderDrawRect( gRenderer,  &opt2 );
                    }

                    SDL_Rect opt3;
                    opt3.x = 250;
                    opt3.y = 284;
                    opt3.h = 26;
                    opt3.w = 150;
                    if(var==3)
                    {
                        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                        SDL_RenderDrawRect( gRenderer,  &opt3 );
                    }

                    SDL_Rect opt4;
                    opt4.x = 250;
                    opt4.y = 330;
                    opt4.h = 26;
                    opt4.w = 100;
                    if(var==4)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &opt4 ); 
                    }

                    SDL_Rect opt5;
                    opt5.x = 250;
                    opt5.y = 380;
                    opt5.h = 26;
                    opt5.w = 80;
                    if(var==5)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &opt5 ); 
                    }

                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                           //quit = true;
                           //break;
                           EXITFROM = level;
                           level = EXIT;
                        }

                        
                            if(e.button.x > opt1.x && e.button.x< opt1.x + opt1.w  && e.button.y > opt1.y && e.button.y < opt1.y + opt1.h)
                            {   
                                var = 1;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    
                                    isloaded=0;
                                    LOADED1=0;
                                    LOADED2=0;
                                    LOADED3=0;
                                    init1=0;
                                    init2=0;
                                    init3=0;
                                    level = NAME;
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }

                            if(e.button.x > load.x && e.button.x< load.x + load.w  && e.button.y > load.y && e.button.y < load.y + load.h)
                            {   
                                var = 6;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {  
                                	EXITFROM = level;
                                	level = LOADMENU;
                                    
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }
                            if(e.button.x > opt2.x && e.button.x< opt2.x + opt2.w  && e.button.y > opt2.y && e.button.y < opt2.y + opt2.h)
                            {   
                                var = 2;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    level = SCOREMENU;
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }

                            if(e.button.x > opt3.x && e.button.x< opt3.x + opt3.w  && e.button.y > opt3.y && e.button.y < opt3.y + opt3.h)
                            {   
                                var = 3;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    HELPFROM= level;
                                    level = HELP;
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }

                            if(e.button.x > opt4.x && e.button.x< opt4.x + opt4.w  && e.button.y > opt4.y && e.button.y < opt4.y + opt4.h)
                            {   
                                var = 4;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    level = CREDITS;
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }

                            if(e.button.x > opt5.x && e.button.x< opt5.x + opt5.w  && e.button.y > opt5.y && e.button.y < opt5.y + opt5.h)
                            {
                                var = 5;
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    //quit = true;
                                    EXITFROM = level;
                                    level = EXIT;
                                }
                                
                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }
                     
                        
                    }
                        

                        SDL_RenderPresent(gRenderer);
                }

                int nametaken=0;
                  while( level==NAME )
                    {
                        bool quit_this = false;
                        
                        SDL_Rect box;
                        box.x = 210;
                        box.y = 165;
                        box.h = 80;
                        box.w = 380;
                        std::string inputText = "";

                        SDL_StartTextInput(); 
                        SDL_Color textColor = { 255,255, 255 };
                        
                        init1=0;
                        init2=0;
                        init3=0;
                        while( !quit_this )
                         {
                            bool renderText = false;
                        //Handle events on queue
                        while( SDL_PollEvent( &e ) != 0 )
                        {
                            //User requests quit
                            if( e.type == SDL_QUIT )
                            {
                               //quit = true;
                               //break;
                                EXITFROM = level;
                               level = EXIT;

                            }
                            if( e.type == SDL_KEYDOWN )
                              {
                                if( e.key.keysym.sym == SDLK_RETURN )
                                {
                                    
                                     level=LEVEL1START;
                                     quit_this = true;
                                    

                                }

                                if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
                                {
                                        inputText.pop_back();
                                        renderText = true;
                                }
                            }
                            else if( e.type == SDL_TEXTINPUT )
                              {
                                if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
                                {
                                  inputText += e.text.text;
                                  renderText = true;
                                }
                              }

                              if( renderText )
                            {

                              if( inputText != " " )
                              {

                                header_text_input_texture.loadFromRenderedText( inputText.c_str(), textColor );

                              }

                              else
                              {

                                header_text_input_texture.loadFromRenderedText( " ", textColor );
                              }

                            }
                            
                        }

                        SDL_RenderClear(gRenderer);
                        
                        gNameTexture.render( 0, 0);
                        header_text_input_texture.render( 220,200 );
                        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                        SDL_RenderDrawRect(gRenderer, &box);
                        SDL_RenderPresent(gRenderer);
                        }
                         SDL_StopTextInput();
                         int len = inputText.length();
                         for(int i=0;i<len;i++)
                         {
                            PlayerName[i] = inputText[i];
                         }
                         //printf("%d\n", len);
                         
                         
                    }

                while( level==LEVEL1START )
                {
                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                          // quit = true;x
                           //break;
                            EXITFROM = level;
                           level = EXIT;
                        }
                        
                        
                        //Handle input for the dot
                        if( e.key.keysym.sym == SDLK_RETURN  &&  e.key.repeat == 0)
                        {
                            //int x;
                            startnow++;
                            if(startnow>1)
                            level=LEVEL1;
                        }

                        
                    }

                    SDL_RenderClear(gRenderer);    
                    level1Texture.render( 0, 0);
                    SDL_RenderPresent(gRenderer);
                    

                }

                var =0;

                while( level==RETURN )
                {
                    SDL_RenderClear(gRenderer);    
                    gReturnTexture.render( 0, 0);
                    SDL_Rect opt1;
                    opt1.x = 112;
                    opt1.y = 215;
                    opt1.h = 50;
                    opt1.w = 100;
                    if(var==1)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &opt1 ); 
                    }
                    SDL_Rect opt2;
                    opt2.x = 548;
                    opt2.y = 215;
                    opt2.h = 50;
                    opt2.w = 100;
                    if(var==2)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &opt2 ); 
                    }
                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                           //quit = true;
                           //break;
                            EXITFROM = level;
                           level = EXIT;
                        }
     
                        if(e.button.x > opt1.x && e.button.x< opt1.x + opt1.w  && e.button.y > opt1.y && e.button.y < opt1.y + opt1.h)
                            {   
                                var = 1;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    level = MENU;
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }
                        if(e.button.x > opt2.x && e.button.x< opt2.x + opt2.w  && e.button.y > opt2.y && e.button.y < opt2.y + opt2.h)
                            {   
                                var = 2;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    level = EXITFROM;
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }

                        
                    }

                    
                    
                    SDL_RenderPresent(gRenderer);
                    

                }
                startnow=0;
                var=0;
                while( level==SAVEMENU )
                {
                	//startnow=0;
                	SDL_Rect slot1;
                	slot1.x = 100;
                	slot1.y = 130+20;
                	slot1.h = 80-25;
                	slot1.w = 600; 

                	SDL_Rect slot2;
                	slot2.x = 100;
                	slot2.y = 130+100+20;
                	slot2.h = 80-25;
                	slot2.w = 600;

                	SDL_Rect slot3;
                	slot3.x = 100;
                	slot3.y = 130+100+100+20;
                	slot3.h = 80-25;
                	slot3.w = 600;


                	
                 


                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                          // quit = true;x
                           //break;
                            EXITFROM = level;
                          	level = EXIT;
                        }
                        
                        
                        //Handle input for the dot
                        if( e.key.keysym.sym == SDLK_ESCAPE  &&  e.key.repeat == 0)
                        {
                            //int x;
                            startnow++;
                            if(startnow>1)
                            level=EXITFROM;
                        }

                        if(e.button.x > slot1.x && e.button.x< slot1.x + slot1.w  && e.button.y > slot1.y && e.button.y < slot1.y + slot1.h)
                        {   
                            var = 1;
                            
                            if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                            {   
                                FILE *dummy,*save;
                                dummy=fopen("savefile1.txt","w");
                                fprintf(dummy,"");
                                fclose(dummy);

                                save=fopen("savefile1.txt","a");
                                fprintf(save,"%d\n",PAUSEFROM);
                                fprintf(save,"%s\n",PlayerName);
                                fprintf(save,"%d\n",currentscore);
                                if(PAUSEFROM == LEVEL1)
                                {
                                    fprintf(save,"%d\n",player1.isAlone);
                                    fprintf(save,"%d\n",player1.xpos);
                                    fprintf(save,"%d\n",P1.position.x );
                                    fprintf(save,"%d\n",P1.withplayer );
                                    for(int index=0;index<=4;index++)
                                        fprintf(save,"%d ",Enemy1.destroyed[index]);
                                    fprintf(save,"\n");
                                    for(int index=0;index<=4;index++)
                                        fprintf(save,"%d\n",Enemy1.enemy[index].x);
                                    fprintf(save,"\n");
                                    for(int index=0;index<=14;index++)
                                        fprintf(save,"%d ",h1.destroyed[index]);
                            }

                            else if(PAUSEFROM == LEVEL2)
                            {
                                fprintf(save,"%d\n",player2.isAlone);
                                fprintf(save,"%d\n",player2.xpos);
                                fprintf(save,"%d\n",P2.position.x );
                                fprintf(save,"%d\n",P2.withplayer );

                                for(int index=0;index<=4;index++)
                                    fprintf(save,"%d ",bird2.destroyed[index]);
                                fprintf(save,"\n");

                                for(int index=0;index<=4;index++)
                                    fprintf(save,"%d\n",bird2.bird[index].x);
                                fprintf(save,"\n");

                                for(int index=0;index<=14;index++)
                                    fprintf(save,"%d ",h2.destroyed[index]);
                            }

                            else if(PAUSEFROM == LEVEL3)
                            {
                                fprintf(save,"%d\n",player3.isAlone);
                                fprintf(save,"%d\n",player3.xpos);
                                fprintf(save,"%d\n",P3.position.x );
                                fprintf(save,"%d\n",P3.withplayer );

                                for(int index=0;index<=4;index++)
                                    fprintf(save,"%d ",Enemy3.destroyed[index]);
                                fprintf(save,"\n");

                                for(int index=0;index<=4;index++)
                                    fprintf(save,"%d\n",Enemy3.enemy[index].x);
                                fprintf(save,"\n");

                                for(int index=0;index<=7;index++)
                                    fprintf(save,"%d ",bird3.destroyed[index]);
                                fprintf(save,"\n");

                                for(int index=0;index<=7;index++)
                                    fprintf(save,"%d\n",bird3.bird[index].x);
                                fprintf(save,"\n");

                                for(int index=0;index<=14;index++)
                                    fprintf(save,"%d ",h3.destroyed[index]);
                            }
                            level=EXITFROM;
                            fclose(save);

                            //printf("%d  %d \n", e.button.x, e.button.y);
                        	}
                        	// }
                        }

            				if(e.button.x > slot2.x && e.button.x< slot2.x + slot2.w  && e.button.y > slot2.y && e.button.y < slot2.y + slot2.h)
	                      	  {   
	                            var = 2;
	                            
	                            if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
	                            {   
	                            	// startnow++;
                              //   if(startnow>1)
                              //   {
	                                FILE *dummy,*save;
	                            dummy=fopen("savefile2.txt","w");
	                            fprintf(dummy,"");
	                            fclose(dummy);

	                            save=fopen("savefile2.txt","a");
	                            fprintf(save,"%d\n",PAUSEFROM);
	                            fprintf(save,"%s\n",PlayerName);
	                            fprintf(save,"%d\n",currentscore);
	                            if(PAUSEFROM == LEVEL1)
	                            {
	                                fprintf(save,"%d\n",player1.isAlone);
	                                fprintf(save,"%d\n",player1.xpos);
	                                fprintf(save,"%d\n",P1.position.x );
	                                 fprintf(save,"%d\n",P1.withplayer );
	                                for(int index=0;index<=4;index++)
	                                    fprintf(save,"%d ",Enemy1.destroyed[index]);
	                                fprintf(save,"\n");
	                                for(int index=0;index<=4;index++)
	                                    fprintf(save,"%d\n",Enemy1.enemy[index].x);
	                                fprintf(save,"\n");
	                                for(int index=0;index<=14;index++)
	                                    fprintf(save,"%d ",h1.destroyed[index]);
	                            }

	                            else if(PAUSEFROM == LEVEL2)
	                            {
	                                fprintf(save,"%d\n",player2.isAlone);
	                                fprintf(save,"%d\n",player2.xpos);
	                                fprintf(save,"%d\n",P2.position.x );
	                                 fprintf(save,"%d\n",P2.withplayer );

	                                for(int index=0;index<=4;index++)
	                                    fprintf(save,"%d ",bird2.destroyed[index]);
	                                fprintf(save,"\n");

	                                for(int index=0;index<=4;index++)
	                                    fprintf(save,"%d\n",bird2.bird[index].x);
	                                fprintf(save,"\n");

	                                for(int index=0;index<=14;index++)
	                                    fprintf(save,"%d ",h2.destroyed[index]);
	                            }

	                            else if(PAUSEFROM == LEVEL3)
	                            {
	                                fprintf(save,"%d\n",player3.isAlone);
	                                fprintf(save,"%d\n",player3.xpos);
	                                fprintf(save,"%d\n",P3.position.x );
	                                 fprintf(save,"%d\n",P3.withplayer );

	                                for(int index=0;index<=4;index++)
	                                    fprintf(save,"%d ",Enemy3.destroyed[index]);
	                                fprintf(save,"\n");

	                                for(int index=0;index<=4;index++)
	                                    fprintf(save,"%d\n",Enemy3.enemy[index].x);
	                                fprintf(save,"\n");

	                                for(int index=0;index<=7;index++)
	                                    fprintf(save,"%d ",bird3.destroyed[index]);
	                                fprintf(save,"\n");

	                                for(int index=0;index<=7;index++)
	                                    fprintf(save,"%d\n",bird3.bird[index].x);
	                                fprintf(save,"\n");

	                                for(int index=0;index<=14;index++)
	                                    fprintf(save,"%d ",h3.destroyed[index]);
	                            }
	                            level=EXITFROM;
	                            fclose(save);
	                            }
	                        // }

	                            //printf("%d  %d \n", e.button.x, e.button.y);
	                        }

		    	            if(e.button.x > slot3.x && e.button.x< slot3.x + slot3.w  && e.button.y > slot3.y && e.button.y < slot3.y + slot3.h)
			                    {   
			                        var = 3;

			                        
			                        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
			                        {   
			                        	// startnow++;
                            //     if(startnow>1)
                            //     {
			                            FILE *dummy,*save;
		                            dummy=fopen("savefile3.txt","w");
		                            fprintf(dummy,"");
		                            fclose(dummy);

		                            save=fopen("savefile3.txt","a");
		                            fprintf(save,"%d\n",PAUSEFROM);
		                            fprintf(save,"%s\n",PlayerName);
		                            fprintf(save,"%d\n",currentscore);
		                            if(PAUSEFROM == LEVEL1)
		                            {
		                                fprintf(save,"%d\n",player1.isAlone);
		                                fprintf(save,"%d\n",player1.xpos);
		                                fprintf(save,"%d\n",P1.position.x );
		                                 fprintf(save,"%d\n",P1.withplayer );
		                                for(int index=0;index<=4;index++)
		                                    fprintf(save,"%d ",Enemy1.destroyed[index]);
		                                fprintf(save,"\n");
		                                for(int index=0;index<=4;index++)
		                                    fprintf(save,"%d\n",Enemy1.enemy[index].x);
		                                fprintf(save,"\n");
		                                for(int index=0;index<=14;index++)
		                                    fprintf(save,"%d ",h1.destroyed[index]);
		                            }

		                            else if(PAUSEFROM == LEVEL2)
		                            {
		                                fprintf(save,"%d\n",player2.isAlone);
		                                fprintf(save,"%d\n",player2.xpos);
		                                fprintf(save,"%d\n",P2.position.x );
		                                 fprintf(save,"%d\n",P2.withplayer );

		                                for(int index=0;index<=4;index++)
		                                    fprintf(save,"%d ",bird2.destroyed[index]);
		                                fprintf(save,"\n");

		                                for(int index=0;index<=4;index++)
		                                    fprintf(save,"%d\n",bird2.bird[index].x);
		                                fprintf(save,"\n");

		                                for(int index=0;index<=14;index++)
		                                    fprintf(save,"%d ",h2.destroyed[index]);
		                            }

		                            else if(PAUSEFROM == LEVEL3)
		                            {
		                                fprintf(save,"%d\n",player3.isAlone);
		                                fprintf(save,"%d\n",player3.xpos);
		                                fprintf(save,"%d\n",P3.position.x );
		                                 fprintf(save,"%d\n",P3.withplayer );

		                                for(int index=0;index<=4;index++)
		                                    fprintf(save,"%d ",Enemy3.destroyed[index]);
		                                fprintf(save,"\n");

		                                for(int index=0;index<=4;index++)
		                                    fprintf(save,"%d\n",Enemy3.enemy[index].x);
		                                fprintf(save,"\n");

		                                for(int index=0;index<=7;index++)
		                                    fprintf(save,"%d ",bird3.destroyed[index]);
		                                fprintf(save,"\n");

		                                for(int index=0;index<=7;index++)
		                                    fprintf(save,"%d\n",bird3.bird[index].x);
		                                fprintf(save,"\n");

		                                for(int index=0;index<=14;index++)
		                                    fprintf(save,"%d ",h3.destroyed[index]);
		                            }
		                            level=EXITFROM;
		                            fclose(save);
			                        }
			                    // }

	                        //printf("%d  %d \n", e.button.x, e.button.y);
                 	   				}

                        
                    }

	                    SDL_RenderClear(gRenderer);    
	                    gSaveTexture.render( 0, 0);
	                    if(var==1)
		                    {
		                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
		                    SDL_RenderDrawRect( gRenderer,  &slot1 ); 
		                    }
	                    if(var==2)
		                    {
		                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
		                    SDL_RenderDrawRect( gRenderer,  &slot2 ); 
		                    }
	                    if(var==3)
		                    {
		                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
		                    SDL_RenderDrawRect( gRenderer,  &slot3 ); 
		                    }

                        SDL_Color nameColor = { 255, 000, 000 };
                        FILE *l1,*l2,*l3;
                        l1=fopen("savefile1.txt","r");
                        
                        //int level,score;
                        char name1[20],name2[20],name3[20],loadlevel[3],loadscore[10];
                        fscanf(l1,"%s",loadlevel);
                        fscanf(l1,"%s",name1);
                        fscanf(l1,"%s",loadscore);

                        //Render text
                        
                        gName1Texture.loadFromRenderedText( name1 , nameColor );
                        gName1Texture.render( 150 ,  130+25 );
                        gName1Texture.loadFromRenderedText( loadlevel , nameColor );
                        gName1Texture.render( 350+45 ,  130+25 );
                        gName1Texture.loadFromRenderedText( loadscore , nameColor );
                        gName1Texture.render( 550+45 ,  130+25 );
                        fclose(l1);

                        l2=fopen("savefile2.txt","r");
                        fscanf(l2,"%s",loadlevel);
                        fscanf(l2,"%s",name2);
                        fscanf(l1,"%s",loadscore);
                        gName2Texture.loadFromRenderedText( name2 , nameColor );
                        gName2Texture.render( 150 ,  130 +100+25);
                        gName2Texture.loadFromRenderedText( loadlevel , nameColor );
                        gName2Texture.render( 350+45 ,  230+25 );
                        gName2Texture.loadFromRenderedText( loadscore , nameColor );
                        gName2Texture.render( 550+45 ,  230+25 );
                        fclose(l2);

                        l3=fopen("savefile3.txt","r");
                        fscanf(l3,"%s",loadlevel);
                        fscanf(l3,"%s",name3);
                        fscanf(l1,"%s",loadscore);
                        gName3Texture.loadFromRenderedText( name3 , nameColor );
                        gName3Texture.render( 150 ,  130 +200+25);
                        gName3Texture.loadFromRenderedText( loadlevel , nameColor );
                        gName3Texture.render( 350+45 ,  330+25 );
                        gName3Texture.loadFromRenderedText( loadscore , nameColor );
                        gName3Texture.render( 550+45 ,  330+25 );
                        fclose(l3);

	                    SDL_RenderPresent(gRenderer);
                	}



                var=0;
                while( level==LOADMENU )
                {
                	SDL_Rect slot1;
                	slot1.x = 100;
                	slot1.y = 130+20;
                	slot1.h = 80-25;
                	slot1.w = 600; 

                	SDL_Rect slot2;
                	slot2.x = 100;
                	slot2.y = 130+100+20;
                	slot2.h = 80-25;
                	slot2.w = 600;

                	SDL_Rect slot3;
                	slot3.x = 100;
                	slot3.y = 130+100+100+20;
                	slot3.h = 80-25;
                	slot3.w = 600;


                	
                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                          // quit = true;x
                           //break;
                            EXITFROM = level;
                           level = EXIT;
                        }
                        
                        
                        //Handle input for the dot
                        if( e.key.keysym.sym == SDLK_ESCAPE  &&  e.key.repeat == 0)
                        {
                            //int x;
                            startnow++;
                            if(startnow>1)
                            level=MENU;
                        }

                        if(e.button.x > slot1.x && e.button.x< slot1.x + slot1.w  && e.button.y > slot1.y && e.button.y < slot1.y + slot1.h)
                        {   
                            var = 1;
                            
                            if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                            {   
                                	isloaded=1;
                                    FILE *load;
                                    load=fopen("savefile1.txt","r");
                                    fscanf(load,"%d",&level);
                                    fscanf(load,"%s",PlayerName);
                                    fscanf(load,"%d",&currentscore);
                                    

                                    if(level == LEVEL1)
                                    {
                                        LOADED1=1;

                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedenemydest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&Enemy1.enemy[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }

                                    else if(level == LEVEL2)
                                    {
                                        LOADED2=1;
                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedbirddest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&bird2.bird[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }

                                    else if(level == LEVEL3)
                                    {
                                        LOADED3=1;
                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedenemydest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&Enemy3.enemy[index].x);

                                        for(int index=0;index<=7;index++)
                                            fscanf(load,"%d",&savedbirddest[index]);

                                        for(int index=0;index<=7;index++)
                                            fscanf(load,"%d",&bird3.bird[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }
                            }

                            //printf("%d  %d \n", e.button.x, e.button.y);
                        }
            		if(e.button.x > slot2.x && e.button.x< slot2.x + slot2.w  && e.button.y > slot2.y && e.button.y < slot2.y + slot2.h)
                      	  {   
                            var = 2;
                            
                            if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                            {   
                                isloaded=1;
                                    FILE *load;
                                    load=fopen("savefile2.txt","r");
                                    fscanf(load,"%d",&level);
                                    fscanf(load,"%s",PlayerName);
                                    fscanf(load,"%d",&currentscore);
                                    

                                    if(level == LEVEL1)
                                    {
                                        LOADED1=1;

                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedenemydest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&Enemy1.enemy[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }

                                    else if(level == LEVEL2)
                                    {
                                        LOADED2=1;
                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedbirddest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&bird2.bird[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }

                                    else if(level == LEVEL3)
                                    {
                                        LOADED3=1;
                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedenemydest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&Enemy3.enemy[index].x);

                                        for(int index=0;index<=7;index++)
                                            fscanf(load,"%d",&savedbirddest[index]);

                                        for(int index=0;index<=7;index++)
                                            fscanf(load,"%d",&bird3.bird[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }
                            }

                            //printf("%d  %d \n", e.button.x, e.button.y);
                        }
    	            if(e.button.x > slot3.x && e.button.x< slot3.x + slot3.w  && e.button.y > slot3.y && e.button.y < slot3.y + slot3.h)
	                    {   
	                        var = 3;
	                        
	                        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
	                        {   
	                            isloaded=1;
                                    FILE *load;
                                    load=fopen("savefile3.txt","r");
                                    fscanf(load,"%d",&level);
                                    fscanf(load,"%s",PlayerName);
                                    fscanf(load,"%d",&currentscore);
                                    

                                    if(level == LEVEL1)
                                    {
                                        LOADED1=1;

                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedenemydest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&Enemy1.enemy[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }

                                    else if(level == LEVEL2)
                                    {
                                        LOADED2=1;
                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedbirddest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&bird2.bird[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }

                                    else if(level == LEVEL3)
                                    {
                                        LOADED3=1;
                                        fscanf(load,"%d",&savedalone);
                                        fscanf(load,"%d",&savedplayerxpos);
                                        fscanf(load,"%d",&savedpartnerxpos);
                                        fscanf(load,"%d",&savedpartneralone);
                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&savedenemydest[index]);

                                        for(int index=0;index<=4;index++)
                                            fscanf(load,"%d",&Enemy3.enemy[index].x);

                                        for(int index=0;index<=7;index++)
                                            fscanf(load,"%d",&savedbirddest[index]);

                                        for(int index=0;index<=7;index++)
                                            fscanf(load,"%d",&bird3.bird[index].x);

                                        for(int index=0;index<=14;index++)
                                            fscanf(load,"%d",&savedheartdest[index]);
                                    }
	                        }

	                        //printf("%d  %d \n", e.button.x, e.button.y);
                 	   }

                        
                    }

                    SDL_RenderClear(gRenderer);    
                    gLoadTexture.render( 0, 0);
                    if(var==1)
	                    {
	                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	                    SDL_RenderDrawRect( gRenderer,  &slot1 ); 
	                    }
                    if(var==2)
	                    {
	                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	                    SDL_RenderDrawRect( gRenderer,  &slot2 ); 
	                    }
                    if(var==3)
	                    {
	                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	                    SDL_RenderDrawRect( gRenderer,  &slot3 ); 
	                    }
                        SDL_Color nameColor = { 255, 000, 000 };
                        FILE *l1,*l2,*l3;
                        l1=fopen("savefile1.txt","r");

                        //int level,score;
                        char name1[20],name2[20],name3[20],loadlevel[3],loadscore[10];
                        fscanf(l1,"%s",loadlevel);
                        fscanf(l1,"%s",name1);
                        fscanf(l1,"%s",loadscore);

                        //Render text
                        
                        gName1Texture.loadFromRenderedText( name1 , nameColor );
                        gName1Texture.render( 150 ,  130+25 );
                        gName1Texture.loadFromRenderedText( loadlevel , nameColor );
                        gName1Texture.render( 350+45 ,  130+25 );
                        gName1Texture.loadFromRenderedText( loadscore , nameColor );
                        gName1Texture.render( 550+45 ,  130+25 );
                        fclose(l1);

                        l2=fopen("savefile2.txt","r");
                        fscanf(l2,"%s",loadlevel);
                        fscanf(l2,"%s",name2);
                        fscanf(l1,"%s",loadscore);
                        gName2Texture.loadFromRenderedText( name2 , nameColor );
                        gName2Texture.render( 150 ,  130 +100+25);
                        gName2Texture.loadFromRenderedText( loadlevel , nameColor );
                        gName2Texture.render( 350+45 ,  230+25 );
                        gName2Texture.loadFromRenderedText( loadscore , nameColor );
                        gName2Texture.render( 550+45 ,  230+25 );
                        fclose(l2);

                        l3=fopen("savefile3.txt","r");
                        fscanf(l3,"%s",loadlevel);
                        fscanf(l3,"%s",name3);
                        fscanf(l1,"%s",loadscore);
                        gName3Texture.loadFromRenderedText( name3 , nameColor );
                        gName3Texture.render( 150 ,  130 +200+25);
                        gName3Texture.loadFromRenderedText( loadlevel , nameColor );
                        gName3Texture.render( 350+45 ,  330+25 );
                        gName3Texture.loadFromRenderedText( loadscore , nameColor );
                        gName3Texture.render( 550+45 ,  330+25 );
                        fclose(l3);


                        SDL_RenderPresent(gRenderer);
                    
                    

                }

                var =0;

                while( level==EXIT )
                {
                    SDL_RenderClear(gRenderer);    
                    gExitTexture.render( 0, 0);
                    SDL_Rect opt1;
                    opt1.x = 115;
                    opt1.y = 215;
                    opt1.h = 50;
                    opt1.w = 100;
                    if(var==1)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &opt1 ); 
                    }
                    SDL_Rect opt2;
                    opt2.x = 548;
                    opt2.y = 215;
                    opt2.h = 50;
                    opt2.w = 100;
                    if(var==2)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &opt2 ); 
                    }
                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                           quit = true;
                           //break;
                           
                        }
     
                        if(e.button.x > opt1.x && e.button.x< opt1.x + opt1.w  && e.button.y > opt1.y && e.button.y < opt1.y + opt1.h)
                            {   
                                var = 1;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    quit = true;
                                    level = -100;
                                    //level = MENU;
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }
                        if(e.button.x > opt2.x && e.button.x< opt2.x + opt2.w  && e.button.y > opt2.y && e.button.y < opt2.y + opt2.h)
                            {   
                                var = 2;
                                
                                if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                                {   
                                    level = EXITFROM;
                                }

                                //printf("%d  %d \n", e.button.x, e.button.y);
                            }

                        
                    }

                    
                    
                    SDL_RenderPresent(gRenderer);
                    

                }
               while( level==PAUSE )
            {
                isloaded=0;

                
                //Handle events on queue

                SDL_RenderClear(gRenderer);    
                gPauseTexture.render( 0, 0);

                SDL_Rect cont;
                    cont.x = 332;
                    cont.y = 155;
                    cont.h = 26;
                    cont.w = 100;
                    if(var==1)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &cont ); 
                    }
                SDL_Rect save;
                    save.x = 332;
                    save.y = 155+ 50 -3;
                    save.h = 26;
                    save.w = 120;
                    if(var==2)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &save ); 
                    }
                SDL_Rect help;
                    help.x = 330;
                    help.y = 155+100 -6;
                    help.h = 26;
                    help.w = 120;
                    if(var==3)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &help ); 
                    }

                SDL_Rect ret;
                    ret.x = 330;
                    ret.y = 155+150 -8;
                    ret.h = 26;
                    ret.w = 180;
                    if(var==5)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &ret ); 
                    }
                SDL_Rect exit;
                    exit.x = 332;
                    exit.y = 155+200 -10;
                    exit.h = 26;
                    exit.w = 100;
                    if(var==4)
                    {
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer,  &exit ); 
                    }
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                       //quit = true;
                       //break;
                        EXITFROM = level;
                       level = EXIT;
                    }

                    if(e.button.x > cont.x && e.button.x< cont.x + cont.w  && e.button.y > cont.y && e.button.y < cont.y + cont.h)
                    {   
                        var = 1;
                        
                        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                        {   
                            //continue;
                            if(PAUSEFROM == LEVEL1)
                            {
                                init1=1;
                            }
                            if(PAUSEFROM == LEVEL2)
                            {
                                init2=1;
                            }
                            if(PAUSEFROM == LEVEL3)
                            {
                                init3=1;
                            }
                            level=PAUSEFROM;
                        }

                    }
                    if(e.button.x > save.x && e.button.x< save.x + save.w  && e.button.y > save.y && e.button.y < save.y + save.h)
                    {   
                        var = 2;
                        
                        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                        {   
                        	EXITFROM = level;
                        	level = SAVEMENU;
                        }
                        

                    }
                    if(e.button.x > help.x && e.button.x< help.x + help.w  && e.button.y > help.y && e.button.y < help.y + help.h)
                    {   
                        var = 3;
                        
                        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                        {   
                            HELPFROM = level;
                            level = HELP;
                        }

                    }
                    if(e.button.x > ret.x && e.button.x< ret.x + ret.w  && e.button.y > ret.y && e.button.y < ret.y + ret.h)
                    {   
                        var = 5;
                        
                        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                        {   
                            EXITFROM = level;
                            level = RETURN;
                        }

                    }
                    if(e.button.x > exit.x && e.button.x< exit.x + exit.w  && e.button.y > exit.y && e.button.y < exit.y + exit.h)
                    {   
                        var = 4;
                        
                        if(e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                        {   
                            //quit = true;
                            EXITFROM = level;
                            level = EXIT;
                        }

                    }

                    
                }


                    
                SDL_RenderPresent(gRenderer);

            }


           

                if(level==LEVEL1)
                {
                    Mix_PlayMusic(BGSound,-1);
                    
                    int numberofspike = 5;
                    int numberofhearts = 15;
                    
                    int numberofenemy = 5;
                    playerlife=10;
                    partnerlife=10;

                    if(!init1)
                    
                    {
                        
                        
                        P1.init();
                        Enemy1.init(numberofenemy);
                        Enemy1.enemy[0].x= 400;
                        Enemy1.enemy[1].x= 650;
                        Enemy1.enemy[2].x= 850;
                        Enemy1.enemy[3].x= 1050;
                        Enemy1.enemy[4].x= 1850;
                        //Enemy1.range[0]=0;

                        
                      
                        player1.init();
                        
                        fire1.init(numberofspike, 250);

                        int fire1posx[]={250,450,780,1080,1500};

                        for(int index=0;index<=4;index++)
                        {
                            fire1.fire[index].x = fire1posx[index];
                        }

                        
                        
                        h1.init(numberofhearts);
                        int heart1posx[]={250,450,780,1080,1500,1800,1200,1300,700,1600,320,600,500,900};
                        int heart1posy[]={820,820,820,820,865,865,865,865,865,865,865,865,865,865};

                        for(int index=0;index<=14;index++)
                        {
                            h1.heart[index].x=heart1posx[index];
                            h1.heart[index].y=heart1posy[index];
                        }
                        
                        init1= 1;



                    }

                    if(isloaded && LOADED1)
                    {
                        player1.isAlone = savedalone;
                        player1.xpos = savedplayerxpos;
                        P1.position.x = savedpartnerxpos;
                        P1.withplayer = savedpartneralone;
                        for(int index=0;index<=14;index++)
                        {
                            h1.destroyed[index]=savedheartdest[index];                            
                        }
                        for(int index=0;index<=4;index++)
                        {
                            Enemy1.destroyed[index]=savedenemydest[index];
                        }
                    }

                    Enemy1.speed[0]= 1;
                    Enemy1.speed[1]= 1;
                    Enemy1.speed[2]= 1;
                    Enemy1.speed[3]= 2;
                    Enemy1.speed[4]= 2;



                    while( level==LEVEL1)
                    {
                        
                        //Handle events on queue
                        while( SDL_PollEvent( &e ) != 0 )
                        {
                            //User requests quit
                            if( e.type == SDL_QUIT )
                            {
                               //quit = true;
                                EXITFROM = level;
                               //break;
                               level = EXIT;
                            }
                            if(e.key.keysym.sym==SDLK_KP_2 && e.key.repeat == 0)
                            {
                                level=LEVEL2START;
                            }
                            if(e.key.keysym.sym==SDLK_ESCAPE)
                            {
                                PAUSEFROM = level;
                                level=PAUSE;
                            }
                            //Handle input for the dot
                            player1.handleEvent( e );
                        }
                        //SDL_SetRenderDrawColor()
                        frameStart = SDL_GetTicks();
                        currentframe = (SDL_GetTicks()/frameDelay) % FPS ;

                        SDL_RenderClear(gRenderer);
                         player1.setCamera( camera );
                       // gSpriteCoupleTexture.render(0,0, &camera);
                        gBackground1.render( 0, 0, &camera);
                        h1.render(numberofhearts, camera, player1.isAlone);
                        Enemy1.render(numberofenemy, currentframe, camera );

                        high();
                        gTextTexture.render(  SCREEN_WIDTH - 160, 20 );
                        gScoreTexture.render(  SCREEN_WIDTH - 70, 20 );
                        
                        player1.interact(P1.position);
                        P1.render(camera, player1.isAlone, player1.leftat, player1.flipflag);
                        fire1.render(numberofspike, camera);

                        player1.attack();

                        //bird1.render(numberofbirds, currentframe, camera );

                        for(int i =0; i<numberofspike;i++)
                        {
                            if(checkCollision(player1.pCollider, fire1.fire[i]))
                            {
                              //  printf("DEAD\n");
                                level=OVER1;
                                

                            }
                            if(checkCollision(P1.position, fire1.fire[i]))
                            {
                             //   printf("DEAD\n");
                                level=OVER2;
                            }
                        }



                        player1.render(currentframe,camera);

                        for(int i =0; i<numberofenemy;i++)
                        {
                            if(!Enemy1.destroyed[i])
                            {
                                if(checkCollision(player1.pCollider, Enemy1.enemy[i]))
                                {

                                 //   printf("DEAD times %d\n", playerlife);
                                    playerlife--;
                                    if(playerlife==0)
                                    {
                                        level=OVER1;
                                    }
                                }

                                if(checkCollision(P1.position, Enemy1.enemy[i]))
                                {
                                 //   printf("Partner dead times %d\n", partnerlife);
                                    partnerlife--;
                                    if(partnerlife==0)
                                    {
                                        level =OVER2;
                                    }
                                }

                            }
                        }


                        Enemy1.destroy(numberofenemy, player1.attackbox);
                        h1.destroy(numberofhearts, player1.pCollider, player1.isAlone);
                        //bird1.destroy(numberofbirds, player1.attackbox);

                        frameTime = SDL_GetTicks() - frameStart;
                          //printf("%d\n", currentframe );

                        if(frameDelay > frameTime)
                        {
                            SDL_Delay(frameDelay - frameTime);
                        }
                        SDL_RenderPresent(gRenderer);

                        if(player1.xpos + 32 >= LEVEL_WIDTH-1 && player1.isAlone==0)
                        {
                            level=LEVEL1END;
                        }
                        //heart += 100;

                    }

                }
                
                while( level==LEVEL1END )
                {
                    Mix_HaltMusic();
                    //Handle events on queue
                    
                    //save=fopen("save.txt","")
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                           //quit = true;
                           //break;
                            EXITFROM = level;
                           level = EXIT;
                        }
     
                        //Handle input for the dot
                        if(e.key.keysym.sym==SDLK_RETURN)
                        {
                            startnow++;
                            if(startnow>1)
                            level=LEVEL2START;
                        }
                        
                    }

                    SDL_RenderClear(gRenderer);
                    
                    level1EndTexture.render( 0, 0);
                    SDL_RenderPresent(gRenderer);

                }
                startnow=0;

                while( level==LEVEL2START )
                {
                    Mix_HaltMusic();
                    //Handle events on queue
                    
                    //save=fopen("save.txt","")
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                           //quit = true;
                           //break;
                            EXITFROM = level;
                           level = EXIT;
                        }
     
                        //Handle input for the dot
                        if(e.key.keysym.sym==SDLK_RETURN)
                        {
                            
                            startnow++;
                            if(startnow>1)
                            level=LEVEL2;
                           
                        }
                        
                    }

                    SDL_RenderClear(gRenderer);
                    
                    level2StartTexture.render( 0, 0);
                    SDL_RenderPresent(gRenderer);

                }

                if(level==LEVEL2)
                {
                    Mix_PlayMusic(BGSound,-1);
                    playerlife=10;
                    partnerlife=10;
                    int numberofbirds= 5;  
                    int numberofspike = 5;
                    int numberofhearts = 15;
                    if(!init2)
                    {
                    P2.init();

                    
                    //Enemy Enemy;  

                    
                    bird2.init(numberofbirds);
                    bird2.bird[0].x = 400;
                    bird2.bird[1].x = 600;
                    bird2.bird[2].x = 800;
                    bird2.bird[3].x = 1600;
                    bird2.bird[4].x = 1800;

                    player2.init();
                    
                    fire2.init(numberofspike, 250);

                    fire2.fire[0].x= 250;
                    fire2.fire[1].x= 450;
                    fire2.fire[2].x=  780;
                    fire2.fire[3].x= 1080;
                    fire2.fire[4].x= 1500;

                    
                    
                    h2.init(numberofhearts);
                    h2.heart[0].x = 250;
                    h2.heart[0].y = 820;
                    h2.heart[1].x = 450;
                    h2.heart[1].y = 820;
                    h2.heart[2].x = 780;
                    h2.heart[2].y = 820;
                    h2.heart[3].x = 1080;
                    h2.heart[3].y = 820;
                    h2.heart[4].x = 1500;
                    h2.heart[4].y = 820;
                    h2.heart[5].x = 1800;
                    h2.heart[6].x = 1200;
                    h2.heart[7].x = 1300;
                    h2.heart[8].x = 700;
                    h2.heart[9].x = 1600;
                    h2.heart[10].x = 1600;
                    h2.heart[11].x = 320;
                    h2.heart[12].x = 600;
                    h2.heart[13].x = 500;
                    h2.heart[14].x = 900;
                    init2=1;
                    }
                    if(isloaded && LOADED2)
                    {
                        player2.isAlone = savedalone;
                        player2.xpos = savedplayerxpos;
                        P2.position.x = savedpartnerxpos;
                        P2.withplayer = savedpartneralone;
                        for(int index=0;index<=14;index++)
                        {
                            h2.destroyed[index]=savedheartdest[index];                            
                        }
                        for(int index=0;index<=4;index++)
                        {
                            bird2.destroyed[index]=savedbirddest[index];
                        }
                    }

                    bird2.speed[0] = 1;
                    bird2.speed[1] = 1;
                    bird2.speed[2] = 1;
                    bird2.speed[3] = 2;
                    bird2.speed[4] = 2;




                    while( level==LEVEL2)
                    {
                        
                        //Handle events on queue
                        while( SDL_PollEvent( &e ) != 0 )
                        {
                            //User requests quit
                            if( e.type == SDL_QUIT )
                            {
                               //quit = true;
                               //break;
                                EXITFROM = level;
                               level = EXIT;
                            }
                            if(e.key.keysym.sym==SDLK_KP_3 && e.key.repeat == 0)
                            {
                                level=LEVEL3START;
                            }

                            if(e.key.keysym.sym==SDLK_ESCAPE)
                            {
                                PAUSEFROM = level;
                                level=PAUSE;
                            }

                            //Handle input for the dot
                            player2.handleEvent( e );
                        }
                        //SDL_SetRenderDrawColor()
                        frameStart = SDL_GetTicks();
                        currentframe = (SDL_GetTicks()/frameDelay) % FPS ;

                        SDL_RenderClear(gRenderer);
                         player2.setCamera( camera );
                       // gSpriteCoupleTexture.render(0,0, &camera);
                        gBackground2.render( 0, 0, &camera);
                        h2.render(numberofhearts, camera, player2.isAlone);

                        high();
                        gTextTexture.render(  SCREEN_WIDTH - 160, 20 );
                        gScoreTexture.render(  SCREEN_WIDTH - 70, 20 );
                        
                        player2.interact(P2.position);
                        P2.render(camera, player2.isAlone, player2.leftat, player2.flipflag);
                        fire2.render(numberofspike, camera);

                        player2.attack();

                        bird2.render(numberofbirds, currentframe, camera );

                        for(int i =0; i<numberofspike;i++)
                        {
                            if(checkCollision(player2.pCollider, fire2.fire[i]))
                            {
                              //  printf("DEAD\n");
                                level=OVER1;
                                

                            }
                            if(checkCollision(P2.position, fire2.fire[i]))
                            {
                               // printf("DEAD\n");
                                level=OVER2;
                            }
                        }
                        player2.render(currentframe,camera);


                        for(int i =0; i<numberofbirds;i++)
                        {
                            if(!bird2.destroyed[i])
                            {
                                if(checkCollision(player2.pCollider, bird2.bird[i]))
                                {

                                //    printf("DEAD times %d\n", playerlife);
                                    playerlife--;
                                    if(playerlife==0)
                                    {
                                        level=OVER1;
                                    }
                                }

                                if(checkCollision(P2.position, bird2.bird[i]))
                                {
                                //    printf("Partner dead times %d\n", partnerlife);
                                    partnerlife--;
                                    if(partnerlife==0)
                                    {
                                        level =OVER2;
                                    }
                                }

                            }
                        }
                        h2.destroy(numberofhearts, player2.pCollider, player2.isAlone);
                        bird2.destroy(numberofbirds, player2.attackbox);

                        frameTime = SDL_GetTicks() - frameStart;
                          //printf("%d\n", currentframe );

                        if(frameDelay > frameTime)
                        {
                            SDL_Delay(frameDelay - frameTime);
                        }
                        SDL_RenderPresent(gRenderer);

                        if(player2.xpos + 32 >= LEVEL_WIDTH-1 && player2.isAlone==0)
                        {
                            level=LEVEL3START;
                        }
                        //heart += 100;

                    }

                }
                startnow=0;
                while( level==LEVEL3START )
                {
                    Mix_HaltMusic();
                    //Handle events on queue
                    
                    //save=fopen("save.txt","")
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                           //quit = true;
                           //break;
                            EXITFROM = level;
                           level = EXIT;
                        }
     
                        //Handle input for the dot
                        if(e.key.keysym.sym==SDLK_RETURN)
                        {
                            
                            startnow++;
                            if(startnow>1)
                            level=LEVEL3;
                           
                        }
                        
                    }

                    SDL_RenderClear(gRenderer);
                    
                    level3StartTexture.render( 0, 0);
                    SDL_RenderPresent(gRenderer);

                }
            

            if(level==LEVEL3)
            {
                Mix_PlayMusic(BGSound,-1);
                //SDL_RenderClear(gRenderer);
                playerlife=10;
                partnerlife=10;
                int numberofbirds= 8; 
                int numberofhearts = 15;
                int numberofspike = 8;
                int numberofenemy=5;
                    
                if(!init3)
                {
                

                player3.init();
                Enemy3.init(numberofenemy);
                P3.init();
                
                h3.init(numberofhearts);
                h3.heart[0].x = 200;
                h3.heart[0].y = 825;
                h3.heart[1].x = 325;
                h3.heart[1].y = 820;
                h3.heart[2].x = 480;
                h3.heart[2].y = 820;
                h3.heart[3].x = 680;
                h3.heart[3].y = 820;
                h3.heart[4].x = 1500;
                h3.heart[4].y = 820;
                h3.heart[5].x = 1800;
                h3.heart[6].x = 1200;
                h3.heart[7].x = 1300;
                h3.heart[8].x = 700;
                h3.heart[9].x = 1600;
                h3.heart[10].x = 1600;
                h3.heart[11].x = 1320;
                h3.heart[12].x = 600;
                h3.heart[13].x = 950;
                h3.heart[14].x = 1720;
                h3.heart[14].y = 800;


                 
                bird3.init(numberofbirds);
                bird3.bird[0].x = 400;
                bird3.bird[1].x = 600;
                bird3.bird[2].x = 800;
                bird3.bird[3].x = 1600;
                bird3.bird[4].x = 1800;
                bird3.bird[5].x = 1500;
                bird3.bird[6].x = 300;
                bird3.bird[7].x = 1000;
                //printf("%d %d %d %d %d \n", bird.bird[0].x, bird.bird[1].x, bird.bird[2].x, bird.bird[3].x, bird.bird[4].x   );

                
                
                fire3.init(numberofspike, 150);
                fire3.fire[0].x= 250;
                fire3.fire[1].x= 450;
                fire3.fire[2].x=  780;
                fire3.fire[3].x= 1580;
                fire3.fire[4].x= 1200;
                fire3.fire[5].x= 1570;
                fire3.fire[6].x= 1280;
                fire3.fire[7].x= 1500;
                init3=1;
                }
                if(isloaded && LOADED3)
                    {
                        player3.isAlone = savedalone;
                        player3.xpos = savedplayerxpos;
                        P3.position.x = savedpartnerxpos;
                        P3.withplayer = savedpartneralone;
                        for(int index=0;index<=14;index++)
                        {
                            h3.destroyed[index]=savedheartdest[index];                            
                        }
                        for(int index=0;index<=4;index++)
                        {
                            bird3.destroyed[index]=savedbirddest[index];
                        }
                        for(int index=0;index<=4;index++)
                        {
                            Enemy3.destroyed[index]=savedenemydest[index];
                        }
                    }



            while( level==LEVEL3 )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                       //quit = true;
                       //break;
                        EXITFROM = level;
                       level = EXIT;
                    }
                    if(e.key.keysym.sym==SDLK_ESCAPE)
                            {
                                PAUSEFROM = level;
                                level=PAUSE;
                            }
 
                    //Handle input for the dot
                    player3.handleEvent( e );
                }

                //SDL_SetRenderDrawColor()
                frameStart = SDL_GetTicks();
                currentframe = (SDL_GetTicks()/frameDelay) % FPS ;
                SDL_RenderClear(gRenderer);
                 player3.setCamera( camera );
               // gSpriteCoupleTexture.render(0,0, &camera);
                gBackground3.render( 0, 0, &camera);
                h3.render(numberofhearts, camera, player3.isAlone);
                high();
                gTextTexture.render(  SCREEN_WIDTH - 160, 20 );
                gScoreTexture.render(  SCREEN_WIDTH - 70, 20 );
              
                player3.attack();
                player3.interact(P3.position);
                //wall.render();
                //wall2.render();
                player3.render(currentframe,camera );
                fire3.render(numberofspike, camera);
                Enemy3.render(numberofenemy, currentframe, camera );


                for(int i =0; i<numberofspike;i++)
                {
                    if(checkCollision(player3.pCollider, fire3.fire[i]))
                    {
                     //   printf("DEAD\n");
                        level=OVER1;
                    }
                    if(checkCollision(P3.position, fire3.fire[i]))
                    {
                       // printf("DEAD\n");
                        level=OVER2;
                    }
                }

                for(int i =0; i<numberofenemy;i++)
                {
                    if(!Enemy3.destroyed[i])
                    {
                        if(checkCollision(player3.pCollider, Enemy3.enemy[i]))
                        {

                            //printf("DEAD times %d\n", playerlife);
                            playerlife--;
                            if(playerlife==0)
                            {
                                level=OVER1;
                            }
                        }

                        if(checkCollision(P3.position, Enemy3.enemy[i]))
                        {
                           // printf("Partner dead times %d\n", partnerlife);
                            partnerlife--;
                            if(partnerlife==0)
                            {
                                level =OVER2;
                            }
                        }

                    }
                }

                bird3.render(numberofbirds, currentframe, camera );
                h3.destroy(numberofhearts, player3.pCollider, player3.isAlone);

                for(int i =0; i<numberofbirds;i++)
                {
                    if(!bird3.destroyed[i])
                    {
                        if(checkCollision(player3.pCollider, bird3.bird[i]))
                        {

                           // printf("DEAD times %d\n", playerlife);
                            playerlife--;
                            if(playerlife==0)
                            {
                                level=OVER1;
                            }
                        }

                        if(checkCollision(P3.position, bird3.bird[i]))
                        {
                           // printf("Partner dead times %d\n", partnerlife);
                            partnerlife--;
                            if(partnerlife==0)
                            {
                                level =OVER2;
                            }
                        }

                    }
                }
                Enemy3.destroy(numberofenemy, player3.attackbox);
                bird3.destroy(numberofbirds, player3.attackbox);
                //Enemy.end(player.pCollider);
                P3.render(camera, player3.isAlone, player3.leftat, player3.flipflag);
                frameTime = SDL_GetTicks() - frameStart;
                  //printf("%d\n", currentframe );

                if(frameDelay > frameTime)
                {
                    SDL_Delay(frameDelay - frameTime);
                }

                SDL_RenderPresent(gRenderer);
                //currentscore++;
                if(player3.xpos+32 >= LEVEL_WIDTH -1 && player3.isAlone==0)
                {
                    //quit=true;
                    level = ENDING;
                }
            }
        }
            
            if(level==OVER1)
            {
                Mix_HaltMusic();
                init1=0;
                init2=0;
                init3=0;
                LOADED1=0;
                LOADED2=0;
                LOADED3=0;
                isloaded=0;
                vector<pair<int,string >> SortScore;
                SortScore.push_back(make_pair(currentscore,PlayerName)) ;
                string AName;
                rp = fopen("HighScore.txt","r");
                for(int index=0;index<5;index++)
                {
                    int DummyVar, DummyScore;
                    char DummyName[100],tempName[100];
                    fgets(tempName,sizeof(tempName),rp);
                    sscanf(tempName,"%s %d",DummyName,&DummyScore);
                    AName=DummyName;
                    SortScore.push_back(make_pair(DummyScore,DummyName));
                }
                fclose(rp);
                sort(SortScore.begin(),SortScore.end(),greater<pair<int,string> >());
                wp=fopen("HighScore.txt","w");

                fprintf(wp,"%s","");
                fclose(wp);
             
                ap=fopen("HighScore.txt","a");
                for(int index=0;index<5;index++)
                {
                    char sname[100];
                    for(int j=0;j<SortScore[index].second.size();j++)
                        sname[j]=SortScore[index].second[j];
                    sname[SortScore[index].second.size()]=0;
                    fprintf(ap,"%s %d\n",sname,SortScore[index].first);
                }
                fclose(ap);
            while(level==OVER1)
            {
                init1=0;
                init2=0;
                init3=0;
                
                
                SDL_RenderClear(gRenderer);
                //if(playerlife==0)
                gameover1.render( 0, 0);
                //else if(partnerlife==0) gameover2.render( 0, 0);

                gScoreTexture.render(  SCREEN_WIDTH/2+5, SCREEN_HEIGHT/2 + 38 );
                SDL_RenderPresent(gRenderer);
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                       //quit = true;
                        EXITFROM = level;
                       level = EXIT;
                       
                    }

                    if(e.key.keysym.sym==SDLK_RETURN)
                        {
                            level=MENU;
                        }
                }
            }
        }

            if(level== OVER2)
            {
                Mix_HaltMusic();
                init1=0;
                init2=0;
                init3=0;
                LOADED1=0;
                LOADED2=0;
                LOADED3=0;
                isloaded=0;
                vector<pair<int,string >> SortScore;
                SortScore.push_back(make_pair(currentscore,PlayerName)) ;
                rp = fopen("HighScore.txt","r");
                string AName;
                for(int index=0;index<5;index++)
                {
                    int DummyScore;
                    char DummyName[100],tempName[100];
                    
                    fgets(tempName,sizeof(tempName),rp);
                    sscanf(tempName,"%s %d",DummyName,&DummyScore);
                    AName=DummyName;
                    SortScore.push_back(make_pair(DummyScore,DummyName));
                }
                fclose(rp);
                sort(SortScore.begin(),SortScore.end(),greater<pair<int,string> >());
                wp=fopen("HighScore.txt","w");

                fprintf(wp,"%s","");
                fclose(wp);
             
                ap=fopen("HighScore.txt","a");
                for(int index=0;index<5;index++)
                {
                    char sname[100];
                    for(int j=0;j<SortScore[index].second.size();j++)
                        sname[j]=SortScore[index].second[j];
                    sname[SortScore[index].second.size()]=0;
                    fprintf(ap,"%s %d\n",sname,SortScore[index].first);
                }
                fclose(ap);
            while(level==OVER2)
            {
                
                SDL_RenderClear(gRenderer);
                gameover2.render( 0, 0);
                gScoreTexture.render(  SCREEN_WIDTH/2+10, SCREEN_HEIGHT/2 + 70 );
                SDL_RenderPresent(gRenderer);
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                       // quit = true;
                       EXITFROM = level;
                       level = EXIT;
                    }

                    if(e.key.keysym.sym==SDLK_RETURN)
                        {
                            level=MENU;
                        }
                }
            }
        }


            if(level== ENDING)
                {
                    Mix_HaltMusic();
                    init1=0;
                    init2=0;
                    init3=0;
                    LOADED1=0;
                    LOADED2=0;
                    LOADED3=0;
                    isloaded=0;
                    vector<pair<int,string >> SortScore;
                    SortScore.push_back(make_pair(currentscore,PlayerName)) ;
                    rp = fopen("HighScore.txt","r");
                    string AName;
                    for(int index=0;index<5;index++)
                    {
                        int DummyScore;
                        char DummyName[100],tempName[100];
                        
                        fgets(tempName,sizeof(tempName),rp);
                        sscanf(tempName,"%s %d",DummyName,&DummyScore);
                        AName=DummyName;
                        SortScore.push_back(make_pair(DummyScore,DummyName));
                    }
                    fclose(rp);
                    sort(SortScore.begin(),SortScore.end(),greater<pair<int,string> >());
                    wp=fopen("HighScore.txt","w");

                    fprintf(wp,"%s","");
                    fclose(wp);
                 
                    ap=fopen("HighScore.txt","a");
                    for(int index=0;index<5;index++)
                    {
                        char sname[100];
                        for(int j=0;j<SortScore[index].second.size();j++)
                            sname[j]=SortScore[index].second[j];
                        sname[SortScore[index].second.size()]=0;
                        fprintf(ap,"%s %d\n",sname,SortScore[index].first);
                    }
                    fclose(ap);
                while(level==ENDING)
                {
                    
                    SDL_RenderClear(gRenderer);
                    levelendTexture.render( 0, 0);
                    gScoreTexture.render(  430, 280  );
                    SDL_RenderPresent(gRenderer);
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                           // quit = true;
                           EXITFROM = level;
                           level = EXIT;
                        }

                        if(e.key.keysym.sym==SDLK_RETURN)
                            {
                                level=MENU;
                            }
                    }
                }
            }

            while( level==HELP )
            {
                Mix_HaltMusic();

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                       // quit = true;
                       EXITFROM = level;
                       level = EXIT;
                       //break;
                       
                    }
 
                    //Handle input for the dot
                    if(e.key.keysym.sym==SDLK_RETURN)
                    {
                        level=HELPFROM;
                    }
                }

                SDL_RenderClear(gRenderer);                
                gHelpTexture.render( 0, 0);
                SDL_RenderPresent(gRenderer);
            }

            while( level==CREDITS )
            {
                Mix_HaltMusic();

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                       //quit = true;
                       EXITFROM = level;
                       level = EXIT;
                       //break;
                       
                    }
 
                    //Handle input for the dot
                    if(e.key.keysym.sym==SDLK_RETURN)
                    {
                        level=MENU;
                    }
                }

                SDL_RenderClear(gRenderer);                
                gCreditTexture.render( 0, 0);
                SDL_RenderPresent(gRenderer);
            }
            while( level==SCOREMENU )
            {
                Mix_HaltMusic();

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                       // quit = true;
                       EXITFROM = level;
                       level = EXIT;
                       //break;
                       
                    }
 
                    //Handle input for the dot
                    if(e.key.keysym.sym==SDLK_RETURN)
                    {
                        level=MENU;
                    }
                }

                SDL_RenderClear(gRenderer);                
                gMenuScore.render( 0, 0);
                HallOfFame();
                //gHighestTexture.render( 100, 145);
                SDL_RenderPresent(gRenderer);
            }

    }
            quit=true;
        }
    }
 
    //Free resources and close SDL
    close();
 
    return 0;
}