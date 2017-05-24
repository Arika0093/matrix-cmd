#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX(a,b)				(a>b?a:b)
#define MIN(a,b)				(a<b?a:b)
#define add_sprintf(a, b, ...)	sprintf(a, "%s"b, a, __VA_ARGS__)

typedef struct {
	int width;
	int height;
} conssize;

char OUTPUT[114514] = "";

void printchar(char ch, int color){
	add_sprintf(OUTPUT, "\033[%dm%c\033[0m", color, ch);
	return;
}

void get_conssize(conssize *cs){
	struct winsize ws;
    // get terminal size
    if( ioctl( STDOUT_FILENO, TIOCGWINSZ, &ws ) != -1 ) {
		cs->width = ws.ws_col;
		cs->height = ws.ws_row;
	}
	return;
}

int get_rand(int min, int max){
	return rand()%(max-min+1)+min;
}

int main(int argc, char *argv[]){
	conssize cs;
	char showtext[810] = "MATRIX";
	int green = 32, black = 30;
	int i = 0, j = 0, count = 0;
	// set rand-initialize
	srand((unsigned)time(NULL));
	// get console-size
	get_conssize(&cs);
	// param copy
	if(argc >= 2){
		strcpy(showtext, argv[1]);
	}
	// loop
	while(1){
		// reset
		strcpy(OUTPUT, "\033[1;1H");
		// print background
		for(j=0; j < cs.height; j++){
			for(i=0; i < cs.width; i++){
				int showval = get_rand(0, 32);
				if(showval >= 26){
					printchar('X', black);
				} else {
					printchar('A'+showval, green);
				}
			}
		}
		// print foreground-back
		if(count > 40){
			int half_h = cs.height/2;
			int size_h = count/40;
			int max_h = 4;
			int loop_sth = MAX(half_h - MIN(size_h, max_h), 0);
			int loop_edh = MIN(half_h + MIN(size_h, max_h) - 1, cs.height);
			int text_lg = strlen(showtext);
			for(j = loop_sth; j < loop_edh; j++){
				add_sprintf(OUTPUT, "\033[%d;%dH", j+1, 1);
				for(i=0; i < cs.width; i++){
					printchar('X', black);
				}
				if(size_h > max_h && j == half_h){
					int half_w = cs.width/2;
					int w_st = MAX(cs.width - text_lg*2 - 1, 0);
					int w_ed = MIN(cs.width + text_lg*2, cs.width);
					int strcount=0;
					add_sprintf(OUTPUT, "\033[%d;%dH", j, MAX(half_w - text_lg, 0));
					for(i=w_st; i<w_ed; i++){
						if(i%2 == 1){
							printchar(showtext[strcount], green);
							strcount++;
						} else {
							printchar(' ', black);
						}
					}
				}
			}
		}
		add_sprintf(OUTPUT, "\033[%d;%dH", cs.height, cs.width);
		// show and wait
		fprintf(stderr, OUTPUT);
		sleep(0.1);
		count++;
	}
	return 0;
}
