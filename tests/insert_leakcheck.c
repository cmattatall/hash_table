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

#include "hash_table.h"


int main(void)
{
    htbl_handle tbl = htbl_ctor(10);

    char *value = malloc(10);
    strcpy(value, "ThisIsAValue");

    htbl_value val = htbl_value_ctor(value, free);
    htbl_insert(tbl, "myKey", val);
    htbl_dtor(tbl);
    return 0;
}