#ifndef S21_GREP_H
#define S21_GREP_H

typedef struct arguments {
  int flagE, flagI, flagC, flagV, flagL, flagN, flagH, flagS, flagO, flagF;
  int lenPattern;
  char pattern[4096];
} arguments;

#endif
