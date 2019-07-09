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
#include <math.h>

#define FPS 60.0
#define CHAR_SIZE 96
#define SHEET_COLLUMNS 4

/*******************************************************************************************************************
********************************************************************************************************************/
// FILES

//Struct ranking
struct rankingUsuario {
	char name[4];
	int pontos;
};
//Struct bullets
typedef struct{
	float posx,posy,m,q;
	int current_collumn, current_line, cont_frames, sheety, sheetx,i, enable;
}b;
//Struct monster
typedef struct{
	float posx,posy,m,q,i;
	int current_collumn, current_line, cont_frames, sheety, sheetx, enable,type;
}m;
//Struct main character
typedef struct {
	float vel, posx, posy;
	int current_collumn, current_line, cont_frames, sheety, sheetx;
} character;
typedef struct rankingUsuario rgp;
//file line counter
int lines(){
    FILE* fp;
	int i = 0;
	if ((fp = fopen("data.txt", "r")) != NULL) {
		i = 0;
		while(!feof(fp)){
			if(fgetc(fp)=='\n')
                i++;
		}
		fclose(fp);
	}

    return i;
}
//function that adds new score and returns the top 10
rgp* add_score(char name[], int score) {
	FILE* fp;
	rgp* a;
	int i, n = lines() + 1, p, igual = 0, indice, bug = 1;
	if ((fp = fopen("data.txt", "r")) != NULL) {
		a = (rgp*)malloc(n * sizeof(rgp));
		i = 0;
		for (i = 0; i < n - 1; i++) {
			fscanf(fp, "%s %d\n", &a[i].name, &a[i].pontos);
		}
		for (p = 0; p < 2; p++) {
			for (i = n - 2; i >= 0; i--) {
				if (strcmp(a[i].name, name) == 0 && a[i].pontos >= score && strcmp(a[i].name, "NON") != 0) {
					igual = 2;
				}
				else if (strcmp(a[i].name, name) == 0 && strcmp(a[i].name, "NON") != 0) {
					igual = 1;
					indice = i;
				}
				if (a != NULL && p == 1 && i > 0 && igual != 2) {
                    //delete a[i].name and a[i].pontos if a[i].name == name and score>a[i].pontos
					if (igual == 1 && i - 2 >= indice && i >= 2) {
						a[i - 2].pontos = a[i - 1].pontos;
						strcpy(a[i - 2].name, a[i - 1].name);
						a[n - 1].pontos = 0;
						strcpy(a[n - 1].name, "NON");
					}
                    //copy lines with pontos<score to down
					if (a[i - 1].pontos < score) {
						a[i].pontos = a[i - 1].pontos;
						strncpy(a[i].name, a[i - 1].name, 3);
						if (i == 1) {
							strcpy(a[0].name, name);
							a[0].pontos = score;
						}

					}
                    //set name and score position
					else if (a[i - 1].pontos >= score && a[i].pontos < score) {
						a[i].pontos = score;
						strcpy(a[i].name, name);
					}
					// if score == 0, set name to last position
					else if (i == n - 2 && score <= a[i].pontos) {
						a[i + 1].pontos = score;
						strcpy(a[i + 1].name, name);
						bug = 0;
					}
				}
			}
		}
		fclose(fp);
		fp = fopen("data.txt", "w");
		//rewrite file
		for (i = 0; i < n - bug; i++) {
			if (i < 10 || (strcmp(a[i].name, "NON") != 0||a[i].pontos!=0)) {
				fprintf(fp, "%s %d\n", a[i].name, a[i].pontos);
			}

		}
		fclose(fp);
	}
	else {
		fp = fopen("data.txt", "w");
		n = 10;
		a = (rgp*)malloc(n * sizeof(rgp));
		fprintf(fp, "%s %d\n", name, score);
		strcpy(a[0].name, name);
		a[0].pontos = score;
		for (i = 1; i < 10; i++) {
			fprintf(fp, "NON 0\n");
			strcpy(a[i].name, "NON");
			a[i].pontos = 0;
		}
		fclose(fp);
	}
	a = (rgp*)realloc(a, 10 * sizeof(rgp));
	return a;
}

/*******************************************************************************************************************
********************************************************************************************************************/

// Bitmap creation function
ALLEGRO_BITMAP* create_bitmap(char* str) {
	ALLEGRO_BITMAP* img = NULL;
	img = al_load_bitmap(str);
	if (!img){
		fprintf(stderr, "Falha ao carregar o arquivo de imagem %s.\n", str);
		exit(1);
	}

	return img;
}
// Audio creation function
ALLEGRO_AUDIO_STREAM* create_audio_stream(char* str) {
	ALLEGRO_AUDIO_STREAM* stream = NULL;
	stream = al_load_audio_stream(str, 4, 1024);
	if (!stream){
		fprintf(stderr, "Failed loading audio stream.\n");
		exit(1);
	}

	return stream;
}

