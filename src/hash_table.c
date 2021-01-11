
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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


typedef struct
{
    htbl_node *curr;
    htbl_node *prev;
} htbl_node_searcher;

static int       hash_code(const struct htbl *tbl, const char *s);
static htbl_node empty_htbl_node(void);

static htbl_node_searcher find_key(const struct htbl *tbl, const char *key);
static htbl_node *        node_ctor(void);
static void               node_dtor(htbl_node *node);
static bool               key_in_tbl(const struct htbl *tbl, const char *key);


htbl_value htbl_value_new(void *value, void (*delete)(void *))
{
    htbl_value valstruct;
    valstruct->delete = delete;
    valstruct->value  = value;
    return valstruct;
}


void htbl_insert(htbl_handle handle, const char *key, htbl_value value)
{
    struct htbl *tbl = (struct htbl *)handle;

    htbl_node_searcher searcher = find_key(tbl, key);

    /* Key doesn't exist in table */
    if (searcher.curr == NULL)
    {
        searcher.curr       = node_ctor();
        searcher.prev->next = searcher.curr;
    }
    searcher.curr->value = value;
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
            node_dtor(tmp);
        }
    }
}


static int hash_code(const struct htbl *tbl, const char *s)
{
    int h = 0;
    while (*s)
    {
        h = 31 * h + (*s++);
    }
    h = h % tbl->size;
    return h;
}


static htbl_node empty_htbl_node(void)
{
    htbl_node node;
    strcpy(node.key, "");
    node.next          = NULL;
    node.value->value  = NULL;
    node.value->delete = NULL;
    return node;
}


static htbl_node_searcher find_key(const struct htbl *tbl, const char *key)
{
    int                key_idx = hash_code(tbl, key);
    htbl_node_searcher searcher;
    searcher.curr = &tbl->nodes[key_idx];
    searcher.prev = NULL;

    while (searcher.curr != NULL)
    {
        if (0 == strcmp(searcher.curr->key, key))
        {
            break;
        }
        else
        {
            searcher.curr = searcher.curr->next;
            if (searcher.prev == NULL)
            {
                searcher.prev = searcher.curr;
            }
        }
    }

    return searcher;
}


static htbl_node *node_ctor(void)
{
    htbl_node *node;
    node  = malloc(sizeof(htbl_node));
    *node = empty_htbl_node();
    return node;
}


static void node_dtor(htbl_node *node)
{
    /* If value has a destructor, call it */
    if (node->value->delete != NULL)
    {
        node->value->delete (node->value->value);
    }
    free(node);
}
