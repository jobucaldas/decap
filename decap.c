/*
 *	// Trabalho final de CAP - Jogo 'DeCAP' em allegro - //
 *  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *	João Victor Bueno de Caldas - 769657
 *	Matheus Ramos de Carvalho   - 769703
 *	Carlos Santana              - 773370
 */

 // Libraries
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define FPS 60.0

// Bitmap creation function
ALLEGRO_BITMAP* create_bitmap(char* str) {
	ALLEGRO_BITMAP* img = NULL;
	img = al_load_bitmap(str);
	if (!img){
		fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
		exit(1);
	}

	return img;
}

ALLEGRO_AUDIO_STREAM* create_audio_stream(char* str) {
	ALLEGRO_AUDIO_STREAM* stream = NULL;
	stream = al_load_audio_stream(str, 4, 1024);
	if (!stream){
		fprintf(stderr, "Failed loading audio stream.\n");
		exit(1);
	}

	return stream;
}

// Draw screen when no screen was called
void draw_noscr(int* current_scr, int* select, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj, int* close_window) {
	int selected[2] = { 0,0 };
	int i;

	al_draw_text(font, al_map_rgb(255, 255, 255), 80, 300 - 60, ALLEGRO_ALIGN_LEFT, "Por algum motivo nenhuma tela foi chamada,");
	al_draw_text(font, al_map_rgb(255, 255, 255), 80 + 140, 300 - 20, ALLEGRO_ALIGN_LEFT, "deseja voltar ao menu?");

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_AXES) {
		if (event_obj.mouse.y > 400 && event_obj.mouse.y < 400 + 36) {
			if (event_obj.mouse.x > 400 - 80 && event_obj.mouse.x < 400 - 60)
				* select = 0;
			else if (event_obj.mouse.x > 400 + 50 - 16 && event_obj.mouse.x < 400 + 70 - 16)
				* select = 1;
		}
		else *select = 20;

		for (i = 0; i < 2; i++)
			if (i == *select)
				selected[i] = 150;
			else
				selected[i] = 0;
	}
	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 1)
			* close_window = 1;
		else if (*select == 0) {
			*current_scr = 0;
			*select = 20;
		}

	al_draw_text(font, al_map_rgb(255 - selected[0], 255 - selected[0], 255 - selected[0]), 400 - 80, 400, ALLEGRO_ALIGN_LEFT, "Sim");
	al_draw_text(font, al_map_rgb(255 - selected[1], 255 - selected[1], 255 - selected[1]), 400 + 50 - 16, 400, ALLEGRO_ALIGN_LEFT, "Não");
}

// Draw game main menu
void draw_menu(int* current_scr, int* select, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj, int* close_window) {
	int selected[5] = { 0,0,0,0,0 };
	int i;

	// Logo
	al_draw_text(font, al_map_rgb(255, 255, 255), 1024 / 2 - 32, 200 / 2, ALLEGRO_ALIGN_LEFT, "DeCAP");

	// Parse mouse axes
	if (event_obj.type == ALLEGRO_EVENT_MOUSE_AXES) {
		// Selects hovered option
		if (event_obj.mouse.x < 350)
			if (event_obj.mouse.y > 300 && event_obj.mouse.y < 300 + 32)
				* select = 0;
			else if (event_obj.mouse.y > 300 + 50 && event_obj.mouse.y < 300 + 50 + 40)
				* select = 1;
			else if (event_obj.mouse.y > 300 + 50 * 2 && event_obj.mouse.y < 300 + 50 * 2 + 40)
				* select = 2;
			else if (event_obj.mouse.y > 300 + 50 * 3 && event_obj.mouse.y < 300 + 50 * 3 + 40)
				* select = 3;
			else if (event_obj.mouse.y > 300 + 50 * 4 && event_obj.mouse.y < 300 + 50 * 4 + 40)
				* select = 4;
			else
				*select = 20;
		else
			*select = 20;

		// Paints selected option grey
		for (i = 0; i < 5; i++)
			if (i == *select)
				selected[i] = 150;
			else
				selected[i] = 0;
	}
	// Parse mouse click
	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 4)
			* close_window = 1;
		else if (*select == 0) {
			*current_scr = 5;
			*select = 0;
		}
		else if (*select == 3) {
			*current_scr = 2;
			*select = 20;
		}
		else if (*select == 1) {
			*current_scr = 4;
			*select = 20;
		}
		else if (*select == 2) {
			*current_scr = 3;
			*select = 20;
		}

	// Opcoes
	al_draw_text(font, al_map_rgb(255 - selected[0], 255 - selected[0], 255 - selected[0]), 20, 300, ALLEGRO_ALIGN_LEFT, "Iniciar");
	al_draw_text(font, al_map_rgb(255 - selected[1], 255 - selected[1], 255 - selected[1]), 20, 300 + 50, ALLEGRO_ALIGN_LEFT, "High Scores");
	al_draw_text(font, al_map_rgb(255 - selected[2], 255 - selected[2], 255 - selected[2]), 20, 300 + 50 * 2, ALLEGRO_ALIGN_LEFT, "Briefing");
	al_draw_text(font, al_map_rgb(255 - selected[3], 255 - selected[3], 255 - selected[3]), 20, 300 + 50 * 3, ALLEGRO_ALIGN_LEFT, "Opções");
	al_draw_text(font, al_map_rgb(255 - selected[4], 255 - selected[4], 255 - selected[4]), 20, 300 + 50 * 4, ALLEGRO_ALIGN_LEFT, "Sair");
}

