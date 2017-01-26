/*
 * main.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "infrastructure/initialization.h"

int main()
{
    infrastructure::initialize();

    /*
     * :D
     */

    infrastructure::finalize();

    return 0;
}


