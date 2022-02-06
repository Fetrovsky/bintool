#include "mz-dumper.h"

#include <chrono>
#include <ctime>
#include <iostream>

#include <mz/mz.h>

using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::operator""y;
using std::cout;
using std::endl;

using typeid_t = void const*;

template <typename T>
struct wrapper {
    static void const* const id;
};

template<typename T>
void const* const wrapper<T>::id = nullptr;

template<typename T>
constexpr typeid_t typeof()
{
    return &wrapper<T>::id;
}

template<typename Optional_Header_Type>
void Show_MZ_Optional_Header(Optional_Header_Type const& oh)
{
    cout
        << "\n  OptionalHeader: "
        << "\n    Magic: " << Get_Magic_Number_Name(oh.Magic)
        << "\n    Major_Linker_Version: " << int(oh.Major_Linker_Version)
        << "\n    Minor_Linker_Version: " << int(oh.Minor_Linker_Version)
        << "\n    Size_Of_Code: " << oh.Size_Of_Code
        << "\n    Size_Of_Initialized_Data: " << oh.Size_Of_Initialized_Data
        << "\n    Size_Of_Uninitialized_Data: " << oh.Size_Of_Uninitialized_Data
        << "\n    Address_Of_Entry_Point: " << oh.Address_Of_Entry_Point
        << "\n    Base_Of_Code: " << oh.Base_Of_Code;

    if constexpr(typeof<Optional_Header_Type>() == typeof<MZ::Optional_Header_Plus>())
        cout << "\n    Base_Of_Data: " << oh.Base_Of_Data;

    cout
        << "\n    Image_Base: " << oh.Image_Base
        << "\n    Section_Alignment: " << oh.Section_Alignment
        << "\n    File_Alignment: " << oh.File_Alignment
        << "\n    Major_Operating_System_Version: " << oh.Major_Operating_System_Version
        << "\n    Minor_Operating_System_Version: " << oh.Minor_Operating_System_Version
        << "\n    Major_Image_Version: " << oh.Major_Image_Version
        << "\n    Minor_Image_Version: " << oh.Minor_Image_Version
        << "\n    Major_Subsystem_Version: " << oh.Major_Subsystem_Version
        << "\n    Minor_Subsystem_Version: " << oh.Minor_Subsystem_Version
        << "\n    Win32_Version_Value: " << oh.Win32_Version_Value
        << "\n    Size_Of_Image: " << oh.Size_Of_Image
        << "\n    Size_Of_Headers: " << oh.Size_Of_Headers
        << "\n    Check_Sum: " << oh.Check_Sum
        << "\n    Subsystem: " << Get_Subsystem_Name(oh.Subsystem)
        << "\n    Dll_Characteristics: " << int(oh.Dll_Characteristics);

    auto const characteristics = Get_Image_DLL_Characteristics_Names(oh.Dll_Characteristics);

    for (auto const ch: characteristics)
        cout << "\n      " << ch;

    cout
        << "\n    Size_Of_Stack_Reserve: " << oh.Size_Of_Stack_Reserve
        << "\n    Size_Of_Stack_Commit: " << oh.Size_Of_Stack_Commit
        << "\n    Size_Of_Heap_Reserve: " << oh.Size_Of_Heap_Reserve
        << "\n    Size_Of_Heap_Commit: " << oh.Size_Of_Heap_Commit
        << "\n    Loader_Flags: " << oh.Loader_Flags
        << "\n    Number_Of_Rva_And_Sizes: " << oh.Number_Of_Rva_And_Sizes;
}

void Show_MZ_File_Details(MZ const& mz)
{
    auto const format_name = Get_File_Format_Name(mz.Get_File_Format());

    auto coff_header = mz.Get_Header();

    auto timestamp = time_point<system_clock>(std::chrono::seconds(coff_header.Time_Date_Stamp));
    auto time = system_clock::to_time_t(timestamp);
    std::string timestamp_as_string = std::ctime(&time);
    timestamp_as_string = timestamp_as_string.substr(0, timestamp_as_string.length() - 1);

    cout
        << "Portable Executable file details:"
        << "\n  Signature: " << coff_header.Signature
        << "\n  Machine: " << Get_Machine_Type_Name(coff_header.Machine)
        << "\n  Number_Of_Sections: " << coff_header.Number_Of_Sections
        << "\n  Time_Date_Stamp: " << timestamp_as_string
        << "\n  Pointer_to_Symbol_Table: " << coff_header.Pointer_to_Symbol_Table
        << "\n  Number_Of_Symbols: " << coff_header.Number_Of_Symbols
        << "\n  Size_Of_Optional_Header: " << coff_header.Size_Of_Optional_Header
        << "\n  Characteristics: " << int(coff_header.Characteristics);

    auto const characteristics = Get_Image_File_Characteristics_Names(coff_header.Characteristics);

    for (auto const ch: characteristics)
        cout << "\n    " << ch;

    cout << '\n';

    switch (auto optional_header = mz.Get_Optional_Header();
            optional_header.index())
    {
        case 0:
            cout << "Optional Header not present." << endl;
            break;

        case 1:
            Show_MZ_Optional_Header(std::get<MZ::Optional_Header>(optional_header));
            break;

        case 2:
            Show_MZ_Optional_Header(std::get<MZ::Optional_Header_Plus>(optional_header));
            break;

    }
}

