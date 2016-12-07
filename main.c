#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL2_rotozoom.h>

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "config.h"
#include "Car.h"
#include "SDL2_Interface.h"

// état du jeu et du programme
char program = PROGRAM_RUN;
char state = MENU_STATE;
char game_state;
char game_mode;
char game_difficulty;
char nb_voies;
char var_begin;
int autopilot;

// statistique du joueurs
char pos = INIT_POS;
float speed;
int score;
FILE* stats;
float moy_speed;
int save_score;
char text_nom[11];
int compteur;
int topos;
int changeNeeded;

// gestion evenement
unsigned int start_pressed = 0;
unsigned int stop_pressed = 0;
unsigned int global_ticks = 0;
unsigned int stats_ticks = 0;
unsigned int anim_ticks = 0;
SDL_Keycode key_pressed = NULL;
const Uint8* state_keyboard;

// window et renderer            
SDL_Window* window = NULL;
SDL_Renderer *renderer = NULL;         

// textures
SDL_Texture *texture_menu_background = NULL; 
SDL_Texture *texture_road = NULL; 
SDL_Texture *texture_car_red = NULL;
SDL_Texture *texture_car_yellow = NULL;
SDL_Texture *texture_speed = NULL;
SDL_Surface *surface_needle = NULL;
SDL_Surface *surface_needle_displayed = NULL;
SDL_Texture *texture_needle = NULL;
SDL_Texture* texture_text_title = NULL;
SDL_Texture* texture_text_jouer = NULL;
SDL_Texture* texture_text_options = NULL;
SDL_Texture* texture_text_quitter = NULL;
SDL_Texture* texture_text_classic = NULL;
SDL_Texture* texture_text_retro = NULL;
SDL_Texture* texture_button_on = NULL;
SDL_Texture* texture_button_off = NULL;
SDL_Texture* texture_text_3voies = NULL;
SDL_Texture* texture_text_4voies = NULL;
SDL_Texture* texture_text_5voies = NULL;
SDL_Texture* texture_text_rejouer = NULL;
SDL_Texture* texture_text_topscore = NULL;
SDL_Texture* texture_text_menu = NULL;
SDL_Texture* texture_text_background_menuig = NULL;
SDL_Texture* texture_anim = NULL;
SDL_Texture* texture_player = NULL;
SDL_Texture* texture_autopilot = NULL;

// sounds
Mix_Chunk *crash = NULL;
Mix_Chunk *background_music = NULL;
Mix_Chunk *start = NULL;

// positions
SDL_Rect rect_car_pos = { CAR_POS_2_3-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };

