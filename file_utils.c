#include "file_utils.h"

void file_rawRead(uint32_t pos, void *buf, uint32_t size, size_t count, FILE *f) {
    fseek(f, pos, SEEK_SET);
    fread(buf, size, count, f);
}

void file_rawWrite(uint32_t pos, void *buf, uint32_t size, size_t count, FILE *f) {
    fseek(f, pos, SEEK_SET);
    fwrite(buf, size, count, f);
}