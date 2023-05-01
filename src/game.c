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
#include "AudioManager.h"
#include "TurretStudent.h"
//#include "../gfc/include/gfc_pak.h"

static int onTitle = 1;
static int areYouSure = 0;
static int currentEditorTileFrame = 0;
static int currentEnemyTileFrame = 0;
static int totalTileFrames = 0;
static int totalEnemyFrames = 0;

void SpawnHazard();
void removeStudent();
void initer(const char* filename, List* AllFurn);
void ProcessEnemyEditorFrame(int frame, List* furnList, int mx, int my);

int main(int argc, char* argv[])
{
    int argi;
    int editorMode = 0;

    for (argi = 1; argi < argc; argi++)
    {
        if (strcmp(argv[argi], "--editor") == 0)
        {
            editorMode = 1;
        }
    }

    /*variable declarations*/
    int done = 0;
    Level* tileMap;

    const Uint8* keys;

    Sprite* sprite;
    Sprite* editorSprite;
    Sprite* mouseSprite;
    Sprite* enemyEditorSprite;
    Sprite* toolSprite;
    Sprite* titleSprite;
    Sprite* sureSprite;
    //NEW
    Entity* playerEntity;//make it pointer probs for all this shit
    Entity* turretEntity;//make it pointer probs for all this shit
    List* Students;
    List* aStarPath;
    List* AllFurnitureList;
    Entity* normalStudent;
    Entity* furnitureItem;
    Entity* entityCreator;


    //NEW

    int mx, my, indexer;
    int qKeyDown = 0;
    int reg1KeyDown = 0;
    int eEditorKeyDown = 0;
    int aEditorKeyDown = 0;
    int Editor1KeyDown = 0;
    int Editor2KeyDown = 0;
    int Editor3KeyDown = 0;
    int dEditorKeyDown = 0;
    float mf = 0;
    float mh = 0;
    float mp = 0;

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
    gfc_audio_init(32, 8, 0, 2, 1, 1);
    entity_manager_init(1024);

    gf2d_body_manager_init(200);
    gf3d_cliplayers_init("config/cliplayers.cfg");
    SDL_ShowCursor(SDL_DISABLE);

    /*demo setup*/

    tileMap = level_load("config/testSave.tilemap");
    totalTileFrames = 18;
    totalEnemyFrames = 4;
    level_set_active_level(tileMap);
    set_current_level_num(1);
    set_current_level_totalStudents(20);
    set_current_level_remainingStudents(get_current_level_totalStudents());


    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    editorSprite = gf2d_sprite_load_image("images/editorframe.png");
    toolSprite = gf2d_sprite_load_image("images/tool.png");   
    mouseSprite = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);
    enemyEditorSprite = gf2d_sprite_load_all("images/enemyEdit.png", 32, 32, 3, 0);
    titleSprite = gf2d_sprite_load_image("images/title.png");
    sureSprite = gf2d_sprite_load_image("images/areyousure.png");

    AllFurnitureList = gfc_list_new();


    playerEntity = piper_entity_new(graph_to_world_pos(9, 9));
    

    initer("config/testSave.tilemap", AllFurnitureList);
   // furnitureItem = SpawnEnemy(2, 7, 9, 0, 0);
   // gfc_list_append(AllFurnitureList, furnitureItem);
  //  furnitureItem = SpawnEnemy(2, 12, 9, 0, 0);
   // gfc_list_append(AllFurnitureList, furnitureItem);
   // furnitureItem = SpawnEnemy(1, 14, 8, 0, 0);
   // gfc_list_append(AllFurnitureList, furnitureItem);
   // furnitureItem = SpawnEnemy(1, 5, 8, 0, 0);
 //   gfc_list_append(AllFurnitureList, furnitureItem);
  //  furnitureItem = karter_new(graph_to_world_pos(4, 6), 4, 6, 10, 6);
 //   gfc_list_append(AllFurnitureList, furnitureItem);
   // furnitureItem = karter_new(graph_to_world_pos(13, 2), 13, 2, 13, 11);
   // gfc_list_append(AllFurnitureList, furnitureItem);
    aStarPath = gfc_list_new();
    set_path_list(aStarPath);

    if (editorMode == 0) {

        turretEntity = Turret_Student_New(vector2d(7, 7), 14, 7);

        Mix_Base_New(graph_to_world_pos(1, 2), vector2d(1, 2));
        Shadow_Clone_PickUp_New(graph_to_world_pos(1, 4), vector2d(1, 4));
        Emerald_PickUp_New(graph_to_world_pos(1, 8), vector2d(1, 8));
        Spellbook_New(graph_to_world_pos(1, 6), vector2d(1, 6));

        Mix_Upgrade_New(graph_to_world_pos(16, 2), vector2d(16, 2));
        Dual_Cast_Upgrade_New(graph_to_world_pos(16, 3), vector2d(16, 3));
        Max_Spellbook_New(graph_to_world_pos(16, 4), vector2d(16, 4));
        Health_Pot_New(graph_to_world_pos(16, 5), vector2d(16, 5));
        play_music(45);

    }
    else {
        turretEntity = NULL;
    }
    PiperData* piperDataPointer;
    int eKeyDown = 0;
    piperDataPointer = (struct PiperData*)playerEntity->data;

    gui_setup_hud(piperDataPointer, get_current_level_remainingStudents);

    Students = gfc_list_new();
    set_student_list(Students);



    int numOfStudents = get_current_level_totalStudents();

   
    
    int hub;
    hub = 0;
    /*main game loop*/
    while (!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //SDL_GetTicks
        /*update things here*/
        SDL_GetMouseState(&mx, &my);



        if (onTitle == 1 && editorMode == 0) 
        {
            gf2d_graphics_clear_screen();// clears drawing buffers
    // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
            gf2d_sprite_draw_image(titleSprite, vector2d(0, 0));

            gf2d_graphics_next_frame();// render current draw frame and skip to the next frame

            music_update();

            if (keys[SDL_SCANCODE_RETURN]) 
            {
                onTitle = 0;
                areYouSure = 0;
            }
            if (keys[SDL_SCANCODE_BACKSPACE])done = 1; // exit condition
        }
        else
        {           
            if (areYouSure == 1) 
            {
                //gf2d_graphics_clear_screen();// clears drawing buffers
                gf2d_sprite_draw_image(sureSprite, vector2d(200, 160));

                gf2d_graphics_next_frame();// render current draw frame and skip to the next frame

                music_update();


                if (keys[SDL_SCANCODE_Y])onTitle = 1;
                if (keys[SDL_SCANCODE_N])areYouSure = 0;

                continue;
            }

            mf += 0.1;
            mh += 0.1;
            mp += 0.1;

            //--------------------EDITOR CODE----------------------------------------------------
            if (editorMode == 1)
            {
                if (mf >= 16.0) {

                    mf = 0;
                }
                Color mouseColor = gfc_color8(255, 100, 255, 200);

                float xe;
                xe = mx / 32;
                if (xe - ceilf(xe) < 0.5)
                    xe = ceilf(xe);
                else
                    xe = floorf(xe);
                int xeInt = (int)xe;


                float ye;
                ye = my / 32;
                if (ye - ceilf(ye) < 0.5)
                    ye = ceilf(ye);
                else
                    ye = floorf(ye);
                int yeInt = (int)ye;

                Vector2D drawTool;
                Vector2D drawTool2;

                int offSetX;
                int offSetY;
                drawTool = graph_to_world_pos(xeInt, yeInt);
                offSetX = drawTool.x - 16;
                offSetY = drawTool.y - 16;
                drawTool2 = vector2d(offSetX, offSetY);

                vector2d_copy(drawTool, drawTool2);


                if (!keys[SDL_SCANCODE_1])
                {
                    Editor1KeyDown = 0;

                }

                if (keys[SDL_SCANCODE_1] && Editor1KeyDown == 0)
                {
                    Editor1KeyDown = 1;
                    ChangeTile(yeInt, xeInt, currentEditorTileFrame + 1);

                    slog("1 key down");
                }

                if (!keys[SDL_SCANCODE_2])
                {
                    Editor2KeyDown = 0;

                }

                if (keys[SDL_SCANCODE_2] && Editor2KeyDown == 0)
                {
                    Editor2KeyDown = 1;
                    ProcessEnemyEditorFrame(currentEnemyTileFrame, AllFurnitureList, xeInt, yeInt);

                    slog("2 key down");
                }

                if (!keys[SDL_SCANCODE_3])
                {
                    Editor3KeyDown = 0;

                }

                if (keys[SDL_SCANCODE_3] && Editor3KeyDown == 0)
                {
                    Editor3KeyDown = 1;
                    level_save("hello", AllFurnitureList);

                    slog("3 key down");
                }

                if (!keys[SDL_SCANCODE_Q])
                {

                    qKeyDown = 0;
                }

                if (keys[SDL_SCANCODE_Q] && qKeyDown == 0)
                {
                    qKeyDown = 1;
                    currentEditorTileFrame -= 1;
                    if (currentEditorTileFrame < 0)
                    {
                        currentEditorTileFrame = totalTileFrames - 1;
                    }
                    slog("q key down");
                }

                if (!keys[SDL_SCANCODE_E])
                {
                    eEditorKeyDown = 0;
                }

                if (keys[SDL_SCANCODE_E] && eEditorKeyDown == 0)
                {
                    eEditorKeyDown = 1;
                    currentEditorTileFrame += 1;
                    if (currentEditorTileFrame >= totalTileFrames)
                    {
                        currentEditorTileFrame = 0;
                    }
                    slog("e key down");
                }

                if (!keys[SDL_SCANCODE_A])
                {
                    aEditorKeyDown = 0;
                }

                if (keys[SDL_SCANCODE_A] && aEditorKeyDown == 0)
                {
                    aEditorKeyDown = 1;
                    currentEnemyTileFrame -= 1;
                    if (currentEnemyTileFrame < 0)
                    {
                        currentEnemyTileFrame = totalEnemyFrames;
                    }
                    slog("a key down");
                }

                if (!keys[SDL_SCANCODE_D])
                {
                    dEditorKeyDown = 0;

                }

                if (keys[SDL_SCANCODE_D] && dEditorKeyDown == 0)
                {
                    dEditorKeyDown = 1;
                    currentEnemyTileFrame += 1;
                    if (currentEnemyTileFrame > totalEnemyFrames)
                    {
                        currentEnemyTileFrame = 0;
                    }
                    slog("d key down");
                }


               

                gf2d_graphics_clear_screen();// clears drawing buffers

                gf2d_sprite_draw_image(sprite, vector2d(0, 0));
                level_draw(level_get_active_level());//need to check
                entity_draw_all();

                if (mx <= 96)
                {
                    gf2d_sprite_draw_image(editorSprite, vector2d(548, 0));
                    gf2d_sprite_draw(level_get_active_level()->tileSet, vector2d(581, 27), NULL, NULL, NULL, NULL, NULL, currentEditorTileFrame);
                    gf2d_sprite_draw(enemyEditorSprite, vector2d(580, 136), NULL, NULL, NULL, NULL, NULL, currentEnemyTileFrame);

                }
                else
                {
                    gf2d_sprite_draw_image(editorSprite, vector2d(0, 0));
                    gf2d_sprite_draw(level_get_active_level()->tileSet, vector2d(33, 27), NULL, NULL, NULL, NULL, NULL, currentEditorTileFrame);
                    gf2d_sprite_draw(enemyEditorSprite, vector2d(32, 136), NULL, NULL, NULL, NULL, NULL, currentEnemyTileFrame);

                }

                gf2d_sprite_draw_image(toolSprite, drawTool);


                gf2d_sprite_draw(
                    mouseSprite,
                    vector2d(mx, my),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    &mouseColor,
                    (int)mf);

               
                gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
            }
            else {
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
                    if (hub == 0) {
                        
                        // PathFinding(7, 7, 7, 10);

                        hub = 1;
                    }
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


                if (!keys[SDL_SCANCODE_1])
                {
                    reg1KeyDown = 0;
                }

             

                if (keys[SDL_SCANCODE_1] && reg1KeyDown == 0)
                {
                    reg1KeyDown = 1;
                   
                        Vector2D currentTurretPos = turretEntity->targetGridPosition;

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

                        //TileInfo tileToCheck;
                        //tileToCheck = get_graph_node(xeInt - 1, yeInt - 1);

                        gfc_list_clear(get_path_list());
                       // gfc_list_delete(get_path_list());
                        turretEntity->markedForDestruction = 1;
                        turret_student_destroy(turretEntity);
                        turretEntity = Turret_Student_New(vector2d(currentTurretPos.x, currentTurretPos.y), xeInt - 1, yeInt - 1);                   
                    
                }

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
                gf2d_sprite_draw_image(sprite, vector2d(0, 0));
                level_draw(level_get_active_level());//need to check
                entity_draw_all();
                //UI elements last
                gui_draw_hud(piperDataPointer, get_current_level_remainingStudents);

                gf2d_graphics_next_frame();// render current draw frame and skip to the next frame

                music_update();
            }
        }
        if (keys[SDL_SCANCODE_ESCAPE] && editorMode == 0) 
        {
            areYouSure = 1; // exit condition
        }
        else if (keys[SDL_SCANCODE_ESCAPE] == 1 && editorMode == 1)
        {
            done = 1;
        }
       // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_free(tileMap);
    entity_free(playerEntity);
    slog("---==== END ====---");
    return 0;
}

