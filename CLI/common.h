/*
 * Copyright 2022 Orange
 * Copyright 2022 Warsaw University of Technology
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include "../include/psabpf.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <jansson.h>

#ifndef P4C_COMMON_H
#define P4C_COMMON_H

#define NEXT_ARG()	({ argc--; argv++; if (argc < 0) fprintf(stderr, "too few parameters\n"); })
#define NEXT_ARGP()	({ (*argc)--; (*argv)++; if (*argc < 0) fprintf(stderr, "too few parameters\n"); })

#define NEXT_ARG_EXIT()  ({ argc--; argv++; if (argc < 1) { fprintf(stderr, "too few parameters\n"); exit(1); }})
#define NEXT_ARGP_EXIT() ({ (*argc)--; (*argv)++; if ((*argc) < 1) { fprintf(stderr, "too few parameters\n"); exit(1); }})

#define NEXT_ARG_RET()  ({ argc--; argv++; if (argc < 1) { fprintf(stderr, "too few parameters\n"); return 1; }})
#define NEXT_ARGP_RET()  ({ (*argc)--; (*argv)++; if ((*argc) < 1) { fprintf(stderr, "too few parameters\n"); return 1; }})

struct cmd {
    const char *cmd;
    int (*func)(int argc, char **argv);
};

typedef struct parser_keyword_value_pair {
    const char *keyword;
    void *destination;
    unsigned dst_size;
    bool required;
    const char *comment;  /* used to print error message */
} parser_keyword_value_pair_t;

bool is_keyword(const char *word, const char *str);

int parse_pipeline_id(int *argc, char ***argv, psabpf_context_t * psabpf_ctx);

/* Optional values are not written when they are missing on command line, so they must be initialized */
int parse_keyword_value_pairs(int *argc, char ***argv, parser_keyword_value_pair_t *kv_pairs);

typedef psabpf_struct_field_t *(*get_next_field_func_t)(void*, void*);
int build_struct_json(void *json_parent, void *ctx, void *entry, get_next_field_func_t get_next_field);

extern const char *program_name;

enum destination_ctx_type_t {
    CTX_MATCH_KEY,
    CTX_MATCH_KEY_TERNARY_MASK,
    CTX_ACTION_DATA,
    CTX_METER_INDEX,
    CTX_COUNTER_KEY,
    CTX_REGISTER_INDEX,
    CTX_REGISTER_DATA,
    CTX_VALUE_SET_DATA,
};

int translate_data_to_bytes(const char *data, void *ctx, enum destination_ctx_type_t ctx_type);
char * convert_bin_data_to_hexstr(const void *data, size_t len);
json_t *create_json_entry_key(psabpf_table_entry_t *entry);
int parse_key_data(int *argc, char ***argv, psabpf_table_entry_t *entry);

/* parent - JSON object, object - any JSON type, index - converted to string */
int set_json_object_at_index(json_t *parent, json_t *object, uint32_t index);

#endif //P4C_COMMON_H
