
#ifndef __AQUA__ANDROID_KOS_MISC_H
#define __AQUA__ANDROID_KOS_MISC_H

#include "lib/macros.h"

#define SYS_PRIVILEGES 0

unsigned long long get_platform(void) {
    return PLATFORM_64BIT | PLATFORM_ANDROID | PLATFORM_MOBILE | PLATFORM_KNOWN;

}

unsigned long long platform_system(unsigned long long command) {
    if (SYS_PRIVILEGES) return (unsigned long long) system((const char*) command);
    else printf("WARNING You do not have SYS_PRIVILEGES to run command `%s` (function `platform_system`)\n", (const char*) command);

    return 1;

}

#endif