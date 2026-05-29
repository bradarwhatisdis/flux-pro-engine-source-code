#pragma once

#include <elf.h>
#include <link.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/types.h>
#include <android/log.h>


// GNU Hash function
static uint32_t gnu_hash(const char *s) {
    uint32_t h = 5381;
    for (unsigned char c = *s; c != '\0'; c = *++s)
        h = h * 33 + c;
    return h;
}

void deadbeef() {}

struct ModuleInfo {
    const char* name;
    uintptr_t base_addr;
    ElfW(Dyn)* dynamic;
};

// Callback to find module by name or address
INLINE int find_module_callback(struct dl_phdr_info *info, size_t size, void *data) {
    ModuleInfo* target = (ModuleInfo*)data;
    
    // If searching by address (check if address falls within any segment)
    if (target->base_addr != 0) {
        for (int i = 0; i < info->dlpi_phnum; i++) {
            uintptr_t start = info->dlpi_addr + info->dlpi_phdr[i].p_vaddr;
            uintptr_t end = start + info->dlpi_phdr[i].p_memsz;
            if (target->base_addr >= start && target->base_addr < end) {
                target->base_addr = info->dlpi_addr;
                // Find PT_DYNAMIC
                for (int j = 0; j < info->dlpi_phnum; j++) {
                    if (info->dlpi_phdr[j].p_type == PT_DYNAMIC) {
                        target->dynamic = (ElfW(Dyn)*)(info->dlpi_addr + info->dlpi_phdr[j].p_vaddr);
                        return 1; // Found
                    }
                }
                return 1; // Found lib but maybe no dynamic section?
            }
        }
    }
    // If searching by name
    else if (target->name && strstr(info->dlpi_name, target->name)) {
        target->base_addr = info->dlpi_addr;
        for (int j = 0; j < info->dlpi_phnum; j++) {
            if (info->dlpi_phdr[j].p_type == PT_DYNAMIC) {
                target->dynamic = (ElfW(Dyn)*)(info->dlpi_addr + info->dlpi_phdr[j].p_vaddr);
                return 1; // Found
            }
        }
    }
    return 0;
}

class ManualLookup {
public:
    static void* dlsym(uintptr_t module_marker, const char* symbol_name) {
        ModuleInfo info = {0};
        info.base_addr = module_marker;
        
        dl_iterate_phdr(find_module_callback, &info);
        
        if (!info.dynamic) return nullptr;

        ElfW(Sym)* symtab = nullptr;
        const char* strtab = nullptr;
        uint32_t* gnu_hash_table = nullptr;
        
        for (ElfW(Dyn)* dyn = info.dynamic; dyn->d_tag != DT_NULL; ++dyn) {
            switch (dyn->d_tag) {
                case DT_SYMTAB: symtab = (ElfW(Sym)*)(info.base_addr + dyn->d_un.d_ptr); break;
                case DT_STRTAB: strtab = (const char*)(info.base_addr + dyn->d_un.d_ptr); break;
                case DT_GNU_HASH: gnu_hash_table = (uint32_t*)(info.base_addr + dyn->d_un.d_ptr); break;
            }
        }

        if (!symtab || !strtab || !gnu_hash_table) return nullptr;

        // GNU Hash Lookup
        uint32_t nbuckets = gnu_hash_table[0];
        uint32_t symndx = gnu_hash_table[1];
        uint32_t maskwords = gnu_hash_table[2];
        uint32_t shift2 = gnu_hash_table[3];
        
        ElfW(Addr)* bloom = (ElfW(Addr)*)(gnu_hash_table + 4);
        uint32_t* buckets = (uint32_t*)(bloom + maskwords);
        uint32_t* chain = buckets + nbuckets;

        uint32_t hash = gnu_hash(symbol_name);
        uint32_t elf_class_bits = sizeof(ElfW(Addr)) * 8;
        uint32_t word = bloom[(hash / elf_class_bits) % maskwords];
        uint32_t mask = 0 | (uint32_t)1 << (hash % elf_class_bits)
                          | (uint32_t)1 << ((hash >> shift2) % elf_class_bits);

        if ((word & mask) != mask) return nullptr;

        uint32_t sym_idx = buckets[hash % nbuckets];
        if (sym_idx < symndx) return nullptr;

        while (true) {
            uint32_t chain_hash = chain[sym_idx - symndx];
            
            if ((chain_hash >> 1) == (hash >> 1) && 
                strcmp(strtab + symtab[sym_idx].st_name, symbol_name) == 0) {
                return (void*)(info.base_addr + symtab[sym_idx].st_value);
            }
            
            if (chain_hash & 1) break;
            sym_idx++;
        }
        
        return nullptr;
    }

