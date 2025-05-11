
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define KEY_SIZE 64

void generate_key (FILE *key_fp, uint8_t* key) {
  uint8_t buffer[KEY_SIZE];
  size_t bytes_read = 0;

  while ((bytes_read = fread(buffer, 1, KEY_SIZE, key_fp)) > 0) {
    if (bytes_read < KEY_SIZE) {
      memset(buffer + bytes_read, 1, KEY_SIZE - bytes_read);
    }

    for (int i = 0; i < KEY_SIZE; i++) {
      key[i] ^= buffer[i];
    }
  };
}

void encrypt(char *target_file, char* key_file) {
  FILE *key_fp = fopen(key_file, "rb");
  if (key_fp == NULL) {
    fprintf(stderr, "Error opening key file: %s\n", key_file);
    exit(EXIT_FAILURE);
  }
  FILE *target_fp = fopen(target_file, "rb");
  if (target_fp == NULL) {
    fprintf(stderr, "Error opening target file: %s\n", target_file);
    fclose(key_fp);
    exit(EXIT_FAILURE);
  }
  uint8_t *key = calloc(KEY_SIZE, sizeof(uint8_t));
  // CHECK CALLOC
  generate_key(key_fp, key);
}