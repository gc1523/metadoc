
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

void rotate_array_n(uint8_t *array, size_t size, int n) {
  for (int i = 0; i < n; i++) {
    uint8_t temp = array[0];
    for (size_t j = 0; j < size - 1; j++) {
      array[j] = array[j + 1];
    }
    array[size - 1] = temp;
  }
}

void encrypt(const char *target_file, const char* key_file) {
  FILE *key_fp = fopen(key_file, "rb");
  if (key_fp == NULL) {
    fprintf(stderr, "Error opening key file: %s\n", key_file);
    exit(EXIT_FAILURE);
  }
  FILE *target_fp = fopen(target_file, "rb+");
  if (target_fp == NULL) {
    fprintf(stderr, "Error opening target file: %s\n", target_file);
    fclose(key_fp);
    exit(EXIT_FAILURE);
  }

  uint8_t *key = calloc(KEY_SIZE, sizeof(uint8_t));
  if (key == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(key_fp);
    fclose(target_fp);
    exit(EXIT_FAILURE);
  }

  generate_key(key_fp, key);
  fclose(key_fp);

  uint8_t buffer[KEY_SIZE];
  size_t bytes_read = 0;
  int rot_val = 0;

  while((bytes_read = fread(buffer, 1, KEY_SIZE, target_fp)) > 0) {

    if (bytes_read < KEY_SIZE) {
      memset(buffer + bytes_read, 0, KEY_SIZE - bytes_read);
    }
    
    rotate_array_n(key, KEY_SIZE, rot_val);
    rot_val = buffer[0];
    for (size_t i = 0; i < bytes_read; i++) {
      buffer[i] ^= key[i];
    }

    fseek(target_fp, -bytes_read, SEEK_CUR);
    fwrite(buffer, 1, bytes_read, target_fp);
    fflush(target_fp);
  }

  fclose(target_fp);
  free(key);
}

void decrypt(const char *target_file, const char* key_file) {
  FILE *key_fp = fopen(key_file, "rb");
  if (key_fp == NULL) {
    fprintf(stderr, "Error opening key file: %s\n", key_file);
    exit(EXIT_FAILURE);
  }
  FILE *target_fp = fopen(target_file, "rb+");
  if (target_fp == NULL) {
    fprintf(stderr, "Error opening target file: %s\n", target_file);
    fclose(key_fp);
    exit(EXIT_FAILURE);
  }
  
  uint8_t *key = calloc(KEY_SIZE, sizeof(uint8_t));
  if (key == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(key_fp);
    fclose(target_fp);
    exit(EXIT_FAILURE);
  }

  generate_key(key_fp, key);
  fclose(key_fp);

  uint8_t buffer[KEY_SIZE];
  size_t bytes_read = 0;
  int rot_val = 0;

  while((bytes_read = fread(buffer, 1, KEY_SIZE, target_fp)) > 0) {

    if (bytes_read < KEY_SIZE) {
      memset(buffer + bytes_read, 0, KEY_SIZE - bytes_read);
    }
    rotate_array_n(key, KEY_SIZE, rot_val);
    for (size_t i = 0; i < bytes_read; i++) {
      buffer[i] ^= key[i];
    }
    rot_val = buffer[0];

    fseek(target_fp, -bytes_read, SEEK_CUR);
    fwrite(buffer, 1, bytes_read, target_fp);
    fflush(target_fp);
  }

  fclose(target_fp);
  free(key);
}