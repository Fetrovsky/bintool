#ifndef MZ_H__INCLUDED
#define MZ_H__INCLUDED

#include "include/file-format.h"

class MZ: public Parsed_File
{
    protected:
        using Parsed_File::Parsed_File;

    public:
        static MZ* Parse(std::string_view buffer)
        {
          return (buffer.substr(0, 2) == "MZ")?  new MZ{buffer}: nullptr;
        }

        virtual File_Format Get_File_Format() const
        {
            // File_Format::MZ_Executable
            // File_Format::MZ_Object
            // File_Format::MZ_DLL
            // File_Format::MZ_Library
            return File_Format::MZ_Executable;
        }

        virtual ~MZ() {}
};

#endif  // MZ_H__INCLUDED

