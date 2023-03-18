#include "gui.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gfc_text.h"
#include "UniversalData.h"

typedef struct
{
    Sprite* hud;
    Sprite* healthFragmentUI;
    Sprite* headUI;
    Sprite* lockDownUI;
    Sprite* lockDownWithUpgradeUI;
    Sprite* dualCastUI;
    Sprite* emeraldPoolUI;
    Sprite* shadowEffPoolUI;
    Sprite* speelbookUI;
    int remainingStudents;
    float   maxHp;  
    float   currentHealth;  
}GUI;

static GUI gui = { 0 };

void gui_close_hud()
{
    gf2d_sprite_free(gui.hud);
}

void gui_setup_hud(PiperData* piperData, int RemainingStudents)
{
    memset(&gui, 0, sizeof(GUI));
    gui.hud = gf2d_sprite_load_all("images/studentFace.png",
        32,
        32,
        1,
        0);
    gui.healthFragmentUI = gf2d_sprite_load_all("images/HealthFrag.png", 32, 32, 1, 0);
    gui.dualCastUI = gf2d_sprite_load_all("images/Sleep.png", 32, 32, 1, 0);
    gui.headUI = gf2d_sprite_load_all("images/studentFace.png", 32, 32, 1, 0);
    gui.emeraldPoolUI = gf2d_sprite_load_all("images/studentFace.png", 32, 32, 1, 0);
    gui.shadowEffPoolUI = gf2d_sprite_load_all("images/studentFace.png", 32, 32, 1, 0);
    gui.lockDownUI = gf2d_sprite_load_all("images/Mix.png", 32, 32, 1, 0);
    gui.lockDownWithUpgradeUI = gf2d_sprite_load_all("images/Mix.png", 32, 32, 1, 0);
    gui.speelbookUI = gf2d_sprite_load_all("images/Ultima.png", 32, 32, 1, 0);
    gui.maxHp = piperData->maxHealth;
    gui.currentHealth = piperData->currentHealth;
    atexit(gui_close_hud);
}

void gui_draw_percent_bar_horizontal(Rect rect, float percent, Vector4D fgColor, Vector4D bgColor, int left)
{
    SDL_Rect r;
    r = gfc_rect_to_sdl_rect(rect);
    Color colorFront = gfc_color(fgColor.x, fgColor.y, fgColor.z, fgColor.w);
               
    Color colorBack = gfc_color(0.1, 0.1, 0.1, 1);
    gf2d_draw_rect_filled(rect, colorBack);
       

    gf2d_draw_rect_filled(gfc_rect(rect.x, rect.y, 150 * percent, rect.h), colorFront);
}

void gui_draw_percent_bar_vertical(Rect rect, float percent, Vector4D fgColor, Vector4D bgColor, int top)
{
    SDL_Rect r;
    r = gfc_rect_to_sdl_rect(rect);
 //   gfc_draw_solid_rect(r, bgColor);
    if (top)
    {
     //   r = gfc_rect_to_sdl_rect(gfc_rect(rect.x, rect.y + (1 - percent) * rect.h, rect.w, rect.h * percent));
    }
    else
    {
      //  r = gfc_rect_to_sdl_rect(gfc_rect(rect.x, rect.y, rect.w, rect.h * percent));
    }
  //  gf2d_draw_solid_rect(r, fgColor);
}

void gui_draw_hud(PiperData* piperData, int RemainingStudents)
{
    Vector4D color = { 255,255,255,255 };
    gui.maxHp = piperData->maxHealth;
    gui.currentHealth = piperData->currentHealth;
    gf2d_sprite_draw(
        gui.hud, vector2d(100, 100),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        0);
    float currentHealthPercent = gui.currentHealth / gui.maxHp;
    float currentStudentPercent = (float)get_current_level_remainingStudents() / (float)get_current_level_totalStudents();
    slog("current student percent %f", currentStudentPercent);
     gui_draw_percent_bar_horizontal(gfc_rect(32, 16, 150, 10), currentHealthPercent, vector4d(255, 0, 0, 255), vector4d(128, 0, 0, 128), 0);
     gui_draw_percent_bar_horizontal(gfc_rect(432, 400, 150, 10), currentStudentPercent, vector4d(0, 0, 255, 255), vector4d(128, 0, 0, 128), 0);
     int i;
     int startinghealthFragUI;
     startinghealthFragUI = 384;
     for (i = 0; i < piperData->currentHealthFragments; i++) {
         gf2d_sprite_draw(
             gui.healthFragmentUI,
             vector2d(startinghealthFragUI, 0),
             NULL,
             NULL,
             NULL,
             NULL,
             NULL,
             0);
         startinghealthFragUI += 32;
     }
     
     int startingBook;
     startingBook = 610;
     for (i = 0; i < piperData->currentSpellBooks; i++) {
         gf2d_sprite_draw(
             gui.speelbookUI,
             vector2d(startingBook, 384),
             NULL,
             NULL,
             NULL,
             NULL,
             NULL,
             0);
         startingBook += 32;
     }



     gf2d_sprite_draw(gui.headUI, vector2d(400, 384), NULL, NULL, NULL, NULL, NULL, 0);
     //gf2d_sprite_draw(gui.speelbookUI, vector2d(496, 384), NULL, NULL, NULL, NULL, NULL, 0);

     if (piperData->sleepUpgrade) gf2d_sprite_draw(gui.dualCastUI, vector2d(32, 366), NULL, NULL, NULL, NULL, NULL, 0);
     if (piperData->mixingUpgrade) gf2d_sprite_draw(gui.lockDownWithUpgradeUI, vector2d(64, 400), NULL, NULL, NULL, NULL, NULL, 0);

   // gui_draw_percent_bar_horizontal(gfc_rect(640, 680, 145, 10), gui.shieldPercent, vector4d(0, 0, 255, 255), vector4d(128, 0, 0, 128), 1);
   // gui_draw_percent_bar_vertical(gfc_rect(475, 680, 15, 30), gui.thrustPercent, vector4d(0, 255, 255, 255), vector4d(128, 0, 0, 128), 1);
   // gui_draw_percent_bar_vertical(gfc_rect(790, 680, 15, 30), gui.chargePercent, vector4d(128 + (127 * gui.chargePercent), 128 - (127 * gui.chargePercent), 128 - (127 * gui.chargePercent), 255), vector4d(128, 0, 0, 128), 1);
    //gfc_text_draw_line("Health", FT_Small, gfc_color8(0, 255, 0, 255), vector2d(495, 665));
   // gfc_text_draw_line
   // gfc_text_draw_line("Shields", FT_Small, gf2d_color8(0, 0, 255, 255), vector2d(672, 665));
}


void gui_set_students(int students)
{
    gui.remainingStudents = students;
}


/*eol@eof*/