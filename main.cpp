#include <iostream>

#include "InitError.h"
#include "Fenetre.h"

#define GRID_X 200
#define GRID_Y 100

#define CELL_ZONE 5
#define CELL_SIZE 5

#define CLEAR_COLOR_R 0
#define CLEAR_COLOR_G 0
#define CLEAR_COLOR_B 0

#define BIKE1_COLOR_R 255
#define BIKE1_COLOR_G 192
#define BIKE1_COLOR_B 203

#define BIKE2_COLOR_R 18
#define BIKE2_COLOR_G 14
#define BIKE2_COLOR_B 252

#define MUR_COLOR_R 255
#define MUR_COLOR_G 0
#define MUR_COLOR_B 255

// Direction dans laquelle peut aller un bike
enum bike_direction {
    HAUT,
    DROITE,
    BAS,
    GAUCHE
};

// Evenements en provenance du clavier
enum evenement {
    EVENT_QUIT = 1,
    EVENT_BIKE1_HAUT,
    EVENT_BIKE1_DROITE,
    EVENT_BIKE1_BAS,
    EVENT_BIKE1_GAUCHE,
    EVENT_BIKE2_HAUT,
    EVENT_BIKE2_DROITE,
    EVENT_BIKE2_BAS,
    EVENT_BIKE2_GAUCHE
};

// Un bike (position et direction)
struct Bike {
    int pos_x;
    int pos_y;
    enum bike_direction direction;
};

// Les états du jeu
enum game_state {
	GAME_LOOP,
	GAME_VICTOIRE1,
	GAME_VICTOIRE2,
	GAME_END,
	GAME_QUIT,
};

// Variables globales
Fenetre fenetre;        // La fenetre permettant l'affichage

char tron_grid[GRID_Y][GRID_X];     // La grille du jeu
struct Bike bike1;                  // Bike1 et bike2
struct Bike bike2;

void configure_fenetre();

// Recuperer composante X de la direction
int get_dir_x(enum bike_direction direction)
{
    if (direction == HAUT || direction == BAS)
        return 0;

    if (direction == DROITE)
        return 1;

    if (direction == GAUCHE)
        return -1;

    return 0;
}

// Recuperer composante Y de la direction
int get_dir_y(enum bike_direction direction)
{
    if (direction == HAUT)
        return -1;

    if (direction == BAS)
        return 1;

    if (direction == DROITE || direction == GAUCHE)
        return 0;

    return 0;
}

// Changement de la direction. Les demi-tours sont interdits
enum bike_direction change_direction(enum bike_direction direction_initiale, enum bike_direction changement)
{
    // Changements de direction non autorisés, les déplacements à 180 degrés
    if ( (direction_initiale == HAUT && changement == BAS) ||
         (direction_initiale == BAS && changement == HAUT) ||
         (direction_initiale == DROITE && changement == GAUCHE) ||
         (direction_initiale == GAUCHE && changement == DROITE) )

        return direction_initiale;

    // modification de direction autorisée
    return changement;
}

// Dessiner une cellule avec sa couleur
void draw_cell(int grid_x, int grid_y, int color)
{
    int cell_gap = (CELL_ZONE - CELL_SIZE) / 2;
    int cell_x = ( grid_x * CELL_ZONE ) + cell_gap;
    int cell_y = ( grid_y * CELL_ZONE ) + cell_gap;

    fenetre.rectangle(cell_x, cell_y, CELL_SIZE, CELL_SIZE, color);
}

// Calcul prochain état du jeu
enum game_state next_state()
{

    // Deplacer bike1
    bike1.pos_x += get_dir_x(bike1.direction);
    bike1.pos_y += get_dir_y(bike1.direction);

    if ( tron_grid[bike1.pos_y][bike1.pos_x] != 0 ) {
        return GAME_VICTOIRE2;
    } else {
        tron_grid[bike1.pos_y][bike1.pos_x] = 1;
        draw_cell(bike1.pos_x, bike1.pos_y, 1);
    }

    // Deplacer bike2
    bike2.pos_x += get_dir_x(bike2.direction);
    bike2.pos_y += get_dir_y(bike2.direction);

    if ( tron_grid[bike2.pos_y][bike2.pos_x] != 0 ) {
        return GAME_VICTOIRE1;
    } else {
        tron_grid[bike2.pos_y][bike2.pos_x] = 2;
        draw_cell(bike2.pos_x, bike2.pos_y, 2);
    }

    fenetre.render();   // mise à jour affichage

    return GAME_LOOP;
}

// Initialiser la grille avec des zéros
void init_grid()
{
    for (int y=0; y < GRID_Y; y++) {
        for (int x=0; x < GRID_X; x++) {
            tron_grid[y][x] = 0;
        }
    }
}

