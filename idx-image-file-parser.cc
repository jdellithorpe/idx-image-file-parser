#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <istream>

/**
 * A utility for parsing IDX image files.
 *
 * Uses the byte ordering described here: http://yann.lecun.com/exdb/mnist/
 *
 * [offset] [type]          [value]          [description] 
 * 0000     32 bit integer  0x00000803(2051) magic number 
 * 0004     32 bit integer  60000            number of images 
 * 0008     32 bit integer  28               number of rows 
 * 0012     32 bit integer  28               number of columns 
 * 0016     unsigned byte   ??               pixel 
 * 0017     unsigned byte   ??               pixel 
 * ........ 
 * xxxx     unsigned byte   ??               pixel
 */

using namespace std;

/**
 * Reverses the byte order in the given array.
 */
void swap_endianness(char* array, size_t size) {
  for (int i = 0; i < size/2; i++) {
    char tmp = array[i];
    array[i] = array[size - i - 1];
    array[size - i - 1] = tmp;
  }
}

/* Print a usage message. */
void usage() {
  printf("usage: idx-image-file-parser idximagefile\n");
  printf("  idxfile:    Input IDX image file format file to parse.\n");
}

int
main(int argc, char *argv[])
{

  if (argc != 2) {
    usage();
    exit(1);
  }

  string inputFilename(argv[1]);

  std::ifstream inputFile;
  inputFile.open(inputFilename, std::ios::binary);

  char buffer[sizeof(uint32_t)];
  inputFile.read(buffer, sizeof(uint32_t));
  swap_endianness(buffer, sizeof(uint32_t));
  uint32_t magicnumber = *((uint32_t*)buffer);

  printf("Magic number: %X\n", magicnumber);

  inputFile.read(buffer, sizeof(uint32_t));
  swap_endianness(buffer, sizeof(uint32_t));
  uint32_t num_images = *((uint32_t*)buffer);

  printf("Number of images: %d\n", num_images);

  inputFile.read(buffer, sizeof(uint32_t));
  swap_endianness(buffer, sizeof(uint32_t));
  uint32_t num_rows = *((uint32_t*)buffer);

  printf("Number of rows: %d\n", num_rows);

  inputFile.read(buffer, sizeof(uint32_t));
  swap_endianness(buffer, sizeof(uint32_t));
  uint32_t num_columns = *((uint32_t*)buffer);

  printf("Number of columns: %d\n", num_columns);

  for (uint32_t n = 0; n < num_images; n++) {
    printf("Image %d\n", n);
    for (uint32_t c = 0; c < num_columns; c++) {
      for (uint32_t r = 0; r < num_rows; r++) {
        inputFile.read(buffer, sizeof(uint8_t));
        uint8_t pixel_value = *((uint8_t*)buffer);
        printf("%4d ", pixel_value);
      }
      printf("\n");
    }
  }

  return 0;
}