// 3 voies
SDL_Rect rect_car_pos1_3 = { CAR_POS_1_3-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos2_3 = { CAR_POS_2_3-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos3_3 = { CAR_POS_3_3-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos4_3 = { CAR_POS_3_3+CAR_POS_3_3-CAR_POS_2_3-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };

// 4 voies
SDL_Rect rect_car_pos1_4 = { CAR_POS_1_4-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos2_4 = { CAR_POS_2_4-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos3_4 = { CAR_POS_3_4-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos4_4 = { CAR_POS_4_4-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };

// 5 voies
SDL_Rect rect_car_pos1_5 = { CAR_POS_1_5-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos2_5 = { CAR_POS_2_5-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos3_5 = { CAR_POS_3_5-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos4_5 = { CAR_POS_4_5-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };
SDL_Rect rect_car_pos5_5 = { CAR_POS_5_5-CAR_W/2, CAR_POS_Y, CAR_W, CAR_H };

// getion du menu
int move = 0;
SDL_Rect rect_button_jouer = { WINDOW_W/2 - 75, WINDOW_H/2-25-60, 150, 50 };
SDL_Rect rect_button_options = { WINDOW_W/2 - 75, WINDOW_H/2-25, 150, 50 };
SDL_Rect rect_button_quitter = { WINDOW_W/2 - 75, WINDOW_H/2-25+60, 150, 50 };

SDL_Rect rect_button_nom = { 	WINDOW_W/2 - 75, 				WINDOW_H/2-25+60-50-10, 150, 50 };
SDL_Rect rect_button_save = { 	WINDOW_W/2 - 75 + 150 + 20, 	WINDOW_H/2-25+60-50-10, 150, 50 };

SDL_Rect rect_button_rejouer = { 	WINDOW_W/2 - 75 - 150 - 20, 	WINDOW_H/2-25+60, 150, 50 };
SDL_Rect rect_button_topscore = { 	WINDOW_W/2 - 75,				WINDOW_H/2-25+60, 150, 50 };
SDL_Rect rect_button_menu = { 		WINDOW_W/2 - 75 + 150 + 20, 	WINDOW_H/2-25+60, 150, 50 };

// gestion du background qui défile
SDL_Rect rect_bg_1 = { 0, BG_1_POS_INIT_Y, WINDOW_W, WINDOW_H };
SDL_Rect rect_bg_2 = { 0, BG_2_POS_INIT_Y, WINDOW_W, WINDOW_H };
SDL_Rect rect_bg_3 = { 0, BG_3_POS_INIT_Y, WINDOW_W, WINDOW_H };

// position speed et needle
SDL_Rect rect_speed = { WINDOW_W-SPEED_W-20, WINDOW_H-SPEED_H-20, SPEED_W, SPEED_H };
SDL_Rect rect_needle = { 0, 0, NEEDLE_W, NEEDLE_H };

// IA cars
int id_car = 0;
Cars_list ia_cars = NULL;
unsigned int last_pop = 0;
Car* toDestroy = NULL;

// calcul fps
int fps = 0;
int frame = 0;
unsigned int timer = 0;

//ttf
TTF_Font* font;
TTF_Font* font_title;
TTF_Font* font_menu;
TTF_Font* font_ingame;
SDL_Color black = { 0, 0, 0 };
SDL_Rect rect_text_title;
SDL_Rect rect_text_jouer;
SDL_Rect rect_text_options;
SDL_Rect rect_text_quitter;
SDL_Rect rect_text_classic;
SDL_Rect rect_text_retro;
SDL_Rect rect_text_5voies;
SDL_Rect rect_text_3voies;
SDL_Rect rect_text_4voies;
SDL_Rect rect_text_menu;
SDL_Rect rect_text_topscore;
SDL_Rect rect_text_rejouer;
SDL_Rect rect_text_player;
SDL_Rect rect_text_autopilot;
SDL_Rect rect_anim_posin;
char buttons[10] = { 0 };

// pour l'animation explosion : 
SDL_Rect rect_anim[16];
int anim;
int explo;

// topscores
Score_list scores_list_3 = NULL;
Score_list scores_list_4 = NULL;
Score_list scores_list_5 = NULL;
Score_list scores_list_3_c = NULL;
Score_list scores_list_4_c = NULL;
Score_list scores_list_5_c = NULL;

/* Prototypes des fonctions */

/*
 * Gestion des entrées clavier et souris	
 */
void event(); 

/*
 * Calcul des positions des voitures, du background, du joueur, de la vitesse...
 */
void work();
void work_ia();
void work_background();

/*
 * Affichage
 */
void display();

/*
 * Initialisation de la SDL2, de SDL_image, de SDL_ttf, de SDL_mixer
 */
void init();

/*
 * Initialisation des variables (pour chaque début de partie)	
 */
void init_var();

/*
 * Chargement des textures menus et text
 */
void load();

/*
 * Chargement des textures pour le jeu
 */
void load_game();

/*
 * Chargement des scores
 */
void loadTopscores();

/*
 * Vider toutes les variables (pointeurs...)
 */
void clear();


/*  Le code */

void event() {
	
	SDL_Event event;

   	SDL_PollEvent(&event);

   	state_keyboard = SDL_GetKeyboardState(NULL);

	if(state == GAME_STATE) 
	{
		if(game_state == GAME_PLAY) 
		{
			if(game_mode == MODE_RETRO) 
			{
				if(autopilot == OFF)
				switch (event.type)
				{
					case SDL_QUIT:
					    program = PROGRAM_STOP;
				    	break;
				    case SDL_KEYDOWN:
				    	if(event.key.keysym.sym == SDLK_LEFT) {
				    		switch(nb_voies)
				    		{
				    			case VOIES_3:
				    				if(pos > POS_2) pos--;
				    				break;
				    			case VOIES_4:
				    				if(pos > POS_1) pos--;
				    				break;
				    			case VOIES_5:
				    				if(pos > POS_1) pos--;
				    				break;
				    		}
				    	}
				    	else if(event.key.keysym.sym == SDLK_RIGHT) {
				    		switch(nb_voies)
				    		{
				    			case VOIES_3:			    				
				    				if(pos < POS_4) pos++;
				    				break;
				    			case VOIES_4:			    				
				    				if(pos < POS_4) pos++;
				    				break;
				    			case VOIES_5:			    				
				    				if(pos < POS_5) pos++;
				    				break;	
				    		}
				    	}
				    	else if(event.key.keysym.sym == SDLK_UP && start_pressed == 0) {
				    		start_pressed = SDL_GetTicks();
				    		stop_pressed = 0;
				    		key_pressed = SDLK_UP;
				    	}
				    	else if(event.key.keysym.sym == SDLK_DOWN && start_pressed == 0) {
				    		start_pressed = SDL_GetTicks();
				    		stop_pressed = 0;
				    		key_pressed = SDLK_DOWN;
				    	}
				    	else if(event.key.keysym.sym == SDLK_ESCAPE) {
				    		state = MENU_STATE;
				    		move = 0;
							SDL_ShowCursor(SDL_ENABLE);	
				    	}
				    	break;	
				    case SDL_KEYUP:
				    	if(event.key.keysym.sym == SDLK_UP) {
				    		start_pressed = 0;
				    		stop_pressed = SDL_GetTicks();
				    		key_pressed = SDLK_l;
				    	}
				    	else if(event.key.keysym.sym == SDLK_DOWN) {
				    		start_pressed = 0;
				    		key_pressed = SDLK_l;
				    	}
				    	break;
			   	}
			   	else {
			   		switch (event.type)
					{
						case SDL_QUIT:
						    program = PROGRAM_STOP;
					    	break;
					    case SDL_KEYDOWN:
					    	if(event.key.keysym.sym == SDLK_ESCAPE) {
					    		state = MENU_STATE;
					    		move = 0;
								SDL_ShowCursor(SDL_ENABLE);	
					    	}
					    	break;	
					}
			   	}
			}
			else if(game_mode == MODE_CLASSIC) 
			{
				if(autopilot == OFF) 
				switch (event.type)
				{
					case SDL_QUIT:
					    program = PROGRAM_STOP;
				    	break;
				    case SDL_KEYDOWN:
				    	if(event.key.keysym.sym == SDLK_UP && start_pressed == 0) {
				    		start_pressed = SDL_GetTicks();
				    		stop_pressed = 0;
				    		key_pressed = SDLK_UP;
				    	}
				    	else if(event.key.keysym.sym == SDLK_DOWN && start_pressed == 0) {
				    		start_pressed = SDL_GetTicks();
				    		stop_pressed = 0;
				    		key_pressed = SDLK_DOWN;
				    	}
				    	else if(event.key.keysym.sym == SDLK_ESCAPE) {
				    		state = MENU_STATE;
				    		move = 0;				
							SDL_ShowCursor(SDL_ENABLE);	    		
				    	}
				    	break;	
				    case SDL_KEYUP:
				    	if(event.key.keysym.sym == SDLK_UP) {
				    		start_pressed = 0;
				    		stop_pressed = SDL_GetTicks();
				    		key_pressed = SDLK_l;
				    	}
				    	else if(event.key.keysym.sym == SDLK_DOWN) {
				    		start_pressed = 0;
				    		key_pressed = SDLK_l;
				    	}
				    	break;
			   	}
			   	else {
			   		switch (event.type)
					{
						case SDL_QUIT:
						    program = PROGRAM_STOP;
					    	break;
					    case SDL_KEYDOWN:
					    	if(event.key.keysym.sym == SDLK_ESCAPE) {
					    		state = MENU_STATE;
					    		move = 0;
								SDL_ShowCursor(SDL_ENABLE);	
					    	}
					    	break;	
					}
			   	}
			}
		}
		else if(game_state == GAME_OVER) {
			switch (event.type)
			{
				case SDL_QUIT:
				    program = PROGRAM_STOP;
				   	break;
			    case SDL_KEYDOWN:
			    	if(event.key.keysym.sym == SDLK_ESCAPE) {
			    		state = MENU_STATE;
			    		move = 0;
			    	}
			    	else {
			    		if(event.key.keysym.sym == SDLK_SPACE && compteur < 10) {				    		
				    		text_nom[compteur] = ' ';
				    		compteur++;
				    		text_nom[compteur] = '\0';  		
				    	}
				    	else if(event.key.keysym.sym == SDLK_BACKSPACE && compteur > 1) {
				    		compteur--;
				    		text_nom[compteur] = '\0';
				    	}
				    	else if(event.key.keysym.sym == SDLK_BACKSPACE && compteur == 1) {
				    		text_nom[compteur] = '\0';
				    		compteur--;
				    		text_nom[compteur] = ' ';
				    	}
			    		else if(compteur < 10 && event.key.keysym.sym != SDLK_BACKSPACE) {
				    		const char* cc = SDL_GetKeyName(event.key.keysym.sym);
				    		char ccc = *cc;
				    		text_nom[compteur] = ccc;
				    		compteur++;
				    		text_nom[compteur] = '\0';
				    	}
			    	}

			    	break;
			    case SDL_MOUSEBUTTONDOWN:
			    	if(event.button.button == SDL_BUTTON_LEFT) 
			    	{
			    		if(event.motion.y >= rect_button_rejouer.y && event.motion.y <= rect_button_rejouer.y + rect_button_rejouer.h) {

							if(event.motion.x >= rect_button_rejouer.x && event.motion.x <= rect_button_rejouer.x+rect_button_rejouer.w) {
								init_var();
								game_state = GAME_BEGIN;
								Mix_PlayChannel(-1, start, 0);
							}
							else if(event.motion.x >= rect_button_topscore.x && event.motion.x <= rect_button_topscore.x+rect_button_topscore.w) {
								state = TOPSCORE_STATE;
								loadTopscores();
							}
							else if(event.motion.x >= rect_button_menu.x && event.motion.x <= rect_button_menu.x+rect_button_menu.w) {
								state = MENU_STATE;
								move = 0;
							}
						}

						if(event.motion.y >= rect_button_save.y && event.motion.y <= rect_button_save.y + rect_button_save.h) {
							if(event.motion.x >= rect_button_save.x && event.motion.x <= rect_button_save.x + rect_button_save.w) {
								FILE* save = fopen("score.save", "a");
								fprintf(save, "%s;%d;%d;%d\n", text_nom,score,nb_voies,game_mode);
								fclose(save);
								save_score = 1;
							}
						}

			    	}		    	
			    	break;
			}

			int x, y;
			SDL_GetMouseState(&x, &y);
			
			buttons[0] = 0;
			buttons[1] = 0;
			buttons[2] = 0;
			buttons[3] = 0;

			if(y >= rect_button_rejouer.y && y <= rect_button_rejouer.y + rect_button_rejouer.h) {

				if(x >= rect_button_rejouer.x && x <= rect_button_rejouer.x+rect_button_rejouer.w) {
					buttons[0] = 1;
				}
				else if(x >= rect_button_topscore.x && x <= rect_button_topscore.x+rect_button_topscore.w) {
					buttons[1] = 1;
				}
				else if(x >= rect_button_menu.x && x <= rect_button_menu.x+rect_button_menu.w) {
					buttons[2] = 1;
				}
			}

			if(y >= rect_button_save.y && y <= rect_button_save.y + rect_button_save.h) {
				if(x >= rect_button_save.x && x <= rect_button_save.x + rect_button_save.w) {
					buttons[3] = 1;
				}
			}

		}
	}
	else if(state == MENU_STATE)
	{
		switch (event.type)
		{
			case SDL_QUIT:
			    program = PROGRAM_STOP;
		    	break;
		    case SDL_KEYDOWN:
		    	if(event.key.keysym.sym == SDLK_ESCAPE) {
		    		if(move == 0) program = PROGRAM_STOP;
		    		move = 0;
		    	}
		    	break;	
		    case SDL_MOUSEBUTTONDOWN:
		    	if(event.button.button == SDL_BUTTON_LEFT) 
		    	{
		    		if(event.motion.x >= WINDOW_W/2 - 75 && event.motion.x <= WINDOW_W/2 - 75 + 150)
		    		{
		    			if(event.motion.y >= WINDOW_H/2-25-60 && event.motion.y <= WINDOW_H/2-25-60 + 50)
		    			{
		    				if(move == 1)	game_mode = MODE_RETRO;
		    				else if(move == 2) nb_voies = VOIES_3; 
		    				else if(move == 3) autopilot = OFF;
		    				move++;
		    			}
		    			else if(event.motion.y >= WINDOW_H/2-25 && event.motion.y <= WINDOW_H/2-25 + 50) 
		    			{
		    				if(move == 0) {
		    					loadTopscores();
		    					state = TOPSCORE_STATE;
		    				}
		    				if(move == 1) game_mode = MODE_CLASSIC;
		    				else if(move == 2) nb_voies = VOIES_4;
		    				else if(move == 3) autopilot = ON;
		    				move++;
		    			}
		    			else if(event.motion.y >= WINDOW_H/2-25+60 && event.motion.y <= WINDOW_H/2-25+60 + 50) 
		    			{
			    			if(move == 0) program = PROGRAM_STOP;
		    				else if(move == 2) nb_voies = VOIES_5;
		    				move++;
		    			}
		    			else {
		    				move = 0;
		    			}
		    		}
		    		else {
		    			move = 0;
		    		}
		    	}		    	
		    	break;
		}

		if(move < 0) move = 0;
		if(move == 4) {
			load_game();
			init_var();
			game_state = GAME_BEGIN;
			Mix_PlayChannel(-1, start, 0);
			state = GAME_STATE;
		}

		int x, y;
		SDL_GetMouseState(&x, &y);

		if(move == 0) {
			buttons[0] = 0;
			buttons[1] = 0;
			buttons[2] = 0;

			if(x >= WINDOW_W/2 - 75 && x <= WINDOW_W/2 - 75 + 150)
		   	{ 		
			   		if(y >= WINDOW_H/2-25-60 && y <= WINDOW_H/2-25-60 + 50)
			   		{
			   			buttons[0] = 1;
			   		}

			   		if(y >= WINDOW_H/2-25 && y <= WINDOW_H/2-25 + 50) 
			   		{
			   			buttons[1] = 1;
			   		}
				    		
			   		if(y >= WINDOW_H/2-25+60 && y <= WINDOW_H/2-25+60 + 50) 
			   		{
			   			buttons[2] = 1;
			   		}
			   	
		   	} 
		}	
		else if(move == 1) 
		{
			buttons[3] = 0;
			buttons[4] = 0;

			if(x >= WINDOW_W/2 - 75 && x <= WINDOW_W/2 - 75 + 150) {
				if(y >= WINDOW_H/2-25-60 && y <= WINDOW_H/2-25-60 + 50)
			   	{
			   		buttons[3] = 1;
			   	}

		   		if(y >= WINDOW_H/2-25 && y <= WINDOW_H/2-25 + 50) 
		   		{
		   			buttons[4] = 1;
		   		}
			}
		}
		else if(move == 2) {
			buttons[5] = 0;
			buttons[6] = 0;
			buttons[7] = 0;

			if(x >= WINDOW_W/2 - 75 && x <= WINDOW_W/2 - 75 + 150)
		   	{   		
			   		if(y >= WINDOW_H/2-25-60 && y <= WINDOW_H/2-25-60 + 50)
			   		{
			   			buttons[5] = 1;
			   		}

			   		if(y >= WINDOW_H/2-25 && y <= WINDOW_H/2-25 + 50) 
			   		{
			   			buttons[6] = 1;
			   		}
				    		
			   		if(y >= WINDOW_H/2-25+60 && y <= WINDOW_H/2-25+60 + 50) 
			   		{
			   			buttons[7] = 1;
			   		}
			   	
		   	} 
		}
		else if(move == 3) 
		{
			buttons[8] = 0;
			buttons[9] = 0;

			if(x >= WINDOW_W/2 - 75 && x <= WINDOW_W/2 - 75 + 150) {
				if(y >= WINDOW_H/2-25-60 && y <= WINDOW_H/2-25-60 + 50)
			   	{
			   		buttons[8] = 1;
			   	}

		   		if(y >= WINDOW_H/2-25 && y <= WINDOW_H/2-25 + 50) 
		   		{
		   			buttons[9] = 1;
		   		}
			}
		}    
	}
	else if(state == TOPSCORE_STATE) {
		switch (event.type)
		{
			case SDL_QUIT:
			    program = PROGRAM_STOP;
			   	break;
		    case SDL_KEYDOWN:
		    	if(event.key.keysym.sym == SDLK_ESCAPE) {
		    		state = MENU_STATE;
		    		move = 0;
		    	}
		    	break;
		}
	}
}

void work_background() {
	// gestion du défilement du background
	rect_bg_1.y += (int) speed;
	rect_bg_2.y += (int) speed;
	rect_bg_3.y += (int) speed;

	if(rect_bg_1.y >= 720) {
		rect_bg_1.y = rect_bg_3.y-720;
	}
	else if(rect_bg_2.y >= 720) {
		rect_bg_2.y = rect_bg_1.y-720;
	}
	else if(rect_bg_3.y >= 720) {
		rect_bg_3.y = rect_bg_2.y-720;
	}
}

void work_ia() {
	// collision entre les ias prévention
		Cars_list tmp = ia_cars;
		Cars_list tmp2 = NULL;

		while(tmp != NULL)
		{			
			tmp2 = tmp->next;

			while(tmp2 != NULL) 
			{
				if((tmp->car->posx >= tmp2->car->posx && tmp->car->posx <= (tmp2->car->posx + CAR_W)) || ((tmp->car->posx + CAR_W) >= tmp2->car->posx && (tmp->car->posx + CAR_W) <= (tmp2->car->posx + CAR_W)))  
				{
					if(game_mode == MODE_RETRO || game_mode == MODE_CLASSIC) 
					{
						if(tmp->car->posy-COLLISION_LIMIT >= tmp2->car->posy && tmp->car->posy-COLLISION_LIMIT <= (tmp2->car->posy + CAR_H))
						{
							tmp->car->speed -= BRAKE;


							if(tmp->car->speed < 0) {
								tmp->car->speed = 0;
							}

							tmp2->car->speed = tmp2->car->speed + rand() % 2 + 1;
						}
						else if(tmp2->car->posy-COLLISION_LIMIT >= tmp->car->posy+CAR_H && tmp2->car->posy-COLLISION_LIMIT <= (tmp->car->posy + CAR_H + COLLISION_LIMIT))
						{
							tmp2->car->speed -= BRAKE;

							if(tmp2->car->speed < 0) {
								tmp2->car->speed = 0;
							}

							tmp->car->speed = tmp->car->speed + rand() % 2 + 1;
						}
					}
				}
				
				tmp2 = tmp2->next;
			}

			tmp = tmp->next;
		}

		// collisions
		tmp = ia_cars;
		tmp2 = NULL;
		Cars_list ia_cars_del = NULL;

		while(tmp != NULL)
		{			
			tmp2 = tmp->next;

			while(tmp2 != NULL) 
			{			
				if( ((tmp->car->posx >= tmp2->car->posx) && (tmp->car->posx <= (tmp2->car->posx + CAR_W))) || (((tmp->car->posx + CAR_W) >= tmp2->car->posx) && ((tmp->car->posx + CAR_W) <= (tmp2->car->posx + CAR_W)))) {
					if( ((tmp->car->posy >= tmp2->car->posy) && (tmp->car->posy <= (tmp2->car->posy + CAR_H))) || (((tmp->car->posy + CAR_H) >= tmp2->car->posy) && ((tmp->car->posy + CAR_H) <= (tmp2->car->posy + CAR_H)))) {
						ia_cars_del = addCar(ia_cars_del, tmp->car);
						ia_cars_del = addCar(ia_cars_del, tmp2->car);		
					}
				}

				if( ((tmp2->car->posx >= tmp->car->posx) && (tmp2->car->posx <= (tmp->car->posx + CAR_W))) || (((tmp2->car->posx + CAR_W) >= tmp->car->posx) && ((tmp2->car->posx + CAR_W) <= (tmp->car->posx + CAR_W)))) {
					if( ((tmp2->car->posy >= tmp->car->posy) && (tmp2->car->posy <= (tmp->car->posy + CAR_H))) || (((tmp2->car->posy + CAR_H) >= tmp->car->posy) && ((tmp2->car->posy + CAR_H) <= (tmp->car->posy + CAR_H)))) {
						ia_cars_del = addCar(ia_cars_del, tmp->car);
						ia_cars_del = addCar(ia_cars_del, tmp2->car);		
					}
				}
				
				tmp2 = tmp2->next;
			}

			tmp = tmp->next;
		}

		// gestion des ias
		tmp = ia_cars;
		while(tmp != NULL)
		{
			tmp->car->posy += (int) (speed - tmp->car->speed);

			if(tmp->car->posy >= 720) 
			{
				ia_cars_del = addCar(ia_cars_del, tmp->car);
				score += 10 + ia_cars_del->car->speed;
			}

			if(game_mode == MODE_RETRO || game_mode == MODE_CLASSIC) {
				if(tmp->car->speed == 0) {
					tmp->car->speed = rand() % 5 + 1;
				}

				if(tmp->car->speed <= 0) {
					tmp->car->speed = 0;
				}
			}			

			tmp = tmp->next;
		}

		tmp = ia_cars_del;
		
		while(tmp != NULL) {
			ia_cars = delCar(ia_cars, tmp->car);
			tmp = tmp->next;
		}
}

void work() {

	if(state == GAME_STATE) {		
		if(game_state == GAME_PLAY) {
			

			if(autopilot == ON) {


					Cars_list tmp = ia_cars;
					int acc = 1;
					int goLeft = 1;
					int goRight = 1;

					while(tmp != NULL) {

						
						// prevention collision pour freinnage
						if((rect_car_pos.y - COLLISION_LIMIT >= tmp->car->posy && rect_car_pos.y - COLLISION_LIMIT <= (tmp->car->posy + CAR_H)) || ((rect_car_pos.y + CAR_H - COLLISION_LIMIT) >= tmp->car->posy && (rect_car_pos.y + CAR_H - COLLISION_LIMIT) <= (tmp->car->posy + CAR_H))) 
						{
							if((rect_car_pos.x >= tmp->car->posx && rect_car_pos.x <= (tmp->car->posx + CAR_W)) || ((rect_car_pos.x + CAR_W) >= tmp->car->posx && (rect_car_pos.x + CAR_W) <= (tmp->car->posx + CAR_W)))  
							{
								if(SDL_GetTicks() >= stop_pressed + 50) {
									speed -= BRAKE;
									if(speed < 0) speed = 0;
									changeNeeded = 1;
									stop_pressed = SDL_GetTicks();
								}
								acc = 0;
							}
						}
						int col = 116;
						// est ce qu'on peut aller à droite ou à gauche
						if((rect_car_pos.y >= tmp->car->posy && rect_car_pos.y <= (tmp->car->posy + CAR_H)) || ((rect_car_pos.y + CAR_H) >= tmp->car->posy && (rect_car_pos.y + CAR_H) <= (tmp->car->posy + CAR_H))) 
						{
								if((rect_car_pos.x-col >= tmp->car->posx && rect_car_pos.x-col <= (tmp->car->posx + CAR_W)) || ((rect_car_pos.x + CAR_W-col) >= tmp->car->posx && (rect_car_pos.x + CAR_W-col) <= (tmp->car->posx + CAR_W))) {
									goLeft = 0;
								}

								if((rect_car_pos.x+col >= tmp->car->posx && rect_car_pos.x+col <= (tmp->car->posx + CAR_W)) || ((rect_car_pos.x + CAR_W+col) >= tmp->car->posx && (rect_car_pos.x + CAR_W+col) <= (tmp->car->posx + CAR_W))) {
									goRight = 0;
								}

								if((int)(tmp->car->posx+CAR_W/2) < rect_car_pos.x && (int)(tmp->car->posx+CAR_W/2) > rect_car_pos.x+CAR_W) {
									goLeft = 0;
								}

								if((int)(tmp->car->posx+CAR_W/2) > rect_car_pos.x && (int)(tmp->car->posx+CAR_W/2) < rect_car_pos.x+CAR_W) {
									goRight = 0;
								}
						}

						tmp = tmp->next;
					}

					// si rien devant on accélère
					if(SDL_GetTicks() >= start_pressed + 100 && acc == 1) {
						if(speed <= 5) {
						speed += ACC/0.75;
						}
						else if(speed <= 10) {
							speed += ACC;
						}
						else if(speed <= 15) {
							speed += ACC/2;
						}
						else {
							speed += ACC/3;
						}
							if(speed >= MAX_SPEED) speed = MAX_SPEED;
						start_pressed = SDL_GetTicks();
					}

				if(game_mode == MODE_RETRO) {

					// on change de voie si ne peut pas acc et que il y a la place
					int goLeftV = 0, goRightV = 0;
					if(acc == 0) {
						if(rand()%2+1 == 1) {
							if(goLeft == 1) {
								goLeftV = 1;

							}
							else {
								goRightV = 1;
							}
						}
						else {
							if(goRight == 1) goRightV = 1;
							else goLeftV = 1;
						}
					}

					if(goLeftV == 1 && goLeft == 1) {
						switch(nb_voies)
						{
							case VOIES_3:
								if(pos > POS_2) pos--;
								break;
							case VOIES_4:
								if(pos > POS_1) pos--;
								break;
							case VOIES_5:
								if(pos > POS_1) pos--;
								break;
						}
					}
					if(goRightV == 1 && goRight == 1) {
						switch(nb_voies)
						{
							case VOIES_3:			    				
								if(pos < POS_4) pos++;
			    				break;
				   			case VOIES_4:			    				
			    				if(pos < POS_4) pos++;
								break;
							case VOIES_5:			    				
			    				if(pos < POS_5) pos++;
				  				break;	
				   		}
				   	}
				}
				else 
				{
					// on change de voie si ne peut pas acc et que il y a la place
					if((topos == rect_car_pos.x && changeNeeded == 1)) {
						if(rand()%2+1 == 1) {
							if(goLeft == 1) {
								topos = rect_car_pos.x-116;
							}
							else {
								topos = rect_car_pos.x+116;
							}
						}
						else {
							if(goRight == 1) {
								topos = rect_car_pos.x+116;
							}
							else {
								topos = rect_car_pos.x-116;
							}
						}
						changeNeeded = 0;
					}

					if(topos > 742) {
						topos = 742;
					}
					if(topos < 254) {
						topos = 254;
					}

					if(topos < rect_car_pos.x && goLeft == 1) 
					{
						rect_car_pos.x -= (int) (speed / 3);
					}
						
					if(topos > rect_car_pos.x && goRight == 1) 
					{
						rect_car_pos.x += (int) (speed / 3);
					}

					switch(nb_voies)
					{
					   	case VOIES_3:
					   		if(rect_car_pos.x <= 370) rect_car_pos.x = 370;
					   		else if(rect_car_pos.x >= 709 - CAR_W) rect_car_pos.x = 709 - CAR_W;
					   		break;
					   	case VOIES_4:
					   		if(rect_car_pos.x <= 312) rect_car_pos.x = 312;
					   		else if(rect_car_pos.x >= 767 - CAR_W) rect_car_pos.x = 767 - CAR_W;
					   		break;
					   	case VOIES_5:
					   		if(rect_car_pos.x <= 254) rect_car_pos.x = 254;
					   		else if(rect_car_pos.x >= 825 - CAR_W) rect_car_pos.x = 825 - CAR_W;
					   		break;
					   
					}
				}
				
			}


			if(autopilot == OFF) {
				// accélération
				if(start_pressed != 0 && SDL_GetTicks() >= start_pressed + 100 && key_pressed == SDLK_UP) {
					if(speed <= 5) {
						speed += ACC/0.75;
					}
					else if(speed <= 10) {
						speed += ACC;
					}
					else if(speed <= 15) {
						speed += ACC/2;
					}
					else {
						speed += ACC/3;
					}

					if(speed >= MAX_SPEED) speed = MAX_SPEED; 
					start_pressed = SDL_GetTicks();
				}
				else if(start_pressed != 0 && SDL_GetTicks() >= start_pressed + 100 && key_pressed == SDLK_DOWN) {
					speed -= BRAKE;
					if(speed < 0) speed = 0; 
					start_pressed = SDL_GetTicks();
				}
				else if(SDL_GetTicks() >= stop_pressed + 50 && key_pressed != SDLK_UP) {
					speed -= DECC;
					if(speed < 0) speed = 0; 
					stop_pressed = SDL_GetTicks();
				}
			}

			if(SDL_GetTicks() >= global_ticks + 15) {	
				work_background();
				work_ia();


				if(autopilot == OFF) {
					if(game_mode == MODE_CLASSIC)
					{
						if(state_keyboard[SDL_SCANCODE_LEFT]) 
						{
							rect_car_pos.x -= (int) (speed / 3);
						}
						
						if(state_keyboard[SDL_SCANCODE_RIGHT]) 
						{
							rect_car_pos.x += (int) (speed / 3);
						}

						switch(nb_voies)
					    {
					    	case VOIES_3:
					    		if(rect_car_pos.x <= 370) rect_car_pos.x = 370;
					    		else if(rect_car_pos.x >= 709 - CAR_W) rect_car_pos.x = 709 - CAR_W;
					    		break;
					    	case VOIES_4:
					    		if(rect_car_pos.x <= 312) rect_car_pos.x = 312;
					    		else if(rect_car_pos.x >= 767 - CAR_W) rect_car_pos.x = 767 - CAR_W;
					    		break;
					    	case VOIES_5:
					    		if(rect_car_pos.x <= 254) rect_car_pos.x = 254;
					    		else if(rect_car_pos.x >= 825 - CAR_W) rect_car_pos.x = 825 - CAR_W;
					    		break;
					    }
					}
				}

				global_ticks = SDL_GetTicks();
			}
		
			// gestion des collisions joueur ias
			Cars_list tmp = ia_cars;
			while(tmp != NULL)
			{
				if((rect_car_pos.y >= tmp->car->posy && rect_car_pos.y <= (tmp->car->posy + CAR_H)) || ((rect_car_pos.y + CAR_H) >= tmp->car->posy && (rect_car_pos.y + CAR_H) <= (tmp->car->posy + CAR_H))) 
				{
					if((rect_car_pos.x >= tmp->car->posx && rect_car_pos.x <= (tmp->car->posx + CAR_W)) || ((rect_car_pos.x + CAR_W) >= tmp->car->posx && (rect_car_pos.x + CAR_W) <= (tmp->car->posx + CAR_W)))  
					{
						Mix_Pause(1);
						Mix_PlayChannel(-1, crash, 0);
						explo = 1;
						game_state = GAME_TOOVER;
						fclose(stats);
						stats = fopen("speed.stats", "r");
						
						int c = 0;
						char buff[10];
						moy_speed = 0;

						toDestroy = tmp->car;

						while(fgets(buff, 10, stats)) {
							moy_speed += atof(buff);
							c++;
						}
						moy_speed = (moy_speed / c);
					}
				}

				tmp = tmp->next;
			}

			
				if(game_mode == MODE_RETRO) 
				{
					// position de la voiture
					if(nb_voies == VOIES_3) {	
						if(pos == POS_2) {
							rect_car_pos = rect_car_pos1_3; 
						}
						else if(pos == POS_3) {
							rect_car_pos = rect_car_pos2_3;
						}
						else if(pos == POS_4) {
							rect_car_pos = rect_car_pos3_3;
						}
					}
					else if(nb_voies == VOIES_4) {
						if(pos == POS_1) {
							rect_car_pos = rect_car_pos1_4; 
						}
						else if(pos == POS_2) {
							rect_car_pos = rect_car_pos2_4; 
						}
						else if(pos == POS_3) {
							rect_car_pos = rect_car_pos3_4;
						}
						else if(pos == POS_4) {
							rect_car_pos = rect_car_pos4_4	;
						}
					}
					else if(nb_voies == VOIES_5) {
						if(pos == POS_1) {
							rect_car_pos = rect_car_pos1_5; 
						}
						else if(pos == POS_2) {
							rect_car_pos = rect_car_pos2_5; 
						}
						else if(pos == POS_3) {
							rect_car_pos = rect_car_pos3_5;
						}
						else if(pos == POS_4) {
							rect_car_pos = rect_car_pos4_5;
						}
						else if(pos == POS_5) {
							rect_car_pos = rect_car_pos5_5;
						}
					}
				}
			

			// mode retro apparition 250 = cool
			// apparition magique de voitures
			if((SDL_GetTicks() - last_pop) >= 100 /*&& id_car <= 3*/) 
			{
				float alea_speed = rand() % MAX_SPEED + 1;
				int alea_posy = rand() % 2880 + 720;

				int alea_pos;

				if(nb_voies == VOIES_3) {
					alea_pos = rand() % 3 + 1;				

					if(alea_pos == 1) alea_pos = rect_car_pos1_3.x;  
					else if(alea_pos == 2) alea_pos = rect_car_pos2_3.x;
					else alea_pos = rect_car_pos3_3.x;
				}
				else if(nb_voies == VOIES_4) {
					alea_pos = rand() % 4 + 1;

					if(alea_pos == 1) alea_pos = rect_car_pos1_4.x;  
					else if(alea_pos == 2) alea_pos = rect_car_pos2_4.x;
					else if(alea_pos == 3) alea_pos = rect_car_pos3_4.x;
					else alea_pos = rect_car_pos4_4.x;
				}
				else {
					alea_pos = rand() % 5 + 1;

					if(alea_pos == 1) alea_pos = rect_car_pos1_5.x;  
					else if(alea_pos == 2) alea_pos = rect_car_pos2_5.x;
					else if(alea_pos == 3) alea_pos = rect_car_pos3_5.x;
					else if(alea_pos == 4) alea_pos = rect_car_pos4_5.x;
					else alea_pos = rect_car_pos5_5.x;
				}
		
				alea_posy *= -1;
				tmp = ia_cars;
				int good = 1;
				while(tmp != NULL)
				{
					if((alea_posy >= tmp->car->posy && alea_posy <= (tmp->car->posy + CAR_H)) || ((alea_posy + CAR_H) >= tmp->car->posy && (alea_posy + CAR_H) <= (tmp->car->posy + CAR_H))) 
					{
						good = 0;
					}

					tmp = tmp->next;
				}

				if(good == 1) {
					Car* new_car = createCar(id_car,alea_pos,alea_posy,alea_speed,0,0,0);
					ia_cars = addCar(ia_cars, new_car);
					last_pop = SDL_GetTicks();
					id_car++;
					// debug 
					// printf("Id: %d\n",id_car-1);
				}		
				else {
					// debug
					// printf("nope\n");
				}
			}
		}
		else if(game_state == GAME_TOOVER) {
			if(explo && ((SDL_GetTicks() >= anim_ticks + 50 && anim <= 7) || (SDL_GetTicks() >= anim_ticks + 200))) {	
				rect_anim_posin = rect_anim[anim];
				anim++;
				if(anim >= 16) {
					game_state = GAME_OVER;
					Mix_Resume(1);
				}
				anim_ticks = SDL_GetTicks();
			}
		}
	}

	if(game_state == GAME_BEGIN && state == GAME_STATE) {
		if(SDL_GetTicks() >= global_ticks + 1000) {
			var_begin--;
			global_ticks = SDL_GetTicks();
		}

		if(var_begin < 0) {
			game_state = GAME_PLAY;
		}
	}

	// calcule des coordonnées du needle
	rect_needle.x = rect_speed.x+SPEED_H/2 - NEEDLE_W/2;
	rect_needle.y = rect_speed.y+SPEED_W/2 - NEEDLE_H/2;

	if(SDL_GetTicks() - timer >= 500) {	
		fps = frame;
		frame = 0;
		timer = SDL_GetTicks();
	}

	if(game_state == GAME_PLAY && state == GAME_STATE && SDL_GetTicks() >= stats_ticks + 100)
	{	
		fprintf(stats, "%.1f\n", speed);
		stats_ticks = SDL_GetTicks();
	}
}

void display() {
	// on efface
	SDL_RenderClear(renderer);
	
	if(state == GAME_STATE) {
		// on affiche la route
		SDL_RenderCopy(renderer, texture_road, NULL, &rect_bg_3);
		SDL_RenderCopy(renderer, texture_road, NULL, &rect_bg_2);
		SDL_RenderCopy(renderer, texture_road, NULL, &rect_bg_1);

		// affichage de la vitesse
		SDL_RenderCopy(renderer, texture_speed, NULL, &rect_speed);
		surface_needle_displayed = rotozoomSurface(surface_needle, 60-speed*10, 1.0, 1);
		texture_needle = getTextureFromSurface(renderer, surface_needle_displayed);
		SDL_RenderCopy(renderer, texture_needle, NULL, &rect_needle);
		
		SDL_FreeSurface(surface_needle_displayed);
		SDL_DestroyTexture(texture_needle);


		// affiche les ias
		Cars_list tmp = ia_cars;
		SDL_Rect rect_car_ia = { 0,0,0,0 };	
		while(tmp != NULL)
		{
			rect_car_ia.x = tmp->car->posx;
			rect_car_ia.y = tmp->car->posy;
			rect_car_ia.w = CAR_W;
			rect_car_ia.h = CAR_H;

			if(toDestroy == tmp->car && toDestroy != NULL) {					
				SDL_Rect rect_anim_pos2 = { toDestroy->posx + CAR_W/2 - 64, toDestroy->posy + CAR_H/2 - 64, 128, 128 };
				SDL_RenderCopy(renderer, texture_anim, &rect_anim_posin, &rect_anim_pos2);	
			} 
			else
				SDL_RenderCopy(renderer, texture_car_yellow, NULL, &rect_car_ia);
			
			// affiche l'id des voitures pour debbug 
			/*char text[5];
			sprintf(text, "%d", tmp->car->id);
			SDL_Texture* texture_text = getTextureFromSurface(renderer,TTF_RenderText_Solid(font,text,black));
			SDL_Rect rect_text = { tmp->car->posx, tmp->car->posy, 15, 15};
			SDL_RenderCopy(renderer, texture_text, NULL, &rect_text);			
			SDL_DestroyTexture(texture_text);*/

			tmp = tmp->next;
		}

		// la voiture du joueur		
		if(explo) {		
			SDL_Rect rect_anim_pos = { rect_car_pos.x + rect_car_pos.w/2 - 64, rect_car_pos.y + rect_car_pos.h/2 - 64, 128, 128 };
			SDL_RenderCopy(renderer, texture_anim, &rect_anim_posin, &rect_anim_pos);	
		}
		else {
			SDL_RenderCopy(renderer, texture_car_red, NULL, &rect_car_pos);
		}

	}
	else if(state == MENU_STATE) 
	{

		SDL_RenderCopy(renderer, texture_menu_background, NULL, NULL);
		SDL_RenderCopy(renderer, texture_text_title, NULL, &rect_text_title);

		if(move == 0) {
			if(buttons[0] == 1 || buttons[0] == 2) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_jouer);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_jouer);

			if(buttons[1] == 1 || buttons[1] == 2) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_options);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_options);

			if(buttons[2] == 1 || buttons[2] == 2) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_quitter);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_quitter);			

			SDL_RenderCopy(renderer, texture_text_jouer, NULL, &rect_text_jouer);
			SDL_RenderCopy(renderer, texture_text_options, NULL, &rect_text_options);
			SDL_RenderCopy(renderer, texture_text_quitter, NULL, &rect_text_quitter);
		}
		else if(move == 1) {
			if(buttons[3] == 1 || buttons[4] == 2) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_jouer);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_jouer);

			if(buttons[4] == 1 || buttons[3] == 2) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_options);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_options);

			SDL_RenderCopy(renderer, texture_text_retro, NULL, &rect_text_retro);
			SDL_RenderCopy(renderer, texture_text_classic, NULL, &rect_text_classic);
		}
		else if(move == 2) {
			if(buttons[5] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_jouer);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_jouer);

			if(buttons[6] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_options);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_options);

			if(buttons[7] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_quitter);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_quitter);			

			SDL_RenderCopy(renderer, texture_text_3voies, NULL, &rect_text_3voies);
			SDL_RenderCopy(renderer, texture_text_4voies, NULL, &rect_text_4voies);
			SDL_RenderCopy(renderer, texture_text_5voies, NULL, &rect_text_5voies);
		}
		else if(move == 3) {

			if(buttons[8] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_jouer);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_jouer);

			if(buttons[9] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_options);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_options);



			SDL_RenderCopy(renderer, texture_player, NULL, &rect_text_player);
			SDL_RenderCopy(renderer, texture_autopilot, NULL, &rect_text_autopilot);
		}

	}
	else if(state == TOPSCORE_STATE) {
		
		SDL_RenderCopy(renderer, texture_menu_background, NULL, NULL);

		SDL_ShowCursor(SDL_ENABLE);	
		char begin_text[20];
		sprintf(begin_text, "Meilleur Scores");
		SDL_Surface* surface_text_begin = TTF_RenderText_Solid(font_ingame,begin_text,black);
		SDL_Texture* texture_begin = getTextureFromSurface(renderer,surface_text_begin);
		SDL_Rect rect_text_begin = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_begin, NULL, NULL, &rect_text_begin.w, &rect_text_begin.h );
		rect_text_begin.x = WINDOW_W/2-rect_text_begin.w/2;
		rect_text_begin.y = 20;//WINDOW_H/2-rect_text_begin.h/2;
		SDL_RenderCopy(renderer, texture_begin, NULL, &rect_text_begin);			
		SDL_FreeSurface(surface_text_begin);			
		SDL_DestroyTexture(texture_begin);

