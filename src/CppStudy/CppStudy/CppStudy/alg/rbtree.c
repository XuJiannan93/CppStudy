#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 0
#define BLACK 1

typedef int KEY_TYPE;

#define RBTREE_ENTRY(name, type) \
    struct name                  \
    {                            \
        struct type *right;      \
        struct type *left;       \
        struct type *parent;     \
        unsigned char color;     \
    }

typedef struct _rbtree_node
{
    KEY_TYPE key;
    void* value;
#if 1
    struct _rbtree_node* right;
    struct _rbtree_node* left;
    struct _rbtree_node* parent;
    unsigned char color;
#else
    RBTREE_ENTRY(, _rbtree_node)
        node;
#endif
} rbtree_node;

typedef struct _rbtree
{
    struct _rbtree_node* root;
    struct _rbtree_node* nil;
} rbtree;

rbtree_node* rbtree_mini(rbtree* T, rbtree_node* x)
{
    while (x->left != T->nil)
    {
        x = x->left;
    }
    return x;
}

rbtree_node* rbtree_maxi(rbtree* T, rbtree_node* x)
{
    while (x->right != T->nil)
    {
        x = x->right;
    }
    return x;
}

rbtree_node* rbtree_successor(rbtree* T, rbtree_node* x)
{
    rbtree_node* y = x->parent;

    if (x->right != T->nil)
        return rbtree_mini(T, x->right);
    while ((y != T->nil) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }
    return y;
}

void rbtree_left_rotate(rbtree* T, rbtree_node* x)
{
    rbtree_node* y = x->right;

    x->right = y->left;
    if (y->left != T->nil)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == T->nil)
        T->root = y;
    else
    {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void rbtree_right_rotate(rbtree* T, rbtree_node* y)
{
    rbtree_node* x = y->left;

    y->left = x->right;
    if (x->right != T->nil)
        x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == T->nil)
        T->root = x;
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void rbtree_insert_fixup(rbtree* T, rbtree_node* z)
{
    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {

            rbtree_node* y = z->parent->parent->right;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    rbtree_left_rotate(T, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                rbtree_right_rotate(T, z->parent->parent);
            }
        }
        else
        {
            rbtree_node* y = z->parent->parent->left;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rbtree_right_rotate(T, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_left_rotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

void rbtree_insert(rbtree* T, rbtree_node* z)
{
    rbtree_node* y = T->nil;
    rbtree_node* x = T->root;
    while (x != T->nil)
    {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else if (z->key > x->key)
            x = x->right;
        else
        {
            // acording to business logic
            return;
        }
    }

    z->parent = y;
    if (y == T->nil)
        T->root = z;
    else
    {
        if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
    }

    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;

    rbtree_insert_fixup(T, z);
}

void rbtree_delete_fixup(rbtree* T, rbtree_node* x)
{
    while ((x != T->root) && (x->color == BLACK))
    {
        if (x == x->parent->left)
        {
            rbtree_node* w = x->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;

                rbtree_left_rotate(T, x->parent);
                w = x->parent->right;
            }

            if ((w->left->color == BLACK) && (w->right->color == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    rbtree_right_rotate(T, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rbtree_left_rotate(T, x->parent);

                x = T->root;
            }
        }
        else
        {
            rbtree_node* w = x->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rbtree_right_rotate(T, x->parent);
                w = x->parent->left;
            }

            if ((w->left->color == BLACK) && (w->right->color == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    rbtree_left_rotate(T, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rbtree_right_rotate(T, x->parent);

                x = T->root;
            }
        }
    }

    x->color = BLACK;
}

rbtree_node* rbtree_delete(rbtree* T, rbtree_node* z)
{
    rbtree_node* y = T->nil;
    rbtree_node* x = T->nil;

    if ((z->left == T->nil) || (z->right == T->nil))
        y = z;
    else
        y = rbtree_successor(T, z);

    x->parent = y->parent;
    if (y->parent == T->nil)
        T->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    if (y != z)
    {
        z->key = y->key;
        z->value = y->value;
    }

    if (y->color == BLACK)
        rbtree_delete_fixup(T, x);

    return y;
}

rbtree_node* rbtree_search(rbtree* T, KEY_TYPE key)
{
    rbtree_node* node = T->root;
    while (node != T->nil)
    {
        if (key < node->key)
            node = node->left;
        else if (key > node->key)
            node = node->right;
        else
            return node;
    }

    return T->nil;
}

void rbtree_traversal(rbtree* T, rbtree_node* node)
{
    if (node != T->nil)
    {
        rbtree_traversal(T, node->left);
        printf("key:%d, color:%d\n", node->key, node->color);
        rbtree_traversal(T, node->right);
    }
}