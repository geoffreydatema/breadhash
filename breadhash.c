/*------------------------------------------------------------------------------------------------------|
                                                                                                        |
                                        @@@.    =@@@@@=                                                 |
                             +@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                         |
                            =@%===+++++%@@++%@@@@@@@@@#*@@@@@@@@@@@                                     |
                     =#*=====-%@@@@%*=:+@@@@@@@@@+-:%@@@@@@@@:@@@@@@@@@@@@@@@                           |
                   .-+.: ..  ..        +#-%@@@*: --::=**+#%@@@@= @@@@@@@@@@@@@@@                        |
                ..::. ++-:             =###=-.  . ::  ...::..-@#.:%@#=:.=@-%@@@@@@                      |
              .. .       .             .:.    ::         ....-@*.-%* ..  =@=   @@@@@@                   |
           ...         ..                   .::        ..   :=@#*%%=..    =%*#*-  #@@@@                 |
         ..                                           :     .:#***+-      :+*##+:   @@@@@:              |
        .                                             ..      =- ..          .:.=#*#*+  @@@@@           |
      ..                                                      :=-:.             -*###@*  +@@@#          |
     .                                                         .-:                 ...*%    @@@@        |
    .                                                                                =-+%     @@@       |
    .                                                                               :+-=*      @@@      |
   ..                                                                                  -#+=*=   #@@     |
    .                                          breadhash                                ..:#*##*:*@     |
    ..                                            0.1                                    ..+#+-#%:@     |
    :.                                                                                      ...-@*@     |
     .:  .                                                                                     %%@      |
     .==-.                                                                                   .#+@       |
       =+---:                                                                              .=:@@        |
          .-=-:                                                                         ===.@@@         |
             :=+=-                                                                    ::..@@@-          |
               :=**+++: ::.                                                           ..*@@@-           |
                  =***%#****+:                 ..                               .      %@@@             |
                        +@@@@@@@%@@+.  ..                                 .--:    .@@@@-                |
                               *@@@@@@@@@@#-...  ..                   .--::-- *@@@@@@#                  |
                                        -@@@@@@@@@@@@*   :#@%*=:-#@@@@@@@@@@@@@@#                       |
                                              =%@@@@@@@@@@@@@@@@@@@@@@@@@@@@-                           |
                                                    .#@@@@+:-*@@@*:                                     |
                                                                                                        |
-------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

typedef enum {
    INPUTSTRING,
    INPUTFILE,
    INPUTUNIMPLEMENTED
} InputMode;

typedef enum {
    OPBASE92,
    OPUNIMPLEMENTED
} OpMode;

typedef struct {
    InputMode input_mode;
    OpMode opmode;
    const char *data;
} BreadhashConfig;

void run_operation(BreadhashConfig *config) {
    if (config->opmode == OPBASE92) {
        if (config->input_mode == INPUTSTRING) {
            printf("Running base92 hash on string: %s\n", config->data);
            // call your base92 hashing logic here, passing config->data
        } else if (config->input_mode == INPUTFILE) {
            fprintf(stderr, "File input not implemented yet.\n");
        } else {
            fprintf(stderr, "Unknown input mode.\n");
        }
    } else {
        fprintf(stderr, "Unknown operation mode.\n");
    }
}

int main(int argc, char *argv[]) {

    // proper usage from the command line would pass in 4 args, so enforce this
    if (argc != 4) {
        fprintf(stderr,
            "Usage:\n"
            "    <input mode> <operation> <data>\n\n"
            "Available input modes:\n"
            "    --file --string\n\n"
            "Available operations:\n"
            "    --base92\n\n"
        );
        return 1;
    }

    BreadhashConfig config;
    config.input_mode = INPUTUNIMPLEMENTED;
    config.opmode = OPUNIMPLEMENTED;
    config.data = argv[3];

    // parse input mode (string or file)
    if (strcmp(argv[1], "--string") == 0) {
        config.input_mode = INPUTSTRING;
    } else if (strcmp(argv[1], "--file") == 0) {
        config.input_mode = INPUTFILE;
    }
    else {
        fprintf(stderr, "Unknown input mode: %s\n", argv[1]);
        return 1;
    }

    // parse operation mode
    if (strcmp(argv[2], "--base92") == 0) {
        config.opmode = OPBASE92;
    } else {
        fprintf(stderr, "Unknown operation: %s\n", argv[2]);
        return 1;
    }

    run_operation(&config);
    return 0;
}