{
		int marge = 20+rect_text_begin.h+20;
		int lvl = 1;

		char save[30];
		sprintf(save, "Top 10 : Retro 5 voies");
		SDL_Surface* surface_text_save = TTF_RenderText_Solid(font_menu,save,black);
		SDL_Texture* texture_save = getTextureFromSurface(renderer,surface_text_save);
		SDL_Rect rect_text_save = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_save, NULL, NULL, &rect_text_save.w, &rect_text_save.h );
		rect_text_save.x = 20;
		rect_text_save.y = marge;
		SDL_RenderCopy(renderer, texture_save, NULL, &rect_text_save);			
		SDL_FreeSurface(surface_text_save);			
		SDL_DestroyTexture(texture_save);

		Score_list tmp = scores_list_5;
		while(tmp != NULL) {

			marge += 20;
			char score_text[100];
			sprintf(score_text, "%d: %s\t%d", lvl, tmp->nom, tmp->score);
			SDL_Surface* surface_text_score = TTF_RenderText_Solid(font_menu,score_text,black);
			SDL_Texture* texture_score = getTextureFromSurface(renderer,surface_text_score);
			SDL_Rect rect_text_score = { 0, 0, 0, 0 };
			SDL_QueryTexture(texture_score, NULL, NULL, &rect_text_score.w, &rect_text_score.h );
			rect_text_score.x = 20;
			rect_text_score.y = marge;
			SDL_RenderCopy(renderer, texture_score, NULL, &rect_text_score);			
			SDL_FreeSurface(surface_text_score);			
			SDL_DestroyTexture(texture_score);

			lvl++;
			tmp = tmp->next;
			
			if(lvl > 10) break;
		}

		marge = 20+rect_text_begin.h+20;
		lvl = 1;

		char nom[30];
		sprintf(nom, "Top 10 : Retro 4 voies");
		SDL_Surface* surface_text_nom = TTF_RenderText_Solid(font_menu,nom,black);
		SDL_Texture* texture_nom = getTextureFromSurface(renderer,surface_text_nom);
		SDL_Rect rect_text_nom = { WINDOW_W/2 - 75 - 150 - 10, 	WINDOW_H/2-25+60-50, 0, 0 };
		SDL_QueryTexture(texture_nom, NULL, NULL, &rect_text_nom.w, &rect_text_nom.h );
		rect_text_nom.x = 200 + rect_text_save.w;
		rect_text_nom.y = marge;
		SDL_RenderCopy(renderer, texture_nom, NULL, &rect_text_nom);			
		SDL_FreeSurface(surface_text_nom);			
		SDL_DestroyTexture(texture_nom);

		tmp = scores_list_4;
		while(tmp != NULL) {

			marge += 20;
			char score_text[100];
			sprintf(score_text, "%d: %s\t%d", lvl, tmp->nom, tmp->score);
			SDL_Surface* surface_text_score = TTF_RenderText_Solid(font_menu,score_text,black);
			SDL_Texture* texture_score = getTextureFromSurface(renderer,surface_text_score);
			SDL_Rect rect_text_score = { 0, 0, 0, 0 };
			SDL_QueryTexture(texture_score, NULL, NULL, &rect_text_score.w, &rect_text_score.h );
			rect_text_score.x = 200 + rect_text_save.w;
			rect_text_score.y = marge;
			SDL_RenderCopy(renderer, texture_score, NULL, &rect_text_score);			
			SDL_FreeSurface(surface_text_score);			
			SDL_DestroyTexture(texture_score);

			lvl++;
			tmp = tmp->next;
			
			if(lvl > 10) break;
		}

		marge = 20+rect_text_begin.h+20;
		lvl = 1;

		char save2[60];
		sprintf(save2, "Top 10 : Retro 3 voies");
		SDL_Surface* surface_text_save2 = TTF_RenderText_Solid(font_menu,save2,black);
		SDL_Texture* texture_save2 = getTextureFromSurface(renderer,surface_text_save2);
		SDL_Rect rect_text_save2 = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_save2, NULL, NULL, &rect_text_save2.w, &rect_text_save2.h );
		rect_text_save2.x = 550 + rect_text_save.w;
		rect_text_save2.y = marge;
		SDL_RenderCopy(renderer, texture_save2, NULL, &rect_text_save2);			
		SDL_FreeSurface(surface_text_save2);			
		SDL_DestroyTexture(texture_save2);

		tmp = scores_list_3;
		while(tmp != NULL) {

			marge += 20;
			char score_text[100];
			sprintf(score_text, "%d: %s\t%d", lvl, tmp->nom, tmp->score);
			SDL_Surface* surface_text_score = TTF_RenderText_Solid(font_menu,score_text,black);
			SDL_Texture* texture_score = getTextureFromSurface(renderer,surface_text_score);
			SDL_Rect rect_text_score = { 0, 0, 0, 0 };
			SDL_QueryTexture(texture_score, NULL, NULL, &rect_text_score.w, &rect_text_score.h );
			rect_text_score.x = 550 + rect_text_save.w;
			rect_text_score.y = marge;
			SDL_RenderCopy(renderer, texture_score, NULL, &rect_text_score);			
			SDL_FreeSurface(surface_text_score);			
			SDL_DestroyTexture(texture_score);

			lvl++;
			tmp = tmp->next;
			
			if(lvl > 10) break;
		}
}