// Draw game screen
void draw_game(int* current_scr, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_BITMAP* bg, ALLEGRO_EVENT event_obj) {
}

// Draw game options screen
void draw_options(int* current_scr, int* select, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj) {
	int selected[2] = { 0,0 };
	int i;

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_AXES) {
		if (event_obj.mouse.x < 350) {
			if (event_obj.mouse.y > 300 + 50 * 1.8 && event_obj.mouse.y < 300 + 50 * 1.8 + 40)
				* select = 0;
			else if (event_obj.mouse.y > 300 + 50 * 4 && event_obj.mouse.y < 300 + 50 * 4 + 40)
				* select = 1;
		}
		else *select = 20;

		for (i = 0; i < 2; i++)
			if (i == *select)
				selected[i] = 150;
			else
				selected[i] = 0;
	}
	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 1) {
			*current_scr = 0;
			*select = 20;
		}

	al_draw_text(font, al_map_rgb(255 - selected[0], 255 - selected[0], 255 - selected[0]), 20, 300 + 50 * 1.8, ALLEGRO_ALIGN_LEFT, "Mutar");
	al_draw_text(font, al_map_rgb(255 - selected[1], 255 - selected[1], 255 - selected[1]), 20, 300 + 50 * 4, ALLEGRO_ALIGN_LEFT, "Voltar");
}

// Draw briefing screen
void draw_briefing(int* current_scr, int* select, ALLEGRO_DISPLAY* window, ALLEGRO_FONT* font, ALLEGRO_FONT* small_font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj) {
	int selected = 0;

	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30, ALLEGRO_ALIGN_LEFT, "Após se preparar para o ultimo boss");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30, ALLEGRO_ALIGN_LEFT, "do seu mmorpg favorito por dias você");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 2, ALLEGRO_ALIGN_LEFT, "acaba perdendo mesmo após tirar mais");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 3, ALLEGRO_ALIGN_LEFT, "de 90% de sua vida total. Não sendo");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 4, ALLEGRO_ALIGN_LEFT, "suficientemente ruim o jogo reinicia");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 5, ALLEGRO_ALIGN_LEFT, "na primeira sala.");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 6, ALLEGRO_ALIGN_LEFT, "Agora você está sonhando estar na dungeon?");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 7, ALLEGRO_ALIGN_LEFT, "Ou será que esse é mesmo o mundo do jogo?");

	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 10, ALLEGRO_ALIGN_LEFT, "Se movimente com as teclas \"W\" \"A\" \"S\" \"D\"!");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 11, ALLEGRO_ALIGN_LEFT, "Use o mouse para mirar em seus inimigos!");
	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30 + 30 * 12, ALLEGRO_ALIGN_LEFT, "Sobreviva o máximo que conseguir!");

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_AXES) {
		if (event_obj.mouse.x < 350) {
			if (event_obj.mouse.y > 300 + 50 * 4 && event_obj.mouse.y < 300 + 50 * 4 + 40)
				* select = 0;
		}
		else *select = 20;

		if (*select == 0)
			selected = 150;
		else
			selected = 0;

	}
	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 0) {
			*current_scr = 0;
			*select = 20;
		}

	al_draw_text(font, al_map_rgb(255 - selected, 255 - selected, 255 - selected), 20, 300 + 50 * 4, ALLEGRO_ALIGN_LEFT, "Voltar");
}

