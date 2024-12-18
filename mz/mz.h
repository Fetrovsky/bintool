#ifndef MZ_H__INCLUDED
#define MZ_H__INCLUDED

#include "include/enum.h"
#include "include/file-format.h"

#include <map>
#include <string_view>
#include <variant>
#include <vector>

using std::string_view;

class MZ: public Parsed_File
{
    public:
        struct COFF_Header;
        struct Optional_Header;
        struct Optional_Header_Plus;
        struct Image_Data_Directories;
        struct Section_Header;
        struct Import_Directory_Table_Entry;
        struct Import_Lookup_Table_Entry;
        struct Hint_Name_Table_Entry;

        enum class Machine_Type: uint16_t;
        enum class Image_Subsystem: uint16_t;
        enum class Image_File_Characteristics: uint16_t;

        enum class Magic_Number: uint16_t;
        enum class Image_DLL_Characteristics: uint16_t;

        enum class Section_Characteristics: uint32_t;

    protected:
        using Parsed_File::Parsed_File;

        Optional_Header get_optional_header() const;
        Optional_Header_Plus get_optional_header_plus() const;

    public:
        static MZ* Parse(std::string_view buffer);

        virtual ~MZ() {}

        virtual File_Format Get_File_Format() const;

        std::string_view Get_String(uint32_t RVA) const;

        COFF_Header const& Get_Header() const;
        std::variant<std::nullptr_t, Optional_Header, Optional_Header_Plus> Get_Optional_Header() const;

        uint32_t Get_COFF_Header_Address() const;
        uint32_t Get_COFF_Header_Size() const;

        uint64_t Get_Optional_Header_Address() const;
        uint32_t Get_Optional_Header_Size() const;

        uint16_t Get_Number_of_Sections() const;
        uint64_t Get_Section_Table_Address() const;
        Section_Header const& Get_Section_Header(uint16_t i) const;
        std::string_view Get_Section_Name(Section_Header const& sh) const;

        Import_Directory_Table_Entry const* Get_Import_Table() const;
        Import_Lookup_Table_Entry const* Get_Import_Lookup_Table(uint32_t RVA) const;
        Hint_Name_Table_Entry const* Get_Hint_Name_Table_Entry(uint32_t RVA) const;

        uint64_t Resolve_RVA(uint64_t rva) const;
};

struct __attribute__((packed)) MZ::COFF_Header
{
    uint32_t Signature;
    Machine_Type Machine;
    uint16_t Number_Of_Sections;
    uint32_t Time_Date_Stamp;
    uint32_t Pointer_to_Symbol_Table;
    uint32_t Number_Of_Symbols;
    uint16_t Size_Of_Optional_Header;
    Image_File_Characteristics Characteristics;
};

enum class MZ::Machine_Type: uint16_t
{
    UNKNOWN      =    0x0,
    AM33         =  0x1d3,
    AMD64        = 0x8664,
    ARM          =  0x1c0,
    ARM64        = 0xaa64,
    ARMNT        =  0x1c4,
    EBC          =  0xebc,
    I386         =  0x14c,
    IA64         =  0x200,
    LOONGARCH32  = 0x6232,
    LOONGARCH64  = 0x6264,
    M32R         = 0x9041,
    MIPS16       =  0x266,
    MIPSFPU      =  0x366,
    MIPSFPU16    =  0x466,
    POWERPC      =  0x1f0,
    POWERPCFP    =  0x1f1,
    R4000        =  0x166,
    RISCV32      = 0x5032,
    RISCV64      = 0x5064,
    RISCV128     = 0x5128,
    SH3          =  0x1a2,
    SH3DSP       =  0x1a3,
    SH4          =  0x1a6,
    SH5          =  0x1a8,
    THUMB        =  0x1c2,
    WCEMIPSV2    =  0x169
};

