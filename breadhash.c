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
    ..                                                                                   ..+#+-#%:@     |
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

// {"!":0,"#":1,"$":2,"%":3,"&":4,"'":5,"(":6,")":7,"*":8,"+":9,",":10,"-":11,".":12,"/":13,"0":14,"1":15,"2":16,"3":17,"4":18,"5":19,"6":20,"7":21,"8":22,"9":23,":":24,";":25,"<":26,"=":27,">":28,"?":29,"@":30,"A":31,"B":32,"C":33,"D":34,"E":35,"F":36,"G":37,"H":38,"I":39,"J":40,"K":41,"L":42,"M":43,"N":44,"O":45,"P":46,"Q":47,"R":48,"S":49,"T":50,"U":51,"V":52,"W":53,"X":54,"Y":55,"Z":56,"[":57,"]":58,"^":59,"_":60,"`":61,"a":62,"b":63,"c":64,"d":65,"e":66,"f":67,"g":68,"h":69,"i":70,"j":71,"k":72,"l":73,"m":74,"n":75,"o":76,"p":77,"q":78,"r":79,"s":80,"t":81,"u":82,"v":83,"w":84,"x":85,"y":86,"z":87,"{":88,"|":89,"}":90,"~":91}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_HASH_LENGTH 64

typedef enum {
    INPUTINTEGER,
    INPUTSTRING,
    INPUTFILE,
    INPUTUNIMPLEMENTED
} InputMode;

typedef enum {
    OPBASE92,
    OPFROMBASE92,
    OPNAIVEADDDEC,
    OPNAIVEADDBASE92,
    OPORDSTACKDEC,
    OPORDSTACKBASE92,
    OPWRAPMINIHASH,
    OPBREADHASH8,
    OPBREADHASH16,
    OPBREADHASH32,
    OPBREADHASH64,
    OPFRAGHASH64_8,
    OPUNIMPLEMENTED
} OpMode;

typedef struct {
    InputMode input_mode;
    OpMode opmode;
    char *data;
} BreadhashConfig;

static const char base92_chars[] = {'!','#','$','%','&','\'','(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','[',']','^','_','`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','{','|','}','~'};

int base92_charmap[128] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,-1,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,-1};

char *base92(unsigned int value) {
    if (value == 0) {
        char *zero_str = malloc(2);
        if (!zero_str) return NULL;
        zero_str[0] = base92_chars[0];
        zero_str[1] = '\0';
        return zero_str;
    }

    char buffer[32];
    int index = sizeof(buffer) - 1;
    buffer[index--] = '\0';

    while (value > 0 && index >= 0) {
        buffer[index--] = base92_chars[value % 92];
        value /= 92;
    }

    char *result = malloc(strlen(&buffer[index + 1]) + 1);
    if (!result) return NULL;
    strcpy(result, &buffer[index + 1]);
    return result;
}

char base92_char(unsigned int value) {
    if (value >= 92) {
        fprintf(stderr, "Error: base92_char() received out-of-range value: %u (must be 0-91)\n", value);
        return '!';
    }
    return base92_chars[value];
}


char *int_to_base92(const char *data) {
    return base92(atoi(data));
}

char *str_to_base92(const char *data) {
    size_t input_length = strlen(data);
    char *result = malloc(2 * input_length + 1);
    if (!result) return NULL;

    result[0] = '\0';
    char *write_ptr = result;

    for (size_t i = 0; i < input_length; i++) {
        char *base92char = base92((unsigned char)data[i]);
        size_t current_length = strlen(base92char);

        memcpy(write_ptr, base92char, current_length);
        write_ptr += current_length;
        free(base92char);
    }
    *write_ptr = '\0';

    return result;
}

char *ordinal_stack(const char *input) {
    size_t len = strlen(input);

    size_t max_len = len * 3 + 1;
    char *result = malloc(max_len);
    if (!result) return NULL;

    char *write_ptr = result;
    for (size_t i = 0; i < len; i++) {
        int written = sprintf(write_ptr, "%u", (unsigned char)input[i]);
        write_ptr += written;
    }

    return result;
}

int from_base92(const char *data) {
    int result = 0;
    size_t len = strlen(data);

    for (size_t i = 0; i < len; i++) {
        char c = data[i];
        int value = base92_charmap[(unsigned char)c];
        if (value == -1) {
            fprintf(stderr, "Invalid base92 character: '%c'\n", c);
            return -1;
        }
        result = result * 92 + value;
    }

    return result;
}

int naive_additive_decimal(const char *input) {
    int sum = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        sum += (unsigned char)input[i];
    }
    return sum;
};

uint32_t minihash(const char *input) {
    unsigned int hash = UINT32_MAX;

    for (size_t i = 0; input[i] != '\0'; i++) {
        hash = ((hash << 4) + hash) * (unsigned char)input[i] + 69;
    }

    return hash;
}

