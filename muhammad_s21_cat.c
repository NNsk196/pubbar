void parseArguments(int argc, char *argv[], opt_t *opt) {
    static struct option long_options[] = {
        {"number-nonblank", no_argument, 0, 'b'},
        {"number", no_argument, 0, 'n'},
        {"squeeze-blank", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    int options;
    while ((options = getopt_long(argc, argv, "enstbv", long_options, NULL)) != -1) {
        switch (options) {
            case 'e':
                opt->flags->flagE = 1;
                break;
            case 'n':
                opt->flags->flagN = 1;
                break;
            case 's':
                opt->flags->flagS = 1;
                break;
            case 't':
                opt->flags->flagT = 1;
                break;
            case 'b':
                opt->flags->flagB = 1;
                break;
            case 'v':
                opt->flags->flagV = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-e] [-n] [-s] [-t] [-b] [-v] file_name\n", argv[0]);
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