//monster initializer
void init_m( m * blts) {
    int k;
    for(k=0;k<50;k++){
        blts[k].enable = 0;
        blts[k].m=0;
        blts[k].q=0;
        blts[k].posx = 1024/2-32;
        blts[k].posy = 576 / 2 - 32;
        blts[k].current_collumn = 0;
        blts[k].current_line = 0;
        blts[k].cont_frames = 0;
        blts[k].sheetx = 0;
        blts[k].sheety = 0;
        blts[k].i=0;
    }
}
//main char initializer
void init_char(character* main_char) {
	main_char->vel = 0.5;
	main_char->posx = 1024/2-32;
	main_char->posy = 576 / 2 - 32;
	main_char->current_collumn = 0;
	main_char->current_line = 3;
	main_char->cont_frames = 10;
	main_char->sheetx = 0;
	main_char->sheety = 96*3;
}
//bullets initializer
void init_blts( b * blts) {
    int k;
    for(k=0;k<3;k++){
        blts[k].enable = 0;
        blts[k].m=0;
        blts[k].q=0;
        blts[k].posx = 1024/2-32;
        blts[k].posy = 576 / 2 - 32;
        blts[k].current_collumn = 0;
        blts[k].current_line = 0;
        blts[k].cont_frames = 0;
        blts[k].sheetx = 0;
        blts[k].sheety = blts[k].current_line * 12;
        blts[k].i=0;
    }
}
// bullets spawn
void spawn_blts(b blts[],float mx,float my, float px, float py){
    int k=0;
    if(blts[k].enable!=1){
        //angular coefficient
        blts[k].m=(py-my)/(px-mx);
        //linear coefficient
        blts[k].q=my-(blts[k].m*mx);
        //set direction
        blts[k].i=((mx-px)?2*(mx-px)/abs(mx-px):2);
        //spawn behaviors
        blts[k].posx=px;

        blts[k].posy=py;

        blts[k].enable=1;
    }
}
//monster spawn
void spawn_m(m mnst[],double delta,float timer, float mx, float my){
    int k=0,t=((int)(timer/delta*10)),r=10*(((int)(timer/delta*13))%4);
    float px = 0, py = 0;
    //spawn timer
    if(t%10==0){
        //random spawnpoint
        t=((t%100)/10)%4;
        switch (t){
            case(0):
                px=1024/2-92/2+r;
                py=-200+r;
                break;
            case(1):
                px=1024/2+r;
                py=776+r;
                break;
            case(2):
                px=1224+r;
                py=576/2-96/2+r;
                break;
            case(3):
                px=-200+r;
                py=576/2-96/2;
                break;
        }
        for(k=0;k<50;k++){
            if(mnst[k].enable==0){
                //zombie or goblin
                mnst[k].type =abs(((int)(px))%3)%2;
                //angular coefficient
                mnst[k].m=(mx==mnst[k].posx?0:((mnst[k].posy-my)/(mnst[k].posx-mx)));
                //linear coefficient
                mnst[k].q=my-(mnst[k].m*mx);
                //direction
                mnst[k].i=0.8*(((int)mx!=(int)mnst[k].posx)?(0.2-(0.1*mnst[k].type))*(mx-mnst[k].posx)/abs(mx-mnst[k].posx):0.2-(0.1*mnst[k].type));

                mnst[k].posx=px;

                mnst[k].posy=py;
                //sprite draw position in spritesheet
                mnst[k].current_line=mnst[k].type*2;
                mnst[k].enable=1;
                k=50;
            }
        }
    }
}

void destr_blts(b*blts,m* mnst,int * score){
    int k=0,j;
    //collision with a border
    if (blts[k].enable == 1 && (blts[k].posx > 1024 || blts[k].posx < 0 || blts[k].posy>576 || blts[k].posy < 0)) {
        blts[k].enable = 0;
    }
    for(j=0;j<50;j++){
        //collision with a monster
        if(blts[k].posx>mnst[j].posx && blts[k].posx<mnst[j].posx+96 && blts[k].posy>mnst[j].posy && blts[k].posy<mnst[j].posy+96 && blts[k].enable==1 &&  mnst[j].enable==1){
            //state definition
            blts[k].enable=0;
            //set died monster behaviors
            mnst[j].enable=2;
            mnst[j].current_line = 6;
            mnst[j].sheety = 6 * 96;
            *score=*score+100;
            j=50;
        }
    }
}
    //bullets move function
void move_blts(b*blts,double delta){
    int k=0;
    if(blts[k].enable==1){
        //set bullet's new coordinate
        blts[k].posx=blts[k].posx+cos(atan(blts[k].m))*blts[k].i/delta;
        blts[k].posy=blts[k].posy+sin(atan(blts[k].m))*blts[k].i/delta;
    }

}
    //monster move function
