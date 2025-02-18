/*
 * Copyright (C) 2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file  patch.c
 * @brief Patching some of the .so internal functions or bridging them to native
 *        for better compatibility.
 */

#include <kubridge.h>
#include <so_util/so_util.h>
#include <libc_bridge/libc_bridge.h>

#include <string.h>

#include "reimpl/io.h"
#include "utils/logger.h"

extern so_module so_mod;

so_hook _ZN17QiFileInputStream4openEPKc_hook;
so_hook _ZN17QiFileInputStream5closeEv_hook;
so_hook _ZNK17QiFileInputStream6isOpenEv_hook;
so_hook _ZNK17QiFileInputStream7getSizeEv_hook;

typedef struct QiString {
	char *data;
	int allocated_size;
	int length;
	char cached[32];
} QiString;

QiString makeQiString(const char* str) {
    QiString qstr;
    qstr.data = strdup(str);
    qstr.allocated_size = strlen(str);
    qstr.length = strlen(str);
    return qstr;
}

typedef struct QiFileInputStream {
	char _unknown0[0xc];
	FILE *file;
    QiString path;
	int length;
	int headpos;
	size_t _unknown1;
} QiFileInputStream;

void hook_debug_log(void* this, char* fmt, int unk) {
    l_info("Debug::log ~ [%i] %s", unk, fmt);
}


FILE* QiFileInputStream_open_hook(QiFileInputStream* this, const char* path) {
    char full_fname[512];
	if (!strstr(path, "ux0:")) {
        if(path[0] == '/') {
            sprintf(full_fname, "ux0:data/smash_hit/assets%s", path);
        } else {
            sprintf(full_fname, "ux0:data/smash_hit/assets/%s", path);
        }
    } else {
        sprintf(full_fname, "%s", path);
    }

    this->file = sceLibcBridge_fopen(full_fname, "rb");//fopen(full_fname, "rb");
    if (this->file == NULL) {
        l_warn("QiFileInputStream_open_hook ~ [%s] : %p", full_fname, this->file);
        return NULL;
    }
    l_info("QiFileInputStream_open_hook ~ [%s] : %p", full_fname, this->file);

    sceLibcBridge_fseek(this->file, 0, SEEK_END);
        this->length = sceLibcBridge_ftell(this->file);
    sceLibcBridge_fseek(this->file, 0, SEEK_SET);

    this->path = makeQiString(full_fname);
    return this->file;
}

FILE* QiFileInputStream_openLeanAndMean_hook(QiFileInputStream* this, const char* path) {
    char full_fname[512];
    if (!strstr(path, "ux0:")) {
        if(path[0] == '/') {
            sprintf(full_fname, "ux0:data/smash_hit/assets%s", path);
        } else {
            sprintf(full_fname, "ux0:data/smash_hit/assets/%s", path);
        }
    } else {
        sprintf(full_fname, "%s", path);
    }

    this->file = sceLibcBridge_fopen(full_fname, "rb");
    if (this->file == NULL) {
        l_warn("QiFileInputStream_openLeanAndMean_hook ~ [%s] : %p", full_fname, this->file);
        return NULL;
    }
    l_info("QiFileInputStream_openLeanAndMean_hook ~ [%s] : %p", full_fname, this->file);

    sceLibcBridge_fseek(this->file, 0, SEEK_END);
        this->length = sceLibcBridge_ftell(this->file);
    sceLibcBridge_fseek(this->file, 0, SEEK_SET);

    this->path = makeQiString(full_fname);
    return this->file != NULL;
}

void QiFileInputStream_close_hook(QiFileInputStream* this) {
    l_info("QiFileInputStream_close_hook ~ [%p] : %p", this->path, this->file);
    sceLibcBridge_fclose(this->file);
    this = NULL;
}

int QiFileInputStream_isOpen_hook(QiFileInputStream* this) {
    l_info("QiFileInputStream_isOpen_hook ~ [%p] : %p", this->path, this->file);
    return this->file != NULL;
}

int QiFileInputStream_getSize_hook(QiFileInputStream* this) {
    l_info("QiFileInputStream_getSize_hook ~ [%p, %p] : %i", this->path, this->file, this->length);
    return this->length;
}


//_ZNK17QiFileInputStream7getSizeEv
//_ZN17QiFileInputStream5closeEv
void so_patch(void) {
    //_ZN5Audio14isMusicEnabledEv
    hook_addr((uintptr_t)so_symbol(&so_mod, "_ZN5Debug3logEPKci"), (uintptr_t)&hook_debug_log);

    hook_addr((uintptr_t)so_symbol(&so_mod, "_ZN17QiFileInputStream4openEPKc"), (uintptr_t)&QiFileInputStream_open_hook);
    hook_addr((uintptr_t)so_symbol(&so_mod, "_ZN17QiFileInputStream15openLeanAndMeanEPKc"), (uintptr_t)&QiFileInputStream_openLeanAndMean_hook);
    hook_addr((uintptr_t)so_symbol(&so_mod, "_ZN17QiFileInputStream5closeEv"), (uintptr_t)&QiFileInputStream_close_hook);
    hook_addr((uintptr_t)so_symbol(&so_mod, "_ZNK17QiFileInputStream6isOpenEv"), (uintptr_t)&QiFileInputStream_close_hook);
    hook_addr((uintptr_t)so_symbol(&so_mod, "_ZNK17QiFileInputStream7getSizeEv"), (uintptr_t)&QiFileInputStream_getSize_hook);
}
