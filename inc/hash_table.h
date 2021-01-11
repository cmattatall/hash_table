#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__
#ifdef __cplusplus
/* clang-format off */
extern "C"
{
/* clang-format on */
#endif /* Start C linkage */

typedef void *             htbl_handle;
typedef struct htbl_value *htbl_value;

htbl_handle htbl_ctor(unsigned int size);
void        htbl_dtor(htbl_handle handle);

htbl_value htbl_value_new(void *value, void (*delete)(void *));
void       htbl_insert(htbl_handle handle, const char *key, htbl_value value);


#ifdef __cplusplus
/* clang-format off */
}
/* clang-format on */
#endif /* End C linkage */
#endif /* __HASH_TABLE_H__ */