#ifndef __GUI_H__
#define __GUI_H__

#include "Piper.h"

void gui_setup_hud(PiperData* piperData, int RemainingStudents);
void gui_draw_hud(PiperData* piperData, int RemainingStudents);
void gui_set_hp(int hp);
void gui_set_students(int students);
void gui_add_fragment();


#endif