void move_m(m*mnst,double delta,double timer,float mx,float my){
    int k=0;
    for(k=0;k<50;k++){
        if (mnst[k].enable==1){
            //movement after leaving the spawn point
            if(mnst[k].posx>50&&mnst[k].posx<924&&mnst[k].posy>50&&mnst[k].posy<400){
                //avoiding division by zero
                if(abs(mnst[k].posx-mx)>4){
                    //if monster==zombie or timer%4==0, update direction
                    if(( ((int)timer)%4==0 )|| mnst[k].type==1){
                        //set angular coefficient
                        mnst[k].m=(mnst[k].posy-my)/(mnst[k].posx-mx);
                        //set linear coefficient
                        mnst[k].q=my-(mnst[k].m*mx);
                        //set direction
                        mnst[k].i=0.5*(((mnst[k].m)!=0)?(0.7-(0.5*mnst[k].type))*(mx-mnst[k].posx)/abs(mx-mnst[k].posx):0.7-(0.5*mnst[k].type));
                    }
                        //set new posx
                        mnst[k].posx=mnst[k].posx+cos(atan(mnst[k].m))*mnst[k].i/delta;

                }

            if(abs(mnst[k].posy-my)>4)
                mnst[k].posy=mnst[k].posy+sin(atan(mnst[k].m))*mnst[k].i/delta;
            //movement before leaving the spawn point
            }else  if(!(mnst[k].posx>50&&mnst[k].posx<924)&&(((int)timer*10)%10==0 || mnst[k].type==1)){
                if(mnst[k].posx<=50)
                    mnst[k].posx=mnst[k].posx+0.1/delta;
                else
                    mnst[k].posx=mnst[k].posx-0.1/delta;
            }else if(!(mnst[k].posy>50&&mnst[k].posy<400)&&(((int)timer*10)%10==0 || mnst[k].type==1)){
                if(mnst[k].posy<100)
                    mnst[k].posy=mnst[k].posy+0.1/delta;
                else
                    mnst[k].posy=mnst[k].posy-0.1/delta;
            }
        //avoiding infinity position
        if(mnst[k].posy>2000||mnst[k].posy<-2000||mnst[k].posx>2000||mnst[k].posx<-2000)
            mnst[k].enable=0;
        }
    }
}
//main char is dead?
int died(m* mnst, character* mc){
    int i,k=0;
    for(i=0;i<50;i++){
        if(mnst[i].enable==1){
            //set collision flag
            if(abs(mnst[i].posy-mc->posy)<96&&abs(mnst[i].posx-(mc->posx))<78){
                k=1;
                i=50;
            }
        }
    }
    return k;
}
//reading keyboard input
char read_keyboard_down(ALLEGRO_EVENT event_obj) {
	if (event_obj.type == ALLEGRO_EVENT_KEY_DOWN) {
		switch (event_obj.keyboard.keycode) {
		case ALLEGRO_KEY_A:
			return 'A';
			break;
		case ALLEGRO_KEY_B:
			return 'B';
			break;
		case ALLEGRO_KEY_C:
			return 'C';
			break;
		case ALLEGRO_KEY_D:
			return 'D';
			break;
		case ALLEGRO_KEY_E:
			return 'E';
			break;
		case ALLEGRO_KEY_F:
			return 'F';
			break;
		case ALLEGRO_KEY_G:
			return 'G';
			break;
		case ALLEGRO_KEY_H:
			return 'H';
			break;
		case ALLEGRO_KEY_I:
			return 'I';
			break;
		case ALLEGRO_KEY_J:
			return 'J';
			break;
		case ALLEGRO_KEY_K:
			return 'K';
			break;
		case ALLEGRO_KEY_L:
			return 'L';
			break;
		case ALLEGRO_KEY_M:
			return 'M';
			break;
		case ALLEGRO_KEY_N:
			return 'N';
			break;
		case ALLEGRO_KEY_O:
			return 'O';
			break;
		case ALLEGRO_KEY_P:
			return 'P';
			break;
		case ALLEGRO_KEY_Q:
			return 'Q';
			break;
		case ALLEGRO_KEY_R:
			return 'R';
			break;
		case ALLEGRO_KEY_S:
			return 'S';
			break;
		case ALLEGRO_KEY_T:
			return 'T';
			break;
		case ALLEGRO_KEY_U:
			return 'U';
			break;
		case ALLEGRO_KEY_V:
			return 'V';
			break;
		case ALLEGRO_KEY_W:
			return 'W';
			break;
		case ALLEGRO_KEY_X:
			return 'X';
			break;
		case ALLEGRO_KEY_Y:
			return 'Y';
			break;
		case ALLEGRO_KEY_Z:
			return 'Z';
			break;
		case ALLEGRO_KEY_BACKSPACE:
			return '\0';
			break;
		case ALLEGRO_KEY_ENTER:
			return '\n';
		default:
			break;
		}
	}
}

