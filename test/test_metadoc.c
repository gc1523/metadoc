#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/metadoc.h"

#define KEY_SIZE 64
#define TEST_KEY_FILE "test/test_key.txt"
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
    printf("%02x ", key[i]);
  }
  printf("\n");

  free(key);
}

int main(void) {
  test_key_generation();
}