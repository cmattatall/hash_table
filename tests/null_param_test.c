/**
 * @file null_param_test.c
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief Source module to test if API is hardened against NULL params
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

int main(void)
{
    htbl_handle tbl = htbl_ctor(10);
    htbl_insert(tbl, NULL, htbl_value_ctor("myValue1", NULL));
    htbl_dtor(tbl);
    return 0;
}