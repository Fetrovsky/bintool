#include "mz-dumper.h"

#include <iostream>

void Show_MZ_File_Details(MZ const& mz)
{
    std::cout << int(mz.Get_File_Format()) << std::endl;
}

