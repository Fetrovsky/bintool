#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <variant>
#include <vector>

#include "include/file-format.h"
#include "elf/elf.h"

#include "elf-dumper.h"


using std::nullptr_t;
using std::string;
using std::string_view;
using std::unique_ptr;

bool Get_Arguments(int argc, char* argv[], string& filename)
{
    if (argc != 2)
        return false;

    filename = argv[1];
    return true;
}

int Usage(string_view program_name)
{
    std::cout
        << "Usage: " << program_name << " <filename>"
        << std::endl;

    return 1;
}

File_Format Probe_MZ_File_Format(string_view file_contents)
{
    return File_Format::MZ_Executable;
}

std::variant<nullptr_t, unique_ptr<ELF>>
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
//
//    if (file_contents.substr(0, 2) == "MZ")
//        return Probe_MZ_File_Format(file_contents);

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

int main(int argc, char* argv[])
{
    string filename, contents;

    if (argc != 2)
        return Usage(argv[0]);

    filename = argv[1];

    if (!Read_File(filename, contents))
        return -2;

    std::cout
        << filename << ": " << contents.length() << " bytes."
        << std::endl;

    auto parsed_content = Parse(contents);

    if (std::holds_alternative<std::nullptr_t>(parsed_content))
    {
        std::cout << "Could not understand the format." << std::endl;
        return -3;
    }

    switch (parsed_content.index())
    {
        case 1: Show_ELF_File_Details(*std::get<unique_ptr<ELF>>(parsed_content)); break;
    }

    return 0;
}