{
		int marge = 20+rect_text_begin.h+20+250;
		int lvl = 1;

		char save[30];
		sprintf(save, "Top 10 : Retro 5 voies");
		SDL_Surface* surface_text_save = TTF_RenderText_Solid(font_menu,save,black);
		SDL_Texture* texture_save = getTextureFromSurface(renderer,surface_text_save);
		SDL_Rect rect_text_save = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_save, NULL, NULL, &rect_text_save.w, &rect_text_save.h );
		rect_text_save.x = 20;
		rect_text_save.y = marge;
		SDL_RenderCopy(renderer, texture_save, NULL, &rect_text_save);			
		SDL_FreeSurface(surface_text_save);			
		SDL_DestroyTexture(texture_save);

		Score_list tmp = scores_list_5_c;
		while(tmp != NULL) {

			marge += 20;
			char score_text[100];
			sprintf(score_text, "%d: %s\t%d", lvl, tmp->nom, tmp->score);
			SDL_Surface* surface_text_score = TTF_RenderText_Solid(font_menu,score_text,black);
			SDL_Texture* texture_score = getTextureFromSurface(renderer,surface_text_score);
			SDL_Rect rect_text_score = { 0, 0, 0, 0 };
			SDL_QueryTexture(texture_score, NULL, NULL, &rect_text_score.w, &rect_text_score.h );
			rect_text_score.x = 20;
			rect_text_score.y = marge;
			SDL_RenderCopy(renderer, texture_score, NULL, &rect_text_score);			
			SDL_FreeSurface(surface_text_score);			
			SDL_DestroyTexture(texture_score);

			lvl++;
			tmp = tmp->next;
			
			if(lvl > 10) break;
		}

		marge = 20+rect_text_begin.h+20+250;
		lvl = 1;

		char nom[30];
		sprintf(nom, "Top 10 : Retro 4 voies");
		SDL_Surface* surface_text_nom = TTF_RenderText_Solid(font_menu,nom,black);
		SDL_Texture* texture_nom = getTextureFromSurface(renderer,surface_text_nom);
		SDL_Rect rect_text_nom = { WINDOW_W/2 - 75 - 150 - 10, 	WINDOW_H/2-25+60-50, 0, 0 };
		SDL_QueryTexture(texture_nom, NULL, NULL, &rect_text_nom.w, &rect_text_nom.h );
		rect_text_nom.x = 200 + rect_text_save.w;
		rect_text_nom.y = marge;
		SDL_RenderCopy(renderer, texture_nom, NULL, &rect_text_nom);			
		SDL_FreeSurface(surface_text_nom);			
		SDL_DestroyTexture(texture_nom);

		tmp = scores_list_4_c;
		while(tmp != NULL) {

			marge += 20;
			char score_text[100];
			sprintf(score_text, "%d: %s\t%d", lvl, tmp->nom, tmp->score);
			SDL_Surface* surface_text_score = TTF_RenderText_Solid(font_menu,score_text,black);
			SDL_Texture* texture_score = getTextureFromSurface(renderer,surface_text_score);
			SDL_Rect rect_text_score = { 0, 0, 0, 0 };
			SDL_QueryTexture(texture_score, NULL, NULL, &rect_text_score.w, &rect_text_score.h );
			rect_text_score.x = 200 + rect_text_save.w;
			rect_text_score.y = marge;
			SDL_RenderCopy(renderer, texture_score, NULL, &rect_text_score);			
			SDL_FreeSurface(surface_text_score);			
			SDL_DestroyTexture(texture_score);

			lvl++;
			tmp = tmp->next;
			
			if(lvl > 10) break;
		}

		marge = 20+rect_text_begin.h+20+250;
		lvl = 1;

		char save2[60];
		sprintf(save2, "Top 10 : Retro 3 voies");
		SDL_Surface* surface_text_save2 = TTF_RenderText_Solid(font_menu,save2,black);
		SDL_Texture* texture_save2 = getTextureFromSurface(renderer,surface_text_save2);
		SDL_Rect rect_text_save2 = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_save2, NULL, NULL, &rect_text_save2.w, &rect_text_save2.h );
		rect_text_save2.x = 550 + rect_text_save.w;
		rect_text_save2.y = marge;
		SDL_RenderCopy(renderer, texture_save2, NULL, &rect_text_save2);			
		SDL_FreeSurface(surface_text_save2);			
		SDL_DestroyTexture(texture_save2);

		tmp = scores_list_3_c;
		while(tmp != NULL) {

			marge += 20;
			char score_text[100];
			sprintf(score_text, "%d: %s\t%d", lvl, tmp->nom, tmp->score);
			SDL_Surface* surface_text_score = TTF_RenderText_Solid(font_menu,score_text,black);
			SDL_Texture* texture_score = getTextureFromSurface(renderer,surface_text_score);
			SDL_Rect rect_text_score = { 0, 0, 0, 0 };
			SDL_QueryTexture(texture_score, NULL, NULL, &rect_text_score.w, &rect_text_score.h );
			rect_text_score.x = 550 + rect_text_save.w;
			rect_text_score.y = marge;
			SDL_RenderCopy(renderer, texture_score, NULL, &rect_text_score);			
			SDL_FreeSurface(surface_text_score);			
			SDL_DestroyTexture(texture_score);

			lvl++;
			tmp = tmp->next;
			
			if(lvl > 10) break;
		}
}

	}

	if(game_state == GAME_PLAY) {
		
		SDL_Rect rect_button_scoreig = { 10, 10, 200, 50};
		SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_scoreig);

		char score_text[20];
		sprintf(score_text, "Score: %d", score);
		SDL_Surface* surface_text_score = TTF_RenderText_Solid(font_menu,score_text,black);
		SDL_Texture* texture_score = getTextureFromSurface(renderer,surface_text_score);
		SDL_Rect rect_text_score = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_score, NULL, NULL, &rect_text_score.w, &rect_text_score.h );
		rect_text_score.x = rect_button_scoreig.x + rect_button_scoreig.w/2 - rect_text_score.w/2;
		rect_text_score.y = rect_button_scoreig.y + rect_button_scoreig.h/2 - rect_text_score.h/2;
		SDL_RenderCopy(renderer, texture_score, NULL, &rect_text_score);			
		SDL_FreeSurface(surface_text_score);			
		SDL_DestroyTexture(texture_score);
	}
	else if(state == GAME_STATE && game_state == GAME_BEGIN) {
		char begin_text[1];
		sprintf(begin_text, "%d", var_begin);
		SDL_Surface* surface_text_begin = TTF_RenderText_Solid(font_ingame,begin_text,black);
		SDL_Texture* texture_begin = getTextureFromSurface(renderer,surface_text_begin);
		SDL_Rect rect_text_begin = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_begin, NULL, NULL, &rect_text_begin.w, &rect_text_begin.h );
		rect_text_begin.x = WINDOW_W/2-rect_text_begin.w/2;
		rect_text_begin.y = WINDOW_H/2-rect_text_begin.h/2;
		SDL_RenderCopy(renderer, texture_begin, NULL, &rect_text_begin);			
		SDL_FreeSurface(surface_text_begin);			
		SDL_DestroyTexture(texture_begin);
		SDL_ShowCursor(SDL_DISABLE);
	}
	else if(game_state == GAME_OVER && state == GAME_STATE) {	
		SDL_ShowCursor(SDL_ENABLE);	
		char begin_text[9];
		sprintf(begin_text, "GAME OVER");
		SDL_Surface* surface_text_begin = TTF_RenderText_Solid(font_ingame,begin_text,black);
		SDL_Texture* texture_begin = getTextureFromSurface(renderer,surface_text_begin);
		SDL_Rect rect_text_begin = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_begin, NULL, NULL, &rect_text_begin.w, &rect_text_begin.h );
		rect_text_begin.x = WINDOW_W/2-rect_text_begin.w/2;
		rect_text_begin.y = 100;//WINDOW_H/2-rect_text_begin.h/2;
		SDL_RenderCopy(renderer, texture_begin, NULL, &rect_text_begin);			
		SDL_FreeSurface(surface_text_begin);			
		SDL_DestroyTexture(texture_begin);

		SDL_Rect rect_background_menuig = { WINDOW_W/2-rect_text_begin.w/2, rect_text_begin.y+rect_text_begin.h+50, 630, 200 };		
		SDL_RenderCopy(renderer, texture_text_background_menuig, NULL, &rect_background_menuig);

		SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_rejouer);
		char score_text[60];
		sprintf(score_text, "Score: %d\tVitesse moyenne: %.1f km/h", score, moy_speed*8);
		SDL_Surface* surface_text_score = TTF_RenderText_Solid(font_menu,score_text,black);
		SDL_Texture* texture_score = getTextureFromSurface(renderer,surface_text_score);
		SDL_Rect rect_text_score = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture_score, NULL, NULL, &rect_text_score.w, &rect_text_score.h );
		rect_text_score.x = rect_background_menuig.x + rect_background_menuig.w/2 - rect_text_score.w/2;
		rect_text_score.y = rect_text_begin.y+rect_text_begin.h+50 + 35;
		SDL_RenderCopy(renderer, texture_score, NULL, &rect_text_score);			
		SDL_FreeSurface(surface_text_score);			
		SDL_DestroyTexture(texture_score);

		if(save_score == 0) {
			SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_nom);

			if(buttons[3] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_save);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_save);	

			char nom[60];
			sprintf(nom, "Votre nom : ");
			SDL_Surface* surface_text_nom = TTF_RenderText_Solid(font_menu,nom,black);
			SDL_Texture* texture_nom = getTextureFromSurface(renderer,surface_text_nom);
			SDL_Rect rect_text_nom = { WINDOW_W/2 - 75 - 150 - 10, 	WINDOW_H/2-25+60-50, 0, 0 };
			SDL_QueryTexture(texture_nom, NULL, NULL, &rect_text_nom.w, &rect_text_nom.h );
			SDL_RenderCopy(renderer, texture_nom, NULL, &rect_text_nom);			
			SDL_FreeSurface(surface_text_nom);			
			SDL_DestroyTexture(texture_nom);

			char save[60];
			sprintf(save, "Enregistrer");
			SDL_Surface* surface_text_save = TTF_RenderText_Solid(font_menu,save,black);
			SDL_Texture* texture_save = getTextureFromSurface(renderer,surface_text_save);
			SDL_Rect rect_text_save = { 0, 0, 0, 0 };
			SDL_QueryTexture(texture_save, NULL, NULL, &rect_text_save.w, &rect_text_save.h );
			rect_text_save.x = rect_button_save.x + rect_button_save.w/2 - rect_text_save.w/2;
			rect_text_save.y = rect_button_save.y + rect_button_save.h/2 - rect_text_save.h/2;
			SDL_RenderCopy(renderer, texture_save, NULL, &rect_text_save);			
			SDL_FreeSurface(surface_text_save);			
			SDL_DestroyTexture(texture_save);

			SDL_Surface* surface_text_entree = TTF_RenderText_Solid(font_menu,text_nom,black);
			SDL_Texture* texture_text_entree = getTextureFromSurface(renderer,surface_text_entree);
			SDL_Rect rect_text_entree = { 0, 0, 0, 0 };
			SDL_QueryTexture(texture_text_entree, NULL, NULL, &rect_text_entree.w, &rect_text_entree.h );
			rect_text_entree.x = rect_button_nom.x + rect_button_nom.w/2 - rect_text_entree.w/2;
			rect_text_entree.y = rect_button_nom.y + rect_button_nom.h/2 - rect_text_entree.h/2;
			SDL_RenderCopy(renderer, texture_text_entree, NULL, &rect_text_entree);			
			SDL_FreeSurface(surface_text_entree);			
			SDL_DestroyTexture(texture_text_entree);
		}
		else if(save_score == 1) {
			char nom[60];
			sprintf(nom, "Votre score a bien ete enregistrer :)");
			SDL_Surface* surface_text_nom = TTF_RenderText_Solid(font_menu,nom,black);
			SDL_Texture* texture_nom = getTextureFromSurface(renderer,surface_text_nom);
			SDL_Rect rect_text_nom = { WINDOW_W/2 - 75 - 150 - 10, 	WINDOW_H/2-25+60-50, 0, 0 };
			SDL_QueryTexture(texture_nom, NULL, NULL, &rect_text_nom.w, &rect_text_nom.h );
			SDL_RenderCopy(renderer, texture_nom, NULL, &rect_text_nom);			
			SDL_FreeSurface(surface_text_nom);			
			SDL_DestroyTexture(texture_nom);
		}

			if(buttons[0] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_rejouer);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_rejouer);

			if(buttons[1] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_topscore);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_topscore);

			if(buttons[2] == 1) 
				SDL_RenderCopy(renderer, texture_button_on, NULL, &rect_button_menu);
			else
				SDL_RenderCopy(renderer, texture_button_off, NULL, &rect_button_menu);			

			SDL_RenderCopy(renderer, texture_text_rejouer, NULL, &rect_text_rejouer);
			SDL_RenderCopy(renderer, texture_text_topscore, NULL, &rect_text_topscore);
			SDL_RenderCopy(renderer, texture_text_menu, NULL, &rect_text_menu);
	}

	char fps_text[4];
	sprintf(fps_text, "%d", fps);
	SDL_Surface* surface_text_fps = TTF_RenderText_Solid(font_menu,fps_text,black);
	SDL_Texture* texture_text2 = getTextureFromSurface(renderer,surface_text_fps);
	SDL_Rect rect_text_fps = { 0, 0, 0, 0 };
	SDL_QueryTexture(texture_text2, NULL, NULL, &rect_text_fps.w, &rect_text_fps.h );
	rect_text_fps.x = WINDOW_W-rect_text_fps.w-10; //WINDOW_W-10-rect_text_fps.w;
	rect_text_fps.y = 10;
	SDL_RenderCopy(renderer, texture_text2, NULL, &rect_text_fps);			
	SDL_FreeSurface(surface_text_fps);			
	SDL_DestroyTexture(texture_text2);

	// renderer
	SDL_RenderPresent(renderer);

	frame++;
}