char read_keyboard_up(ALLEGRO_EVENT event_obj) {
	if (event_obj.type == ALLEGRO_EVENT_KEY_UP) {
		switch (event_obj.keyboard.keycode) {
		case ALLEGRO_KEY_A:
			return 'A';
			break;
		case ALLEGRO_KEY_B:
			return 'B';
			break;
		case ALLEGRO_KEY_C:
			return 'C';
			break;
		case ALLEGRO_KEY_D:
			return 'D';
			break;
		case ALLEGRO_KEY_E:
			return 'E';
			break;
		case ALLEGRO_KEY_F:
			return 'F';
			break;
		case ALLEGRO_KEY_G:
			return 'G';
			break;
		case ALLEGRO_KEY_H:
			return 'H';
			break;
		case ALLEGRO_KEY_I:
			return 'I';
			break;
		case ALLEGRO_KEY_J:
			return 'J';
			break;
		case ALLEGRO_KEY_K:
			return 'K';
			break;
		case ALLEGRO_KEY_L:
			return 'L';
			break;
		case ALLEGRO_KEY_M:
			return 'M';
			break;
		case ALLEGRO_KEY_N:
			return 'N';
			break;
		case ALLEGRO_KEY_O:
			return 'O';
			break;
		case ALLEGRO_KEY_P:
			return 'P';
			break;
		case ALLEGRO_KEY_Q:
			return 'Q';
			break;
		case ALLEGRO_KEY_R:
			return 'R';
			break;
		case ALLEGRO_KEY_S:
			return 'S';
			break;
		case ALLEGRO_KEY_T:
			return 'T';
			break;
		case ALLEGRO_KEY_U:
			return 'U';
			break;
		case ALLEGRO_KEY_V:
			return 'V';
			break;
		case ALLEGRO_KEY_W:
			return 'W';
			break;
		case ALLEGRO_KEY_X:
			return 'X';
			break;
		case ALLEGRO_KEY_Y:
			return 'Y';
			break;
		case ALLEGRO_KEY_Z:
			return 'Z';
			break;
		case ALLEGRO_KEY_BACKSPACE:
			return '\0';
			break;
		case ALLEGRO_KEY_ENTER:
			return '\n';
		default:
			break;
		}
	}
}

// Draw animation
void draw_sprite(int posx, int posy, int* char_frames, int* collumn, int* line, int*sheetx, int* sheety, int size, double delta, ALLEGRO_BITMAP* spritesheet,ALLEGRO_EVENT event_obj) {
	int frames = 100;

	if (event_obj.type == ALLEGRO_EVENT_TIMER) {
		*char_frames = *char_frames+1;

		if (*char_frames >= frames * delta) {
			*char_frames = 0;
			*collumn = *collumn+1;
			if (*collumn >= SHEET_COLLUMNS) {
				*collumn = 0;
				*sheety = *line * size;
			}
			*sheetx = *collumn * size;
		}
	}

    al_draw_bitmap_region(spritesheet, *sheetx, *sheety, size, size, posx, posy, 0);
}

// Draw game main menu
void draw_menu(int* play_button, int* current_scr, int* select, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj, int* close_window) {
	int selected[5] = { 0,0,0,0,0 };
	int i;

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
	}
	// Paints selected option grey
	for (i = 0; i < 5; i++)
		if (i == *select)
			selected[i] = 150;
		else
			selected[i] = 0;

	// Parse mouse click
	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 4)
			* close_window = 1;
		else if (*select == 0) {
			*play_button = 1;
			*current_scr = 3;
			*select = 20;
		}
		else if (*select == 3) {
			*current_scr = 2;
			*select = 20;
		}
		else if (*select == 1) {
			*play_button = 0;
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

void draw_score(int* current_scr, int* score, char* nick, int* select, ALLEGRO_FONT* font, ALLEGRO_EVENT event_obj) {
	int selected[2] = { 0,0 };
	int i;

	al_draw_text(font, al_map_rgb(255, 255, 255), 1024/2-100, 576/2/2, ALLEGRO_ALIGN_LEFT, "Seu Score");
	al_draw_textf(font, al_map_rgb(255, 255, 255), 1024 / 2 * 0.75, 576/2-10, ALLEGRO_ALIGN_LEFT, "%s%10d", nick, *score);

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_AXES) {
		if (event_obj.mouse.y > 400 && event_obj.mouse.y < 400 + 36) {
			if (event_obj.mouse.x > 250 && event_obj.mouse.x < 250 + 100)
				* select = 0;
			else if (event_obj.mouse.x > 1024 / 2 + 100 && event_obj.mouse.x < 1024 / 2 + 100 + 50)
				* select = 1;
			else *select = 20;
		}
		else *select = 20;
	}
	for (i = 0; i < 2; i++)
		if (i == *select)
			selected[i] = 150;
		else
			selected[i] = 0;

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 1) {
			*current_scr = 4;
			*select = 20;
		}
		else if (*select == 0) {
			*current_scr = 0;
			*select = 20;
		}

	al_draw_text(font, al_map_rgb(255 - selected[0], 255 - selected[0], 255 - selected[0]), 1024/2-200, 400, ALLEGRO_ALIGN_LEFT, "Menu");
	al_draw_text(font, al_map_rgb(255 - selected[1], 255 - selected[1], 255 - selected[1]), 1024/2 + 100, 400, ALLEGRO_ALIGN_LEFT, "Scores");
}

