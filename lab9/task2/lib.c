#include "lib.h"
#include <stdio.h>

int get_shdr_index(const char *name, void *start){
        /* first, get the header */
        Elf32_Ehdr *header = (Elf32_Ehdr *)start;
        /* now, get a pointer to the first section header */
        Elf32_Shdr *sh_tbl = (Elf32_Shdr *) (start + header->e_shoff);
        /* and a pointer to the section header string table: */
        char *str_tbl;
        int i=1, found=0;
        
        /* first, check for a section header string table: */
        if (header->e_shnum < header->e_shstrndx ||
	    header->e_shstrndx == SHN_UNDEF){
                printf("no section header string table. sorry \n");
                return(-1);
        }
        str_tbl = (char *) (start + sh_tbl[header->e_shstrndx].sh_offset);

        /* now, go over all sections and find the one requested:  */
        for(i=0; i<header->e_shnum; i++){
                if (strcmp(name, str_tbl + sh_tbl[i].sh_name) == 0){
                        found = 1;
                        break;
                }
        }

        return found?i:(-1);
}

Elf32_Shdr *get_shdr(const char *name, void *start){
        /* first, get the header */
        Elf32_Ehdr *header = (Elf32_Ehdr *)start;
        /* now, get a pointer to the first section header */
        Elf32_Shdr *sh_tbl = (Elf32_Shdr *) (start + header->e_shoff);
        /* and a pointer to the section header string table: */
        char *str_tbl;
        int i=1, found=0;
        
        /* first, check for a section header string table: */
        if (header->e_shnum < header->e_shstrndx || header->e_shstrndx == SHN_UNDEF){
                printf("no section header string table. sorry \n");
                return NULL;
        }
        str_tbl = (char *) (start + sh_tbl[header->e_shstrndx].sh_offset);

        /* now, go over all sections and find the one requested:  */
        for(i=0; i<header->e_shnum; i++){
                if (strcmp(name, str_tbl + sh_tbl[i].sh_name) == 0){
                        found = 1;
                        break;
                }
        }

        return found?(sh_tbl + i):NULL;
}

Elf32_Shdr *get_shdr_from_index(int index, void *start){
        Elf32_Ehdr *header = (Elf32_Ehdr *)start;
        Elf32_Shdr *sh_tbl = (Elf32_Shdr *) (start + header->e_shoff);
        if (index >= header->e_shnum ) return NULL;
        
        return sh_tbl + index;
}

char *get_section_name(int index, void *start){
        Elf32_Ehdr *header = (Elf32_Ehdr *)start;        
        Elf32_Shdr *sh_tbl = (Elf32_Shdr *) (start + header->e_shoff);
        Elf32_Shdr *Sheader = get_shdr_from_index(index, start);
        char *str_tbl;
        str_tbl = (char *) (start + sh_tbl[header->e_shstrndx].sh_offset);

        return str_tbl+Sheader->sh_name;
}

Elf32_Sym *get_symbol(const char *name, void *start){
        Elf32_Shdr *symtab = get_shdr(".symtab", start);
        Elf32_Shdr *strtab = get_shdr(".strtab", start);
        Elf32_Sym  *symbol;
        int sym_num = symtab->sh_size / symtab->sh_entsize;
        int i, found=0;
        char *symbol_names;
        
        if (symtab == NULL || strtab == NULL){
                printf("no %s table, sorry \n", symtab==NULL?"symbol":"string");
                return NULL;
        }

        symbol_names = start + strtab->sh_offset;
        symbol = (Elf32_Sym *) (start + symtab->sh_offset);

        for(i=0; i< sym_num ; i++){
                if (strcmp(name, symbol_names + symbol[i].st_name)==0){
                        found=1;
                        break;
                }
        }
        
        return found?symbol+i:NULL;

}

char *get_symbol_name(Elf32_Sym  *symbol, void *start){
        Elf32_Shdr *strtab = get_shdr(".strtab", start);
        char *symbol_names = NULL;
        
        if (strtab == NULL){
                printf("no string table, sorry \n");
                return NULL;
        }
        
        symbol_names = start + strtab->sh_offset;
        return symbol_names + symbol->st_name;
}

Elf32_Sym *get_symbol_from_index(int index, void *start){
        Elf32_Shdr *symtab = get_shdr(".symtab", start);
        Elf32_Sym  *symbol=NULL;
        int sym_num = symtab->sh_size / symtab->sh_entsize;
        
        if (symtab == NULL){
                printf("no symbol table, sorry \n");
                return NULL;
        } if (index >= sym_num) return NULL;
        

        symbol = (Elf32_Sym *) (start + symtab->sh_offset +index*symtab->sh_entsize );

        return symbol;

}

int for_all_symbols(void *start, int (*func)(void *,Elf32_Sym *)){
        Elf32_Shdr *symtab_head = get_shdr(".symtab", start);
        Elf32_Sym  *symtab=NULL;   
        int sym_num;
        int ans = 0;

        if (!symtab_head) return ans;

        sym_num = symtab_head->sh_size / symtab_head->sh_entsize - 1;
        symtab = (Elf32_Sym *) (start + symtab_head->sh_offset);

        for(; sym_num>=1; sym_num--) {
                ans = func(start, symtab + sym_num);
                if(ans) return ans;
        }
        
        return ans;
}

