/**
 * @file insert_leakcheck.c
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief Source module to test leak check on insert and destroy
 * @version 0.1
 * @date 2021-01-11
 *
 * @copyright Copyright (c) 2021 Carl Mattatall
 *
 */
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "hash_table.h"


static const char *keys[] = {
    "key1a",  "key2a",  "key3a",  "key4a",  "key5a",  "key6a",  "key7a",
    "key8a",  "key9a",  "key10a", "key11a", "key12a", "key13a", "key1b",
    "key2b",  "key3b",  "key4b",  "key5b",  "key6b",  "key7b",  "key8b",
    "key9b",  "key10b", "key11b", "key12b", "key13b", "key1c",  "key2c",
    "key3c",  "key4c",  "key5c",  "key6c",  "key7c",  "key8c",  "key9c",
    "key10c", "key11c", "key12c", "key13c",
};

static const char *vals[] = {
    "val1a",  "val2a",  "val3a",  "val4a",  "val5a",  "val6a",  "val7a",
    "val8a",  "val9a",  "val10a", "val11a", "val12a", "val13a", "val1b",
    "val2b",  "val3b",  "val4b",  "val5b",  "val6b",  "val7b",  "val8b",
    "val9b",  "val10b", "val11b", "val12b", "val13b", "val1c",  "val2c",
    "val3c",  "val4c",  "val5c",  "val6c",  "val7c",  "val8c",  "val9c",
    "val10c", "val11c", "val12c", "val13c",
};


int main(void)
{
    assert((sizeof(vals) / sizeof(*vals)) == (sizeof(keys) / sizeof(*keys)));
    htbl_handle  tbl = htbl_ctor(10);
    unsigned int i;
    unsigned int max_i = sizeof(vals) / sizeof(*vals);
    for (i = 0; i < max_i; i++)
    {
        char *key = malloc(strlen(keys[i]) + 1);
        strcpy(key, ""); /* append \0 */

        char *val = malloc(strlen(vals[i]) + 1);
        strcpy(val, ""); /* append \0 */
        htbl_insert(tbl, key, htbl_value_ctor(val, free));
        free(key);
    }
    htbl_dtor(tbl);
    return 0;
}