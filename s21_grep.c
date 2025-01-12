#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void patternAdd(arguments *arg, char *pattern) {
  if (arg->lenPattern != 0) {
    strcat(arg->pattern + arg->lenPattern, "|");
    arg->lenPattern++;
  }
  arg->lenPattern += sprintf(arg->pattern + arg->lenPattern, "(%s)", pattern);
}

void addRegFromFile(arguments *arg, char *filePath) {
  FILE *file = fopen(filePath, "r");

  if (file == NULL) {
    if (!arg->flagS) {
      perror(filePath);
      exit(EXIT_FAILURE);
    }
  }
  char *line = NULL;
  size_t memLen = 0;
  int read = getline(&line, &memLen, file);
  while (read != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    patternAdd(arg, line);
    read = getline(&line, &memLen, file);
  }
  free(line);
  fclose(file);
}

arguments argumentParser(int argc, char **argv) {
  arguments arg = {0};
  int opt;

  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        arg.flagE = 1;
        patternAdd(&arg, optarg);
        break;
      case 'i':
        arg.flagI = REG_ICASE;
        break;
      case 'v':
        arg.flagV = 1;
        break;
      case 'c':
        arg.flagC = 1;
        break;
      case 'l':
        arg.flagL = 1;
        break;
      case 'n':
        arg.flagN = 1;
        break;
      case 'h':
        arg.flagH = 1;
        break;
      case 'f':
        arg.flagF = 1;
        addRegFromFile(&arg, optarg);
        break;
      case 'o':
        arg.flagO = 1;
        break;
      default:
        break;
    }
  }
  if (arg.lenPattern == 0) {
    patternAdd(&arg, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) {
    arg.flagH = 1;
  }
  return arg;
}

void outputLine(char *line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') {
    putchar('\n');
  }
}

void printMath(regex_t *re, char *line) {
  regmatch_t math;
  int offset = 0;
  while (1) {
    int result = regexec(re, line + offset, 1, &math, 0);
    if (result != 0) {
      break;
    }
    for (int i = math.rm_so; i < math.rm_eo; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    offset += math.rm_eo;
  }
}

void processLine(arguments arg, char *filePath, regex_t *reg) {
  FILE *file = fopen(filePath, "r");
  if (file == NULL) {
    if (!arg.flagS) {
      perror(filePath);
      exit(EXIT_FAILURE);
    }
  }

  char *line = NULL;
  size_t memLen = 0;
  int read = 0;
  int lineNumber = 1;
  int count = 0;
  read = getline(&line, &memLen, file);
  while (read != -1) {
    int result = regexec(reg, line, 0, NULL, 0);

    if ((result == 0 && !arg.flagV) || (arg.flagV && result != 0)) {
      if (!arg.flagC && !arg.flagL) {
        if (!arg.flagH) {
          printf("%s:", filePath);
        }
        if (arg.flagN) {
          printf("%d:", lineNumber);
        }
        if (arg.flagO) {
          printMath(reg, line);
        } else {
          outputLine(line, read);
        }
      }
      count++;
    }
    read = getline(&line, &memLen, file);
    lineNumber++;
  }
  free(line);
  if (arg.flagC && !arg.flagL) {
    if (!arg.flagH) {
      printf("%s:", filePath);
    }
    printf("%d\n", count);
  }
  if (arg.flagL && count > 0) {
    printf("%s\n", filePath);
  }
  fclose(file);
}

void outputFunction(arguments arg, int argc, char **argv) {
  regex_t re;
  int error = regcomp(&re, arg.pattern, REG_EXTENDED | arg.flagI);
  if (error) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  for (int i = optind; i < argc; i++) {
    processLine(arg, argv[i], &re);
  }
  regfree(&re);
}

int main(int argc, char **argv) {
  arguments arg = argumentParser(argc, argv);
  outputFunction(arg, argc, argv);
  return 0;
}
