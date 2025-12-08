#include <stdio.h>
#include <libdragon.h>
#include <unistd.h>

enum Stage {
    START_SCREEN,
    CHOOSE_SPRITE,
    DRESS_UP,
    SHOW_FINAL,
};

enum Name {
	UNSET,
    ZOE,
    REY,
    LIZZIE,
};

struct Character {
	enum Name name;
	int top;
	int bottom;
	int shoe;
	int extra;
};

struct Character character = {UNSET, -1, -1, -1, -1}; 
enum Stage current_stage = START_SCREEN;
uint32_t last_switch = 0;

void update_start_screen(joypad_inputs_t in);
void draw_start_screen(display_context_t disp, sprite_t **background);

void update_choose_sprite(joypad_inputs_t in);
void draw_choose_sprite(display_context_t disp, sprite_t **background, joypad_inputs_t input);

void update_dress_up(joypad_inputs_t in);
void draw_dress_up(display_context_t disp, sprite_t **background, sprite_t **clothes, sprite_t **preview, joypad_inputs_t input);

void update_show_final(joypad_inputs_t in);
void draw_show_final(display_context_t disp, sprite_t **background, sprite_t **clothes);

int main(void){
	//display, console, joypad etc. setup
    console_init();
    debug_init_usblog();
    console_set_debug(true);
    joypad_init();
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
 
	//loading in the files
	const char *background_files[] = { // background files
        "/startScreen_1_2.sprite", 
        "/chars_1_1.sprite",
        "/charArrows_2_2.sprite",
		"/chooseOutift_1_1.sprite",
		"/charBases_2_2.sprite",
		"/outfitArrows_3_2.sprite",
		"/categorySelect_2_2.sprite",
		"/end_screen_1_2.sprite"
	};

	const char *zoe_char_files[] = { // zoe char files
		"/zoeTops_3_2.sprite",
		"/zoePants_3_2.sprite",
		"/zoeShoes_2_2.sprite",
		"/zoeExtra_2_2.sprite"
	};

	const char *zoe_preview_files[] = {  // zoe preview files
		"/zoeTopsPrev_2_3.sprite",
		"/zoePantsPrev_2_3.sprite",
		"/zoeShoesPrev_2_2.sprite",
		"/zoeExtraPrev_2_2.sprite"
    };

	const char *rey_char_files[] = { // rey char files
		"/reyTops_3_2.sprite",
		"/reyBottoms_3_2.sprite", 
		"/reyShoes_2_1.sprite", 
		"/reyExtra_2_2.sprite" 
	};

	const char *rey_preview_files[] = {  // rey preview files
		"/reyTopsPrev_2_3.sprite", 
		"/reyBottomsPrev_2_3.sprite", 
		"/reyShoesPrev_1_2.sprite", 
		"/reyExtraPrev_2_2.sprite" 
    };

	const char *lizzie_char_files[] = { // lizzie char files 
		"/lizzieTops_3_2.sprite",
		"/lizzieBottoms_3_2.sprite",
		"/lizzieShoes_2_2.sprite",
		"/lizzieExtra_2_2.sprite"
	};

	const char *lizzie_preview_files[] = { // lizzie preview files
		"/lizzieTopsPrev_2_3.sprite",
		"/lizzieBottomsPrev_2_3.sprite", 
		"/lizzieShoesPrev_2_2.sprite",
		"/lizzieExtraPrev_2_2.sprite"
	};

	//I make the assumption here that there will be the same number of clothes as
	// preview files for the clothes, which should be the case
    const int NUM_BACK = sizeof(background_files) / sizeof(background_files[0]);
    const int NUM_ZOE= sizeof(zoe_char_files) / sizeof(zoe_char_files[0]);
    const int NUM_REY = sizeof(rey_char_files) / sizeof(rey_char_files[0]);
	const int NUM_LIZZIE = sizeof(lizzie_char_files) / sizeof(lizzie_char_files[0]);

    sprite_t *background[NUM_BACK];

	sprite_t *zoe_char[NUM_ZOE];
	sprite_t *zoe_preview[NUM_ZOE];

	sprite_t *rey_char[NUM_REY];
	sprite_t *rey_preview[NUM_REY];

    sprite_t *lizzie_char[NUM_LIZZIE];
    sprite_t *lizzie_preview[NUM_LIZZIE];

	//populate the arrays
	//background:
    for (int i = 0; i < NUM_BACK; i++) {
        int fp = dfs_open(background_files[i]);
        if (fp < 0) {
            printf("Failed to open %s\n", background_files[i]);
            continue;
        }

        background[i] = malloc(dfs_size(fp));
        dfs_read(background[i], 1, dfs_size(fp), fp);
        dfs_close(fp);
    }
	//zoe:
	for (int i = 0; i < NUM_ZOE; i++) {
        int fp1 = dfs_open(zoe_char_files[i]);
        if (fp1 < 0) {
            printf("Failed to open %s\n", zoe_char_files[i]);
            continue;
        }
		int fp2 = dfs_open(zoe_preview_files[i]);
        if (fp2 < 0) {
            printf("Failed to open %s\n", zoe_preview_files[i]);
            continue;
        }
        zoe_char[i] = malloc(dfs_size(fp1));
        zoe_preview[i] = malloc(dfs_size(fp2));

        dfs_read(zoe_char[i], 1, dfs_size(fp1), fp1);
		dfs_read(zoe_preview[i], 1, dfs_size(fp2), fp2);
		
        dfs_close(fp1);
		dfs_close(fp2);
    }
	//rey:
	for (int i = 0; i < NUM_REY; i++) {
        int fp1 = dfs_open(rey_char_files[i]);
        if (fp1 < 0) {
            printf("Failed to open %s\n", rey_char_files[i]);
            continue;
        }
		int fp2 = dfs_open(rey_preview_files[i]);
        if (fp2 < 0) {
            printf("Failed to open %s\n", rey_preview_files[i]);
            continue;
        }
        rey_char[i] = malloc(dfs_size(fp1));
        rey_preview[i] = malloc(dfs_size(fp2));

        dfs_read(rey_char[i], 1, dfs_size(fp1), fp1);
		dfs_read(rey_preview[i], 1, dfs_size(fp2), fp2);
		
        dfs_close(fp1);
		dfs_close(fp2);
    }
	//lizzie:
	for (int i = 0; i < NUM_LIZZIE; i++) {
        int fp1 = dfs_open(lizzie_char_files[i]);
        if (fp1 < 0) {
            printf("Failed to open %s\n", lizzie_char_files[i]);
            continue;
        }
		int fp2 = dfs_open(lizzie_preview_files[i]);
        if (fp2 < 0) {
            printf("Failed to open %s\n", lizzie_preview_files[i]);
            continue;
        }
        lizzie_char[i] = malloc(dfs_size(fp1));
        lizzie_preview[i] = malloc(dfs_size(fp2));

        dfs_read(lizzie_char[i], 1, dfs_size(fp1), fp1);
		dfs_read(lizzie_preview[i], 1, dfs_size(fp2), fp2);
		
        dfs_close(fp1);
		dfs_close(fp2);
	}

	// display context and state switching logic
    display_context_t disp;
    while (1){
		while (!(disp = display_lock()));

		joypad_poll();
        joypad_inputs_t input = joypad_get_inputs(0);

		sprite_t **clothes;
		sprite_t **preview;

		if (character.name == REY){
			clothes = rey_char;
			preview = rey_preview;
		} else if (character.name == ZOE) {
			clothes = zoe_char;
			preview = zoe_preview;
		} else {
			clothes = lizzie_char;
			preview = lizzie_preview;
		} 
		graphics_fill_screen(disp, graphics_make_color(199, 171, 215, 255));


      	switch (current_stage) {
            case START_SCREEN:   update_start_screen(input);   break;
            case CHOOSE_SPRITE:  update_choose_sprite(input);  break;
            case DRESS_UP:       update_dress_up(input);       break;
            case SHOW_FINAL:     update_show_final(input);     break;
        }

        switch (current_stage) {
            case START_SCREEN:   draw_start_screen(disp, background); break;
            case CHOOSE_SPRITE:  draw_choose_sprite(disp, background, input); break;
            case DRESS_UP:       draw_dress_up(disp, background, clothes, preview, input); break;
            case SHOW_FINAL:     draw_show_final(disp, background, clothes); break;
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

void draw_start_screen(display_context_t disp, sprite_t **background){
	uint32_t now = get_ticks();
	if (now - last_switch > TICKS_FROM_MS(800)) {
		start_screen_frame = (start_screen_frame == 0) ? 1 : 0;
		last_switch = now;
	}

     graphics_draw_sprite_trans_stride(
         disp,
        0, 0, background[0], start_screen_frame);
}

//=============== CHOOSE SPRITE CODE ==========================
enum Name hover = ZOE;

void update_choose_sprite(joypad_inputs_t input){
	if (input.btn.a) {
		character.name = hover;
		current_stage = DRESS_UP;
	}
}

void draw_choose_sprite(display_context_t disp, sprite_t **background, joypad_inputs_t input){
	graphics_draw_sprite_trans_stride(disp, 0, 0, background[1], 0);
	if(hover == ZOE){
		graphics_draw_sprite_trans_stride(disp, 0, 0, background[2], 0);
		if (input.btn.d_right) {
			hover = REY;
		}
	} else if (hover == REY){
		graphics_draw_sprite_trans_stride(disp, 0, 0, background[2], 1);
		if (input.btn.d_right) {
			hover = LIZZIE;
		}else if (input.btn.d_left) {
			hover = ZOE;
		}
	} else {
		graphics_draw_sprite_trans_stride(disp, 0, 0, background[2], 2);
		if (input.btn.d_left) {
			hover = REY;
		}
	}
}

//=============== DRESS UP CODE ==========================
int arrow = 0;
int cat_select = -1;
bool a_release = false;

void update_dress_up(joypad_inputs_t input){

}

void draw_dress_up(display_context_t disp, sprite_t **background, sprite_t **clothes, sprite_t **preview, joypad_inputs_t input){
	if (input.btn.a == 0) {
            a_release = true;
    } 
		if (preview[0]){
			printf("yay");
		}
	graphics_draw_sprite_trans_stride(disp, 0, 0, background[3], 0);
	if (cat_select >= 0) {
		graphics_draw_sprite_trans_stride(disp, 172, 0, background[6], cat_select);
		graphics_draw_sprite_trans_stride(disp, 172, 98, preview[0], 0);
	}
	if (input.btn.d_down && arrow < 5) {
			arrow ++;
	} else if (input.btn.d_up && arrow > 0) {
			arrow --;
	}
	if (arrow < 4 && input.btn.a && a_release) {
		cat_select = arrow;
	}
	graphics_draw_sprite_trans_stride(disp, 172, 0, background[5], arrow);
	if (character.name == ZOE){
		graphics_draw_sprite_trans_stride(disp, 0, 0, background[4], 0);
	} else if (character.name == REY){
		graphics_draw_sprite_trans_stride(disp, 0, 0, background[4], 1);
	} else {
		graphics_draw_sprite_trans_stride(disp, 0, 0, background[4], 2);
	}
}

//=============== SHOW FINAL CODE ==========================
void update_show_final(joypad_inputs_t input){

}
void draw_show_final(display_context_t disp, sprite_t **background, sprite_t **clothes){

}