    // static INLINE void walk_exported_symbols(uintptr_t module_marker, std::function<bool(const char*, void*)> callback) {
    template <typename Callback>
    static INLINE void walk_exported_symbols(uintptr_t module_marker, Callback callback) {
        static ModuleInfo info = {0};
        static uintptr_t last_marker = 0;

        if (last_marker != module_marker) {
            info = {0};
            info.base_addr = module_marker;
            dl_iterate_phdr(find_module_callback, &info);
            if (info.dynamic) last_marker = module_marker;
        }
        
        if (!info.dynamic) return;

        ElfW(Sym)* symtab = nullptr;
        const char* strtab = nullptr;
        uint32_t* gnu_hash_table = nullptr;
        
        for (ElfW(Dyn)* dyn = info.dynamic; dyn->d_tag != DT_NULL; ++dyn) {
            switch (dyn->d_tag) {
                case DT_SYMTAB: symtab = (ElfW(Sym)*)(info.base_addr + dyn->d_un.d_ptr); break;
                case DT_STRTAB: strtab = (const char*)(info.base_addr + dyn->d_un.d_ptr); break;
                case DT_GNU_HASH: gnu_hash_table = (uint32_t*)(info.base_addr + dyn->d_un.d_ptr); break;
            }
        }

        if (!symtab || !strtab || !gnu_hash_table) return;

        uint32_t nbuckets = gnu_hash_table[0];
        uint32_t symndx = gnu_hash_table[1];
        uint32_t maskwords = gnu_hash_table[2];
        
        ElfW(Addr)* bloom = (ElfW(Addr)*)(gnu_hash_table + 4);
        uint32_t* buckets = (uint32_t*)(bloom + maskwords);
        uint32_t* chain = buckets + nbuckets;

        // Iterate through all buckets
        for (uint32_t i = 0; i < nbuckets; ++i) {
            uint32_t sym_idx = buckets[i];
            if (sym_idx == 0) continue;

            // Walk the chain for this bucket
            while (true) {
                const char* sym_name = strtab + symtab[sym_idx].st_name;
                void* sym_addr = (void*)(info.base_addr + symtab[sym_idx].st_value);
                
                if (callback(sym_name, sym_addr)) return; // Stop if callback returns true

                uint32_t chain_hash = chain[sym_idx - symndx];
                if (chain_hash & 1) break; // End of chain
                sym_idx++;
            }
        }
    }

    static consteval uint32_t _fnv1a(const char* str) { // Compile-time FNV-1a hash, omitted in binary
        uint32_t hash = 2166136261u;
        for (int i = 0; str[i] != 0; ++i) {
            hash ^= (uint8_t)str[i];
            hash *= 16777619u;
        }
        return hash;
    }

    static INLINE uint32_t fnv1a(const char* str) {
	OBF_BEGIN
        int i;
        uint32_t hash;
        V(hash) = O(2166136261u); // 0x811c9dc5
        FOR (V(i) = N(0), V(str[i]) != N(0), ++V(i))
            V(hash) ^= (uint8_t)V(str[i]);
            V(hash) *= O(16777619u); // 0x1000193
        ENDFOR
        RETURN(hash);
    OBF_END
    }

    static INLINE void* dlsym_hashed(uint32_t target_hash) {
        void* found_addr = nullptr;
        walk_exported_symbols((uintptr_t)&deadbeef, [&](const char* name, void* addr) -> bool {
            if (fnv1a(name) == target_hash) {
                found_addr = addr;
                return true;
            }
            return false;
        });
        return found_addr;
    }
    
    static INLINE void dlsym_call_hashed(uint32_t target_hash) {
        walk_exported_symbols((uintptr_t)&deadbeef, [&](const char* name, void* addr) INLINE -> bool {
            if (fnv1a(name) == target_hash) {
                if (addr) ((void(*)())addr)();
                return true;
            }
            return false;
        });
    }
};

#define HASHF(x) O(ManualLookup::_fnv1a(x))
#define CALL(x) ManualLookup::dlsym_call_hashed(HASHF(GET_N(x)))
#define GET_S(x) ManualLookup::dlsym_hashed(HASHF(GET_N(x)))
