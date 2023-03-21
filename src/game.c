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
#include "WaterSpill.h"
#include "UniversalData.h"
#include "gui.h"
#include "Upgrades.h"
//#include "../gfc/include/gfc_pak.h"


void SpawnHazard();

void removeStudent();

int main(int argc, char* argv[])
{
    /*variable declarations*/
    int done = 0;
    Level* tileMap;

    const Uint8* keys;

    Sprite* sprite;
    //NEW
    Entity* playerEntity;//make it pointer probs for all this shit
    List* Students;
    List* AllFurnitureList;
    Entity* normalStudent;
    Entity* furnitureItem;
    Entity* entityCreator;


    //NEW

    int mx, my, indexer;
    float mf = 0;
    float mh = 0;
    float mp = 0;
    Color mouseColor = gfc_color8(255, 100, 255, 200);

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Super BeWitched!",
        1200,
        720,
        640,
        416,
        vector4d(0, 0, 0, 255),
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
    set_current_level_num(1);
    set_current_level_totalStudents(20);
    set_current_level_remainingStudents(get_current_level_totalStudents());


    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");


    AllFurnitureList = gfc_list_new();

    playerEntity = piper_entity_new(graph_to_world_pos(9, 9));
    furnitureItem = Rugby_New(graph_to_world_pos(7, 9), vector2d(7, 9));
    gfc_list_append(AllFurnitureList, furnitureItem);
    furnitureItem = Rugby_New(graph_to_world_pos(12, 9), vector2d(12, 9));
    gfc_list_append(AllFurnitureList, furnitureItem);
    furnitureItem = Bubbles_New(graph_to_world_pos(14, 8), vector2d(14, 8), vector2d(11, 8));
    gfc_list_append(AllFurnitureList, furnitureItem);
    furnitureItem = Bubbles_New(graph_to_world_pos(5, 8), vector2d(5, 8), vector2d(6, 8));
    gfc_list_append(AllFurnitureList, furnitureItem);
    furnitureItem = karter_new(graph_to_world_pos(4, 6), 4, 6, 10, 6);
    gfc_list_append(AllFurnitureList, furnitureItem);
    furnitureItem = karter_new(graph_to_world_pos(13, 2), 13, 2, 13, 11);
    gfc_list_append(AllFurnitureList, furnitureItem);

    PiperData* piperDataPointer;
    int eKeyDown = 0;
    piperDataPointer = (struct PiperData*)playerEntity->data;

    gui_setup_hud(piperDataPointer, get_current_level_remainingStudents);

    Students = gfc_list_new();
    set_student_list(Students);
    int numOfStudents = get_current_level_totalStudents();

    Mix_Base_New(graph_to_world_pos(1, 2), vector2d(1, 2));
    Shadow_Clone_PickUp_New(graph_to_world_pos(1, 4), vector2d(1, 4));
    Emerald_PickUp_New(graph_to_world_pos(1, 8), vector2d(1, 8));
    Spellbook_New(graph_to_world_pos(1, 6), vector2d(1, 6));

    Mix_Upgrade_New(graph_to_world_pos(16, 2), vector2d(16, 2));
    Dual_Cast_Upgrade_New(graph_to_world_pos(16, 3), vector2d(16, 3));
    Max_Spellbook_New(graph_to_world_pos(16, 4), vector2d(16, 4));
    Health_Pot_New(graph_to_world_pos(16, 5), vector2d(16, 5));


    /*main game loop*/
    while (!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx, &my);
        mf += 0.1;
        mh += 0.1;
        mp += 0.1;


        if (mf >= 16.0) {

            mf = 0;
            if (numOfStudents > 0) {
                //studentSpawnPos = Determine_Spawn_Position();
                normalStudent = Determine_Student_To_Spawn(get_current_level_remainingStudents(), playerEntity);// normal_student_new(graph_to_world_pos(studentSpawnPos.x, studentSpawnPos.y), studentSpawnPos.x, studentSpawnPos.y);
                gfc_list_append(Students, normalStudent);

                numOfStudents--;

            }
            else 
            {
                if (piperDataPointer->currentHealth == piperDataPointer->maxHealth) 
                {
                    Spellbook_New(graph_to_world_pos(8, 6), vector2d(8, 6));
                    //spawn spell book
                }
                
                Health_Pot_New(graph_to_world_pos(10, 6), vector2d(10, 6));

                //spawn health pot
                set_current_level_num(get_current_level_num());
                set_current_level_totalStudents(get_current_level_totalStudents() + 4);
                set_current_level_remainingStudents(get_current_level_totalStudents());                
                numOfStudents = get_current_level_totalStudents();
            }
        }

        if (mh >= 40.0) {
            mh = 0;
            SpawnHazard();
        }

        if (mp >= 60) {
            mp = 0;

        }



        entity_think_all();//need to check
        tileMap_Update(level_get_active_level());//need to check
        entity_update_all();//need to check

        if (Students->count > 0) {
            for (indexer = Students->count - 1; indexer >= 0; indexer--) {
                normalStudent = gfc_list_get_nth(Students, indexer);
                if (!normalStudent)continue;
                if (normalStudent->markedForDestruction > 0) {
                    removeStudent(Students, normalStudent);
                }
            }
        }

        int useSpell = 0;
        if (keys[SDL_SCANCODE_TAB] && piperDataPointer->currentSpellBooks > 0)
        {
            piperDataPointer->currentSpellBooks -= 1;
            useSpell = 1;

        }// exit condition


        if (!keys[SDL_SCANCODE_E])
        {
            eKeyDown = 0;
        }



        if (keys[SDL_SCANCODE_E] && eKeyDown == 0)
        {
            eKeyDown = 1;
            if (piperDataPointer->emeraldAbility == 1) {
                int NumPools;
                NumPools = checkEmeraldPoolStatus();
                if (NumPools == 1)
                {
                    slog("Here");
                    int isEmerald1Carpet;
                    isEmerald1Carpet = 0;

                    float xe;
                    xe = playerEntity->position.x / 32;
                    if (xe - ceilf(xe) < 0.5)
                        xe = ceilf(xe);
                    else
                        xe = floorf(xe);
                    int xeInt = (int)xe;


                    float ye;
                    ye = playerEntity->position.y / 32;
                    if (ye - ceilf(ye) < 0.5)
                        ye = ceilf(ye);
                    else
                        ye = floorf(ye);
                    int yeInt = (int)ye;

                    TileInfo tileToCheck;
                    tileToCheck = get_graph_node(xeInt - 1, yeInt - 1);
                    if (tileToCheck.tileFrame >= 2 && tileToCheck.tileFrame <= 12) {//isCarpet
                        isEmerald1Carpet = 1;
                    }


                    if (isEmerald1Carpet == 1)
                        Emerald_New(playerEntity->position, vector2d(xeInt - 1, yeInt - 1), 1);
                    else
                    {
                        resetEmeraldPoolStatus();
                    }
                }
            }
        }    



        if (AllFurnitureList->count > 0) {
            for (indexer = AllFurnitureList->count - 1; indexer >= 0; indexer--) {
                furnitureItem = gfc_list_get_nth(AllFurnitureList, indexer);
                if (!furnitureItem)continue;

                if (useSpell == 1)
                {
                    furnitureItem->markedForDestruction = 1;
                }

                if (furnitureItem->markedForDestruction > 0)
                {
                    furnitureItem->markedForDestruction = 1;
                    //normal_student_remove_from_list(Students, normalStudent);
                }
            }
        }

        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level_get_active_level());//need to check
            entity_draw_all();
            //UI elements last
        gui_draw_hud(piperDataPointer, get_current_level_remainingStudents);
        

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
       // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_free(tileMap);
    entity_free(playerEntity);
    slog("---==== END ====---");
    return 0;
}


void SpawnPowerUp() {
    TileInfo randomTile;
    randomTile = Get_Random_GridNode();
    float ranNum;
    ranNum = gfc_random();
    if (ranNum > 0.7) {
        Health_Pot_New(graph_to_world_pos(randomTile.coordinates.x, randomTile.coordinates.y), vector2d(randomTile.coordinates.x, randomTile.coordinates.y));
    }
    else
    {
        Broom_New(graph_to_world_pos(randomTile.coordinates.x, randomTile.coordinates.y), vector2d(randomTile.coordinates.x, randomTile.coordinates.y));
    }
}

void SpawnHazard() 
{
    TileInfo randomTile;
    randomTile = Get_Random_GridNode();
    Water_Spill_New(graph_to_world_pos(randomTile.coordinates.x, randomTile.coordinates.y), vector2d(randomTile.coordinates.x, randomTile.coordinates.y));

}

void removeStudent(List* student_list, Entity* self)
{
    set_current_level_remainingStudents(get_current_level_remainingStudents() - 1);
    if (self->markedForDestruction == 2) 
    {
        GetPiperData()->currentHealth -= 1;
    }
    gfc_list_delete_data(student_list, self);
    level_remove_entity(self);
    entity_free(self);
}

/*eol@eof*/
