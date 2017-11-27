#include "elf.h"

#include <string_view>

#include <include/file-format.h>

#include <iostream>

using std::string_view;


ELF* ELF::Parse(string_view buffer)
{
    if (buffer.length() < sizeof(ELF64::Header))
        return nullptr;

    auto const& header = Parse_As<ELF64::Header>(&buffer[0]);
    auto const& elf_ident = Parse_As<ELF64::ELF_Identification>(header.Ident);

    if (Make_Magic(elf_ident.File_Identification) != "\x7f""ELF")
        return nullptr;

    return new ELF64::ELF64 { buffer };
}

