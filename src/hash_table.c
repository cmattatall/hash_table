
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

typedef struct htbl_node htbl_node;

struct htbl_node
{
    char              key[50];
    htbl_value        value;
    struct htbl_node *next;
};


struct htbl
{
    htbl_node *  nodes;
    unsigned int size;
};

struct htbl_value
{
    void *value;
    void (*delete)(void *);
};

static int        hashCode(const char *s);
static htbl_node  empty_htbl_node(void);
static htbl_node *traverse(const htbl_node *node);


htbl_value htbl_value_new(void *value, void (*delete)(void *))
{
    htbl_value valstruct;
    valstruct->delete = delete;
    valstruct->value  = value;
    return valstruct;
}


int htbl_insert(htbl_handle handle, const char *key, htbl_value value)
{
    struct htbl *tbl = (struct htbl *)handle;

    int idx = hashCode(key);
    idx     = idx % tbl->size;
}


htbl_handle htbl_ctor(unsigned int size)
{
    htbl_handle handle;
    handle           = malloc(size);
    struct htbl *tbl = (struct htbl *)handle;
    if (tbl != NULL)
    {
        tbl->size = size;
    }

    unsigned int i;
    for (i = 0; i < tbl->size; i++)
    {
        tbl->nodes[i] = empty_htbl_node();
    }
    return (htbl_handle)tbl;
}


void htbl_dtor(htbl_handle handle)
{
    struct htbl *tbl = (struct htbl *)handle;

    unsigned int i;
    htbl_node *  current_node;
    htbl_node *  tmp;
    for (i = 0; i < tbl->size; i++)
    {
        current_node = &tbl->nodes[i];
        while (current_node->next != NULL)
        {
            tmp          = current_node;
            current_node = current_node->next;

            /* If value has a destructor, call it */
            if (tmp->value->delete != NULL)
            {
                tmp->value->delete (tmp->value->value);
            }

            /* Free the last node we traversed */
            free(tmp);
        }
    }
}


static int hashCode(const char *s)
{
    int h = 0;
    while (*s)
    {
        h = 31 * h + (*s++);
    }
    return h;
}


static htbl_node empty_htbl_node(void)
{
    htbl_node node;
    strcpy(node.key, "");
    node.next  = NULL;
    node.value = NULL;
    return node;
}
