
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hash_table.h"

typedef struct htbl_node htbl_node;

struct htbl_node
{
    char *            key;
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
    void (*dtor)(void *);
};


typedef struct
{
    htbl_node *dest;
    htbl_node *parent;
} htbl_node_searcher;

static int                hash_code(const struct htbl *tbl, const char *s);
static htbl_node_searcher find_key(const struct htbl *tbl, const char *key);
static htbl_node *        node_ctor(const char *key, unsigned int keylen);
static void               node_dtor(htbl_node *node);
static void               node_update(htbl_node *node, htbl_value value);
static bool               key_in_tbl(const struct htbl *tbl, const char *key);
static void               htbl_value_dtor(htbl_value value);


int htbl_remove(htbl_handle handle, const char *key)
{
    int          status = 1;
    struct htbl *tbl    = (struct htbl *)handle;

    htbl_node_searcher searcher = find_key(tbl, key);

    /* If node is in the table */
    if (searcher.dest != NULL)
    {
        searcher.parent->next = searcher.dest->next;
        node_dtor(searcher.dest);
        status = 0;
    }
    return status;
}


htbl_value htbl_value_ctor(void *value, void (*dtor)(void *))
{
    htbl_value valstruct = malloc(sizeof(*valstruct));
    if (valstruct != NULL)
    {
        valstruct->dtor  = dtor;
        valstruct->value = value;
    }
    return valstruct;
}


void htbl_insert(htbl_handle handle, const char *key, htbl_value value)
{
    struct htbl *tbl = (struct htbl *)handle;

    htbl_node_searcher searcher = find_key(tbl, key);

    if (searcher.parent == NULL)
    {
        /* First item in chain (no collisions), dont need to construct */
        if (key != NULL)
        {
            unsigned int len   = strlen(key) + 1;
            searcher.dest->key = malloc(len);
            strncpy(searcher.dest->key, key, len);
        }
    }
    else if (searcher.dest == NULL)
    {
        /* no match, key not in table. We have to add it */
        if (key != NULL)
        {
            unsigned int len = strlen(key) + 1;
            searcher.dest    = node_ctor(key, len);
        }

        /* Link parent node to child node */
        searcher.parent->next = searcher.dest;
    }
    searcher.dest->value = value;
}


int htbl_update(htbl_handle handle, const char *key, htbl_value value)
{
    int                status   = 0;
    struct htbl *      tbl      = (struct htbl *)handle;
    htbl_node_searcher searcher = find_key(tbl, key);
    if (searcher.dest == NULL && searcher.parent == NULL)
    {
        /* Key doesn't exist in table */
        status = 1;
    }
    else
    {
        node_update(searcher.dest, value);
    }
    return status;
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

    tbl->nodes = malloc(tbl->size * sizeof(*(tbl->nodes)));
    if (tbl->nodes == NULL)
    {
        free(tbl);
    }
    else
    {
        unsigned int i;
        for (i = 0; i < tbl->size; i++)
        {
            tbl->nodes[i].key   = NULL;
            tbl->nodes[i].next  = NULL;
            tbl->nodes[i].value = NULL;
        }
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
    int idx;
    if (tbl == NULL || s == NULL)
    {
        idx = -1;
    }
    else
    {
        int h = 0;
        while (*s)
        {
            h = 31 * h + (*s++);
        }
        idx = h % tbl->size;
    }
    return idx;
}


static void reset_htbl_node(htbl_node *node)
{
    if (node != NULL)
    {
        if (node->key != NULL)
        {
            strcpy(node->key, "");
        }
        node->next  = NULL;
        node->value = htbl_value_ctor(NULL, NULL);
    }
}


static htbl_node_searcher find_key(const struct htbl *tbl, const char *key)
{
    int                key_idx = hash_code(tbl, key);
    htbl_node_searcher searcher;
    if (key_idx == -1)
    {
        searcher.dest   = NULL;
        searcher.parent = NULL;
    }
    else
    {
        searcher.dest   = &tbl->nodes[key_idx];
        searcher.parent = NULL;

        /* Case of 0 collisions */
        if (searcher.dest->key == NULL)
        {
            return searcher;
        }
        else
        {
            do
            {
                if (searcher.dest->key != NULL)
                {
                    /* If we match the key */
                    if ((0 == strcmp(searcher.dest->key, key)))
                    {
                        break;
                    }
                }

                searcher.parent = searcher.dest;
                searcher.dest   = searcher.dest->next;
            } while (searcher.dest != NULL);


#if 0
            while (true)
            {
                /* If we haven't walked to the end of the chain */
                if (searcher.dest != NULL)
                {
                    if (searcher.dest->key != NULL)
                    {
                        /* If we match the key */
                        if ((0 == strcmp(searcher.dest->key, key)))
                        {
                            break;
                        }
                    }
                }

                searcher.parent = searcher.dest;
                searcher.dest   = searcher.dest->next;

                if (searcher.dest == NULL)
                {
                    break;
                }
            }
#endif
        }
    }
    return searcher;
}


static htbl_node *node_ctor(const char *key, unsigned int keylen)
{
    htbl_node *node;
    node = malloc(sizeof(*node));
    if (node != NULL)
    {
        node->key = malloc(keylen);
        reset_htbl_node(node);
        if (node->key != NULL)
        {
            strncpy(node->key, key, keylen);
        }
    }
    return node;
}


static void node_dtor(htbl_node *node)
{
    /* If value has a destructor, call it */
    if (node != NULL)
    {
        if (node->key != NULL)
        {
            free(node->key);
        }
        htbl_value_dtor(node->value);
        free(node);
    }
}

static void htbl_value_dtor(htbl_value value)
{
    if (value != NULL)
    {
        if (value->dtor != NULL)
        {
            value->dtor(value->value);
        }
        free(value);
    }
}


static void node_update(htbl_node *node, htbl_value value)
{
    if (node != NULL)
    {
        htbl_value_dtor(node->value);
        node->value = value;
    }
}
