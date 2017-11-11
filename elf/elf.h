#ifndef ELF_H__INCLUDED
#define ELF_H__INCLUDED

#include <cstdint>

#include <string_view>

#include "include/file-format.h"

#include <iostream>

class ELF
{
    private:
    protected:
        ELF() {}

    public:
        static ELF* Parse(std::string_view buffer);

        virtual File_Format Get_File_Format() const = 0;

        virtual bool Is_ELF64() const { return false; }

        virtual ~ELF() {}
};

namespace ELF64
{
    using Address   = uint64_t;
    using Offset    = uint64_t;
    using Byte      = uint8_t;
    using Half      = uint16_t;
    using Word      = uint32_t;
    using Sword     = int32_t;
    using Xword     = uint64_t;
    using Sxword    = int64_t;

    struct __attribute__((packed)) Header
    {
        uint8_t Ident[16];
        Half    Type;
        Half    Machine;
        Word    Version;
        Address Entry;
        Offset  Program_Header_Offset;
        Offset  Section_Header_Offset;
        Word    Flags;
        Half    ELF_Header_Size;
        Half    Program_Header_Entry_Size;
        Half    Program_Header_Entry_Count;
        Half    Section_Header_Entry_Size;
        Half    Section_Header_Entry_Count;
    };

    struct __attribute__((packed)) ELF_Identification
    {
        uint8_t File_Identification[4];
        uint8_t File_Class;
        uint8_t Data_Encoding;
        uint8_t File_Version;
        uint8_t OS_ABI;
        uint8_t ABI_Version;
        uint8_t Padding_Start;
        uint8_t Padding[];
    };

    enum class File_Type
    {
        None            =      0,
        Relocatable     =      1,
        Executable      =      2,
        Dynamic         =      3,
        Core            =      4,
        LO_OS           = 0xfe00,
        HI_OS           = 0xfeff,
        LO_Processor    = 0xff00,
        HI_Proessor     = 0xffff
    };

    class ELF64: public ELF
    {
        private:
            Header _header;

        protected:

        public:
            ELF64(Header header):
                ELF(),
                _header(header)
            {}

            File_Format Get_File_Format() const override
            {
                switch (Get_File_Type())
                {
                    case File_Type::Relocatable:    return File_Format::ELF64_Object;
                    case File_Type::Executable:     return File_Format::ELF64_Executable;
                    case File_Type::Dynamic:        return File_Format::ELF64_Shared_Object;
                    case File_Type::Core:           return File_Format::ELF64_Core_Dump;

                    case File_Type::None:
                    default:
                        return File_Format::Unknown;
                }
            }

            File_Type Get_File_Type() const
            {
                return File_Type(_header.Type);
            }

            bool Is_ELF64() const override { return true; }

            ~ELF64() override {}
    };
}

#endif  // ELF_H__INCLUDED

