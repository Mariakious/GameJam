#include <SDL2/SDL_image.h>
#include "engine/game.h"
#include "engine/common.h"
#include "engine/sprite.h"
#include "engine/interface.h"
#include "engine/timers.h"
#include "engine/audio.h"
#include "engine/custom_math.h"

#define WIDTH 1280
#define HEIGHT 720
#define PLAYER_SPEED 320
#define FIRE_RATE 15
int STOP_FIRE = 0;
#define MAX_BULL_ON_SCREEN 10 // lower numbers may risk the bullets despawn before they hit anything



struct orn
{
    ng_sprite_t bul[MAX_BULL_ON_SCREEN];
    SDL_Texture *bul_texture[MAX_BULL_ON_SCREEN];

    int j, last_fired, x[MAX_BULL_ON_SCREEN], y[MAX_BULL_ON_SCREEN]; // for counting them
}bullets;


static struct
{
    ng_game_t game;
    ng_interval_t game_tick;

    // A collection of assets used by entities
    // Ideally, they should have been automatically loaded
    // by iterating over the res/ folder and filling in a hastable

    SDL_Texture *player_texture, *explosion_texture /**cross_texture*/; // gia tis eikones
    Mix_Chunk *gem_sfx;
    TTF_Font *main_font;

    // edo pairnei sprites objects?
    ng_sprite_t /*cross*/ player;
    ng_label_t welcome_text;
    ng_animated_sprite_t explosion;
} ctx;

int fire(int x, int y) { // fire gives the bullet one of 4 directions to go and then sets its start position to player x/y
    bullets.last_fired++;
    if (bullets.last_fired > MAX_BULL_ON_SCREEN - 1) bullets.last_fired = 0;
        bullets.x[bullets.last_fired] = x;
        bullets.y[bullets.last_fired] = y;
        bullets.bul[bullets.last_fired].transform.x = ctx.player.transform.x;
        bullets.bul[bullets.last_fired].transform.y = ctx.player.transform.y;
}
// this commented code checks if a bullet is out of bounds, might be useful, might be wrong
// && (bullets.bul[bullets.j].transform.x > 0 || 
//        bullets.bul[bullets.j].transform.x < (WIDTH - 2 * bullets.bul[bullets.j].transform.w)) && (bullets.bul[bullets.j].transform.y > 0 || 
//        bullets.bul[bullets.j].transform.y < (HEIGHT - 2 * bullets.bul[bullets.j].transform.h))
static void create_actors(void)
{
    ng_game_create(&ctx.game, "Hello World!", WIDTH, HEIGHT);
    
    // loads assets
    ctx.main_font = TTF_OpenFont("res/free_mono.ttf", 16);

    ctx.player_texture = IMG_LoadTexture(ctx.game.renderer, "res/gingy.png");

    ctx.explosion_texture = IMG_LoadTexture(ctx.game.renderer, "res/explosion.png");
    //ctx.cross_texture = IMG_LoadTexture(ctx.game.renderer, "res/cross.png");
    for (bullets.j = 0; bullets.j < MAX_BULL_ON_SCREEN; bullets.j++) {
        bullets.bul_texture[bullets.j] = IMG_LoadTexture(ctx.game.renderer, "res/red_orn.png");
    }
    
    
    ctx.gem_sfx = ng_audio_load("res/gem.wav");
    
    // create sprites
        // player
    ng_interval_create(&ctx.game_tick, 50);
    ng_sprite_create(&ctx.player, ctx.player_texture);
    ng_sprite_set_scale(&ctx.player, 2.0f);
    ctx.player.transform.y = HEIGHT - ctx.player.transform.h - 10;
        // explosion
    ng_animated_create(&ctx.explosion, ctx.explosion_texture, 9);
    ng_sprite_set_scale(&ctx.explosion.sprite, 2.0f);
    ctx.explosion.sprite.transform.x = 200.0f;
        // ballsss
    for (bullets.j = 0; bullets.j < MAX_BULL_ON_SCREEN; bullets.j++) {
        ng_sprite_create(&bullets.bul[bullets.j], bullets.bul_texture[bullets.j]);
        ng_sprite_set_scale(&bullets.bul[bullets.j], 2.0f);
        bullets.bul[bullets.j].transform.x = -100000;
        bullets.bul[bullets.j].transform.y = -100000;
    }
        // crosshair
    //ng_sprite_create(&ctx.cross, ctx.cross_texture);
    //ng_sprite_set_scale(&ctx.cross, 2.0f);

   // ng_label_create(&ctx.welcome_text, ctx.main_font, 300);
   // ng_label_set_content(&ctx.welcome_text, ctx.game.renderer,
   //         "I don't really know what to write in here. "
   //         "You'll notice that you can't type Greek. To do that, "
   //         "just replace the internals with TTF_RenderUTF8_Solid");
}


