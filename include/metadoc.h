#ifndef METADOC_H
#define METADOC_H

#include <stdio.h>
#include <stdint.h>

uint8_t* generate_key(FILE *key_fp, uint8_t* key);

void encrypt(const char *target_file, const char* key_file);
void decrypt(const char *target_file, const char* key_file);


#endif