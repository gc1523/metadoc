#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/metadoc.h"
#include <assert.h>
#include <stdbool.h>


#define KEY_SIZE 64
#define TEST_KEY_FILE "test/test_key.txt"

const char *source_file = "test/original_super_secret_info.txt";
const char *encrypted_file = "test/encrypted.blob";
const char *decrypted_file = "test/decrypted.txt";

void test_key_generation() {
  FILE *key_fp = fopen(TEST_KEY_FILE, "rb");
  if (key_fp == NULL) {
    fprintf(stderr, "Error opening key file: %s\n", TEST_KEY_FILE);
    exit(EXIT_FAILURE);
  }
  uint8_t* key = calloc(KEY_SIZE, sizeof(uint8_t));
  generate_key(key_fp, key);
  fclose(key_fp);
  for (int i = 0; i < KEY_SIZE; i++) {
    printf("%02x", key[i]);
  }
  printf("\n");

  free(key);
}

void create_file_copy(const char *source_file, const char *target_file) {
  FILE *source_fp = fopen(source_file, "rb");
  if (source_fp == NULL) {
    fprintf(stderr, "Error opening source file: %s\n", source_file);
    exit(EXIT_FAILURE);
  }
  FILE *target_fp = fopen(target_file, "wb");
  if (target_fp == NULL) {
    fprintf(stderr, "Error opening target file: %s\n", target_file);
    fclose(source_fp);
    exit(EXIT_FAILURE);
  }
  uint8_t buffer[KEY_SIZE];
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_fp)) > 0) {
    fwrite(buffer, 1, bytes_read, target_fp);
  }
  fclose(source_fp);
  fclose(target_fp);
}

void test_encryption() {
  create_file_copy(source_file, encrypted_file);
  const char *key_file = TEST_KEY_FILE;
  encrypt(encrypted_file, key_file);
}

void test_decryption() {
  create_file_copy(encrypted_file, decrypted_file);
  const char *key_file = TEST_KEY_FILE;
  decrypt(decrypted_file, key_file);
}

bool check_files_equal(const char *file1, const char *file2) {
  FILE *fp1 = fopen(file1, "rb");
  FILE *fp2 = fopen(file2, "rb");
  if (fp1 == NULL || fp2 == NULL) {
    fprintf(stderr, "Error opening files for comparison\n");
    return false;
  }
  uint8_t buffer1[KEY_SIZE], buffer2[KEY_SIZE];
  size_t bytes_read1, bytes_read2;
  while ((bytes_read1 = fread(buffer1, 1, sizeof(buffer1), fp1)) > 0 &&
         (bytes_read2 = fread(buffer2, 1, sizeof(buffer2), fp2)) > 0) {
    if (bytes_read1 != bytes_read2 || memcmp(buffer1, buffer2, bytes_read1) != 0) {
      fclose(fp1);
      fclose(fp2);
      return false;
    }
  }
  fclose(fp1);
  fclose(fp2);
  return true;
}

int main(void) {
  test_key_generation();
  test_encryption();
  test_decryption();
  assert(check_files_equal(source_file, decrypted_file));
}