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
#include <stdlib.h>
#include <string.h>

typedef enum {
    INPUTINTEGER,
    INPUTSTRING,
    INPUTFILE,
    INPUTUNIMPLEMENTED
} InputMode;

typedef enum {
    OPBASE92,
    OPFROMBASE92,
    OPUNIMPLEMENTED
} OpMode;

typedef struct {
    InputMode input_mode;
    OpMode opmode;
    const char *data;
} BreadhashConfig;

const char base92_chars[] = {'!','#','$','%','&','\'','(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','[',']','^','_','`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','{','|','}','~'};

int base92_charmap[128] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,-1,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,-1};

char *base92(unsigned int v) {
    if (v == 0) {
        char *zero_str = malloc(2);
        zero_str[0] = base92_chars[0];
        zero_str[1] = '\0';
        return zero_str;
    }

    char buffer[32];
    int index = sizeof(buffer) - 1;
    buffer[index--] = '\0';

    while (v > 0 && index >= 0) {
        buffer[index--] = base92_chars[v % 92];
        v /= 92;
    }

    char *result = malloc(strlen(&buffer[index + 1]) + 1);
    strcpy(result, &buffer[index + 1]);
    return result;
}

void int_to_base92(const char *data) {
    char *base92char = base92(atoi(data));
    printf("%s", base92char);
    free(base92char);
}

void str_to_base92(const char *data) {
    for (int i = 0; i < strlen(data); i++) {
        char *base92char = base92(data[i]);
        printf("%s", base92char);
        free(base92char);
    }
}

void frombase92(const char *data) {

    int result = 0;
    int len = strlen(data);

    for (int i = 0; i < len; i++) {
        char c = data[i];
        int value = base92_charmap[(int)c];
        if (value == -1) {
            fprintf(stderr, "Invalid base92 character: '%c'\n", c);
            return;
        }
        result = result * 92 + value;
    }

    printf("%i", result);
}

void run_operation(BreadhashConfig *config) {
    if (config->opmode == OPBASE92) {
        if (config->input_mode == INPUTINTEGER) {
            int_to_base92(config->data);
        } else if (config->input_mode == INPUTSTRING) {
            str_to_base92(config->data);
        } else if (config->input_mode == INPUTFILE) {
            fprintf(stderr, "File input not implemented yet.\n");
        } else {
            fprintf(stderr, "Unknown input mode.\n");
        }
    } else if (config->opmode == OPFROMBASE92) {
        if (config->input_mode == INPUTSTRING) {
            frombase92(config->data);
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
            "    --int --str --file\n\n"
            "Available operations:\n"
            "    --base92 --frombase92\n\n"
        );
        return 1;
    }

    BreadhashConfig config;
    config.input_mode = INPUTUNIMPLEMENTED;
    config.opmode = OPUNIMPLEMENTED;
    config.data = argv[3];

    // parse input mode (string or file)
    if (strcmp(argv[1], "--int") == 0) {
        config.input_mode = INPUTINTEGER;
    } else if (strcmp(argv[1], "--str") == 0) {
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
    } else if (strcmp(argv[2], "--frombase92") == 0) {
        config.opmode = OPFROMBASE92;
    } else {
        fprintf(stderr, "Unknown operation: %s\n", argv[2]);
        return 1;
    }

    run_operation(&config);
    return 0;
}