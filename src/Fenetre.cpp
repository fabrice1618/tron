#include "Fenetre.h"

Fenetre::Fenetre()
{
    //constructeur
    gettimeofday(&last_refresh, NULL);
}

bool Fenetre::refresh_tick()
{
    struct timeval now;
    int diff_ms;

    gettimeofday(&now, NULL);
    diff_ms =
        (now.tv_sec - last_refresh.tv_sec) * 1000 +
        (now.tv_usec - last_refresh.tv_usec) / 1000;

    if (diff_ms > REFRESH_DELAY) {
        last_refresh = now;
        return true;
    }

    return false;
}

void Fenetre::initialise(Uint32 flags)
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();

    if ( SDL_CreateWindowAndRenderer( screen_w, screen_h, SDL_WINDOW_SHOWN, &m_window, &m_renderer ) != 0 )
        throw InitError();
}

void Fenetre::set_size(int screen_w, int screen_h)
{
    this->screen_w = screen_w;
    this->screen_h = screen_h;
}

void Fenetre::add_color(int composante_red, int composante_green, int composante_blue)
{
        if (palette_count < PALETTE_SIZEMAX) {

            palette[palette_count].composante_red = composante_red;
            palette[palette_count].composante_green = composante_green;
            palette[palette_count].composante_blue = composante_blue;

            palette_count++;
        }
}

void Fenetre::add_keymap_event(Uint8 key_scancode, int evenement)
{
        if (keyboard_event_count < KEYMAP_SIZEMAX) {

            keyboard_event[keyboard_event_count].key_scancode = key_scancode;
            keyboard_event[keyboard_event_count].evenement = evenement;

            keyboard_event_count++;
        }
}

void Fenetre::read_keyboard_event()
{
    keystate = SDL_GetKeyboardState(NULL);

    // Parcourir toutes les touches enregistrées
    for (int touche=0; touche < keyboard_event_count; touche++) {

        if (keystate[keyboard_event[touche].key_scancode]) {
            // Si la touche est pressée, récupérer événement correspondant
            int evenement = keyboard_event[touche].evenement;

            // Ajouter événement dans liste événements
            if (event_list_count < KEYMAP_SIZEMAX) {

                event_list[event_list_count] = evenement;
                event_list_count++;
            }
        }
    }
}

int Fenetre::get_event()
{
    if (event_list_count == 0)
        return NO_EVENT;

    return event_list[--event_list_count];
}

void Fenetre::efface()
{
    // Clear the window with a black background
    SDL_SetRenderDrawColor( m_renderer, palette[0].composante_red, palette[0].composante_green, palette[0].composante_blue, 255 );
    SDL_RenderClear( m_renderer );

    // Show the window
    SDL_RenderPresent( m_renderer );
}

void Fenetre::render()
{
    SDL_RenderPresent( m_renderer );
}

void Fenetre::rectangle(int pos_x, int pos_y, int size_x, int size_y, int couleur)
{
    SDL_Rect cell;

    cell.x = pos_x;
    cell.y = pos_y;
    cell.w = size_x;
    cell.h = size_y;

    SDL_SetRenderDrawColor( m_renderer,
                           palette[couleur].composante_red,
                           palette[couleur].composante_green,
                           palette[couleur].composante_blue,
                           255 );

    SDL_RenderFillRect( m_renderer, &cell );
}

// Verification si fermeture de la fenetre
bool Fenetre::close_event()
{
    SDL_Event evenements;
    bool close_event = false;

    while ( !close_event && SDL_PollEvent(&evenements) ) {
        if ((evenements.type==SDL_WINDOWEVENT) && (evenements.window.event == SDL_WINDOWEVENT_CLOSE))
            close_event = true;
        }

    return close_event;
}

Fenetre::~Fenetre()
{
    //destructeur
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}
