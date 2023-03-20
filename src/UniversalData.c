#include "UniversalData.h"

static int remainingStudents;
static int currentLevel;
static int totalStudents;

List* pointerToStudentList;

Entity* emeraldPool1;
static int currentPools;
List* carpetTileList;


List* getCarpetTileList() {
	return carpetTileList;
}
void setCarpetTileList(List* carpetPointer) {
	carpetTileList = carpetPointer;
}

int GetPoolNum() {
	return currentPools;
}

Entity* GetEmeraldPool(int index) {
	return emeraldPool1;
}

void SetEmeraldPool(Entity* pool) 
{
	emeraldPool1 = pool;
}


void resetEmeraldPoolStatus() {
	currentPools = 0;
}

int checkEmeraldPoolStatus() 
{
	if (currentPools >= 1) {
		emeraldPool1->markedForDestruction = 1;
		currentPools = 0;
		return 3;
	}
	else 
	{
		currentPools += 1;
		return currentPools;
	}
}


List* get_student_list() {
	return pointerToStudentList;
}

void set_student_list(List* pointer) {
	pointerToStudentList = pointer;
}

int get_current_level_num() {
	return currentLevel;
}

void set_current_level_num(int newNum) {
	currentLevel = newNum;
}

int get_current_level_totalStudents() {
	return totalStudents;
}

void set_current_level_totalStudents(int newNum) {
	totalStudents = newNum;
}

int get_current_level_remainingStudents() {
	return remainingStudents;

}
void set_current_level_remainingStudents(int newNum) {
	remainingStudents = newNum;
}