static inline string_view Get_Machine_Type_Name(MZ::Machine_Type mt)
{
    static std::map<MZ::Machine_Type, string_view> enum_map = {
        {MZ::Machine_Type::UNKNOWN, "The content of this field is assumed to be applicable to any machine type"sv},
        {MZ::Machine_Type::AM33, "Matsushita AM33"sv},
        {MZ::Machine_Type::AMD64, "x64"sv},
        {MZ::Machine_Type::ARM, "ARM little endian"sv},
        {MZ::Machine_Type::ARM64, "ARM64 little endian"sv},
        {MZ::Machine_Type::ARMNT, "ARM Thumb-2 little endian"sv},
        {MZ::Machine_Type::EBC, "EFI byte code"sv},
        {MZ::Machine_Type::I386, "Intel 386 or later processors and compatible processors"sv},
        {MZ::Machine_Type::IA64, "Intel Itanium processor family"sv},
        {MZ::Machine_Type::LOONGARCH32, "LoongArch 32-bit processor family"sv},
        {MZ::Machine_Type::LOONGARCH64, "LoongArch 64-bit processor family"sv},
        {MZ::Machine_Type::M32R, "Mitsubishi M32R little endian"sv},
        {MZ::Machine_Type::MIPS16, "MIPS16"sv},
        {MZ::Machine_Type::MIPSFPU, "MIPS with FPU"sv},
        {MZ::Machine_Type::MIPSFPU16, "MIPS16 with FPU"sv},
        {MZ::Machine_Type::POWERPC, "Power PC little endian"sv},
        {MZ::Machine_Type::POWERPCFP, "Power PC with floating point support"sv},
        {MZ::Machine_Type::R4000, "MIPS little endian"sv},
        {MZ::Machine_Type::RISCV32, "RISC-V 32-bit address space"sv},
        {MZ::Machine_Type::RISCV64, "RISC-V 64-bit address space"sv},
        {MZ::Machine_Type::RISCV128, "RISC-V 128-bit address space"sv},
        {MZ::Machine_Type::SH3, "Hitachi SH3"sv},
        {MZ::Machine_Type::SH3DSP, "Hitachi SH3 DSP"sv},
        {MZ::Machine_Type::SH4, "Hitachi SH4"sv},
        {MZ::Machine_Type::SH5, "Hitachi SH5"sv},
        {MZ::Machine_Type::THUMB, "Thumb"sv},
        {MZ::Machine_Type::WCEMIPSV2, "MIPS little-endian WCE v2"sv}
    };

    return enum_map[MZ::Machine_Type{mt}];
}

enum class MZ::Image_File_Characteristics: uint16_t
{
    RELOCS_STRIPPED = 0x0001,
    EXECUTABLE_IMAGE = 0x0002,
    LINE_NUMS_STRIPPED = 0x0004,
    LOCAL_SYMS_STRIPPED = 0x0008,
    AGGRESSIVE_WS_TRIM = 0x0010,
    LARGE_ADDRESS_AWARE = 0x0020,
    MBZ_40 = 0x0040,
    BYTES_REVERSED_LO = 0x0080,
    _32BIT_MACHINE = 0x0100,
    DEBUG_STRIPPED = 0x0200,
    REMOVABLE_RUN_FROM_SWAP = 0x0400,
    NET_RUN_FROM_SWAP = 0x0800,
    SYSTEM = 0x1000,
    DLL = 0x2000,
    UP_SYSTEM_ONLY = 0x4000,
    BYTES_REVERSED_HI = 0x8000,
};

ENABLE_BITWISE_OPERATIONS(MZ::Image_File_Characteristics);

