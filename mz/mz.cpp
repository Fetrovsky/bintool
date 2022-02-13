#include "mz.h"

#include <iostream>
#include <string_view>

MZ* MZ::Parse(std::string_view buffer)
{
    auto const& mz_signature = Parse_As<uint16_t>(&buffer[0]);

    if (mz_signature != 0x5a4d)
        return nullptr;

    auto signature_offset = Parse_As<uint32_t>(&buffer[0x3c]);
    std::cout << "Signature offset found at 0x3c: " << std::hex << signature_offset << std::endl;

    auto signature = Parse_As<uint32_t>(&buffer[signature_offset]);
    std::cout
        << "Signature found at " << std::hex << signature_offset
        << ": " << signature << std::endl;

    if (signature == 0x4550)
        std::cout << "PE signature found.  This is a Portable Executable file." << std::endl;
    else
        return nullptr;

    // Assuming 32-bit PE file.  16-bit and 64-bit support to be added later.
    auto mz = new MZ{buffer};

    return mz;
}

MZ::COFF_Header const& MZ::Get_Header() const
{
    return get_header<COFF_Header>(Get_COFF_Header_Address());
}

MZ::Optional_Header MZ::get_optional_header() const
{
    return get_header<Optional_Header>(Get_Optional_Header_Address());
}

MZ::Optional_Header_Plus MZ::get_optional_header_plus() const
{
    return get_header<Optional_Header_Plus>(Get_Optional_Header_Address());
}

std::variant<std::nullptr_t, MZ::Optional_Header, MZ::Optional_Header_Plus> MZ::Get_Optional_Header() const
{
    switch (get_field<Magic_Number>(Get_Optional_Header_Address()))
    {
        case Magic_Number::PE32:
            return get_optional_header();

        case Magic_Number::PE32_PLUS:
            return get_optional_header_plus();
        
        default:
            return nullptr;
    }
}

uint32_t MZ::Get_COFF_Header_Address() const
{
    return get_field<uint32_t>(0x3c);
}

uint32_t MZ::Get_COFF_Header_Size() const
{
    return sizeof(COFF_Header);
}

uint64_t MZ::Get_Optional_Header_Address() const
{
    return Get_COFF_Header_Address() + Get_COFF_Header_Size();
}

uint32_t MZ::Get_Optional_Header_Size() const
{
    return Get_Header().Size_Of_Optional_Header;
}

File_Format MZ::Get_File_Format() const
{
    // File_Format::MZ_Executable
    // File_Format::MZ_Object
    // File_Format::MZ_DLL
    // File_Format::MZ_Library
    return File_Format::MZ_Executable;
}

uint16_t MZ::Get_Number_of_Sections() const
{
    return Get_Header().Number_Of_Sections;
}

uint64_t MZ::Get_Section_Table_Address() const
{
    return Get_Optional_Header_Address() + Get_Optional_Header_Size();
}

MZ::Section_Header const& MZ::Get_Section_Header(uint16_t i) const
{
    auto const& First_Section_Header = get_field<Section_Header>(Get_Section_Table_Address());
    return (&First_Section_Header)[i];
}

std::string_view MZ::Get_Section_Name(MZ::Section_Header const& sh) const
{
    std::string_view Name_Field(&sh.Name[0], 8);

    size_t nul_terminator = Name_Field.find('\0');
    if (nul_terminator != std::string_view::npos)
        Name_Field = Name_Field.substr(0, nul_terminator);

    //
    // TODO: For longer names, this field contains a slash (/) that is followed
    //       by an ASCII representation of a decimal number that is an offset
    //       into the string table.
    //

    return Name_Field;
}

