#include "../headers/utilities.h"

void clearScreen() {

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}