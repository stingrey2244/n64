#include <stdio.h>
#include <stdlib.h>
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
	sprite_t **clothes;
	sprite_t **preview;
	int *wardrobe_count;
	int *curr_outfit;
};

int curr_outfit[4] = {-1, -1, -1, -1};
struct Character character = {UNSET, NULL, NULL, NULL, curr_outfit}; 

enum Stage current_stage = START_SCREEN;
enum Stage previous_stage = START_SCREEN;

//loading in the files
const char *start_files[] = {
	"/startScreen_1_2.sprite"
};

const char *choose_files[] = {
	"/chars_1_1.sprite",
	"/charArrows_2_2.sprite"
};

const char *outfit_files[] = {
	"/chooseOutift_1_1.sprite",
	"/charBases_2_2.sprite",
	"/outfitArrows_3_2.sprite",
	"/categorySelect_2_2.sprite"
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

const char *end_files[] = { 
	"/end_screen_1_2.sprite"
};

//I make the assumption here that there will be the same number of clothes as
#define NUM_START  (sizeof(start_files) / sizeof(start_files[0]))
#define NUM_CHOOSE (sizeof(choose_files) / sizeof(choose_files[0]))
#define NUM_OUTFIT (sizeof(outfit_files) / sizeof(outfit_files[0]))
#define NUM_ZOE    (sizeof(zoe_char_files) / sizeof(zoe_char_files[0]))
#define NUM_REY    (sizeof(rey_char_files) / sizeof(rey_char_files[0]))
#define NUM_LIZZIE (sizeof(lizzie_char_files) / sizeof(lizzie_char_files[0]))
#define NUM_END    (sizeof(end_files) / sizeof(end_files[0]))

//note: I know it's awful to hardcode these but I couldn't figure out how else to do it
// number of items in each wardrobe [tops, pants, shoes, extra]
int zoe_wardrobe_count[4] = {5, 5, 3, 3};
int rey_wardrobe_count[4] = {5, 5, 2, 3};
int lizzie_wardrobe_count[4] = {5, 5, 3, 4};

sprite_t *start[NUM_START];
sprite_t *choose[NUM_CHOOSE];
sprite_t *outfit[NUM_OUTFIT];

sprite_t *zoe_char[NUM_ZOE];
sprite_t *zoe_preview[NUM_ZOE];

sprite_t *rey_char[NUM_REY];
sprite_t *rey_preview[NUM_REY];

sprite_t *lizzie_char[NUM_LIZZIE];
sprite_t *lizzie_preview[NUM_LIZZIE];

sprite_t *end[NUM_END];

sprite_t* load_sprite(const char *path) {
    int fp = dfs_open(path);
    if (fp < 0) {
        printf("FAILED TO OPEN SPRITE: %s\n", path);
        return NULL;
    }

    int size = dfs_size(fp);
    sprite_t *spr = malloc(size);
    if (!spr) {
		console_init();
        printf("FAILED TO ALLOCATE MEMORY FOR: %s\n", path);
		console_render();
        dfs_close(fp);
        return NULL;
    }

    dfs_read(spr, 1, size, fp);
    dfs_close(fp);

    return spr;
}

void load_sprite_array(sprite_t **end, const char **files, int size) {
    for (int i = 0; i < size; i++)
        end[i] = load_sprite(files[i]);
}

void free_sprite_array(sprite_t **sprites, int size) {
    for (int i = 0; i < size; i++) {
        if (sprites[i]) {
            free(sprites[i]);
            sprites[i] = NULL;
        }
    }
}

void load_start_assets() {
    load_sprite_array(start, start_files, NUM_START);
}

void free_start_assets() {
    free_sprite_array(start, NUM_START);
}

void load_choose_assets() {
    load_sprite_array(choose, choose_files, NUM_CHOOSE);
}

void free_choose_assets() {
    free_sprite_array(choose, NUM_CHOOSE);
}

void load_outfit_assets() {
    load_sprite_array(outfit, outfit_files, NUM_OUTFIT);
    if (character.name == ZOE) {
        load_sprite_array(zoe_char, zoe_char_files, NUM_ZOE);
        load_sprite_array(zoe_preview, zoe_preview_files, NUM_ZOE);
    } else if (character.name == REY) {
        load_sprite_array(rey_char, rey_char_files, NUM_REY);
        load_sprite_array(rey_preview, rey_preview_files, NUM_REY);
    } else if (character.name == LIZZIE) {
        load_sprite_array(lizzie_char, lizzie_char_files, NUM_LIZZIE);
        load_sprite_array(lizzie_preview, lizzie_preview_files, NUM_LIZZIE);
    }
}

void free_outfit_assets() {
	//free_sprite_array(outfit, NUM_OUTFIT);
     if (character.name == ZOE) {
        //free_sprite_array(zoe_char, NUM_ZOE);
        free_sprite_array(zoe_preview, NUM_ZOE);
    } else if (character.name == REY) {
        //free_sprite_array(rey_char, NUM_REY);
        free_sprite_array(rey_preview, NUM_REY);
    } else if (character.name == LIZZIE) {
        //free_sprite_array(lizzie_char, NUM_LIZZIE);
        free_sprite_array(lizzie_preview, NUM_LIZZIE);
    }
}

void load_end_assets() {
    load_sprite_array(end, end_files, NUM_END);
}

void free_end_assets() {
    free_sprite_array(end, NUM_END);
	free_sprite_array(outfit, NUM_OUTFIT);
	if (character.name == ZOE) {
		free_sprite_array(zoe_char, NUM_ZOE);
	} else if (character.name == REY) {
		free_sprite_array(rey_char, NUM_REY);
	} else if (character.name == LIZZIE) {
		free_sprite_array(lizzie_char, NUM_LIZZIE);
    }
}

void dress_up(display_context_t disp, enum Stage curr_stage){
	int x = 0;
	if (curr_stage == SHOW_FINAL){
		x = 71;
	}
	bool tights = character.name == REY && character.curr_outfit[1] == 2 && character.curr_outfit[2] != -1;
	for (int i = 2; i >= 0; i--){
		if (character.curr_outfit[i] != -1) {
			graphics_draw_sprite_trans_stride(disp, x, 0, character.clothes[i], character.curr_outfit[i]);
		}
	}
	if (character.curr_outfit[3] != -1) {
		graphics_draw_sprite_trans_stride(disp, x, 0, character.clothes[3], character.curr_outfit[3]);
	} 
	if(tights){
		graphics_draw_sprite_trans_stride(disp, x, 0, character.clothes[2], character.curr_outfit[2]);
	}
}
void update_start_screen(joypad_inputs_t input);
void draw_start_screen(display_context_t disp);

void update_choose_sprite(joypad_inputs_t input);
void draw_choose_sprite(display_context_t disp, joypad_inputs_t input);

void update_dress_up(joypad_inputs_t input);
void draw_dress_up(display_context_t disp, joypad_inputs_t input);

void update_show_final(joypad_inputs_t input);
void draw_show_final(display_context_t disp);

int main(void){
	//display, console, joypad etc. setup
    console_init();
    debug_init_usblog();
    console_set_debug(true);
    joypad_init();
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
	
	load_start_assets();
    display_context_t disp;
    while (1){
		if (current_stage != previous_stage){
			switch (previous_stage) {
                case START_SCREEN:  free_start_assets(); break;
                case CHOOSE_SPRITE: free_choose_assets(); break;
                case DRESS_UP:      free_outfit_assets(); break;
                case SHOW_FINAL:    free_end_assets(); break;
            }

            switch (current_stage) {
                case START_SCREEN:  load_start_assets(); break;
                case CHOOSE_SPRITE: load_choose_assets(); break;
                case DRESS_UP:      load_outfit_assets(); break;
                case SHOW_FINAL:    load_end_assets(); break;
            }
			previous_stage = current_stage;
        }
		
		while (!(disp = display_lock()));

		joypad_poll();
        joypad_inputs_t input = joypad_get_inputs(0);
		
		if (character.name == REY){
			character.clothes = rey_char;
			character.preview = rey_preview;
			character.wardrobe_count = rey_wardrobe_count;
		} else if (character.name == ZOE) {
			character.clothes = zoe_char;
			character.preview = zoe_preview;
			character.wardrobe_count = zoe_wardrobe_count;
		} else {
			character.clothes = lizzie_char;
			character.preview = lizzie_preview;
			character.wardrobe_count = lizzie_wardrobe_count;
		} 
		graphics_fill_screen(disp, graphics_make_color(199, 171, 215, 255));
      	switch (current_stage) {
            case START_SCREEN:   update_start_screen(input);   break;
            case CHOOSE_SPRITE:  update_choose_sprite(input);  break;
            case DRESS_UP:       update_dress_up(input);       break;
            case SHOW_FINAL:     update_show_final(input);     break;
        }

        switch (current_stage) {
            case START_SCREEN:   draw_start_screen(disp); break;
            case CHOOSE_SPRITE:  draw_choose_sprite(disp, input); break;
            case DRESS_UP:       draw_dress_up(disp, input); break;
            case SHOW_FINAL:     draw_show_final(disp); break;
        }

        display_show(disp);
    }
}

//========================== START SCREEN CODE ==========================
void update_start_screen(joypad_inputs_t input){
	if (input.btn.start) {
		current_stage = CHOOSE_SPRITE;
	}
}

int start_screen_frame = 0;
uint32_t last_switch = 0;

void draw_start_screen(display_context_t disp){
	uint32_t now = get_ticks();
	if (now - last_switch > TICKS_FROM_MS(800)) {
		start_screen_frame = (start_screen_frame == 0) ? 1 : 0;
		last_switch = now;
	}
	graphics_draw_sprite_trans_stride(
		disp,
		0, 0, start[0], start_screen_frame);
}

//=============== CHOOSE SPRITE CODE ==========================
enum Name hover = ZOE;

void update_choose_sprite(joypad_inputs_t input){
	if (input.btn.a) {
		character.name = hover;
		current_stage = DRESS_UP;
	}
}

void draw_choose_sprite(display_context_t disp, joypad_inputs_t input){
	graphics_draw_sprite_trans_stride(disp, 0, 0, choose[0], 0);
	if(hover == ZOE){
		graphics_draw_sprite_trans_stride(disp, 0, 0, choose[1], 0);
		if (input.btn.d_right) {
			hover = REY;
		}
	} else if (hover == REY){
		graphics_draw_sprite_trans_stride(disp, 0, 0, choose[1], 1);
		if (input.btn.d_right) {
			hover = LIZZIE;
		}else if (input.btn.d_left) {
			hover = ZOE;
		}
	} else {
		graphics_draw_sprite_trans_stride(disp, 0, 0, choose[1], 2);
		if (input.btn.d_left) {
			hover = REY;
		}
	}
}

//=============== DRESS UP CODE ==========================
int arrow = 0;
int cat_select = -1;
bool a_release = false;
int curr_clothes_select = 0; //for the current index in the wardrobe selected 

void update_dress_up(joypad_inputs_t input){
	if (arrow == 5 && input.btn.a){
		current_stage = SHOW_FINAL;
	}
}

void draw_dress_up(display_context_t disp, joypad_inputs_t input){
	if (input.btn.a == 0) {
            a_release = true;
    } 
	graphics_draw_sprite_trans_stride(disp, 0, 0, outfit[0], 0);
	graphics_draw_sprite_trans_stride(disp, 172, 0, outfit[2], arrow);
	if (character.name == ZOE){
		graphics_draw_sprite_trans_stride(disp, 0, 0, outfit[1], 0);
	} else if (character.name == REY){
		graphics_draw_sprite_trans_stride(disp, 0, 0, outfit[1], 1);
	} else {
		graphics_draw_sprite_trans_stride(disp, 0, 0, outfit[1], 2);
	}
	if (cat_select >= 0) {
		graphics_draw_sprite_trans_stride(disp, 172, 0, outfit[3], cat_select);
		graphics_draw_sprite_trans_stride(disp, 172, 103, character.preview[cat_select], curr_clothes_select);
		dress_up(disp, DRESS_UP);
	}
	if (input.btn.d_down && arrow < 5) {
			arrow ++;
	} else if (input.btn.d_up && arrow > 0) {
			arrow --;
	}
	if (arrow < 4 && input.btn.a && a_release) {
		curr_clothes_select = 0;
		cat_select = arrow;
	}
	if (arrow == 4 && cat_select >= 0){ // outfit select code!!!
		if (input.btn.a){
			character.curr_outfit[cat_select] = curr_clothes_select;
		}
		if (input.btn.d_left && curr_clothes_select > 0) {
			curr_clothes_select--;
		} 
		if (input.btn.d_right && curr_clothes_select < (character.wardrobe_count[cat_select] - 1)) {
			curr_clothes_select++;
		}
	}
}

//=============== SHOW FINAL CODE ==========================

int end_screen_frame = 0;
uint32_t end_last_switch = 0;

void update_show_final(joypad_inputs_t input){
	if (input.btn.a) {
		current_stage = START_SCREEN;
		//reset all the globals
		arrow = 0;
		cat_select = -1;
		a_release = false;
		curr_clothes_select = 0;
		hover = ZOE;
		for (int i = 0; i < 4; i++)
			character.curr_outfit[i] = -1;
		character.clothes = NULL;
		character.preview = NULL;
		character.wardrobe_count = NULL;
	}
}
void draw_show_final(display_context_t disp){
	uint32_t now = get_ticks();
	if (now - end_last_switch > TICKS_FROM_MS(800)) {
		end_screen_frame = (end_screen_frame == 0) ? 1 : 0;
		end_last_switch = now;
	}
	graphics_draw_sprite_trans_stride(
		disp,
		0, 0, end[0], end_screen_frame);
	if (character.name == ZOE){
		graphics_draw_sprite_trans_stride(disp, 71, 0, outfit[1], 0);
	} else if (character.name == REY){
		graphics_draw_sprite_trans_stride(disp, 71, 0, outfit[1], 1);
	} else {
		graphics_draw_sprite_trans_stride(disp, 71, 0, outfit[1], 2);
	}
	dress_up(disp, SHOW_FINAL);
}