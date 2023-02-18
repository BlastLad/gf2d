#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "../gfc/include/gfc_input.h"
#include "Entity.h"
#include "piper.h"
#include "NormalStudent.h"
#include "TileMap.h"
#include "camera.h"
//#include "../gfc/include/gfc_pak.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    int remainingStudents = 20;
    Level *tileMap;
    const Uint8 * keys;
    Sprite *sprite;
    //NEW
    Entity *playerEntity;//make it pointer probs for all this shit
    List *Students;
    Entity* normalStudent;


    //NEW
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Super BeWitched!",
        1200,
        720,
        664,
        424,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/

    tileMap = level_load("config/test.tilemap");
    level_set_active_level(tileMap);    

    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    playerEntity = piper_entity_new(graph_to_world_pos(8, 6));

    Students = gfc_list_new();

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
       

        if (mf >= 16.0) {

            mf = 0;
            if (remainingStudents > 0) {
                normalStudent = normal_student_new(graph_to_world_pos(7, 2), 7, 2);
                gfc_list_append(Students, normalStudent);
                remainingStudents--;
                get_next_carpet_tile(normalStudent->currentGridPositionX, normalStudent->currentGridPositionY, normalStudent);
                slog("success");
            }
        }

        entity_think_all();
        entity_update_all();

        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level_get_active_level());
            entity_draw_all();
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_free(tileMap);
    entity_free(playerEntity);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
