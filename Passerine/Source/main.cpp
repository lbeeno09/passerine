#include "Core/Editor.h"

#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        Editor editor(1280, 720, L"Passerine");
        editor.Run();
    }
    catch(std::exception& e)
    {
        std::cerr << "ERROR:\t" << e.what() << '\n';

        return -1;
    }

    return 0;
}