static inline string_view Get_Image_File_Characteristics_Name(MZ::Image_File_Characteristics ifc)
{
    static std::map<MZ::Image_File_Characteristics, string_view> enum_map = {
        {MZ::Image_File_Characteristics::RELOCS_STRIPPED, "Image only, Windows CE, and Microsoft Windows NT and later. This indicates that the file does not contain base relocations and must therefore be loaded at its preferred base address. If the base address is not available, the loader reports an error. The default behavior of the linker is to strip base relocations from executable (EXE) files."},
        {MZ::Image_File_Characteristics::EXECUTABLE_IMAGE, "Image only. This indicates that the image file is valid and can be run. If this flag is not set, it indicates a linker error."},
        {MZ::Image_File_Characteristics::LINE_NUMS_STRIPPED, "COFF line numbers have been removed. This flag is deprecated and should be zero."},
        {MZ::Image_File_Characteristics::LOCAL_SYMS_STRIPPED, "COFF symbol table entries for local symbols have been removed. This flag is deprecated and should be zero."},
        {MZ::Image_File_Characteristics::AGGRESSIVE_WS_TRIM, "Obsolete. Aggressively trim working set. This flag is deprecated for Windows 2000 and later and must be zero."},
        {MZ::Image_File_Characteristics::LARGE_ADDRESS_AWARE, "Application can handle > 2-GB addresses."},
        {MZ::Image_File_Characteristics::MBZ_40, "This flag is reserved for future use."},
        {MZ::Image_File_Characteristics::BYTES_REVERSED_LO, "Little endian: the least significant bit (LSB) precedes the most significant bit (MSB) in memory. This flag is deprecated and should be zero."},
        {MZ::Image_File_Characteristics::_32BIT_MACHINE, "Machine is based on a 32-bit-word architecture."},
        {MZ::Image_File_Characteristics::DEBUG_STRIPPED, "Debugging information is removed from the image file."},
        {MZ::Image_File_Characteristics::REMOVABLE_RUN_FROM_SWAP, "If the image is on removable media, fully load it and copy it to the swap file."},
        {MZ::Image_File_Characteristics::NET_RUN_FROM_SWAP, "If the image is on network media, fully load it and copy it to the swap file."},
        {MZ::Image_File_Characteristics::SYSTEM, "The image file is a system file, not a user program."},
        {MZ::Image_File_Characteristics::DLL, "The image file is a dynamic-link library (DLL). Such files are considered executable files for almost all purposes, although they cannot be directly run."},
        {MZ::Image_File_Characteristics::UP_SYSTEM_ONLY, "The file should be run only on a uniprocessor machine."},
        {MZ::Image_File_Characteristics::BYTES_REVERSED_HI, "Big endian: the MSB precedes the LSB in memory. This flag is deprecated and should be zero."},
    };

    return enum_map[MZ::Image_File_Characteristics{ifc}];
}

static inline constexpr auto Get_Image_File_Characteristics_Names = [](MZ::Image_File_Characteristics ifc)
{
    return Get_Enum_Names<MZ::Image_File_Characteristics>(ifc, &Get_Image_File_Characteristics_Name);
};

enum class MZ::Magic_Number: uint16_t
{
    PE32      =  0x10b,
    PE32_PLUS =  0x20b,
};

static inline string_view Get_Magic_Number_Name(MZ::Magic_Number mn)
{
    using Magic_Number = MZ::Magic_Number;

    static std::map<Magic_Number, string_view> enum_map = {
        {Magic_Number::PE32, "PE32"sv},
        {Magic_Number::PE32_PLUS, "PE32+"sv}
    };

    return enum_map[Magic_Number{mn}];
}

enum class MZ::Image_Subsystem: uint16_t
{
    UNKNOWN = 0,
    NATIVE = 1,
    WINDOWS_GUI = 2,
    WINDOWS_CUI = 3,
    OS2_CUI = 5,
    POSIX_CUI = 7,
    NATIVE_WINDOWS = 8,
    WINDOWS_CE_GUI = 9,
    EFI_APPLICATION = 10,
    EFI_BOOT_SERVICE_DRIVER = 11,
    EFI_RUNTIME_DRIVER = 12,
    EFI_ROM = 13,
    XBOX = 14,
    WINDOWS_BOOT_APPLICATION = 16
};

static inline string_view Get_Subsystem_Name(MZ::Image_Subsystem s)
{
    static std::map<MZ::Image_Subsystem, string_view> enum_map = {
        {MZ::Image_Subsystem::UNKNOWN, "An unknown subsystem"},
        {MZ::Image_Subsystem::NATIVE, "Device drivers and native Windows processes"},
        {MZ::Image_Subsystem::WINDOWS_GUI, "The Windows graphical user interface (GUI) subsystem"},
        {MZ::Image_Subsystem::WINDOWS_CUI, "The Windows character subsystem"},
        {MZ::Image_Subsystem::OS2_CUI, "The OS/2 character subsystem"},
        {MZ::Image_Subsystem::POSIX_CUI, "The Posix character subsystem"},
        {MZ::Image_Subsystem::NATIVE_WINDOWS, "Native Win9x driver"},
        {MZ::Image_Subsystem::WINDOWS_CE_GUI, "Windows CE"},
        {MZ::Image_Subsystem::EFI_APPLICATION, "An Extensible Firmware Interface (EFI) application"},
        {MZ::Image_Subsystem::EFI_BOOT_SERVICE_DRIVER, "An EFI driver with boot services"},
        {MZ::Image_Subsystem::EFI_RUNTIME_DRIVER, "An EFI driver with run-time services"},
        {MZ::Image_Subsystem::EFI_ROM, "An EFI ROM image"},
        {MZ::Image_Subsystem::XBOX, "XBOX"},
        {MZ::Image_Subsystem::WINDOWS_BOOT_APPLICATION, "Windows boot application."}
    };

    return enum_map[MZ::Image_Subsystem{s}];
}