char *breadhash(const char *input, int hash_length) {
    size_t input_length = strlen(input);
    if (hash_length <= 0 || hash_length > MAX_HASH_LENGTH || input_length == 0) return NULL;

    unsigned char *result = calloc(hash_length + 1, sizeof(char));
    if (!result) return NULL;

    unsigned char entropy[MAX_HASH_LENGTH] = {0};
    int filled = 0;
    int step = input_length / hash_length;
    if (step == 0) step = 1;

    int current_step = step;
    int round = 0;

    while (filled < MAX_HASH_LENGTH) {
        for (size_t i = 0; i < input_length && filled < MAX_HASH_LENGTH; i += current_step) {
            entropy[filled++] = (unsigned char)input[i];
        }
        current_step++;
        round++;
        if (current_step > input_length) current_step = 1 + (round % input_length);
    }

    memcpy(result, entropy, hash_length);
    result[hash_length] = '\0';

    uint32_t bread = UINT32_MAX / 2;
    for (int i = 0; input[i] != '\0'; i++) {
        bread = (bread * 42) ^ input[i];
        for (int j = 0; j < hash_length; j++) {
            result[j] = base92_char((result[j] * bread + ((input[i] ^ (i * 42 + j * 69)) % 256)) % 92);
        }
    }

    for (int i = input_length - 1; i >= 0; i--) {
        bread = (bread * 69) ^ input[i];
        for (int j = hash_length - 1; j >= 0; j--) {
            result[j] = base92_char((result[j] + (bread * ((j + 42) % 69))) % 92);
        }
    }

    return (char *)result;
}

char **fraghash(const char *input, int num_fragments, int hash_length) {
    size_t input_length = strlen(input);
    if (num_fragments <= 0 || hash_length <= 0 || input_length == 0) return NULL;

    if (input_length < (size_t)num_fragments) {
        fprintf(stderr, "Error: Input length (%zu) is too short to split into %i fragments.\n", input_length, num_fragments);
        return NULL;
    }
    
    char **hashes = calloc(num_fragments, sizeof(char *));
    if (!hashes) return NULL;

    size_t base_fragment_size = input_length / num_fragments;
    size_t offset = 0;

    for (int i = 0; i < num_fragments; i++) {
        size_t frag_size = (i == num_fragments - 1) ? (input_length - offset) : base_fragment_size;

        char *fragment = calloc(frag_size + 1, sizeof(char));
        if (!fragment) {
            for (int j = 0; j < i; j++) free(hashes[j]);
            free(hashes);
            return NULL;
        }

        memcpy(fragment, input + offset, frag_size);
        fragment[frag_size] = '\0';
        offset += frag_size;

        hashes[i] = breadhash(fragment, hash_length);
        free(fragment);

        if (!hashes[i]) {
            for (int j = 0; j <= i; j++) free(hashes[j]);
            free(hashes);
            return NULL;
        }
    }

    return hashes;
}

void print_hash_block(const char *hash, int hash_length, int line_width) {
    for (int i = 0; i < hash_length; i++) {
        putchar(hash[i]);
        if ((i + 1) % line_width == 0)
            putchar('\n');
    }
}

