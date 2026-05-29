#include "dlfcn.hpp"

#define TAG_NAME    "dlfcn"
#define log_info(fmt, args...) __android_log_print(ANDROID_LOG_INFO, TAG_NAME, (const char *) fmt, ##args)
#define log_err(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG_NAME, (const char *) fmt, ##args)
#define log_dbg(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG_NAME, (const char *) fmt, ##args)

int fdlclose(void *handle) {
    if (handle) {
        struct ctx *ctx = (struct ctx *) handle;
        if (ctx->dynsym) free(ctx->dynsym);    /* we're saving dynsym and dynstr */
        if (ctx->dynstr) free(ctx->dynstr);    /* from library file just in case */
        free(ctx);
    }
    return 0;
}

void *fake_dlopen_with_path(const char *libpath, int flags) {
    FILE *maps;
    char buff[256], name[256];
    struct ctx *ctx = 0;
    off_t load_addr, size;
    int k, fd = -1, found = 0;
    char *shoff;
    Elf_Ehdr *elf = (Elf_Ehdr *) MAP_FAILED;

#define fatal(fmt, args...) do { log_err(fmt,##args); goto err_exit; } while(0)

    maps = fopen("/proc/self/maps", "r");
    if (!maps) fatal("failed to open maps");

    while (!found && fgets(buff, sizeof(buff), maps)) {
        if (strstr(buff, libpath) && (strstr(buff, "r-xp") || strstr(buff, "r--p"))) found = 1;
    }
    fclose(maps);

    if (!found) fatal("%s not found in my userspace", libpath);

    if (sscanf(buff, "%lx-%*lx %*s %*s %*s %*s %s", &load_addr, name) != 2)
        fatal("failed to read load address for %s", libpath);

    log_info("%s loaded in Android at 0x%08lx", libpath, load_addr);

    libpath = name;

    /* Now, mmap the same library once again */

    fd = open(libpath, O_RDONLY);
    if (fd < 0) fatal("failed to open %s", libpath);

    size = lseek(fd, 0, SEEK_END);
    if (size <= 0) fatal("lseek() failed for %s", libpath);

    elf = (Elf_Ehdr *) mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    fd = -1;

    if (elf == MAP_FAILED) fatal("mmap() failed for %s", libpath);

    ctx = (struct ctx *) calloc(1, sizeof(struct ctx));
    if (!ctx) fatal("no memory for %s", libpath);

    ctx->load_addr = (void *) load_addr;
    shoff = ((char *) elf) + elf->e_shoff;

    for (k = 0; k < elf->e_shnum; k++, shoff += elf->e_shentsize) {

        Elf_Shdr *sh = (Elf_Shdr *) shoff;
        log_dbg("%s: k=%d shdr=%p type=%x", __func__, k, sh, sh->sh_type);

        switch (sh->sh_type) {

            case SHT_DYNSYM:
                if (ctx->dynsym) fatal("%s: duplicate DYNSYM sections", libpath); /* .dynsym */
                ctx->dynsym = malloc(sh->sh_size);
                if (!ctx->dynsym) fatal("%s: no memory for .dynsym", libpath);
                memcpy(ctx->dynsym, ((char *) elf) + sh->sh_offset, sh->sh_size);
                ctx->nsyms = (sh->sh_size / sizeof(Elf_Sym));
                break;

            case SHT_STRTAB:
                if (ctx->dynstr) break;    /* .dynstr is guaranteed to be the first STRTAB */
                ctx->dynstr = malloc(sh->sh_size);
                if (!ctx->dynstr) fatal("%s: no memory for .dynstr", libpath);
                memcpy(ctx->dynstr, ((char *) elf) + sh->sh_offset, sh->sh_size);
                break;

            case SHT_PROGBITS:
                if (!ctx->dynstr || !ctx->dynsym) break;
                /* won't even bother checking against the section name */
                ctx->bias = (off_t) sh->sh_addr - (off_t) sh->sh_offset;
                k = elf->e_shnum;  /* exit for */
                break;
        }
    }

    munmap(elf, size);
    elf = 0;

    if (!ctx->dynstr || !ctx->dynsym) fatal("dynamic sections not found in %s", libpath);

#undef fatal

    log_dbg("%s: ok, dynsym = %p, dynstr = %p", libpath, ctx->dynsym, ctx->dynstr);

    return ctx;

    err_exit:
    if (fd >= 0) close(fd);
    if (elf != MAP_FAILED) munmap(elf, size);
    fdlclose(ctx);
    return 0;
}

void *fdlopen(const char *filename, int flags) {
    char buf[512] = {0};
    void *handle = NULL;
    //sysmtem
    strcpy(buf, kSystemLibDir);
    strcat(buf, filename);
    handle = fake_dlopen_with_path(buf, flags);
    if (handle) {
        return handle;
    }

    // apex in ns com.android.runtime
    memset(buf, 0, sizeof(buf));
    strcpy(buf, kApexLibDir);
    strcat(buf, filename);
    handle = fake_dlopen_with_path(buf, flags);
    if (handle) {
        return handle;
    }

    // apex in ns com.android.art
    memset(buf, 0, sizeof(buf));
    strcpy(buf, kApexArtNsLibDir);
    strcat(buf, filename);
    handle = fake_dlopen_with_path(buf, flags);
    if (handle) {
        return handle;
    }

    //odm
    memset(buf, 0, sizeof(buf));
    strcpy(buf, kOdmLibDir);
    strcat(buf, filename);
    handle = fake_dlopen_with_path(buf, flags);
    if (handle) {
        return handle;
    }

    //vendor
    memset(buf, 0, sizeof(buf));
    strcpy(buf, kVendorLibDir);
    strcat(buf, filename);
    handle = fake_dlopen_with_path(buf, flags);
    if (handle) return handle;

    return fake_dlopen_with_path(filename, flags);
}

void *fdlsym(void *handle, const char *name) {
    int k;
    struct ctx *ctx = (struct ctx *) handle;
    Elf_Sym *sym = (Elf_Sym *) ctx->dynsym;
    char *strings = (char *) ctx->dynstr;

    for (k = 0; k < ctx->nsyms; k++, sym++)
        if (strcmp(strings + sym->st_name, name) == 0) {
            /*  NB: sym->st_value is an offset into the section for relocatables,
            but a VMA for shared libs or exe files, so we have to subtract the bias */
            void *ret = (char *) ctx->load_addr + sym->st_value - ctx->bias;
            log_info("%s found at %p", name, ret);
            return ret;
        }
    
    return 0;
}

const char *fdlerror() { return NULL; }