enum class MZ::Image_DLL_Characteristics: uint16_t
{
    MBZ_1 = 0x0001,
    MBZ_2 = 0x0002,
    MBZ_4 = 0x0004,
    MBZ_8 = 0x0008,
    HIGH_ENTROPY_VA = 0x0020,
    DYNAMIC_BASE = 0x0040,
    FORCE_INTEGRITY = 0x0080,
    NX_COMPAT = 0x0100,
    NO_ISOLATION = 0x0200,
    NO_SEH = 0x0400,
    NO_BIND = 0x0800,
    APPCONTAINER = 0x1000,
    WDM_DRIVER = 0x2000,
    GUARD_CF = 0x4000,
    TERMINAL_SERVER_AWARE = 0x8000
};

ENABLE_BITWISE_OPERATIONS(MZ::Image_DLL_Characteristics);

static inline string_view Get_Image_DLL_Characteristics_Name(MZ::Image_DLL_Characteristics idc)
{
    static std::map<MZ::Image_DLL_Characteristics, string_view> enum_map = {
        {MZ::Image_DLL_Characteristics::MBZ_1, "Reserved, must be zero."},
        {MZ::Image_DLL_Characteristics::MBZ_2, "Reserved, must be zero."},
        {MZ::Image_DLL_Characteristics::MBZ_4, "Reserved, must be zero."},
        {MZ::Image_DLL_Characteristics::MBZ_8, "Reserved, must be zero."},
        {MZ::Image_DLL_Characteristics::HIGH_ENTROPY_VA, "Image can handle a high entropy 64-bit virtual address space."},
        {MZ::Image_DLL_Characteristics::DYNAMIC_BASE, "DLL can be relocated at load time."},
        {MZ::Image_DLL_Characteristics::FORCE_INTEGRITY, "Code Integrity checks are enforced."},
        {MZ::Image_DLL_Characteristics::NX_COMPAT, "Image is NX compatible."},
        {MZ::Image_DLL_Characteristics::NO_ISOLATION, "Isolation aware, but do not isolate the image."},
        {MZ::Image_DLL_Characteristics::NO_SEH, "Does not use structured exception (SE) handling. No SE handler may be called in this image."},
        {MZ::Image_DLL_Characteristics::NO_BIND, "Do not bind the image."},
        {MZ::Image_DLL_Characteristics::APPCONTAINER, "Image must execute in an AppContainer."},
        {MZ::Image_DLL_Characteristics::WDM_DRIVER, "A WDM driver."},
        {MZ::Image_DLL_Characteristics::GUARD_CF, "Image supports Control Flow Guard."},
        {MZ::Image_DLL_Characteristics::TERMINAL_SERVER_AWARE, "Terminal Server aware."}
    };

    return enum_map[MZ::Image_DLL_Characteristics{idc}];
}

static inline constexpr auto Get_Image_DLL_Characteristics_Names = [](MZ::Image_DLL_Characteristics ifc)
{
    return Get_Enum_Names<MZ::Image_DLL_Characteristics>(ifc, &Get_Image_DLL_Characteristics_Name);
};

//
//
// Offset:
//      PE: 96
//      PE+: 112
//
struct __attribute__((packed)) MZ::Image_Data_Directories {
    struct Entry {
        uint64_t Virtual_Address:32;
        uint64_t Size:32;
    };

    Entry Export_Table;
    Entry Import_Table;
    Entry Resource_Table;
    Entry Exception_Table;
    Entry Certificate_Table;
    Entry Base_Relocation_Table;
    Entry Debug;
    Entry Architecture;
    Entry Global_Ptr;
    Entry TLS_Table;
    Entry Load_Config_Table;
    Entry Bound_Import;
    Entry IAT;
    Entry Delay_Import_Descriptor;
    Entry CLR_Runtime_Header;
    Entry Reserved_MBZ;
};

