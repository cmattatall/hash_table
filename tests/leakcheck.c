/**
 * @file leakcheck.c
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief Test to check hashtable API for memory leaks
 * @version 0.1
 * @date 2021-01-11
 *
 * @copyright Copyright (c) 2021 Carl Mattatall
 *
 *
 * @note RUN WITH A MEMCHECK TOOL LIKE VALGRIND OR PURIFY
 */

#include "hash_table.h"


int main(void)
{
    htbl_handle tbl = htbl_ctor(10);
    htbl_dtor(tbl);
    return 0;
}