// Dessine le mur autour du jeu
void dessine_mur()
{
    for (int y=0; y < GRID_Y; y++) {
        tron_grid[y][0] = 3;
        tron_grid[y][GRID_X-1] = 3;

        draw_cell(0, y, 3);
        draw_cell(GRID_X-1, y, 3);
    }

    for (int x=0; x < GRID_X; x++) {
        tron_grid[0][x] = 3;
        tron_grid[GRID_Y-1][x] = 3;

        draw_cell(x, 0, 3);
        draw_cell(x, GRID_Y-1, 3);
    }

    fenetre.render();
}

// Programme principal
int main( int argc, char * argv[] )
{
    enum game_state state = GAME_LOOP;
    int evenement;

    configure_fenetre();
    init_grid();    // Remplir la grille à 0 et 3 pour les murs.
    dessine_mur();  // dessiner et 3 pour les murs.

    // Bike1 initialisation
    bike1.pos_x = 1;
    bike1.pos_y = GRID_Y / 2;
    bike1.direction = DROITE;

    // Bike2 initialisation
    bike2.pos_x = GRID_X - 1;
    bike2.pos_y = GRID_Y / 2;
    bike2.direction = GAUCHE;

    while (state == GAME_LOOP) {

        // Verification si utilisateur clique sur la "croix"
        if ( fenetre.close_event() )
            state = GAME_QUIT;

        // Lire le clavier et transformer en evenements
        fenetre.read_keyboard_event();

        while ( (evenement = fenetre.get_event()) != NO_EVENT) {

            switch (evenement) {
                // ESC fermeture fenetre
                case EVENT_QUIT:
                    state = GAME_QUIT;
                    break;

                // Mouvements de bike1
                case EVENT_BIKE1_HAUT:
                    bike1.direction = change_direction(bike1.direction, HAUT);
                    break;
                case EVENT_BIKE1_DROITE:
                    bike1.direction = change_direction(bike1.direction, DROITE);
                    break;
                case EVENT_BIKE1_BAS:
                    bike1.direction = change_direction(bike1.direction, BAS);
                    break;
                case EVENT_BIKE1_GAUCHE:
                    bike1.direction = change_direction(bike1.direction, GAUCHE);
                    break;

                // Mouvements de bike2
                case EVENT_BIKE2_HAUT:
                    bike2.direction = change_direction(bike2.direction, HAUT);
                    break;
                case EVENT_BIKE2_DROITE:
                    bike2.direction = change_direction(bike2.direction, DROITE);
                    break;
                case EVENT_BIKE2_BAS:
                    bike2.direction = change_direction(bike2.direction, BAS);
                    break;
                case EVENT_BIKE2_GAUCHE:
                    bike2.direction = change_direction(bike2.direction, GAUCHE);
                    break;

                // evenement non reconnu
                default:
                    break;
            }
        }

        // Si le delai de 100 ms est écoulé, prochain état du jeu
        if (state != GAME_QUIT && fenetre.refresh_tick() ) {
            state = next_state();
        }

    }

    return EXIT_SUCCESS;

}

// Configuration de l'affichage
void configure_fenetre()
{
    try {
        // Initalise la fenetre
        fenetre.set_size(GRID_X * CELL_ZONE, GRID_Y * CELL_ZONE);
        fenetre.initialise();

        // Crée les couleurs de la palette
        fenetre.add_color(CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B);     // 0 = Couleur pour effacer écran)
        fenetre.add_color(BIKE1_COLOR_R, BIKE1_COLOR_G, BIKE1_COLOR_B);     // 1 = bike 1
        fenetre.add_color(BIKE2_COLOR_R, BIKE2_COLOR_G, BIKE2_COLOR_B);     // 2 = Bike 2
        fenetre.add_color(MUR_COLOR_R, MUR_COLOR_G, MUR_COLOR_B);     // 3 = Couleur du mur

        // Création des associations touche clavier -> événement
        // Escape -> fin application
        fenetre.add_keymap_event(SDL_SCANCODE_ESCAPE, EVENT_QUIT);
        // Mouvements bike1
        fenetre.add_keymap_event(SDL_SCANCODE_UP,       EVENT_BIKE1_HAUT);
        fenetre.add_keymap_event(SDL_SCANCODE_RIGHT,    EVENT_BIKE1_DROITE);
        fenetre.add_keymap_event(SDL_SCANCODE_DOWN,     EVENT_BIKE1_BAS);
        fenetre.add_keymap_event(SDL_SCANCODE_LEFT,     EVENT_BIKE1_GAUCHE);
        // Mouvements bike2
        fenetre.add_keymap_event(SDL_SCANCODE_A,    EVENT_BIKE2_HAUT);
        fenetre.add_keymap_event(SDL_SCANCODE_D,    EVENT_BIKE2_DROITE);
        fenetre.add_keymap_event(SDL_SCANCODE_Q,    EVENT_BIKE2_BAS);
        fenetre.add_keymap_event(SDL_SCANCODE_S,    EVENT_BIKE2_GAUCHE);

    }
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;

        exit( EXIT_FAILURE );
    }

}