// Draw game screen
void draw_game(int* has_sound, int* score, int* wave, double time, m* mnst, b* blts, character* main_char, int* current_scr, int* select, double delta, char* nick, ALLEGRO_FONT* font, ALLEGRO_BITMAP* spritesheet, ALLEGRO_BITMAP* bullet, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_BITMAP* bg, ALLEGRO_SAMPLE* shot_sound, ALLEGRO_EVENT event_obj) {
	int  cont = 0;

	switch (read_keyboard_down(event_obj)) {
	case 'W':
		*select = 1;
		break;
	case 'A':
		*(select + 1) = 1;
		break;
	case 'S':
		*select = 2;
		break;
	case 'D':
		*(select + 1) = 2;
		break;
	default:
		break;
	}

	switch (read_keyboard_up(event_obj)) {
	case 'W':
		if (*select == 1)
			* select = 0;
		break;
	case 'A':
		if (*(select + 1) == 1)
			* (select + 1) = 0;
		break;
	case 'S':
		if (select[0] == 2)
			select[0] = 0;
		break;
	case 'D':
		if (*(select + 1) == 2)
			* (select + 1) = 0;
		break;
	default:
		break;
	}


	if (*select == 1 && (main_char->posy - main_char->vel / delta) > 25)
		main_char->posy = main_char->posy - main_char->vel / delta;
	if (*(select + 1) == 1 && (main_char->posx + main_char->vel / delta) > 25)
		main_char->posx = main_char->posx - main_char->vel / delta;
	if (*select == 2 && (main_char->posy + main_char->vel / delta) < 576 - 125)
		main_char->posy = main_char->posy + main_char->vel / delta;
	if (*(select + 1) == 2 && (main_char->posx + main_char->vel / delta) < 1024 - 100)
		main_char->posx = main_char->posx + main_char->vel / delta;

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		main_char->current_collumn = 0;
		if (event_obj.mouse.x - 32 < main_char->posx) {
			spawn_blts(blts, event_obj.mouse.x, event_obj.mouse.y, main_char->posx, main_char->posy);
			main_char->sheety = 96 * 5;
		}
		else {
			main_char->sheety = 96 * 4;
			spawn_blts(blts, event_obj.mouse.x, event_obj.mouse.y, main_char->posx + 86, main_char->posy);
		}
	}
	else{
        if((main_char->current_collumn)==3){
            if (*select > 0 || *(select + 1) > 0)
                main_char->current_line = 1;
            else
                main_char->current_line = 3;
            main_char->sheety = 96 * main_char->current_line;
        }

	}

    int i=0;
    move_blts(blts, delta);
    destr_blts(blts,mnst,score);
    move_m(mnst, delta,time,main_char->posx, main_char->posy);
    if(blts[i].enable==1){
        if(*has_sound)
            al_play_sample(shot_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

        blts[i].sheety = 0;
        draw_sprite(blts[i].posx,blts[i].posy,&blts[i].cont_frames,&blts[i].current_collumn,&blts[i].current_line,&blts[i].sheetx,&blts[i].sheety,12,delta,bullet,event_obj);
    }
    for(i=0;i<50;i++){
		if (mnst[i].enable == 1)
			cont++;
		if (mnst[i].enable != 0)
			draw_sprite(mnst[i].posx, mnst[i].posy, &main_char->cont_frames, &main_char->current_collumn, &mnst[i].current_line, &mnst[i].sheetx, &mnst[i].sheety, 96, delta, spritesheet, event_obj);
    }

    if (*score+cont*100<((*wave)*100*(*wave)+100)){
        if(cont==0)
            init_m(mnst);
        spawn_m(mnst,delta,time,main_char->posx, main_char->posy);
    }else if(cont==0){
        *wave=*wave+1;
		init_m(mnst);
    }

	draw_sprite(main_char->posx, main_char->posy, &main_char->cont_frames, &main_char->current_collumn, &main_char->current_line, &main_char->sheetx, &main_char->sheety,96,delta, spritesheet, event_obj);
    al_draw_textf(font, al_map_rgb(255 , 255 , 255 ), 1024 / 2 - 75, 30, ALLEGRO_ALIGN_RIGHT, "SCORE %d ",*score+*wave*1000);
    al_draw_textf(font, al_map_rgb(255 , 255 , 255 ), 1024 / 2 + 75, 30, ALLEGRO_ALIGN_LEFT, "WAVE %d ",*wave);
    if (died(mnst,main_char)==1){
        *score=*score+*wave*1000;
        init_m(mnst);
        init_blts(blts);
        init_char(main_char);
        *current_scr=6;
    }
}