void init() {
	// initialisation de la SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        printf("Unable to init SDL2: %s\n", SDL_GetError());
        exit(-1);
    }

    // création de la fenêtre    
   	window = SDL_CreateWindow(GAME_TITLE,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);

    if(!window)
    {
    	printf("Unable to create the window: %s\n", SDL_GetError());
    	exit(-1);
   	}

   	// réation du rendu
   	renderer = SDL_CreateRenderer(window, -1, 0);

   	if(!renderer) 
   	{
    	printf("Unable to create the renderer: %s\n", SDL_GetError());
    	exit(-1);
   	}

   	if(TTF_Init() == -1) 
   	{
   		exit(-1);
   	}

   	if(Mix_Init(0)) 
   	{
    	printf("Unable to init the mixer: %s\n", SDL_GetError());
   		exit(-1);
   	}

   	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
   		exit(-1);
   	} 

   	srand(time(NULL));
}

void init_var() {
	// statistique du joueurs
	pos = INIT_POS;
	speed = 0;
	score = 0;
	anim = 0;
	explo = 0;
	save_score = 0;
	compteur = 0;
	changeNeeded = 0;

	int i;
	for(i=0; i<10; i++) {
		text_nom[i] = ' ';
	}
	text_nom[10] = '\0';

	start_pressed = 0;
	stop_pressed = 0;

	if(nb_voies == VOIES_3 || nb_voies == VOIES_5)
		rect_car_pos.x = CAR_POS_2_3-CAR_W/2;
	else 
		rect_car_pos.x = CAR_POS_3_4-CAR_W/2;

	rect_car_pos.y = CAR_POS_Y;
	rect_car_pos.h = CAR_H;
	rect_car_pos.w = CAR_W;
	topos = rect_car_pos.x;
	
	rect_bg_1.x = 0;
	rect_bg_1.y = BG_1_POS_INIT_Y;
	rect_bg_1.w = WINDOW_W;
	rect_bg_1.h = WINDOW_H;
	
	rect_bg_2.x = 0;
	rect_bg_2.y = BG_2_POS_INIT_Y;
	rect_bg_2.w = WINDOW_W;
	rect_bg_2.h = WINDOW_H;
	
	rect_bg_3.x = 0;
	rect_bg_3.y = BG_3_POS_INIT_Y;
	rect_bg_3.w = WINDOW_W;
	rect_bg_3.h = WINDOW_H;

	rect_speed.x = WINDOW_W-SPEED_W-20;
	rect_speed.y = WINDOW_H-SPEED_H-20;
	rect_speed.w = SPEED_W;
	rect_speed.h = SPEED_H;
	
	rect_needle.x = 0;
	rect_needle.y = 0;
	rect_needle.w = NEEDLE_W;
	rect_needle.h = NEEDLE_H;

	id_car = 0;
	ia_cars = NULL;
	last_pop = 0;

	var_begin = 4;

	stats = fopen("speed.stats","w+");
}