void run_operation(BreadhashConfig *config) {
    if (config->opmode == OPBASE92) {
        if (config->input_mode == INPUTINTEGER) {
            char *result = int_to_base92(config->data);
            printf("%s", result);
            free(result);
        } else if (config->input_mode == INPUTSTRING) {
            char *result = str_to_base92(config->data);
            printf("%s", result);
            free(result);
        } else if (config->input_mode == INPUTFILE) {
            char *result = str_to_base92(config->data);
            printf("%s", result);
            free(result);
        } else {
            fprintf(stderr, "Unknown input mode.\n");
        }
    } else if (config->opmode == OPFROMBASE92) {
        if (config->input_mode == INPUTSTRING) {
            int result = from_base92(config->data);
            printf("%i", result);
        }
    } else if (config->opmode == OPNAIVEADDDEC) {
        if (config->input_mode != INPUTINTEGER) {
            int result = naive_additive_decimal(config->data);
            printf("%i", result);
        }
    } else if (config->opmode == OPNAIVEADDBASE92) {
        if (config->input_mode != INPUTINTEGER) {
            char *result = base92(naive_additive_decimal(config->data));
            printf("%s", result);
            free(result);
        }
    } else if (config->opmode == OPORDSTACKDEC) {
        if (config->input_mode != INPUTINTEGER) {
            char *result = ordinal_stack(config->data);
            printf("%s", result);
            free(result);
        }
    } else if (config->opmode == OPORDSTACKBASE92) {
        if (config->input_mode != INPUTINTEGER) {
            char *result = str_to_base92(config->data);
            printf("%s", result);
            free(result);
        }
    } else if (config->opmode == OPWRAPMINIHASH) {
        if (config->input_mode != INPUTINTEGER) {
            uint32_t result = minihash(config->data);
            printf("%u", result);
        }
    } else if (config->opmode == OPBREADHASH8) {
        if (config->input_mode != INPUTINTEGER) {
            char *result = breadhash(config->data, 8);
            printf("%s", result);
            free(result);
        }
    } else if (config->opmode == OPBREADHASH16) {
        if (config->input_mode != INPUTINTEGER) {
            char *result = breadhash(config->data, 16);
            printf("%s", result);
            free(result);
        }
    } else if (config->opmode == OPBREADHASH32) {
        if (config->input_mode != INPUTINTEGER) {
            char *result = breadhash(config->data, 32);
            printf("%s", result);
            free(result);
        }
    } else if (config->opmode == OPBREADHASH64) {
        if (config->input_mode != INPUTINTEGER) {
            char *result = breadhash(config->data, 64);
            printf("%s", result);
            free(result);
        }
    } else if (config->opmode == OPFRAGHASH64_8) {
        if (config->input_mode != INPUTINTEGER) {
            char **results = fraghash(config->data, 8, 64);
            if (results) {
                for (int i = 0; i < 8; i++) {
                    print_hash_block(results[i], 64, 16);
                    putchar('\n');
                    free(results[i]);
                }
                free(results);
            }
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
            "    --base92 --from-base92\n"
            "    --naive-additive-decimal --naive-additive-base92\n"
            "    --ordinal-stack-decimal --ordinal-stack-base92\n"
            "    --minihash\n"
            "    --breadhash8 --breadhash16\n"
            "    --breadhash32 --breadhash64\n"
            "    --fraghash64-8\n\n"
        );
        return 1;
    }

    BreadhashConfig config;
    config.input_mode = INPUTUNIMPLEMENTED;
    config.opmode = OPUNIMPLEMENTED;

    // parse input mode (string or file)
    if (strcmp(argv[1], "--int") == 0) {
        config.input_mode = INPUTINTEGER;
        config.data = argv[3];
    } else if (strcmp(argv[1], "--str") == 0) {
        config.input_mode = INPUTSTRING;
        config.data = argv[3];
    } else if (strcmp(argv[1], "--file") == 0) {
        config.input_mode = INPUTFILE;

        FILE *file = fopen(argv[3], "rb");
        if (!file) {
            fprintf(stderr, "Error opening file: %s\n", argv[2]);
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        config.data = malloc(file_size + 1);
        if (!config.data) {
            fprintf(stderr, "Memory allocation failed for file buffer.\n");
            fclose(file);
            return 1;
        }

        fread(config.data, 1, file_size, file);
        config.data[file_size] = '\0';

        fclose(file);
    }
    else {
        fprintf(stderr, "Unknown input mode: %s\n", argv[1]);
        return 1;
    }

    // parse operation mode
    if (strcmp(argv[2], "--base92") == 0) {
        config.opmode = OPBASE92;
    } else if (strcmp(argv[2], "--from-base92") == 0) {
        config.opmode = OPFROMBASE92;
    } else if (strcmp(argv[2], "--naive-additive-decimal") == 0) {
        config.opmode = OPNAIVEADDDEC;
    } else if (strcmp(argv[2], "--naive-additive-base92") == 0) {
        config.opmode = OPNAIVEADDBASE92;
    } else if (strcmp(argv[2], "--ordinal-stack-decimal") == 0) {
        config.opmode = OPORDSTACKDEC;
    } else if (strcmp(argv[2], "--ordinal-stack-base92") == 0) {
        config.opmode = OPORDSTACKBASE92;
    } else if (strcmp(argv[2], "--minihash") == 0) {
        config.opmode = OPWRAPMINIHASH;
    } else if (strcmp(argv[2], "--breadhash8") == 0) {
        config.opmode = OPBREADHASH8;
    } else if (strcmp(argv[2], "--breadhash16") == 0) {
        config.opmode = OPBREADHASH16;
    } else if (strcmp(argv[2], "--breadhash32") == 0) {
        config.opmode = OPBREADHASH32;
    } else if (strcmp(argv[2], "--breadhash64") == 0) {
        config.opmode = OPBREADHASH64;
    } else if (strcmp(argv[2], "--breadhash64-8") == 0) {
        config.opmode = OPFRAGHASH64_8;
    } else {
        fprintf(stderr, "Unknown operation: %s\n", argv[2]);
        return 1;
    }

    run_operation(&config);

    if (config.input_mode == INPUTFILE) {
        free(config.data);
    }
    return 0;
}
