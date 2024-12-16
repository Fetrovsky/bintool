#ifndef FILE_FORMAT__INCLUDED
#define FILE_FORMAT__INCLUDED

#include "include/interface.h"

#include <string_view>
#include <map>

#include "include/array-view.h"

using std::operator""sv;

enum class File_Format
{
    ELF_Executable,
    ELF_Object,
    ELF_Shared_Object,
    ELF64_Executable,
    ELF64_Object,
    ELF64_Shared_Object,
    ELF64_Core_Dump,
    AR_Arch,
    AR_BigAF,
    MZ_Executable,
    MZ_Object,
    MZ_DLL,
    MZ_Library,
    Unknown
};

inline std::string_view const& Get_File_Format_Name(File_Format value)
{
    static std::map<File_Format, std::string_view> const File_Formats = {
        { File_Format::ELF_Executable, "ELF_Executable"sv },
        { File_Format::ELF_Object, "ELF_Object"sv },
        { File_Format::ELF_Shared_Object, "ELF_Shared_Object"sv },
        { File_Format::ELF64_Executable, "ELF64_Executable"sv },
        { File_Format::ELF64_Object, "ELF64_Object"sv },
        { File_Format::ELF64_Shared_Object, "ELF64_Shared_Object"sv },
        { File_Format::ELF64_Core_Dump, "ELF64_Core_Dump"sv },
        { File_Format::AR_Arch, "AR_Arch"sv },
        { File_Format::AR_BigAF, "AR_BigAF"sv },
        { File_Format::MZ_Executable, "MZ_Executable"sv },
        { File_Format::MZ_Object, "MZ_Object"sv },
        { File_Format::MZ_DLL, "MZ_DLL"sv },
        { File_Format::MZ_Library, "MZ_Library"sv },
        { File_Format::Unknown, "Unknown"sv }
    };

    return File_Formats.at(value);
}

template<typename Target_Type, typename Item_Type>
inline Target_Type const& Parse_As(Item_Type const item_array[])
{
    return *reinterpret_cast<Target_Type const*>(&item_array[0]);
}

template<typename Item_Type, size_t N>
inline std::string_view Make_Magic(Item_Type (&item_array)[N])
{
    return std::string_view(reinterpret_cast<char const*>(item_array), N);
}

class Parsed_File: public Base_Class
{
    private:
        std::string_view _buffer;

    protected:
        Parsed_File(std::string_view buffer): _buffer{buffer} {}

    public:
        template<typename T>
        T const* As(uint32_t offset) const
        { return reinterpret_cast<T const*>(&buffer()[offset]); }

        template<typename T>
        T const& get_field(size_t offset) const
        { return *As<T>(offset); }

        std::string_view Get_String(uint32_t offset) const
        { return std::string_view(As<char>(offset)); }

        template<typename Entry>
        array_view<Entry> get_table(size_t offset, size_t entry_count)
        { return array_view<Entry>(&get_field<Entry>(offset), entry_count); }

        template<typename Entry>
        array_view<Entry const> get_table(size_t offset, size_t entry_count) const
        { return array_view<Entry const>(&get_field<Entry const>(offset), entry_count); }

        template<typename Header>
        Header const& get_header(uint32_t offset = 0) const
        { return get_field<Header const>(offset); }

//        template<typename Header>
//        Header const& get_header(uint32_t offset = 0)
//        { return get_field<Header>(offset); }

        virtual File_Format Get_File_Format() const = 0;

        virtual std::string_view buffer() const { return _buffer; }
};

#endif  // FILE_FORMAT__INCLUDED

