#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "../gfc/include/gfc_input.h"
#include "Entity.h"
#include "piper.h"
#include "StudentManager.h"
#include "NormalStudent.h"
#include "TileMap.h"
#include "camera.h"
#include "ClipLayers.h"
#include "Rugby.h"
#include "Bubbles.h"
#include "Karter.h"
//#include "../gfc/include/gfc_pak.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    Level* tileMap;

    int remainingStudents = 20;
   
    const Uint8 * keys;

    Sprite *sprite;
    //NEW
    Entity *playerEntity;//make it pointer probs for all this shit
    List *Students;
    Entity* normalStudent;
    Entity* entityCreator;


    //NEW
    
    int mx,my, indexer;
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
    gf2d_body_manager_init(200);
    gf3d_cliplayers_init("config/cliplayers.cfg");
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/

    tileMap = level_load("config/test.tilemap");
    level_set_active_level(tileMap);    

    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);

    playerEntity = piper_entity_new(graph_to_world_pos(7, 6));
    Rugby_New(graph_to_world_pos(7, 9), vector2d(7, 9));
    Bubbles_New(graph_to_world_pos(12, 8), vector2d(12, 8), vector2d(11, 8));
    Bubbles_New(graph_to_world_pos(14, 8), vector2d(14, 8), vector2d(15, 8));
    karter_new(graph_to_world_pos(6, 6), 6, 6, 15, 6);


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
                //studentSpawnPos = Determine_Spawn_Position();
                normalStudent = Determine_Student_To_Spawn();// normal_student_new(graph_to_world_pos(studentSpawnPos.x, studentSpawnPos.y), studentSpawnPos.x, studentSpawnPos.y);
                gfc_list_append(Students, normalStudent);

                remainingStudents--;
            }
        }

        entity_think_all();//need to check
        tileMap_Update(level_get_active_level());//need to check
        entity_update_all();//need to check

       /* if (Students->count > 0) {
            for (indexer = Students->count - 1; indexer >= 0; indexer--) {
                normalStudent = gfc_list_get_nth(Students, indexer);
                if (!normalStudent)continue;
                if (normalStudent->markedForDestruction > 0) {
                    normal_student_remove_from_list(Students, normalStudent);
                }
            }
        }*/

        


        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level_get_active_level());//need to check
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
       // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_free(tileMap);
    entity_free(playerEntity);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