// Draw high scores
void draw_hiscore(int* current_scr, int* select, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj) {
	int selected = 0;

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_AXES) {
		if (event_obj.mouse.x < 350) {
			if (event_obj.mouse.y > 300 + 50 * 4 && event_obj.mouse.y < 300 + 50 * 4 + 40)
				* select = 0;
		}
		else *select = 20;

		if (*select == 0)
			selected = 150;
		else
			selected = 0;

	}
	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 0) {
			*current_scr = 0;
			*select = 20;
		}

	al_draw_text(font, al_map_rgb(255 - selected, 255 - selected, 255 - selected), 20, 300 + 50 * 4, ALLEGRO_ALIGN_LEFT, "Voltar");
}

void draw_name(int* current_scr, int* select, char* nick, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj) {
	al_draw_text(font, al_map_rgb(0, 0, 0), 512-200, 300 - 160, ALLEGRO_ALIGN_LEFT, "Digite um nick (3 letras)");
	al_draw_textf(font, al_map_rgb(0, 0, 0), 512 - 30, 576/2-16, ALLEGRO_ALIGN_LEFT, "%s", nick);

	if (event_obj.type == ALLEGRO_EVENT_KEY_DOWN) {
		int plus = 0;
		switch (event_obj.keyboard.keycode) {
		case ALLEGRO_KEY_A:
			if (*select < 3)
				nick[*select] = 'A';
			plus = 1;
			break;
		case ALLEGRO_KEY_B:
			if (*select < 3)
				nick[*select] = 'B';
			plus = 1;
			break;
		case ALLEGRO_KEY_C:
			if (*select < 3)
				nick[*select] = 'C';
			plus = 1;
			break;
		case ALLEGRO_KEY_D:
			if (*select < 3)
				nick[*select] = 'D';
			plus = 1;
			break;
		case ALLEGRO_KEY_E:
			if (*select < 3)
				nick[*select] = 'E';
			plus = 1;
			break;
		case ALLEGRO_KEY_F:
			if (*select < 3)
				nick[*select] = 'F';
			plus = 1;
			break;
		case ALLEGRO_KEY_G:
			if (*select < 3)
				nick[*select] = 'G';
			plus = 1;
			break;
		case ALLEGRO_KEY_H:
			if (*select < 3)
				nick[*select] = 'H';
			plus = 1;
			break;
		case ALLEGRO_KEY_I:
			if (*select < 3)
				nick[*select] = 'I';
			plus = 1;
			break;
		case ALLEGRO_KEY_J:
			if (*select < 3)
				nick[*select] = 'J';
			plus = 1;
			break;
		case ALLEGRO_KEY_K:
			if (*select < 3)
				nick[*select] = 'K';
			plus = 1;
			break;
		case ALLEGRO_KEY_L:
			if (*select < 3)
				nick[*select] = 'L';
			plus = 1;
			break;
		case ALLEGRO_KEY_M:
			if (*select < 3)
				nick[*select] = 'M';
			plus = 1;
			break;
		case ALLEGRO_KEY_N:
			if (*select < 3)
				nick[*select] = 'N';
			plus = 1;
			break;
		case ALLEGRO_KEY_O:
			if (*select < 3)
				nick[*select] = 'O';
			plus = 1;
			break;
		case ALLEGRO_KEY_P:
			if (*select < 3)
				nick[*select] = 'P';
			plus = 1;
			break;
		case ALLEGRO_KEY_Q:
			if (*select < 3)
				nick[*select] = 'Q';
			plus = 1;
			break;
		case ALLEGRO_KEY_R:
			if (*select < 3)
				nick[*select] = 'R';
			plus = 1;
			break;
		case ALLEGRO_KEY_S:
			if (*select < 3)
				nick[*select] = 'S';
			plus = 1;
			break;
		case ALLEGRO_KEY_T:
			if (*select < 3)
				nick[*select] = 'T';
			plus = 1;
			break;
		case ALLEGRO_KEY_U:
			if (*select < 3)
				nick[*select] = 'U';
			plus = 1;
			break;
		case ALLEGRO_KEY_V:
			if (*select < 3)
				nick[*select] = 'V';
			plus = 1;
			break;
		case ALLEGRO_KEY_W:
			if (*select < 3)
				nick[*select] = 'W';
			plus = 1;
			break;
		case ALLEGRO_KEY_X:
			if (*select < 3)
				nick[*select] = 'X';
			plus = 1;
			break;
		case ALLEGRO_KEY_Y:
			if (*select < 3)
				nick[*select] = 'Y';
			plus = 1;
			break;
		case ALLEGRO_KEY_Z:
			if (*select < 3)
				nick[*select] = 'Z';
			plus = 1;
			break;
		case ALLEGRO_KEY_ENTER:
			*select = 20;
			*current_scr = 1;
			break;
		default:
			break;
		}

		if (plus)
			*select = *select + 1;
	}

	if(*select>=3)
		al_draw_text(font, al_map_rgb(0,0,0), 400 - 120, 400, ALLEGRO_ALIGN_LEFT, "Pressione ENTER para continuar");
}

