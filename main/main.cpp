#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <span>
#include <string>
#include <sstream>
#include <memory>
#include <variant>
#include <vector>

#include <include/file-format.h>
#include <elf/elf.h>
#include <mz/mz.h>

#include "command-line-arguments.h"
#include "elf-dumper.h"
#include "mz-dumper.h"

using std::nullptr_t;
using std::string;
using std::string_view;
using std::unique_ptr;

int Usage(string_view program_name)
{
    std::cout
        << "Usage: " << program_name << " <filename>"
        << std::endl;

    return 1;
}

std::variant<nullptr_t, unique_ptr<Parsed_File>>
Parse(string_view file_contents)
{
    if (file_contents.length() < 8)
        return nullptr;

    if (auto elf = unique_ptr<ELF>(ELF::Parse(file_contents));
             elf != nullptr)
        return elf;

//    if (file_contents.substr(0, 8) == "!<arch>\n")
//        return File_Format::AR_Arch;
//
//    if (file_contents.substr(0, 8) == "!<bigaf>")
//        return File_Format::AR_BigAF;

    if (auto mz = unique_ptr<MZ>(MZ::Parse(file_contents));
             mz != nullptr)
        return mz;

    return nullptr;
}

bool Read_File(string const& file_name, string& file_contents)
{
    std::ifstream stream(file_name.c_str());

    if (!stream)
    {
        std::cout << "Could not open file " << file_name << std::endl;
        return false;
    }

    stream.seekg(0, std::ios_base::end);
    auto const file_size = stream.tellg();
    stream.seekg(0, std::ios_base::beg);

    file_contents.resize(file_size);
    stream.read(&file_contents[0], file_size);

    return true;
}

void Show_File_Details(Parsed_File const& parsed_file, Command_Line_Arguments const& /*arguments*/)
{
    auto const file_format = parsed_file.Get_File_Format();
    auto const file_format_name = Get_File_Format_Name(file_format);

    std::cout << "This is a file of type " << file_format_name << "." << std::endl;

    switch(file_format)
    {
        case File_Format::ELF_Executable:
        case File_Format::ELF_Object:
        case File_Format::ELF_Shared_Object:
            Show_ELF_File_Details(static_cast<ELF const&>(parsed_file));
            break;

        case File_Format::ELF64_Executable:
        case File_Format::ELF64_Object:
        case File_Format::ELF64_Shared_Object:
        case File_Format::ELF64_Core_Dump:
            Show_ELF_File_Details(static_cast<ELF64::ELF64 const&>(parsed_file));
            break;

        case File_Format::AR_Arch:
            std::cout << "Arch not understood." << std::endl;
            break;

        case File_Format::AR_BigAF:
            std::cout << "Arch not understood." << std::endl;
            break;

        case File_Format::MZ_Executable:
        case File_Format::MZ_Object:
        case File_Format::MZ_DLL:
        case File_Format::MZ_Library:
            Show_MZ_File_Details(static_cast<MZ const&>(parsed_file));
            break;

        default:
            std::cout << "Unknown file format " << file_format_name << std::endl;
            break;
    }
}

int main(int argc, char* argv[])
{
    Command_Line_Arguments arguments {
        {{"--verbose", "-v"}},
        {{"--section", "-s"}}
    };

    if (!arguments.Parse(std::span(argv, argc)))
    {
        return Usage(argv[0]);
    }

    if (arguments.Standalone().size() != 1)
        return Usage(argv[0]);

    string filename = arguments.Standalone()[0];
    string contents;

    if (!Read_File(filename, contents))
        return -2;

    std::cout
        << filename << ": " << contents.length() << "(0x" << std::hex << contents.length() << ")" << " bytes."
        << std::endl;

    switch (auto parsed_content = Parse(contents); parsed_content.index())
    {
        case 0:
            std::cout << "Could not understand the format." << std::endl;
            return -3;
            break;

        case 1:
            Show_File_Details(*std::get<unique_ptr<Parsed_File>>(parsed_content), arguments);
            break;
    }

    return 0;
}