// Draw game options screen
void draw_options(int* has_sound, int* current_scr, int* select, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj) {
	int selected[2] = { 0,0 };
	int i;

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_AXES) {
		if (event_obj.mouse.x < 350) {
			if (event_obj.mouse.y > 300 + 50 * 1.8 && event_obj.mouse.y < 300 + 50 * 1.8 + 40)
				* select = 0;
			else if (event_obj.mouse.y > 300 + 50 * 4 && event_obj.mouse.y < 300 + 50 * 4 + 40)
				* select = 1;
			else *select = 20;
		}
		else *select = 20;
	}
	for (i = 0; i < 2; i++)
		if (i == *select)
			selected[i] = 150;
		else
			selected[i] = 0;

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 1) {
			*current_scr = 0;
			*select = 20;
		}
		else if (*select == 0)
			if (*has_sound == 1)
				*has_sound = 0;
			else
				*has_sound = 1;

	al_draw_text(font, al_map_rgb(255 - selected[0], 255 - selected[0], 255 - selected[0]), 20, 300 + 50 * 1.8, ALLEGRO_ALIGN_LEFT, "Mutar");
	al_draw_text(font, al_map_rgb(255 - selected[1], 255 - selected[1], 255 - selected[1]), 20, 300 + 50 * 4, ALLEGRO_ALIGN_LEFT, "Voltar");
}

// Draw briefing screen
void draw_briefing(int* play_button, int* current_scr, int* select, ALLEGRO_DISPLAY* window, ALLEGRO_FONT* font, ALLEGRO_FONT* small_font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj) {
	int selected = 0;

	al_draw_text(small_font, al_map_rgb(255, 255, 255), 30, 30, ALLEGRO_ALIGN_LEFT, "Após se preparar para o último boss");
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
		if (*play_button) {
			if (event_obj.mouse.x > 350) {
				if (event_obj.mouse.y > 300 + 50 * 4 && event_obj.mouse.y < 300 + 50 * 4 + 40)
					* select = 0;
				else *select = 20;
			}
			else *select = 20;
		}
		else {
			if (event_obj.mouse.x < 350) {
				if (event_obj.mouse.y > 300 + 50 * 4 && event_obj.mouse.y < 300 + 50 * 4 + 40)
					* select = 0;
				else *select = 20;
			}
			else *select = 20;
		}
	}
	if (*select == 0)
		selected = 150;
	else
		selected = 0;

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 0) {

			if (*play_button) {
				*select = 0;
				*current_scr = 5;
			}
			else {
				*select = 20;
				*current_scr = 0;
			}
		}

	if (*play_button) {
		al_draw_text(font, al_map_rgb(255 - selected, 255 - selected, 255 - selected), 1024 - 200, 300 + 50 * 4, ALLEGRO_ALIGN_LEFT, "Continuar");
	} else
		al_draw_text(font, al_map_rgb(255 - selected, 255 - selected, 255 - selected), 20, 300 + 50 * 4, ALLEGRO_ALIGN_LEFT, "Voltar");
}

