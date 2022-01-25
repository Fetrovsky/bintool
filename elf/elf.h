#ifndef ELF_H__INCLUDED
#define ELF_H__INCLUDED

#include <cstdint>
#include <string_view>
#include <sstream>

#include "include/file-format.h"
#include "include/range.h"

#include <iostream>

using std::string;
using std::string_view;

class ELF: public Parsed_File
{
    protected:
        using Parsed_File::Parsed_File;

    public:
        static ELF* Parse(std::string_view buffer);

        virtual File_Format Get_File_Format() const = 0;

        virtual bool Is_ELF64() const { return false; }

        virtual ~ELF() {}
};  // class ELF

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
        Address Entry_Point;
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
        HI_Processor    = 0xffff
    };

    template<typename T>
    bool Get_Enum_Name_From_Range(
            T range_low, T range_hi,
            string_view low_name,
            T value,
            string& value_name)
    {
        if (!Range<T>(range_low, range_hi + 1).Contains(value))
            return false;

        std::ostringstream name_stream;

        name_stream
                << low_name.substr(3)
                << "{" << int(value - range_low) << "}";

        value_name = std::move(name_stream.str());

        return true;
    }

    inline string_view Get_File_Type_Name(uint32_t value)
    {
        std::map<File_Type, string> enum_map = {
            { File_Type::None,           "None"          },
            { File_Type::Relocatable,    "Relocatable"   },
            { File_Type::Executable,     "Executable"    },
            { File_Type::Dynamic,        "Dynamic"       },
            { File_Type::Core,           "Core"          },
            { File_Type::LO_OS,          "LO_OS"         },
            { File_Type::HI_OS,          "HI_OS"         },
            { File_Type::LO_Processor,   "LO_Processor"  },
            { File_Type::HI_Processor,   "HI_Processor"  }
        };

        return enum_map[File_Type(value)];
    }

    struct __attribute__((packed)) Program_Header_Entry
    {
        Word Type;
        Word Flags;
        Offset Segment_Offset;
        Address Virtual_Address;
        Address Physical_Address;
        Xword Size_In_File;
        Xword Size_In_Memory;
        Xword Alignment;
    };

    enum class Segment_Type
    {
        Null            = 0,
        Load            = 1,
        Dynamic         = 2,
        Interpret       = 3,
        Note            = 4,
        Shared_Library  = 5,
        Program_Header  = 6,
        LO_OS           = 0x60000000,
        HI_OS           = 0x6fffffff,
        LO_Processor    = 0x70000000,
        HI_Processor    = 0x7fffffff
    };

    inline string_view Get_Segment_Type_Name(uint32_t value)
    {
        std::map<Segment_Type, string> enum_map = {
            { Segment_Type::Null,           "Null"           },
            { Segment_Type::Load,           "Load"           },
            { Segment_Type::Dynamic,        "Dynamic"        },
            { Segment_Type::Interpret,      "Interpret"      },
            { Segment_Type::Note,           "Note"           },
            { Segment_Type::Shared_Library, "Shared_Library" },
            { Segment_Type::Program_Header, "Program_Header" },
            { Segment_Type::LO_OS,          "LO_OS"          },
            { Segment_Type::HI_OS,          "HI_OS"          },
            { Segment_Type::LO_Processor,   "LO_Processor"   },
            { Segment_Type::HI_Processor,   "HI_Processor"   }
        };

        return enum_map[Segment_Type(value)];
    }

    enum class Segment_Flags
    {
        None            = 0,
        E               = 1,
        W               = 2,
        R               = 4,
        Mask_OS         = 0x00ff0000,
        Mask_Processor  = 0x00ff0000
    };

    inline string Get_Segment_Flag_Names(uint32_t value)
    {
        string result;

        if (value & uint32_t(Segment_Flags::E)) result.push_back('E');
        if (value & uint32_t(Segment_Flags::W)) result.push_back('W');
        if (value & uint32_t(Segment_Flags::R)) result.push_back('R');

        return result;
    }

    struct __attribute__((packed)) Section_Header_Entry
    {
        Word Name;
        Word Type;
        Xword Flags;
        Address Virtual_Address;
        Offset Segment_Offset;
        Xword Size;
        Word Link;
        Word Info;
        Xword Address_Alignment;
        Xword Entry_Size;
    };

    enum class Section_Type
    {
        Null                =          0,
        Program_Bits        =          1,
        Symbol_Table        =          2,
        String_Table        =          3,
        Rela_Relocatable    =          4,
        Hash_Table          =          5,
        Dynamic_Tables      =          6,
        Note                =          7,
        No_Bits             =          8,
        Rel_Relocatable     =          9,
        Reserved            =         10,
        LO_OS               = 0x60000000,
        HI_OS               = 0x6fffffff,
        LO_Processor        = 0x70000000,
        HI_Processor        = 0x7fffffff
    };

    inline string_view Get_Section_Type_Name(uint32_t value)
    {
        std::map<Section_Type, string> enum_map = {
            { Section_Type::Null, "Null" },
            { Section_Type::Program_Bits, "Program_Bits" },
            { Section_Type::Symbol_Table, "Symbol_Table" },
            { Section_Type::String_Table, "String_Table" },
            { Section_Type::Rela_Relocatable, "Rela_Relocatable" },
            { Section_Type::Hash_Table, "Hash_Table" },
            { Section_Type::Dynamic_Tables, "Dynamic_Tables" },
            { Section_Type::Note, "Note" },
            { Section_Type::No_Bits, "No_Bits" },
            { Section_Type::Rel_Relocatable, "Rel_Relocatable" },
            { Section_Type::Reserved, "Reserved" },
            { Section_Type::LO_OS, "LO_OS" },
            { Section_Type::HI_OS, "HI_OS" },
            { Section_Type::LO_Processor, "LO_Processor" },
            { Section_Type::HI_Processor, "HI_Processor" }
        };

        return enum_map[Section_Type(value)];
    }

    enum class Section_Flags: uint32_t
    {
        W           =          1,
        A           =          2,
        X           =          3,
        Mask_OS     = 0x0f000000,
        Mask_Proc   = 0xf0000000
    };

    inline string Get_Section_Flag_Names(uint32_t value)
    {
        string result;

        if (value & uint32_t(Section_Flags::W)) result.push_back('W');
        if (value & uint32_t(Section_Flags::A)) result.push_back('A');
        if (value & uint32_t(Section_Flags::X)) result.push_back('X');

        return result;
    }

    class ELF64: public ELF
    {
        private:
        protected:

        public:
            using ELF::ELF;

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

            bool Is_ELF64() const override { return true; }

            Header const& Get_Header() const
            { return get_header<Header>(); }

            File_Type Get_File_Type() const 
            { return File_Type(Get_Header().Type); }

            array_view<Program_Header_Entry const> Get_Program_Header_Table() const
            {
                return
                    get_table<Program_Header_Entry>(
                            Get_Header().Program_Header_Offset,
                            Get_Header().Program_Header_Entry_Count);
            }

            array_view<Section_Header_Entry const> Get_Section_Header_Table() const
            {
                return
                    get_table<Section_Header_Entry>(
                            Get_Header().Section_Header_Offset,
                            Get_Header().Section_Header_Entry_Count);
            }

            ~ELF64() override {}
    };  // class ELF64
}

#endif  // ELF_H__INCLUDED