void ProcessEnemyEditorFrame(int frame, List* furnList, int mx, int my) 
{
    frame = frame + 1;
    Entity* furnitureItem;
    
    if (frame > 0 && frame < 3) 
    {
        furnitureItem = SpawnEnemy(frame, mx, my, 0, 0);
        gfc_list_append(furnList, furnitureItem);
    }
    else if (frame == 3) 
    {
        if (mx < 15) {
            furnitureItem = SpawnEnemy(frame, mx, my, mx + 3, my);
            gfc_list_append(furnList, furnitureItem);
        }
        else
        {
            furnitureItem = SpawnEnemy(frame, mx, my, mx - 3, my);
            gfc_list_append(furnList, furnitureItem);
        }
    }
    else if (frame == 4) 
    {
        if (my < 8) {
            furnitureItem = SpawnEnemy(frame, mx, my, mx, my + 3);
            gfc_list_append(furnList, furnitureItem);
        }
        else
        {
            furnitureItem = SpawnEnemy(frame, mx, my, mx, my - 3);
            gfc_list_append(furnList, furnitureItem);
        }
    }
    else if (frame == 5) 
    {
        for (int i = gfc_list_get_count(furnList) - 1; i >= 0; i--) 
        {
            Entity* ent;
            ent = gfc_list_get_nth(furnList, i);
            int curX, curY;
            curX = ent->currentGridPosition.x;
            curY = ent->currentGridPosition.y;
            if (curX == mx && curY == my) 
            {
                gfc_list_delete_nth(furnList, i);
                level_remove_entity(ent);
                entity_free(ent);
                break;
            }
        }
    }
   
    // gfc_list_append(AllFurnitureList, furnitureItem);

}

