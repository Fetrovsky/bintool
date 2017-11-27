#include "elf-dumper.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <elf/elf.h>


template<typename T>
string Integer_As_Decimal_String(T value)
{
    std::ostringstream stream;

    stream << std::dec << value;

    return stream.str();
}

template<typename T>
string Integer_As_Hexadecimal_String(T value)
{
    std::ostringstream stream;

    stream << std::hex << value;

    return stream.str();
}

template<typename Element>
void Prepare_Table(
        array_view<Element const> table,
        std::vector<std::vector<string>>& matrix,
        std::vector<string> const& field_names,
        std::function<std::vector<string>(int, Element const&)> transform)
{
    matrix.clear();
    matrix.reserve(table.size());

    size_t field_count = field_names.size();

    matrix.push_back(field_names);
    matrix.push_back(std::vector<string>(field_count));

    for (size_t i = 0; i < field_count; ++i)
        matrix[1][i].assign(matrix[0][i].length(), '-');

    int index = 0;

    for (auto const& entry: table)
        matrix.push_back(std::move(transform(index++, entry)));
}

void Print_Table(std::vector<std::vector<string>> const& table)
{
    std::vector<size_t> field_lengths(table[0].size(), 0);

    for (auto const& entry: table)
        for (int i = 0; i < field_lengths.size(); ++i)
            field_lengths[i] = std::max(field_lengths[i], entry[i].length() + 1);

    for (auto const& entry: table)
    {
        for (int i = 0; i < field_lengths.size(); ++i)
            std::cout << std::setw(field_lengths[i]) << entry[i];

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void Show_ELF_File_Details(ELF64::ELF64 const& elf)
{
    auto elf_header = elf.Get_Header();

    std::cout
        << "ELF header values:\n"
        << "  Machine: " << elf_header.Machine << "\n"
        << "  Version: " << elf_header.Version << "\n"
        << "  Entry_Point: " << elf_header.Entry_Point << "\n"
        << "  Program_Header_Offset: " << elf_header.Program_Header_Offset << "\n"
        << "  Section_Header_Offset: " << elf_header.Section_Header_Offset << "\n"
        << "  Flags: " << elf_header.Flags << "\n"
        << "  ELF_Header_Size: " << elf_header.ELF_Header_Size << "\n"
        << "  Program_Header_Entry_Size: " << elf_header.Program_Header_Entry_Size << "\n"
        << "  Program_Header_Entry_Count: " << elf_header.Program_Header_Entry_Count << "\n"
        << "  Section_Header_Entry_Size: " << elf_header.Section_Header_Entry_Size << "\n"
        << "  Section_Header_Entry_Count: " << elf_header.Section_Header_Entry_Count << "\n"
        << std::endl;

    std::vector<std::vector<string>> table;

    Prepare_Table<ELF64::Program_Header_Entry>(
        elf.Get_Program_Header_Table(),
        table,
        std::vector<string> {
            "Index",
            "Type",
            "Flags",
            "Segment_Offset",
            "Virtual_Address",
            "Physical_Address",
            "Size_In_File",
            "Size_In_Memory",
            "Alignment"
        },
        [](int index, ELF64::Program_Header_Entry const& entry) -> std::vector<string>
        {
            return std::vector<string> {
                Integer_As_Decimal_String(index),
                Integer_As_Hexadecimal_String(entry.Type),
                Integer_As_Hexadecimal_String(entry.Flags),
                Integer_As_Hexadecimal_String(entry.Segment_Offset),
                Integer_As_Hexadecimal_String(entry.Virtual_Address),
                Integer_As_Hexadecimal_String(entry.Physical_Address),
                Integer_As_Hexadecimal_String(entry.Size_In_File),
                Integer_As_Hexadecimal_String(entry.Size_In_Memory),
                Integer_As_Hexadecimal_String(entry.Alignment)
            };
        });

    Print_Table(table);

    Prepare_Table<ELF64::Program_Header_Entry>(
        elf.Get_Program_Header_Table(),
        table,
        std::vector<string> {
            "Index",
            "Type",
            "Flags",
            "Segment_Offset",
            "Virtual_Address",
            "Physical_Address",
            "Size_In_File",
            "Size_In_Memory",
            "Alignment"
        },
        [](int index, ELF64::Program_Header_Entry const& entry) -> std::vector<string>
        {
            return std::vector<string> {
                Integer_As_Decimal_String(index),
                string(ELF64::Get_Segment_Type_Name(entry.Type)),
                string(ELF64::Get_Segment_Flag_Names(entry.Flags)),
                Integer_As_Hexadecimal_String(entry.Segment_Offset),
                Integer_As_Hexadecimal_String(entry.Virtual_Address),
                Integer_As_Hexadecimal_String(entry.Physical_Address),
                Integer_As_Hexadecimal_String(entry.Size_In_File),
                Integer_As_Hexadecimal_String(entry.Size_In_Memory),
                Integer_As_Hexadecimal_String(entry.Alignment)
            };
        });

    Print_Table(table);

    Prepare_Table<ELF64::Section_Header_Entry>(
        elf.Get_Section_Header_Table(),
        table,
        std::vector<string> {
            "Index",
            "Name",
            "Type",
            "Flags",
            "Virtual_Address",
            "Segment_Offset",
            "Size",
            "Link",
            "Info",
            "Address_Alignment",
            "Entry_Size"
        },
        [](int index, ELF64::Section_Header_Entry const& entry) -> std::vector<string>
        {
            return std::vector<string> {
                Integer_As_Decimal_String(index++),
                Integer_As_Decimal_String(entry.Name),
                Integer_As_Hexadecimal_String(entry.Type),
                Integer_As_Hexadecimal_String(entry.Flags),
                Integer_As_Hexadecimal_String(entry.Virtual_Address),
                Integer_As_Hexadecimal_String(entry.Segment_Offset),
                Integer_As_Decimal_String(entry.Size),
                Integer_As_Decimal_String(entry.Link),
                Integer_As_Decimal_String(entry.Info),
                Integer_As_Hexadecimal_String(entry.Address_Alignment),
                Integer_As_Decimal_String(entry.Entry_Size)
            };
        });

    Print_Table(table);

    Prepare_Table<ELF64::Section_Header_Entry>(
        elf.Get_Section_Header_Table(),
        table,
        std::vector<string> {
            "Index",
            "Name",
            "Type",
            "Flags",
            "Virtual_Address",
            "Segment_Offset",
            "Size",
            "Link",
            "Info",
            "Address_Alignment",
            "Entry_Size"
        },
        [](int index, ELF64::Section_Header_Entry const& entry) -> std::vector<string>
        {
            return std::vector<string> {
                Integer_As_Decimal_String(index++),
                Integer_As_Decimal_String(entry.Name),
                string(ELF64::Get_Section_Type_Name(entry.Type)),
                string(ELF64::Get_Section_Flag_Names(entry.Flags)),
                Integer_As_Hexadecimal_String(entry.Virtual_Address),
                Integer_As_Hexadecimal_String(entry.Segment_Offset),
                Integer_As_Decimal_String(entry.Size),
                Integer_As_Decimal_String(entry.Link),
                Integer_As_Decimal_String(entry.Info),
                Integer_As_Hexadecimal_String(entry.Address_Alignment),
                Integer_As_Decimal_String(entry.Entry_Size)
            };
        });

    Print_Table(table);
}

void Show_ELF_File_Details(ELF const& elf)
{
    auto const format_name = Get_File_Format_Name(elf.Get_File_Format());

    std::cout << "This is a file of type " << format_name << "." << std::endl;

    if (elf.Is_ELF64())
        Show_ELF_File_Details(static_cast<ELF64::ELF64 const&>(elf));
}

