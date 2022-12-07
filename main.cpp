#include <exception>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <ctime>

#define GRID_X 100
#define GRID_Y 50

#define CELL_ZONE 5
#define CELL_SIZE 4

#define BIKE1_COLOR_R 255
#define BIKE1_COLOR_G 192
#define BIKE1_COLOR_B 203

#define BIKE2_COLOR_R 18
#define BIKE2_COLOR_G 14
#define BIKE2_COLOR_B 252

struct Bike {
    int pos_x;
    int pos_y;
    int dir_x;
    int dir_y;
};

enum game_state {
	GAME_LOOP,
	GAME_END1,
	GAME_END2,
	GAME_QUIT,
};

// Variables globales
char tron_grid[GRID_Y][GRID_X];
struct Bike bike1;
struct Bike bike2;

/*
Exception : InitError
*/
class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}

const char * InitError::what() const throw()
{
    return msg.c_str();
}

/*
Classe gérant l'affichage
*/

class SDL
{
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
public:
    SDL( Uint32 flags = 0 );
    virtual ~SDL();
    void draw();
};


SDL::SDL( Uint32 flags )
{
    int screen_w = GRID_X * CELL_ZONE;
    int screen_h = GRID_Y * CELL_ZONE;

    if ( SDL_Init( flags ) != 0 )
        throw InitError();

    if ( SDL_CreateWindowAndRenderer( screen_w, screen_h, SDL_WINDOW_SHOWN,
                                      &m_window, &m_renderer ) != 0 )
        throw InitError();
}

SDL::~SDL()
{
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}

void SDL::draw()
{
    SDL_Rect cell;
    int cell_gap = (CELL_ZONE - CELL_SIZE) / 2;

    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;

    // Clear the window with a black background
    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
    SDL_RenderClear( m_renderer );

    // Show the window
    SDL_RenderPresent( m_renderer );

    // Dessiner la grille
    for ( int y = 0; y < GRID_Y; y++ ) {
        for ( int x = 0; x < GRID_X; x++ ) {

            if (tron_grid[y][x] == 1) {
                // Couleur des cellules
                SDL_SetRenderDrawColor( m_renderer, BIKE1_COLOR_R, BIKE1_COLOR_G, BIKE1_COLOR_B, 255 );

                cell.x = ( x * CELL_ZONE ) + cell_gap;
                cell.y = ( y * CELL_ZONE ) + cell_gap;

                SDL_RenderFillRect( m_renderer, &cell );
            }

            if (tron_grid[y][x] == 2) {
                // Couleur des cellules
                SDL_SetRenderDrawColor( m_renderer, BIKE2_COLOR_R, BIKE2_COLOR_G, BIKE2_COLOR_B, 255 );

                cell.x = ( x * CELL_ZONE ) + cell_gap;
                cell.y = ( y * CELL_ZONE ) + cell_gap;

                SDL_RenderFillRect( m_renderer, &cell );
            }
        }
    }

    SDL_RenderPresent( m_renderer );
}

enum game_state next_state()
{

    bike1.pos_x += bike1.dir_x;
    bike1.pos_y += bike1.dir_y;

    if ( (bike1.pos_x < 0 || bike1.pos_x > GRID_X) ||
    (bike1.pos_y < 0 || bike1.pos_y > GRID_Y) ) {
        return GAME_END1;
    }

    bike2.pos_x += bike2.dir_x;
    bike2.pos_y += bike2.dir_y;

    if ( (bike2.pos_x < 0 || bike2.pos_x > GRID_X) ||
    (bike2.pos_y < 0 || bike2.pos_y > GRID_Y) ) {
        return GAME_END2;
    }

    tron_grid[bike1.pos_y][bike1.pos_x] = 1;
    tron_grid[bike2.pos_y][bike2.pos_x] = 2;

    return GAME_LOOP;
}


void init_grid()
{
    for (int y=0; y < GRID_Y; y++) {
        for (int x=0; x < GRID_X; x++) {
            tron_grid[y][x] = 0;
        }
    }
}



int main( int argc, char * argv[] )
{
    SDL_Event evenements;
    enum game_state state = GAME_LOOP;

    try
    {
        SDL sdl( SDL_INIT_VIDEO | SDL_INIT_TIMER );
        init_grid();

        bike1.pos_x = 0;
        bike1.pos_y = GRID_Y / 2;
        bike1.dir_x = 1;
        bike1.dir_y = 0;


        bike2.pos_x = GRID_X;
        bike2.pos_y = GRID_Y / 2;
        bike2.dir_x = -1;
        bike2.dir_y = 0;

        while (state != GAME_QUIT) {
        	// Fermeture de la fenetre
            while ( (state != GAME_QUIT) && SDL_PollEvent(&evenements) ) {
                if ((evenements.type==SDL_WINDOWEVENT) && (evenements.window.event == SDL_WINDOWEVENT_CLOSE))
                    state = GAME_QUIT;
            }

            // Traiter les événements clavier
            const Uint8 *keystate = SDL_GetKeyboardState(NULL);

            if (keystate[SDL_SCANCODE_ESCAPE]) {
//                std::cout << "ESC" << std::endl;
                state = GAME_QUIT;
            }

            if (keystate[SDL_SCANCODE_DOWN]) {
//                std::cout << "DOWN" << std::endl;
                bike2.dir_y = 1;
            }
            if (keystate[SDL_SCANCODE_UP]) {
//                std::cout << "UP" << std::endl;
                bike2.dir_y = -1;
            }

            if (keystate[SDL_SCANCODE_RIGHT]) {
                bike2.dir_x = 1;
            }
            if (keystate[SDL_SCANCODE_LEFT]) {
                bike2.dir_x = -1;
            }


            if (keystate[SDL_SCANCODE_A]) {
                bike1.dir_y = 1;
            }
            if (keystate[SDL_SCANCODE_Q]) {
                bike1.dir_y = -1;
            }
            if (keystate[SDL_SCANCODE_D]) {
                bike1.dir_x = 1;
            }
            if (keystate[SDL_SCANCODE_S]) {
                bike1.dir_x = -1;
            }

            if (state != GAME_QUIT) {
                sdl.draw();
                state = next_state();
                SDL_Delay( 100 );
            }

        }

        return EXIT_SUCCESS;
    }
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }

    return EXIT_FAILURE;
}