void init_anim() {
	int i,j,k=0;

	for(i=0; i<4; i++) {
		for(j=0; j<4; j++) {
			rect_anim[k].x = 128*j;
			rect_anim[k].y = 128*i;
			rect_anim[k].w = 128;
			rect_anim[k].h = 128;
			k++;
		}
	}	
}

void load() {
	texture_menu_background = loadTexture(renderer,IMG_MENU_BACKGROUND);

	texture_button_on = loadTexture(renderer,IMG_BUTTON_ON);
	texture_button_off = loadTexture(renderer,IMG_BUTTON_OFF);

	font = TTF_OpenFont("fonts/keepcalm.ttf", 15);
	font_title = TTF_OpenFont("fonts/fonttitle.ttf", 60);
	font_ingame = TTF_OpenFont("fonts/fonttitle.ttf", 120);
	font_menu = TTF_OpenFont("fonts/font1.ttf", 20);

	char text[15];
	sprintf(text, "Crazy Drive");
	SDL_Surface* surface_title = TTF_RenderText_Solid(font_title,text,black);
	texture_text_title = getTextureFromSurface(renderer,surface_title);
	rect_text_title.x = WINDOW_H/2 - rect_text_title.h/2;
	rect_text_title.y = 100;
	SDL_QueryTexture(texture_text_title, NULL, NULL, &rect_text_title.w, &rect_text_title.h);
	SDL_FreeSurface(surface_title);

	char text2[10];
	sprintf(text2, "Jouer");
	surface_title = TTF_RenderText_Solid(font_menu,text2,black);
	texture_text_jouer = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_jouer, NULL, NULL, &rect_text_jouer.w, &rect_text_jouer.h);
	rect_text_jouer.x = rect_button_jouer.x + rect_button_jouer.w/2 - rect_text_jouer.w/2;
	rect_text_jouer.y = rect_button_jouer.y + rect_button_jouer.h/2 - rect_text_jouer.h/2;
	SDL_FreeSurface(surface_title);

	char text3[10];
	sprintf(text3, "Options");
	surface_title = TTF_RenderText_Solid(font_menu,text3,black);
	texture_text_options = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_options, NULL, NULL, &rect_text_options.w, &rect_text_options.h);
	rect_text_options.x = rect_button_options.x + rect_button_options.w/2 - rect_text_options.w/2;
	rect_text_options.y = rect_button_options.y + rect_button_options.h/2 - rect_text_options.h/2;
	SDL_FreeSurface(surface_title);

	char text4[10];
	sprintf(text4, "Quitter");
	surface_title = TTF_RenderText_Solid(font_menu,text4,black);
	texture_text_quitter = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_quitter, NULL, NULL, &rect_text_quitter.w, &rect_text_quitter.h);
	rect_text_quitter.x = rect_button_quitter.x + rect_button_quitter.w/2 - rect_text_quitter.w/2;
	rect_text_quitter.y = rect_button_quitter.y + rect_button_quitter.h/2 - rect_text_quitter.h/2;
	SDL_FreeSurface(surface_title);

	char text6[10];
	sprintf(text6, "Retro");
	surface_title = TTF_RenderText_Solid(font_menu,text6,black);
	texture_text_retro = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_retro, NULL, NULL, &rect_text_retro.w, &rect_text_retro.h);
	rect_text_retro.x = rect_button_jouer.x + rect_button_jouer.w/2 - rect_text_retro.w/2;
	rect_text_retro.y = rect_button_jouer.y + rect_button_jouer.h/2 - rect_text_retro.h/2;
	SDL_FreeSurface(surface_title);

	char text5[15];
	sprintf(text5, "Classique");
	surface_title = TTF_RenderText_Solid(font_menu,text5,black);
	texture_text_classic = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_classic, NULL, NULL, &rect_text_classic.w, &rect_text_classic.h);
	rect_text_classic.x = rect_button_options.x + rect_button_options.w/2 - rect_text_classic.w/2;
	rect_text_classic.y = rect_button_options.y + rect_button_options.h/2 - rect_text_classic.h/2;
	SDL_FreeSurface(surface_title);

	char text7[10];
	sprintf(text7, "3 voies");
	surface_title = TTF_RenderText_Solid(font_menu,text7,black);
	texture_text_3voies = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_3voies, NULL, NULL, &rect_text_3voies.w, &rect_text_3voies.h);
	rect_text_3voies.x = rect_button_jouer.x + rect_button_jouer.w/2 - rect_text_3voies.w/2;
	rect_text_3voies.y = rect_button_jouer.y + rect_button_jouer.h/2 - rect_text_3voies.h/2;
	SDL_FreeSurface(surface_title);

	char text8[10];
	sprintf(text8, "4 voies");
	surface_title = TTF_RenderText_Solid(font_menu,text8,black);
	texture_text_4voies = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_4voies, NULL, NULL, &rect_text_4voies.w, &rect_text_4voies.h);
	rect_text_4voies.x = rect_button_options.x + rect_button_options.w/2 - rect_text_4voies.w/2;
	rect_text_4voies.y = rect_button_options.y + rect_button_options.h/2 - rect_text_4voies.h/2;
	SDL_FreeSurface(surface_title);

	char text9[10];
	sprintf(text9, "5 voies");
	surface_title = TTF_RenderText_Solid(font_menu,text9,black);
	texture_text_5voies = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_5voies, NULL, NULL, &rect_text_5voies.w, &rect_text_5voies.h);
	rect_text_5voies.x = rect_button_quitter.x + rect_button_quitter.w/2 - rect_text_5voies.w/2;
	rect_text_5voies.y = rect_button_quitter.y + rect_button_quitter.h/2 - rect_text_5voies.h/2;
	SDL_FreeSurface(surface_title);

	char text10[15];
	sprintf(text10, "Menu");
	surface_title = TTF_RenderText_Solid(font_menu,text10,black);
	texture_text_menu = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_menu, NULL, NULL, &rect_text_menu.w, &rect_text_menu.h);
	rect_text_menu.x = rect_button_menu.x + rect_button_menu.w/2 - rect_text_menu.w/2;
	rect_text_menu.y = rect_button_menu.y + rect_button_menu.h/2 - rect_text_menu.h/2;
	SDL_FreeSurface(surface_title);

	char text12[15];
	sprintf(text12, "Top scores");
	surface_title = TTF_RenderText_Solid(font_menu,text12,black);
	texture_text_topscore = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_topscore, NULL, NULL, &rect_text_topscore.w, &rect_text_topscore.h);
	rect_text_topscore.x = rect_button_topscore.x + rect_button_topscore.w/2 - rect_text_topscore.w/2;
	rect_text_topscore.y = rect_button_topscore.y + rect_button_topscore.h/2 - rect_text_topscore.h/2;
	SDL_FreeSurface(surface_title);

	char text11[15];
	sprintf(text11, "Rejouer");
	surface_title = TTF_RenderText_Solid(font_menu,text11,black);
	texture_text_rejouer = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_text_rejouer, NULL, NULL, &rect_text_rejouer.w, &rect_text_rejouer.h);
	rect_text_rejouer.x = rect_button_rejouer.x + rect_button_rejouer.w/2 - rect_text_rejouer.w/2;
	rect_text_rejouer.y = rect_button_rejouer.y + rect_button_rejouer.h/2 - rect_text_rejouer.h/2;
	SDL_FreeSurface(surface_title);

	char manuel[15];
	sprintf(manuel, "Manuel");
	surface_title = TTF_RenderText_Solid(font_menu,manuel,black);
	texture_player = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_player, NULL, NULL, &rect_text_player.w, &rect_text_player.h);
	rect_text_player.x = rect_button_jouer.x + rect_button_jouer.w/2 - rect_text_player.w/2;
	rect_text_player.y = rect_button_jouer.y + rect_button_jouer.h/2 - rect_text_player.h/2;
	SDL_FreeSurface(surface_title);

	char autopilot[15];
	sprintf(autopilot, "Autopilote");
	surface_title = TTF_RenderText_Solid(font_menu,autopilot,black);
	texture_autopilot = getTextureFromSurface(renderer,surface_title);
	SDL_QueryTexture(texture_autopilot, NULL, NULL, &rect_text_autopilot.w, &rect_text_autopilot.h);
	rect_text_autopilot.x = rect_button_options.x + rect_button_options.w/2 - rect_text_autopilot.w/2;
	rect_text_autopilot.y = rect_button_options.y + rect_button_options.h/2 - rect_text_autopilot.h/2;
	SDL_FreeSurface(surface_title);

	background_music = Mix_LoadWAV(BACKGROUND_MUSIC);

	if(Mix_PlayChannel(1, background_music, -1) == -1) {
		printf("Error playing background music : %s\n", Mix_GetError());
	}
}

