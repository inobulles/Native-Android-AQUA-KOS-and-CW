
#ifndef __AQUA__ANDROID_KOS_PLACEHOLDERS_H
#define __AQUA__ANDROID_KOS_PLACEHOLDERS_H

void platform(void) { printf("TODO `platform`\n"); }
unsigned long long is_device_supported(const char* device) { printf("TODO `is_device_supported`\n"); return 0; }

unsigned long long* get_device(unsigned long long device, const char* extra) { printf("TODO `get_device`\n"); return nullptr; }
void send_device(unsigned long long device, const char* extra, unsigned long long* data) { printf("TODO `send_device`\n"); }

#endif