// Draw current screen (game, menu, etc)
void draw_scr(int* current_scr, int* select, char* nick, ALLEGRO_DISPLAY* window, ALLEGRO_FONT* font, ALLEGRO_FONT* small_font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj, ALLEGRO_BITMAP* bg, int* close_window) {
	/*
	 *  0 - Menu
	 *  1 - Jogo
	 *  2 - Opções
	 *  3 - Briefing
	 *  4 - High Scores
	 */

	switch (*current_scr) {
	case 0:
		draw_menu(current_scr, select, font, has_event, event_queue, event_obj, close_window);
		break;
	case 1:
		draw_game(current_scr, font, has_event, event_queue, bg, event_obj);
		break;
	case 2:
		draw_options(current_scr, select, font, has_event, event_queue, event_obj);
		break;
	case 3:
		draw_briefing(current_scr, select, window, font, small_font, has_event, event_queue, event_obj);
		break;
	case 4:
		draw_hiscore(current_scr, select, font, has_event, event_queue, event_obj);
		break;
	case 5:
		draw_name(current_scr, select, nick, font, has_event, event_queue, event_obj);
		break;
	default:
		draw_noscr(current_scr, select, font, has_event, event_queue, event_obj, close_window);
		break;
	}
}

// Main function
int main(void) {
	// Allegro objects
	ALLEGRO_DISPLAY* window = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_EVENT event_obj;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_TIMEOUT timeout;
	ALLEGRO_FONT* font = NULL;
	ALLEGRO_FONT* small_font = NULL;

	// Variables
	int current_scr = 0;
	int old_scr = current_scr;
	int close_window = 0;
	int select = 20;
	char nick[3];

	/* Initialize allegro */
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		exit(1);
	}
	if (!al_init_image_addon()) {
		fprintf(stderr, "Failed to initialize allegro's image addon.\n");
		exit(1);
	}
	if (!al_init_font_addon()) {
		fprintf(stderr, "Failed to initialize allegro's font addon.\n");
		exit(1);
	}
	if (!al_init_ttf_addon()) {
		fprintf(stderr, "Failed to initialize allegro's ttf addon.\n");
		exit(1);
	}

	/*
	// Load bitmaps
	ALLEGRO_BITMAP* bg = create_bitmap("img/menu.png");
	ALLEGRO_BITMAP* character = create_bitmap("img/char.png");
	ALLEGRO_BITMAP* zombie = create_bitmap("img/zombie.png");
	ALLEGRO_BITMAP* ghost = create_bitmap("img/ghost.png");
	ALLEGRO_BITMAP* goblin = create_bitmap("img/goblin.png");
	ALLEGRO_BITMAP* bullet = create_bitmap("img/bullet.png");
	*/

	// Window creation
	window = al_create_display(1024, 576);
	if (!window) {
		fprintf(stderr, "Failed creating a window.\n");
		exit(1);
	}
	al_set_window_title(window, "DeCAP");

	// Initialize timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to initialize timer.\n");
		exit(1);
	}

	// Initialize mouse
	if (!al_install_mouse()) {
		al_destroy_display(window);
		fprintf(stderr, "Failed to initialize mouse support.\n");
		exit(1);
	}
	if (!al_set_system_mouse_cursor(window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)) {
		al_destroy_display(window);
		fprintf(stderr, "Failed to initialize mouse cursor.\n");
		exit(1);
	}

	// Configure keyboard
	if (!al_install_keyboard()) {
		fprintf(stderr, "Failed initializing the keyboard.\n");
		exit(1);
	}

	// Configure audio
	if (!al_install_audio()) {
		fprintf(stderr, "Failed initializing audio.\n");
		exit(1);
	}
	if (!al_init_acodec_addon()) {
		fprintf(stderr, "Failed initializing audio codec.\n");
		exit(1);
	}
	if (!al_reserve_samples(5)) {
		fprintf(stderr, "Failed reserving samples.\n");
		exit(1);
	}

	// Create event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Failed initializing event queue.\n");
		al_destroy_display(window);
		exit(1);
	}
	al_register_event_source(event_queue, al_get_display_event_source(window));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	int has_event = al_wait_for_event_until(event_queue, &event_obj, &timeout);

	// Create font
	font = al_load_font("font/pc98.ttf", 32, 0);
	if (!font) {
		al_destroy_display(window);
		fprintf(stderr, "Failed creating the font.\n");
		exit(1);
	}
	small_font = al_load_font("font/pc98.ttf", 24, 0);
	if (!small_font) {
		al_destroy_display(window);
		fprintf(stderr, "Failed creating the font.\n");
		exit(1);
	}

	// Configure timeout
	al_init_timeout(&timeout, 0.05);

	// Audio
	// Initialize bgm
	ALLEGRO_AUDIO_STREAM* bgm = NULL;
	bgm = create_audio_stream("bgm/menu.ogg");
	if (!bgm) {
		fprintf(stderr, "Failed creating audio stream.\n");
		exit(1);
	}
	al_attach_audio_stream_to_mixer(bgm, al_get_default_mixer());
	al_set_audio_stream_playmode(bgm, ALLEGRO_PLAYMODE_LOOP);

	// Images
	// Initialize images
	ALLEGRO_BITMAP* bg = NULL;
	bg = al_load_bitmap("bg/main_menu.png");
	if (!bg) {
		fprintf(stderr, "Failed crating bg bitmap.\n");
		exit(1);
	}

	/* Game loop */
	while (!close_window) {
		// Gets frame inputs
		has_event = al_wait_for_event_until(event_queue, &event_obj, &timeout);

		// Close window if close button is pressed
		if (event_obj.type == ALLEGRO_EVENT_DISPLAY_CLOSE && has_event)
			close_window = 1;

		// Loads audio once
		if (current_scr != old_scr) {
			if (current_scr == 1) {
				al_destroy_audio_stream(bgm);
				bgm = create_audio_stream("bgm/gameplay.ogg");
				if (!bgm) {
					fprintf(stderr, "Failed initializing song.\n");
					exit(1);
				}
				al_attach_audio_stream_to_mixer(bgm, al_get_default_mixer());
				al_set_audio_stream_playmode(bgm, ALLEGRO_PLAYMODE_LOOP);

				bg = al_load_bitmap("bg/arena.png");
				if (!bg) {
					fprintf(stderr, "Failed crating bg bitmap.\n");
					exit(1);
				}
			} else if (current_scr == 3) {
				bg = al_load_bitmap("bg/room.png");
				if (!bg) {
					fprintf(stderr, "Failed crating bg bitmap.\n");
					exit(1);
				}
			} else if (current_scr == 0) {
				bg = al_load_bitmap("bg/main_menu.png");
				if (!bg) {
					fprintf(stderr, "Failed crating bg bitmap.\n");
					exit(1);
				}
			}
			else if (current_scr == 5) {
				bg = al_load_bitmap("bg/name.png");
				if (!bg) {
					fprintf(stderr, "Failed crating bg bitmap.\n");
					exit(1);
				}
			}
			else if (current_scr == 2) {
				bg = al_load_bitmap("bg/option.png");
				if (!bg) {
					fprintf(stderr, "Failed crating bg bitmap.\n");
					exit(1);
				}
			}
			old_scr = current_scr;
		}

		// Update display
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_bitmap(bg, 0, 0, 0);
		draw_scr(&current_scr, &select, nick, window, font, small_font, has_event, event_queue, event_obj, bg, &close_window);
		al_flip_display();
	}

	// Close program
	al_destroy_display(window);
	al_destroy_event_queue(event_queue);
	return 0;
}