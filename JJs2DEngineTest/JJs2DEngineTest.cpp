#include "pch.h"
#include <iostream>

#include "RunProgram.h"

int main()
{
    try
    {
        RunProgram();
    }
    catch (const std::exception& exc)
    {
        std::cerr << exc.what() << std::endl;
    }

    return 0;
}
