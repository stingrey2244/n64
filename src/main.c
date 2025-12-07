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
        "/startScreen_1_2.sprite",
        "/chars.sprite"
        // "/char_arrows.sprite",
		// "/char_bases.sprite"
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
	int i = 0;
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

int start_screen_frame = 0;

void draw_start_screen(display_context_t disp, sprite_t **sprites){
	graphics_fill_screen(disp, graphics_make_color (199, 171, 215, 255));

	uint32_t now = get_ticks();
	if (now - last_switch > TICKS_FROM_MS(1000)) {
		start_screen_frame = (start_screen_frame == 0) ? 1 : 0;
		last_switch = now;
	}

     graphics_draw_sprite_trans_stride(
         disp,
        0, 0, sprites[0], start_screen_frame);
}

//=============== CHOSE SPRITE CODE ==========================
void update_choose_sprite(joypad_inputs_t input){
	current_stage = CHOOSE_SPRITE;
}
void draw_choose_sprite(display_context_t disp, sprite_t **sprites){
	// int cx = (display_get_width() - sprites[0]->width) / 2;
	// int cy = (display_get_height() - sprites[0]->height) / 2;
	// graphics_draw_sprite_trans(disp, cx, cy, sprites[2]);
	// graphics_draw_sprite_trans(disp, 172, 98, sprites[3]);
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
