#include "UniversalData.h"
#include "Piper.h"
#include "TileMap.h"

static int remainingStudents;
static int currentLevel;
static int totalStudents;

List* pointerToStudentList;
List* pointerToPathList;
List* AllFurnitureList;

Entity* emeraldPool1;
static int currentPools;
List* carpetTileList;
List* powerUpList;




static HellFireData hellFireData = {
	0,
	0,
	0,
	0,
	0
};

HellFireData* GetHellFireData() {
	return &hellFireData;
}

List* getFurnitureList() {
	return &AllFurnitureList;
}

void setFurnitureList(List* carpetPointer) 
{
	AllFurnitureList = carpetPointer;
}

List* get_powerup_list() {
	return powerUpList;
}
List* set_powerup_list(List* pointer) {
	powerUpList = pointer;
}

void set_path_list(List* pointer) {
	pointerToPathList = pointer;
}

List* get_path_list() {
	return pointerToPathList;
}

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


int OnPowerUpCollect(int value)
{
	int indexer;
	Entity* normalStudent;

	if (powerUpList->count > 0) {
		for (indexer = powerUpList->count - 1; indexer >= 0; indexer--) {
			normalStudent = gfc_list_get_nth(powerUpList, indexer);
			if (!normalStudent)continue;
			
			normalStudent->markedForDestruction = 1;

				gfc_list_delete_data(powerUpList, normalStudent);
			
		}
	}



	PiperData* piperDataPointer;
	piperDataPointer = GetPiperData();

	piperDataPointer->currency -= value;

	if (piperDataPointer->currency <= 0) {
		piperDataPointer->currency = 0;
	}

	piperDataPointer->powerUpCollected = 1;
	

	return 0;
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