#include <stdio.h>
#include <libdragon.h>
#include <unistd.h>

enum Stage {
    START_SCREEN,
    CHOOSE_SPRITE,
    DRESS_UP,
    SHOW_FINAL,
};

enum Stage current_stage = START_SCREEN;
uint32_t last_switch = 0;
int current = 0;

void update_start_screen(joypad_inputs_t in);
void draw_start_screen(display_context_t disp, sprite_t **sprites);

void update_choose_sprite(joypad_inputs_t in);
void draw_choose_sprite(display_context_t disp, sprite_t **sprites);

void update_dress_up(joypad_inputs_t in);
void draw_dress_up(display_context_t disp);

void update_show_final(joypad_inputs_t in);
void draw_show_final(display_context_t disp);

int main(void){
    console_init();
    debug_init_usblog();
    console_set_debug(true);
    joypad_init();
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
 
	const char *files[] = {
        "/startScreen1.sprite",
        "/startScreen2.sprite",
        "/background.sprite"
    };

    const int NUM_SPRITES = sizeof(files) / sizeof(files[0]);

    sprite_t *sprites[NUM_SPRITES];

    for (int i = 0; i < NUM_SPRITES; i++) {
        int fp = dfs_open(files[i]);
        if (fp < 0) {
            printf("Failed to open %s\n", files[i]);
            continue;
        }

        sprites[i] = malloc(dfs_size(fp));
        dfs_read(sprites[i], 1, dfs_size(fp), fp);
        dfs_close(fp);
    }

    display_context_t disp;

    while (1){
		while (!(disp = display_lock()));

		joypad_poll();
        joypad_inputs_t input = joypad_get_inputs(0);

      	switch (current_stage) {
            case START_SCREEN:   update_start_screen(input);   break;
            case CHOOSE_SPRITE:  update_choose_sprite(input);  break;
            case DRESS_UP:       update_dress_up(input);       break;
            case SHOW_FINAL:     update_show_final(input);     break;
        }

        switch (current_stage) {
            case START_SCREEN:   draw_start_screen(disp, sprites); break;
            case CHOOSE_SPRITE:  draw_choose_sprite(disp, sprites); break;
            case DRESS_UP:       draw_dress_up(disp);              break;
            case SHOW_FINAL:     draw_show_final(disp);            break;
        }

        display_show(disp);
    }
}

//=============== START SCREEN CODE ==========================
void update_start_screen(joypad_inputs_t input){
	if (input.btn.start) {
		current_stage = CHOOSE_SPRITE;
	}
}
void draw_start_screen(display_context_t disp, sprite_t **sprites){
	uint32_t now = get_ticks();
	if (now - last_switch > TICKS_FROM_MS(1000)) {
		current = (current == 0) ? 1 : 0;
		last_switch = now;
	}
	int cx = (display_get_width() - sprites[current]->width) / 2;
	int cy = (display_get_height() - sprites[current]->height) / 2;
	graphics_draw_sprite_trans(disp, cx, cy, sprites[current]);
}


//=============== CHOSE SPRITE CODE ==========================
void update_choose_sprite(joypad_inputs_t input){

}
void draw_choose_sprite(display_context_t disp, sprite_t **sprites){
	debugf((char *)0x1);
}

//=============== DRESS UP CODE ==========================
void update_dress_up(joypad_inputs_t input){

}
void draw_dress_up(display_context_t disp){

}

//=============== SHOW FINAL CODE ==========================
void update_show_final(joypad_inputs_t input){

}
void draw_show_final(display_context_t disp){

}