// Draw highs
void draw_hiscore(rgp *score ,char name[], int scores, int* current_scr, int* select, ALLEGRO_FONT* font, ALLEGRO_FONT* small_font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj) {
	int selected = 0;
	int i;

	for (i = 0; i < 10; i++){
		al_draw_textf(font, al_map_rgb(255 - selected, 255 - selected, 255 - selected), (1024 / 2) - 264, 92 + 33 * i * 1.32, ALLEGRO_ALIGN_LEFT, "%s", score[i].name);
		al_draw_textf(font, al_map_rgb(255 - selected, 255 - selected, 255 - selected), (1024 / 2) + 178, 92 + 33 * i * 1.32, ALLEGRO_ALIGN_LEFT, "%10d", score[i].pontos);
	}


	if (event_obj.type == ALLEGRO_EVENT_MOUSE_AXES) {
		if (event_obj.mouse.x < 350) {
			if (event_obj.mouse.y > 300 + 50 * 4.7 && event_obj.mouse.y < 300 + 50 * 4.7 + 40)
				* select = 0;
			else *select = 20;
		}
		else *select = 20;
	}
	if (*select == 0)
		selected = 150;
	else
		selected = 0;

	if (event_obj.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		if (*select == 0) {
			*current_scr = 0;
			*select = 20;
		}

	al_draw_text(font, al_map_rgb(255 - selected, 255 - selected, 255 - selected), 20, 300 + 50 * 4.7, ALLEGRO_ALIGN_LEFT, "Voltar");
}

// Draw name asking
void draw_name(int* current_scr, int* select, char* nick, ALLEGRO_FONT* font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj) {
	al_draw_text(font, al_map_rgb(0, 0, 0), 512-200, 300 - 160, ALLEGRO_ALIGN_LEFT, "Digite um nick (3 letras)");
	al_draw_textf(font, al_map_rgb(0, 0, 0), 512 - 30, 576/2-16, ALLEGRO_ALIGN_LEFT, "%s", nick);

	if (event_obj.type == ALLEGRO_EVENT_KEY_DOWN) {
		int plus = 0;
		char buffer = read_keyboard_down(event_obj);
		if (*select < 3&& buffer != '\n') {
			nick[*select] = buffer;
			plus = 1;
		}
		if (buffer == '\0'){
            *select = *select - 1;
			nick[*select] = '\0';
			plus = 0;
            if(*select<0)
                *select=0;
		}
		else if (buffer == '\n'&& *select==3) {
			*select = 0;
			*(select+1) = 0;
			nick[3] = '\0';
			*current_scr = 1;
		}

		if (plus && *select<3)
			*select = *select + 1;
	}

	if(*select==3)
		al_draw_text(font, al_map_rgb(0,0,0), 400 - 120, 400, ALLEGRO_ALIGN_LEFT, "Pressione ENTER para continuar");
}

// Draw current screen (game, menu, etc)
void draw_scr(int* play_button, rgp* arquivo ,int* has_sound, int* wave,double time,m* mnst,b * blts,int*  score, character* main_char, int* current_scr, int* select, char* nick, double delta, ALLEGRO_BITMAP* spritesheet,ALLEGRO_BITMAP* bullet, ALLEGRO_DISPLAY* window, ALLEGRO_FONT* font, ALLEGRO_FONT* small_font, int has_event, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT event_obj, ALLEGRO_BITMAP* bg, ALLEGRO_SAMPLE* shot_sound, int* close_window) {
	/*
	 *  0 - Menu
	 *  1 - Jogo
	 *  2 - Opções
	 *  3 - Briefing
	 *  4 - High Scores
	 *  5 - Insert Name
	 *  6 - Score
	 */

	switch (*current_scr) {
	case 0:
		draw_menu(play_button, current_scr, select, font, has_event, event_queue, event_obj, close_window);
		break;
	case 1:
		draw_game(has_sound, score,wave,time,mnst,blts,main_char, current_scr, select, delta, nick, font, spritesheet, bullet ,has_event, event_queue, bg, shot_sound, event_obj);
		break;
	case 2:
		draw_options(has_sound, current_scr, select, font, has_event, event_queue, event_obj);
		break;
	case 3:
		draw_briefing(play_button, current_scr, select, window, font, small_font, has_event, event_queue, event_obj);
		break;
	case 4:
		draw_hiscore(arquivo,nick, *score, current_scr, select, font, small_font, has_event, event_queue, event_obj);
		break;
	case 5:
		draw_name(current_scr, select, nick, font, has_event, event_queue, event_obj);
		break;
	case 6:
		draw_score(current_scr, score, nick, select, font, event_obj);
		break;
	default:
		break;
	}
}

// Initialize some allegro objects
void initialize_allegro() {
	/* Initialize allegro */
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		exit(1);
	}
	if (!al_init_image_addon()) {
		fprintf(stderr, "Failed to initialize allegro's image addon.\n");
		exit(1);
	}
	al_init_font_addon();

	if (!al_init_ttf_addon()) {
		fprintf(stderr, "Failed to initialize allegro's ttf addon.\n");
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
	int current_scr = 0, old_scr = current_scr, close_window = 0, select[2] = { 20,20 };
	char nick[5];
	int i,wave=0;
	int score = 0;
	int has_sound = 1;
	int play_button = 0;
	//create and initialize of pointers  and nickname
	rgp* scores = add_score("NON", 0);
    b *blts;
    blts=(b*) malloc(3*sizeof(b));
    init_blts(blts);
    m *mnst;
    mnst=(m*) malloc(50*sizeof(m));
    init_m(mnst);
	for (i = 0; i < 5; i++)
		nick[i] = '\0';

	/* Hard coded initialization */
	initialize_allegro();

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
	ALLEGRO_BITMAP* cursor_bmp = create_bitmap("img/cursor.png");
	ALLEGRO_MOUSE_CURSOR* cursor = al_create_mouse_cursor(cursor_bmp, 0, 0);
	if (!al_set_mouse_cursor(window, cursor)) {
		al_destroy_display(window);
		fprintf(stderr, "Failed to initialize mouse cursor.\n");
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
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	int has_event = al_wait_for_event_until(event_queue, &event_obj, &timeout);
	al_start_timer(timer);

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

	// Initialize bgm
	ALLEGRO_AUDIO_STREAM* bgm = NULL;
	bgm = create_audio_stream("bgm/menu.ogg");
	al_attach_audio_stream_to_mixer(bgm, al_get_default_mixer());
	al_set_audio_stream_playmode(bgm, ALLEGRO_PLAYMODE_LOOP);
	ALLEGRO_SAMPLE* shot_sound = NULL;
	shot_sound = al_load_sample("sound/shot.ogg");

	// Initialize images
	ALLEGRO_BITMAP* bg = create_bitmap("bg/main_menu.png");
	ALLEGRO_BITMAP* gamso = create_bitmap("img/gamso.png");
	ALLEGRO_BITMAP* spritesheet = create_bitmap("char/spritesheet.png");
    ALLEGRO_BITMAP* bullet = create_bitmap("char/bullet.png");

	double old_time = al_get_time();

	// Create character
	character main_char;
	init_char(&main_char);

	/* Game loop */
	while (!close_window) {
		// Gets frame inputs
		has_event = al_wait_for_event_until(event_queue, &event_obj, &timeout);

		// Close window if close button is pressed
		if (event_obj.type == ALLEGRO_EVENT_DISPLAY_CLOSE && has_event)
			close_window = 1;

		double new_time = al_get_time();
		double delta = new_time - old_time;
		double fps = 1 / (delta * 1000);
		old_time = new_time;

		// Load when screen is changed once
		if (current_scr != old_scr) {
			if (current_scr == 1) {
				al_destroy_audio_stream(bgm);

				bgm = create_audio_stream("bgm/gameplay.ogg");
				al_attach_audio_stream_to_mixer(bgm, al_get_default_mixer());
				al_set_audio_stream_playmode(bgm, ALLEGRO_PLAYMODE_LOOP);

				bg = create_bitmap("bg/arena.png");
			} else if (current_scr == 6) {
				scores = add_score(nick, score);

				al_destroy_audio_stream(bgm);

				bgm = create_audio_stream("bgm/menu.ogg");
				al_attach_audio_stream_to_mixer(bgm, al_get_default_mixer());
				al_set_audio_stream_playmode(bgm, ALLEGRO_PLAYMODE_LOOP);

				bg = create_bitmap("bg/room.png");
			} else if (current_scr == 3) {
				bg = create_bitmap("bg/room.png");
			} else if (current_scr == 0) {
				*nick = '\0';
				*(nick + 1) = '\0';
				*(nick + 2) = '\0';
				score = 0;
				wave = 0;

				bg = create_bitmap("bg/main_menu.png");
			} else if (current_scr == 5) {
				bg = create_bitmap("bg/name.png");
			}
			else if (current_scr == 2) {
				bg = create_bitmap("bg/option.png");
			}
			else if (current_scr == 4) {
				*nick = '\0';
				*(nick + 1) = '\0';
				*(nick + 2) = '\0';
				score = 0;
				wave = 0;

                bg = create_bitmap("bg/hiscores.png");
            }
			select[1] = 0;
            old_scr = current_scr;
        }

		// Update display
		al_clear_to_color(al_map_rgb(0, 0, 0));
		if (new_time < 5) {
			al_draw_bitmap(gamso, 1024-576-576/2+20, 0, 0);
			al_draw_text(font, al_map_rgb(255, 255, 255), 280, 10, ALLEGRO_ALIGN_LEFT, "Uma produção Lunare Team e");
		} else {
			al_draw_bitmap(bg, 0, 0, 0);
			draw_scr(&play_button, scores, &has_sound, &wave, new_time, mnst, blts, &score, &main_char, &current_scr, select, nick, fps, spritesheet, bullet, window, font, small_font, has_event, event_queue, event_obj, bg, shot_sound, &close_window);
		}
		if (!has_sound) {
			al_set_audio_stream_playing(bgm, 0);
		}
		else{
			al_set_audio_stream_playing(bgm, 1);
		}
		al_flip_display();
	}

	// Close program
	al_destroy_bitmap(cursor_bmp);
	al_destroy_bitmap(spritesheet);
	al_destroy_bitmap(bullet);
	al_destroy_bitmap(bg);
	al_destroy_bitmap(gamso);
	al_destroy_audio_stream(bgm);
	al_destroy_sample(shot_sound);
	al_destroy_timer(timer);
	al_destroy_display(window);
	al_destroy_event_queue(event_queue);
	free(blts);
	free(mnst);
	free(scores);
	return 0;
}
