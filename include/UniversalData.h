#ifndef __UNIVERSAL_DATA__
#define __UNIVERSAL_DATA__

#include "gfc_list.h";
#include "Entity.h"

List* getCarpetTileList();
void setCarpetTileList(List* carpetPointer);
void resetEmeraldPoolStatus();
int GetPoolNum();
List* get_student_list();
void set_student_list(List* pointer);

int checkEmeraldPoolStatus();
Entity* GetEmeraldPool(int index);

void SetEmeraldPool(Entity* pool);


int get_current_level_num();
void set_current_level_num(int newNum);

int get_current_level_totalStudents();
void set_current_level_totalStudents(int newNum);

int get_current_level_remainingStudents();
void set_current_level_remainingStudents(int newNum);


#endif
