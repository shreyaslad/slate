#ifndef __RBTREE_H__
#define __RBTREE_H__

#include <stddef.h>
#include <stdint.h>
#include <alloc.h>

enum rb_color { RB_COLOR_BLACK, RB_COLOR_RED };

struct rb_node_t {
    struct rb_node_t *desc[2];
    struct rb_node_t *anc;
    enum rb_color color;
};

typedef int (*rb_comp)(struct rb_node_t *, struct rb_node_t *, void *);

struct rb_root_t {
    struct rb_node_t *root;
    size_t node_size;
};

static inline struct rb_node_t *rb_get_par(struct rb_node_t *node) {
    if (node == NULL) {
        return NULL;
    }
    return node->anc;
}

static inline void rb_set_par(struct rb_node_t *node, struct rb_node_t *par) {
    if (node != NULL) {
        node->anc = par;
    }
}

static inline struct rb_node_t *rb_get_desc(struct rb_node_t *node, int pos) {
    if (node == NULL) {
        return NULL;
    }
    return node->desc[pos];
}

static inline void rb_set_desc(struct rb_node_t *node, int pos,
                               struct rb_node_t *desc) {
    if (node != NULL) {
        node->desc[pos] = desc;
        rb_set_par(rb_get_desc(node, pos), node);
    }
}

static inline struct rb_node_t *rb_get_gpar(struct rb_node_t *node) {
    return rb_get_par(rb_get_par(node));
}

static inline enum rb_color rb_get_color(struct rb_node_t *node) {
    if (node == NULL) {
        return RB_COLOR_BLACK;
    }
    return node->color;
}

static inline void rb_set_color(struct rb_node_t *node, enum rb_color color) {
    if (node != NULL) {
        node->color = color;
    }
}

static inline int rb_get_pos(struct rb_node_t *node) {
    return rb_get_desc(rb_get_par(node), 1) == node;
}

static inline struct rb_node_t *rb_get_sib(struct rb_node_t *node) {
    return rb_get_desc(rb_get_par(node), 1 - rb_get_pos(node));
}

static inline struct rb_node_t *rb_get_unc(struct rb_node_t *node) {
    return rb_get_sib(rb_get_par(node));
}

static inline void *rb_rotate(struct rb_node_t *root, int direction,
                              struct rb_root_t *tree) {
    int root_pos = rb_get_pos(root);
    struct rb_node_t *root_parent = rb_get_par(root);
    struct rb_node_t *new_root = rb_get_desc(root, 1 - direction);
    struct rb_node_t *middle = rb_get_desc(new_root, direction);
    rb_set_desc(root, 1 - direction, middle);
    rb_set_desc(new_root, direction, root);
    if (root_parent != NULL) {
        rb_set_desc(root_parent, root_pos, new_root);
    } else {
        tree->root = new_root;
        rb_set_color(new_root, RB_COLOR_BLACK);
        rb_set_par(new_root, NULL);
    }
}

static inline void rb_fix_insertion(struct rb_node_t *node,
                                    struct rb_root_t *root) {
    if (node == NULL) {
        return;
    }
    if (rb_get_color(rb_get_par(node)) == RB_COLOR_BLACK) {
        return;
    }
    struct rb_node_t *uncle = rb_get_unc(node);
    struct rb_node_t *par = rb_get_par(node);
    struct rb_node_t *gpar = rb_get_par(par);
    int node_pos = rb_get_pos(node);
    if (rb_get_color(uncle) == RB_COLOR_BLACK) {
        // black uncle
        if (rb_get_pos(par) == node_pos) {
            // line
            rb_rotate(gpar, 1 - node_pos, root);
            rb_set_color(gpar, RB_COLOR_RED);
            rb_set_color(par, RB_COLOR_BLACK);
        } else {
            // triangle
            rb_rotate(par, 1 - node_pos, root);
            rb_rotate(gpar, node_pos, root);
            rb_set_color(gpar, RB_COLOR_RED);
            rb_set_color(node, RB_COLOR_BLACK);
        }
    } else {
        // red uncle
        rb_set_color(par, RB_COLOR_BLACK);
        rb_set_color(uncle, RB_COLOR_BLACK);
        rb_set_color(gpar, RB_COLOR_RED);
        rb_fix_insertion(gpar, root);
    }
}

// 0 - node is added already, 1 - new node added
static inline int rb_insert(struct rb_root_t *root, rb_comp comp,
                            void *comp_arg, struct rb_node_t *node) {
    rb_set_desc(node, 0, NULL);
    rb_set_desc(node, 1, NULL);
    if (root->root == NULL) {
        root->root = node;
        rb_set_color(root->root, RB_COLOR_BLACK);
        return 1;
    }
    struct rb_node_t *cur = root->root;
    while (cur != NULL) {
        int cmp = comp(node, cur, comp_arg);
        if (cmp == 0) {
            return 0;
        }
        int pos = cmp == 1;
        struct rb_node_t *next = rb_get_desc(cur, pos);
        if (next == NULL) {
            rb_set_desc(cur, pos, node);
            rb_set_color(node, RB_COLOR_RED);
            break;
        }
        cur = next;
    }
    rb_fix_insertion(node, root);
    rb_set_color(root->root, RB_COLOR_BLACK);
    return 1;
}

