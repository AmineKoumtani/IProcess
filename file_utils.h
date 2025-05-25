#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdint.h>
#include <stdio.h>

void file_rawRead(uint32_t pos, void *buf, uint32_t size, size_t count, FILE *f);
void file_rawWrite(uint32_t pos, void *buf, uint32_t size, size_t count, FILE *f);

#endif