#ifndef FLAGS_H
#define FLAGS_H

typedef struct {
int flagS;
int flagB;
int flagN;
int flagE;
int flagV;
int flagT;
} Flags;

typedef struct {
    Flags *flags;
    char **filename;
} opt_t;

#endif
