#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processFile(FILE *file, Flags flags) {
  char buffer[4096];
  int lineNumber = 1;
  int lastLineEmpty = 0;

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (flags.flagS && strcmp(buffer, "\n") == 0) {
      if (lastLineEmpty) {
        continue;
      }
      lastLineEmpty = 1;
    } else {
      lastLineEmpty = 0;
    }

    if (flags.flagB && buffer[0] != '\n') {
      printf("%6d\t", lineNumber++);
    } else if (flags.flagN && !flags.flagB && buffer[1] != '\n') {
      printf("%6d\t", lineNumber++);
    }

    for (char *p = buffer; *p != '\0'; p++) {
      if (flags.flagT && *p == '\t') {
        printf("^I");
      } else if (flags.flagE && *p == '\n') {
        printf("$\n");
      } else if (flags.flagV && (*p < 32 || *p >= 127) && *p != '\n' &&
                 *p != '\t') {
        if (*p == 127) {
          printf("^?");
        } else {
          printf("^%c", *p + '@');
        }
      } else {
        putchar(*p);
      }
    }
  }
}

void parseArguments(int argc, char *argv[], opt_t *opt) {
  static struct option long_options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 0}};

  int options;
  while ((options = getopt_long(argc, argv, "enstTEbv", long_options, NULL)) !=
         -1) {
    switch (options) {
      case 'e':
        opt->flags->flagV = 1;
        opt->flags->flagE = 1;
        break;
      case 'n':
        opt->flags->flagN = 1;
        break;
      case 's':
        opt->flags->flagS = 1;
        break;
      case 'T':
        opt->flags->flagT = 1;
        break;
      case 'b':
        opt->flags->flagB = 1;
        break;
      case 'v':
        opt->flags->flagV = 1;
        break;
      case 'E':
        opt->flags->flagE = 1;
        break;
      case 't':
        opt->flags->flagV = 1;
        opt->flags->flagT = 1;
        break;
      default:
        fprintf(stderr,
                "Usage: %s [-e] [-n] [-s] [-t] [-b] [-v] [-T] [-E] file_name\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (optind < argc) {
    *opt->filename = argv[optind];
  } else {
    fprintf(stderr, "Expected file name after options\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = {0};
  char *filename = NULL;

  opt_t opt = {&flags, &filename};

  parseArguments(argc, argv, &opt);

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }

  processFile(file, flags);

  fclose(file);
  return 0;
}
