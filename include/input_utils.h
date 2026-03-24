#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include <stddef.h>

int readInt(const char* prompt, int* outValue);
int readPositiveInt(const char* prompt, int* outValue);
int readNonNegativeInt(const char* prompt, int* outValue);
int readFloat(const char* prompt, float* outValue);
int readNonNegativeFloat(const char* prompt, float* outValue);
int readLine(const char* prompt, char* outBuffer, size_t outSize);

#endif
