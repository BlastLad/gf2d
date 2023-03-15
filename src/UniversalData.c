#include "UniversalData.h"


static int remainingStudents;
static int currentLevel;
static int totalStudents;


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