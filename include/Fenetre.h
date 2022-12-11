#ifndef FENETRE_H
#define FENETRE_H

#include <SDL2/SDL.h>
#include <sys/time.h>

/*
Classe gérant l'affichage
*/

#include "InitError.h"

#define PALETTE_SIZEMAX 16
#define KEYMAP_SIZEMAX  25
#define NO_EVENT 0

#define REFRESH_DELAY 100

struct Color {
    int composante_red;
    int composante_green;
    int composante_blue;
};

struct Keymap_event {
    Uint8 key_scancode;
    int evenement;
};


class Fenetre
{
    public:
        Fenetre();
        bool refresh_tick();
        void initialise(Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_TIMER);
        void set_size(int screen_w, int screen_h);
        void add_color(int composante_red, int composante_green, int composante_blue);
        void efface();
        void render();
        void rectangle(int pos_x, int pos_y, int size_x, int size_y, int couleur = 0);
        bool close_event();
        void add_keymap_event(Uint8 key_scancode, int evenement);
        void read_keyboard_event();
        int get_event();
        virtual ~Fenetre();

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        // Taille de la fenetre
        int screen_w = 640;
        int screen_h = 480;
        // Couleurs enregistrées dans la palette
        struct Color palette[PALETTE_SIZEMAX];
        int palette_count = 0;
        // Touches enregistrées avec événement correspondant
        struct Keymap_event keyboard_event[KEYMAP_SIZEMAX];
        int keyboard_event_count = 0;
        // Etat actuel du clavier
        const Uint8* keystate = nullptr;
        // liste des événements
        int event_list[KEYMAP_SIZEMAX];
        int event_list_count = 0;
        // Timer refresh window
        struct timeval last_refresh;

};

#endif // FENETRE_H