void initer(const char* filename, List* AllFurn)
{
    int tile;
    int i, c, enemyC;
    int j, d, enemyD;
    int tileFPL;
    const char* str;
    SJson* json, * lj, * list, * row, * item, * enemyList, * enemyRow;

    if (!filename)return NULL;
    json = sj_load(filename);
    if (!json)return NULL;

    lj = sj_object_get_value(json, "level");

    if (!lj)
    {
        slog("file %s missing level object", filename);
        sj_free(json);
        return NULL;
    }

    enemyList = sj_object_get_value(lj, "furnitureLayout");
    enemyC = sj_array_get_count(enemyList);
    enemyRow = sj_array_get_nth(enemyList, 0);
    enemyD = sj_array_get_count(enemyRow);

    if ((enemyC * enemyD) == 0)
    {
        slog("No enemies");
    }
    else
    {
        Entity* furnitureItem;
        slog("here 1");

        for (i = 0; i < enemyC; i++)// i is row
        {
            slog("here 2");

            enemyRow = sj_array_get_nth(enemyList, i);
            slog("here 3");

            if (!enemyRow)continue;
            slog("here 4");

            enemyD = sj_array_get_count(enemyRow);
            int type;
            int spawnX;
            int spawnY;
            int tarX;
            int tarY;
            for (j = 0; j < enemyD; j++)// j is column
            {
                item = sj_array_get_nth(enemyRow, j);
                if (!item)continue;

                sj_get_integer_value(item, &tile);
                if (j == 0) {
                    type = tile;
                }
                else if (j == 1)
                {
                    spawnX = tile;
                }
                else if (j == 2) {
                    spawnY = tile;
                }
                else if (j == 3)
                {
                    tarX = tile;
                }
                else if (j == 4)
                {
                    tarY = tile;
                }

            }

            if (type != 3)
            {
                furnitureItem = SpawnEnemy(type, spawnX, spawnY, 0, 0);
                gfc_list_append(AllFurn, furnitureItem);
            }
            else if (type == 3) 
            {
                furnitureItem = SpawnEnemy(type, spawnX, spawnY, tarX, tarY);
                gfc_list_append(AllFurn, furnitureItem);
            }

        }
    
    }

    sj_free(json);
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