struct __attribute__((packed)) MZ::Optional_Header
{
    Magic_Number Magic;
    uint8_t Major_Linker_Version;
    uint8_t Minor_Linker_Version;
    uint32_t Size_Of_Code;
    uint32_t Size_Of_Initialized_Data;
    uint32_t Size_Of_Uninitialized_Data;
    uint32_t Address_Of_Entry_Point;
    uint32_t Base_Of_Code;
    uint32_t Base_Of_Data;
    uint32_t Image_Base;
    uint32_t Section_Alignment;
    uint32_t File_Alignment;
    uint16_t Major_Operating_System_Version;
    uint16_t Minor_Operating_System_Version;
    uint16_t Major_Image_Version;
    uint16_t Minor_Image_Version;
    uint16_t Major_Subsystem_Version;
    uint16_t Minor_Subsystem_Version;
    uint32_t Win32_Version_Value;
    uint32_t Size_Of_Image;
    uint32_t Size_Of_Headers;
    uint32_t Check_Sum;
    Image_Subsystem Subsystem;
    Image_DLL_Characteristics Dll_Characteristics;
    uint32_t Size_Of_Stack_Reserve;
    uint32_t Size_Of_Stack_Commit;
    uint32_t Size_Of_Heap_Reserve;
    uint32_t Size_Of_Heap_Commit;
    uint32_t Loader_Flags;
    uint32_t Number_Of_Rva_And_Sizes;
    Image_Data_Directories Image_Data_Directories;
};  // namespace Optional_Header

struct __attribute__((packed)) MZ::Optional_Header_Plus
{
    Magic_Number Magic;
    uint8_t Major_Linker_Version;
    uint8_t Minor_Linker_Version;
    uint32_t Size_Of_Code;
    uint32_t Size_Of_Initialized_Data;
    uint32_t Size_Of_Uninitialized_Data;
    uint32_t Address_Of_Entry_Point;
    uint32_t Base_Of_Code;
    uint64_t Image_Base;
    uint32_t Section_Alignment;
    uint32_t File_Alignment;
    uint16_t Major_Operating_System_Version;
    uint16_t Minor_Operating_System_Version;
    uint16_t Major_Image_Version;
    uint16_t Minor_Image_Version;
    uint16_t Major_Subsystem_Version;
    uint16_t Minor_Subsystem_Version;
    uint32_t Win32_Version_Value;
    uint32_t Size_Of_Image;
    uint32_t Size_Of_Headers;
    uint32_t Check_Sum;
    Image_Subsystem  Subsystem;
    Image_DLL_Characteristics Dll_Characteristics;
    uint64_t Size_Of_Stack_Reserve;
    uint64_t Size_Of_Stack_Commit;
    uint64_t Size_Of_Heap_Reserve;
    uint64_t Size_Of_Heap_Commit;
    uint32_t Loader_Flags;
    uint32_t Number_Of_Rva_And_Sizes;
    Image_Data_Directories Image_Data_Directories;
};  // namespace Optional_Header

struct __attribute__((packed)) MZ::Section_Header
{
    char Name[8];
    uint32_t Virtual_Size;
    uint32_t Virtual_Address;
    uint32_t Size_Of_Raw_Data;
    uint32_t Pointer_To_Raw_Data;
    uint32_t Pointer_To_Relocations;
    uint32_t Pointer_To_Linenumbers;
    uint16_t Number_Of_Relocations;
    uint16_t Number_Of_Linenumbers;
    Section_Characteristics Characteristics;
};