static inline struct rb_node_t *rb_find_replacement(struct rb_node_t *node) {
    node = rb_get_desc(node, 0);
    struct rb_node_t *prev = node;
    while (node != NULL) {
        prev = node;
        node = rb_get_desc(node, 1);
    }
    return prev;
}

static inline void rb_memswap(char *p1, char *p2, size_t size) {
    if (size % 8 == 0) {
        uint64_t *pl1 = (uint64_t *)p1;
        uint64_t *pl2 = (uint64_t *)p2;
        size_t count = size / 8;
        for (size_t i = 0; i < count; ++i) {
            uint64_t tmp = *pl1;
            *pl1 = *pl2;
            *pl2 = tmp;
        }
    } else {
        for (size_t i = 0; i < size; ++i) {
            char tmp = *p1;
            *p1 = *p2;
            *p2 = tmp;
        }
    }
}

static inline void rb_swap_nodes_content(struct rb_node_t *node1,
                                         struct rb_node_t *node2,
                                         struct rb_root_t *root) {
    char *content1 = (char *)(node1 + 1);
    char *content2 = (char *)(node2 + 1);
    size_t size = root->node_size - sizeof(struct rb_node_t);
    rb_memswap(content1, content2, size);
}

static struct rb_node_t *rb_remove_internal_nodes(struct rb_root_t *root,
                                                struct rb_node_t *node) {
    if (rb_get_desc(node, 0) != NULL && rb_get_desc(node, 1) != NULL) {
        struct rb_node_t *next = rb_find_replacement(node);
        rb_swap_nodes_content(next, node, root);
        return next;
    }
    return node;
}

static void rb_fix_double_black(struct rb_root_t *root, struct rb_node_t *node) {
    if (rb_get_par(node) == NULL) {
        return;
    }
    // red subling case
    struct rb_node_t *par = rb_get_par(node);
    struct rb_node_t *sib = rb_get_sib(node);
    if (rb_get_color(sib) == RB_COLOR_RED) {
        rb_rotate(par, rb_get_pos(node), root);
        rb_set_color(sib, RB_COLOR_BLACK);
        rb_set_color(par, RB_COLOR_RED);
        sib = rb_get_sib(node);
        // now the sibling will be black
    }
    // black sibling case
    // if sibling's child are all black
    if (rb_get_color(rb_get_desc(sib, 0)) == RB_COLOR_BLACK &&
        rb_get_color(rb_get_desc(sib, 1)) == RB_COLOR_BLACK) {
        if (rb_get_color(par) == RB_COLOR_RED) {
            rb_set_color(par, RB_COLOR_BLACK);
            rb_set_color(sib, RB_COLOR_RED);
        } else {
            rb_set_color(sib, RB_COLOR_RED);
            rb_fix_double_black(root, par);
        }
        return;
    }
    // visualizer preferred 1 instead of 0, so I used
    // it to remain consistent
    struct rb_node_t *red_nephew = rb_get_desc(sib, 1);
    if (rb_get_color(red_nephew) == RB_COLOR_BLACK) {
        red_nephew = rb_get_desc(sib, 0);
    }
    if (rb_get_pos(node) == rb_get_pos(red_nephew)) {
        rb_rotate(sib, 1 - rb_get_pos(node), root);
        rb_rotate(par, rb_get_pos(node), root);
        rb_set_color(red_nephew, RB_COLOR_BLACK);
    } else {
        rb_rotate(par, rb_get_pos(node), root);
        rb_set_color(red_nephew, RB_COLOR_BLACK);
    }
}

// 0 - there is no such node, 1 - node deleted
static inline void rb_delete(struct rb_root_t *root, struct rb_node_t *node) {
    // handle case with internal node
    node = rb_remove_internal_nodes(root, node);
    // get node's child
    struct rb_node_t *chld = rb_get_desc(node, 0);
    if (chld == NULL) {
        chld = rb_get_desc(node, 1);
    }
    // if it is a single-child parent, it must be black
    if (chld != NULL) {
        rb_set_desc(rb_get_par(node), rb_get_pos(node), chld);
        rb_set_color(chld, RB_COLOR_BLACK);
        kfree(node);
        return;
    }
    // if node to be deleted is a red leaf, remove leaf, done
    if (chld == NULL && rb_get_color(node) == RB_COLOR_RED) {
        rb_set_desc(rb_get_par(node), rb_get_pos(node), NULL);
        kfree(node);
        return;
    }
    // now node is nothing but black leaf
    rb_fix_double_black(root, node);
    rb_set_desc(rb_get_par(node), rb_get_pos(node), NULL);
    kfree(node);
}

static inline struct rb_node_t *rb_query(struct rb_root_t *root,
                                       struct rb_node_t *node, rb_comp comp,
                                       void *comp_arg) {
    struct rb_node_t *current = root->root;
    while (current != NULL) {
        int cmp = comp(node, current, comp_arg);
        int pos = cmp == 1;
        if (cmp == 0) {
            return current;
        } else {
            current = rb_get_desc(current, pos);
        }
    }
    return NULL;
}

#endif
