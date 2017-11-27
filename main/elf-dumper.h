#ifndef ELF_DUMPER_H__INCLUDED
#define ELF_DUMPER_H__INCLUDED

#include <elf/elf.h>

void Show_ELF_File_Details(ELF64::ELF64 const& elf);
void Show_ELF_File_Details(ELF const& elf);

#endif  // ELF_DUMPER_H__INCLUDED