enum class MZ::Section_Characteristics: uint32_t
{
    RESERVED_0 = 0x00000000,
    RESERVED_1 = 0x00000001,
    RESERVED_2 = 0x00000002,
    RESERVED_4 = 0x00000004,
    IMAGE_SCN_TYPE_NO_PAD = 0x00000008,
    RESERVED_10 = 0x00000010,
    IMAGE_SCN_CNT_CODE = 0x00000020,
    IMAGE_SCN_CNT_INITIALIZED_DATA = 0x00000040,
    IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x00000080,
    IMAGE_SCN_LNK_OTHER = 0x00000100,
    IMAGE_SCN_LNK_INFO = 0x00000200,
    RESERVED_400 = 0x00000400,
    IMAGE_SCN_LNK_REMOVE = 0x00000800,
    IMAGE_SCN_LNK_COMDAT = 0x00001000,
    IMAGE_SCN_GPREL = 0x00008000,
    IMAGE_SCN_MEM_PURGEABLE = 0x00020000,
    IMAGE_SCN_MEM_16BIT = 0x00020000,
    IMAGE_SCN_MEM_LOCKED = 0x00040000,
    IMAGE_SCN_MEM_PRELOAD = 0x00080000,
    IMAGE_SCN_ALIGN_1BYTES = 0x00100000,
    IMAGE_SCN_ALIGN_2BYTES = 0x00200000,
    IMAGE_SCN_ALIGN_4BYTES = 0x00300000,
    IMAGE_SCN_ALIGN_8BYTES = 0x00400000,
    IMAGE_SCN_ALIGN_16BYTES = 0x00500000,
    IMAGE_SCN_ALIGN_32BYTES = 0x00600000,
    IMAGE_SCN_ALIGN_64BYTES = 0x00700000,
    IMAGE_SCN_ALIGN_128BYTES = 0x00800000,
    IMAGE_SCN_ALIGN_256BYTES = 0x00900000,
    IMAGE_SCN_ALIGN_512BYTES = 0x00A00000,
    IMAGE_SCN_ALIGN_1024BYTES = 0x00B00000,
    IMAGE_SCN_ALIGN_2048BYTES = 0x00C00000,
    IMAGE_SCN_ALIGN_4096BYTES = 0x00D00000,
    IMAGE_SCN_ALIGN_8192BYTES = 0x00E00000,
    IMAGE_SCN_LNK_NRELOC_OVFL = 0x01000000,
    IMAGE_SCN_MEM_DISCARDABLE = 0x02000000,
    IMAGE_SCN_MEM_NOT_CACHED = 0x04000000,
    IMAGE_SCN_MEM_NOT_PAGED = 0x08000000,
    IMAGE_SCN_MEM_SHARED = 0x10000000,
    IMAGE_SCN_MEM_EXECUTE = 0x20000000,
    IMAGE_SCN_MEM_READ = 0x40000000,
    IMAGE_SCN_MEM_WRITE = 0x80000000
};

ENABLE_BITWISE_OPERATIONS(MZ::Section_Characteristics);