// A place to handle queued events.
static void handle_event(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_KEYDOWN:
        // Press space for a sound effect!
        if (event->key.keysym.sym == SDLK_SPACE)
            ng_audio_play(ctx.gem_sfx);
        break;
    case SDL_MOUSEMOTION:
        //ctx.cross.transform.x = event->motion.x - ctx.cross.transform.w / 2;
        //ctx.cross.transform.y = event->motion.y - ctx.cross.transform.h / 2;
        break;
    }

}

static void update_and_render_scene(float delta)
{


    // Handling "continuous" events, which are now repeatable
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_A] && ctx.player.transform.x > 0) {
        ctx.player.transform.x -= PLAYER_SPEED* delta;
        // change the frame to the proper one here for player movemnt
    }

    if (keys[SDL_SCANCODE_D] && ctx.player.transform.x < (WIDTH - 2 * ctx.player.src.w)) {
        ctx.player.transform.x += PLAYER_SPEED* delta;
        // change the frame to the proper one here for player movemnt
    }

    if (keys[SDL_SCANCODE_W] && ctx.player.transform.y > 0) {
        ctx.player.transform.y -= PLAYER_SPEED* delta;
        // change the frame to the proper one here for player movemnt
    }

    if (keys[SDL_SCANCODE_S] && ctx.player.transform.y < (HEIGHT - 2 * ctx.player.src.h)) {
        ctx.player.transform.y += PLAYER_SPEED* delta;
        // change the frame to the proper one here for player movemnt
    } 

    if (keys[SDL_SCANCODE_RIGHT]) {
        if (STOP_FIRE == 0) fire(1, 0);
        STOP_FIRE++;
        if (STOP_FIRE == FIRE_RATE) STOP_FIRE = 0;
    } 
    if (keys[SDL_SCANCODE_LEFT]) {
        if (STOP_FIRE == 0) fire(-1, 0);
        STOP_FIRE++;
        if (STOP_FIRE == FIRE_RATE) STOP_FIRE = 0;
    }
    if (keys[SDL_SCANCODE_UP]) {
        if (STOP_FIRE == 0) fire(0, -1);
        STOP_FIRE++;
        if (STOP_FIRE == FIRE_RATE) STOP_FIRE = 0;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        if (STOP_FIRE == 0) fire(0, 1);
        STOP_FIRE++;
        if (STOP_FIRE == FIRE_RATE) STOP_FIRE = 0;
    }
    
    
    // update all bullets position
    for (bullets.j = 0; bullets.j < MAX_BULL_ON_SCREEN; bullets.j++) {
        bullets.bul[bullets.j].transform.x += 10*bullets.x[bullets.j];
        bullets.bul[bullets.j].transform.y += 10*bullets.y[bullets.j];
    }
    // Update the explosion's frame once every 100ms
    if (ng_interval_is_ready(&ctx.game_tick))
    {
        ng_animated_set_frame(&ctx.explosion, (ctx.explosion.frame + 1)
                % ctx.explosion.total_frames);
    }
    // rendering
    ng_sprite_render(&ctx.player, ctx.game.renderer);
   
    ng_sprite_render(&ctx.explosion.sprite, ctx.game.renderer);
    ng_sprite_render(&ctx.welcome_text.sprite, ctx.game.renderer);

    for (bullets.j = 0; bullets.j < MAX_BULL_ON_SCREEN; bullets.j++) {
        ng_sprite_render(&bullets.bul[bullets.j], ctx.game.renderer);
    }
        
    //ng_sprite_render(&ctx.cross, ctx.game.renderer);
}

int main()
{
    bullets.last_fired = -1;
    create_actors();
    ng_game_start_loop(&ctx.game,
            handle_event, update_and_render_scene);
}