void load_game() {
	if(nb_voies == VOIES_3) 
	{
		texture_road = loadTexture(renderer,IMG_ROAD_3);
	}
	else if(nb_voies == VOIES_4) 
	{
		texture_road = loadTexture(renderer,IMG_ROAD_4);
	}
	else if(nb_voies == VOIES_5) 
	{
		texture_road = loadTexture(renderer,IMG_ROAD_5);
	}

	texture_car_yellow = loadTexture(renderer,IMG_CAR_YELLOW);
	texture_car_red = loadTexture(renderer,IMG_CAR_RED);

	texture_speed = loadTexture(renderer, IMG_SPEED);
	surface_needle = loadSurface(IMG_NEEDLE);

	texture_text_background_menuig = loadTexture(renderer,IMG_BUTTON_OFF);

	texture_anim = loadTexture(renderer,IMG_ANIM);

	crash = Mix_LoadWAV(CRASH_PATH);
	start = Mix_LoadWAV(START);

	init_anim();
}

void loadTopscores() {
	
	scores_list_3 = delScore(scores_list_3);
	scores_list_4 = delScore(scores_list_4);
	scores_list_5 = delScore(scores_list_5);
	scores_list_3_c = delScore(scores_list_3_c);
	scores_list_4_c = delScore(scores_list_4_c);
	scores_list_5_c = delScore(scores_list_5_c);

	FILE* save = fopen("score.save", "r");

	if(save != NULL) {
		char buff[50];

		while(fgets(buff, 50, save)) {
			const char lmt[2] = ";";
			char* tok_nom;
			char* tok_score;
			char* tok_voies;
			char* tok_mode;

			tok_nom = strtok(buff, lmt);
			tok_score = strtok(NULL, lmt);
			tok_voies = strtok(NULL, lmt);
			tok_mode = strtok(NULL, lmt);

			int sc = atoi(tok_score);
			int voies = atoi(tok_voies);
			int mode_s = atoi(tok_mode);


			if(mode_s == 1) {
				if(voies == 3) {
					scores_list_3 = addScore(scores_list_3, sc, tok_nom);
				}
				else if(voies == 4) {
					scores_list_4 = addScore(scores_list_4, sc, tok_nom);
				}
				else if(voies == 5) {
					scores_list_5 = addScore(scores_list_5, sc, tok_nom);
				}
			}
			else {
				if(voies == 3) {
					scores_list_3_c = addScore(scores_list_3_c, sc, tok_nom);
				}
				else if(voies == 4) {
					scores_list_4_c = addScore(scores_list_4_c, sc, tok_nom);
				}
				else if(voies == 5) {
					scores_list_5_c = addScore(scores_list_5_c, sc, tok_nom);
				}
			}
		}

		fclose(save);
	}	
	else {
		state = MENU_STATE;
	}
}

void clear() {
	SDL_DestroyTexture(texture_road);
    SDL_DestroyTexture(texture_car_yellow);
    SDL_DestroyTexture(texture_car_red);
    SDL_DestroyTexture(texture_speed);
	SDL_DestroyTexture(texture_text_title);
	SDL_DestroyTexture(texture_button_on);
	SDL_DestroyTexture(texture_button_off);

    SDL_FreeSurface(surface_needle);

	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(window);

	TTF_CloseFont(font);
	TTF_CloseFont(font_title);
	TTF_CloseFont(font_menu);

	Mix_FreeChunk(crash);

	Mix_Quit();
	TTF_Quit();	
    SDL_Quit();
}

int main(int argc, char** argv)
{
	// initialise la SDL, la fênetre et le rendus
    init();

   	// on charge les images
    load();

    while (program == PROGRAM_RUN)
	{	
		event();
		work();
		display();

        SDL_Delay(DELAY);
    }
        
   	// on vide toutes les ressources proprement    
   	clear();

    return 0;
}