static inline string_view Get_Section_Characteristics_Name(MZ::Section_Characteristics sc)
{
    static std::map<MZ::Section_Characteristics, string_view> enum_map = {
        {MZ::Section_Characteristics::RESERVED_0, "Reserved for future use."},
        {MZ::Section_Characteristics::RESERVED_1, "Reserved for future use."},
        {MZ::Section_Characteristics::RESERVED_2, "Reserved for future use."},
        {MZ::Section_Characteristics::RESERVED_4, "Reserved for future use."},
        {MZ::Section_Characteristics::IMAGE_SCN_TYPE_NO_PAD, "The section should not be padded to the next boundary. This flag is obsolete and is replaced by IMAGE_SCN_ALIGN_1BYTES. This is valid only for object files."},
        {MZ::Section_Characteristics::RESERVED_10, "Reserved for future use."},
        {MZ::Section_Characteristics::IMAGE_SCN_CNT_CODE, "The section contains executable code."},
        {MZ::Section_Characteristics::IMAGE_SCN_CNT_INITIALIZED_DATA, "The section contains initialized data."},
        {MZ::Section_Characteristics::IMAGE_SCN_CNT_UNINITIALIZED_DATA, "The section contains uninitialized data."},
        {MZ::Section_Characteristics::IMAGE_SCN_LNK_OTHER, "Reserved for future use."},
        {MZ::Section_Characteristics::IMAGE_SCN_LNK_INFO, "The section contains comments or other information. The .drectve section has this type. This is valid for object files only."},
        {MZ::Section_Characteristics::RESERVED_400, "Reserved for future use."},
        {MZ::Section_Characteristics::IMAGE_SCN_LNK_REMOVE, "The section will not become part of the image. This is valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_LNK_COMDAT, "The section contains COMDAT data. For more information, see COMDAT Sections (Object Only). This is valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_GPREL, "The section contains data referenced through the global pointer (GP)."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_PURGEABLE, "Reserved for future use."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_16BIT, "Reserved for future use."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_LOCKED, "Reserved for future use."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_PRELOAD, "Reserved for future use."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_1BYTES, "Align data on a 1-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_2BYTES, "Align data on a 2-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_4BYTES, "Align data on a 4-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_8BYTES, "Align data on an 8-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_16BYTES, "Align data on a 16-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_32BYTES, "Align data on a 32-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_64BYTES, "Align data on a 64-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_128BYTES, "Align data on a 128-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_256BYTES, "Align data on a 256-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_512BYTES, "Align data on a 512-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_1024BYTES, "Align data on a 1024-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_2048BYTES, "Align data on a 2048-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_4096BYTES, "Align data on a 4096-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_ALIGN_8192BYTES, "Align data on an 8192-byte boundary. Valid only for object files."},
        {MZ::Section_Characteristics::IMAGE_SCN_LNK_NRELOC_OVFL, "The section contains extended relocations."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_DISCARDABLE, "The section can be discarded as needed."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_NOT_CACHED, "The section cannot be cached."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_NOT_PAGED, "The section is not pageable."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_SHARED, "The section can be shared in memory."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_EXECUTE, "The section can be executed as code."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_READ, "The section can be read."},
        {MZ::Section_Characteristics::IMAGE_SCN_MEM_WRITE, "The section can be written to."}
    };

    return enum_map[MZ::Section_Characteristics{sc}];
}

static inline constexpr auto Get_Section_Characteristics_Names = [](MZ::Section_Characteristics sc)
{
    return Get_Enum_Names<MZ::Section_Characteristics>(sc, &Get_Section_Characteristics_Name);
};

struct __attribute__((packed)) MZ::Import_Directory_Table_Entry
{
    //
    //  The RVA of the import lookup table.
    //  This table contains a name or ordinal for each import.
    //  (The name "Characteristics" is used in Winnt.h, but no longer describes this field.)
    //
    uint32_t Import_Lookup_Table_RVA;

    //
    //  The stamp that is set to zero until the image is bound.
    // After the image is bound, this field is set to the time/data stamp of the DLL.
    //
    uint32_t Time_Date_Stamp;

    //
    //  The index of the first forwarder reference.
    //
    uint32_t Forwarder_Chain;

    //
    //  The address of an ASCII string that contains the name of the DLL. This address is relative to the image base.
    //
    uint32_t Name_RVA;

    //
    //  The RVA of the import address table.
    //  The contents of this table are identical to the contents of the import lookup table until the image is bound.
    //
    uint32_t Import_Address_Table_RVA;
};

struct __attribute((packed)) MZ::Import_Lookup_Table_Entry
{
    union {
        uint64_t As_Uint64;

        struct {
            //
            // A 31-bit RVA of a hint/name table entry.  This field is used only if the Ordinal/Name Flag
            // bit field is 0 (import by name).  For PE32+ bits 62-31 must be zero.
            //
            uint64_t Hint_Or_Name_Table_RVA:31;

            // Bits 62-31 must be zero.
            uint64_t Reserved_Name_MBZ: 32;

            uint64_t Name_Flag: 1;  // Must be zero.
        };

        struct {
            //
            // A 16-bit ordinal number.  This field is used only if the Ordinal/Name Flag bit field is 1 (import by ordinal).
            // Bits 30-15 or 62-15 must be 0.
            //
            uint64_t Ordinal_Number:16;

            // Bits 62-31 must be zero.
            uint64_t Reserved_Ordinal_MBZ: 47;

            uint64_t Ordinal_Flag: 1;  // Must be one.
        };
    };
};

struct __attribute((packed)) MZ::Hint_Name_Table_Entry
{
    //
    // An index into the export name pointer table.  A match is attempted first with this value.
    // If it fails, a binary search is performed on the DLL's export name pointer table.
    //
    uint16_t Hint;

    //
    // An ASCII string that contains the name to import.  This is the string that must be matched
    // to the public name in the DLL.  This string is case sensitive and terminated by a null byte.
    //
    char Name[];  // MUL-terminated.

    // A trailing zero-pad byte that appears after the trailing null byte, if necessary,
    // to align the next entry on an even boundary.
};

#endif  // MZ_H__INCLUDED

