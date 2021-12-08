#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "b64.h"

// gnu coreutils base64
char *base64_(const char *buf, size_t len) {
  FILE *fp;
  char path[1035];
  char *out = calloc(1, 1);

  // write the raw data into a temporary file
  fp = fopen("/tmp/fuzz.data", "wb");
  fwrite(buf, 1, len, fp);
  fclose(fp);

  // base64 on the temp file
  fp = popen("base64 --wrap=0 /tmp/fuzz.data", "r");
  if (fp == NULL) {
    return NULL;
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path), fp) != NULL) {
    char *nextline;
    asprintf(&nextline, "%s%s", out, path);
    free(out);
    out = nextline;
  }

  /* close */
  pclose(fp);

  return out;
}

#if !defined(FUZZING)
int main(int argc, char **argv) {
  if (argc > 1) {
    {
      char *out = base64_(argv[1], strlen(argv[1]));
      printf("%s\n", out);
      free(out);
    }
    {
      char *out = b64_encode(argv[1], strlen(argv[1]));
      printf("%s\n", out);
      free(out);
    }
  }

  return 0;
}
#else
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  int err = 0;

  // test encode
  char *reference = base64_(Data, Size);
  char *actual = b64_encode(Data, Size);
  if (strcmp(reference, actual) != 0) {
    printf("error %s:%s\n", reference, actual);
    err = 1;
  }

  // test decode
  size_t outsize;
  char *decoded = b64_decode_ex(actual, strlen(actual), &outsize);
  assert(outsize == Size);
  assert(memcmp(Data, decoded, Size) == 0);

  free(reference);
  free(actual);
  free(decoded);

  assert(err == 0);

  return 0;
}
#endif
