#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <flat-tree/flat-tree.h>
#include <sha256/sha256.h>

#include "merkle/allocator.h"
#include "merkle/merkle.h"

static unsigned long int
push(merkle_node_list_t *nodes, merkle_node_t *node);

static merkle_node_t *
pop(merkle_node_list_t *nodes);

static unsigned long int
default_node_callback(
  unsigned char **hash,
  merkle_node_t *node,
  merkle_node_list_t *roots
) {
  unsigned long int size = 32;
  *hash = merkle_alloc(size);
  sha256_hash(*hash, node->data, node->size);
  return size;
}

static unsigned long int
default_parent_callback(
  unsigned char **hash,
  merkle_node_t *left,
  merkle_node_t *right
) {
  unsigned long int size = 32;
  sha256_t sha256;
  *hash = merkle_alloc(size);
  sha256_init(&sha256);
  sha256_update(&sha256, left->hash, left->hash_size);
  sha256_update(&sha256, right->hash, right->hash_size);
  sha256_final(&sha256, *hash);
  return size;
}

int
merkle_init(merkle_t *merkle, merkle_options_t options) {
  if (0 == merkle) {
    return EFAULT;
  }

  if (0 == options.codec.node) {
    options.codec.node = default_node_callback;
  }

  if (0 == options.codec.parent) {
    options.codec.parent = default_parent_callback;
  }

  merkle->blocks = 0;
  merkle->codec = options.codec;

  merkle->roots.length = 0;
  merkle->roots.alloc = 0;
  merkle->roots.list = merkle_alloc(sizeof(merkle_node_t));
  memset(merkle->roots.list, 0, sizeof(merkle_node_t));
  return 0;
}

merkle_node_list_t *
merkle_next(
  merkle_t *merkle,
  unsigned char *data,
  unsigned long int size,
  merkle_node_list_t *nodes
) {
  if (0 == merkle) {
    errno = EFAULT;
    return 0;
  }

  if (0 == data || 0 == size) {
    errno = EINVAL;
    return 0;
  }

  if (0 == nodes) {
    nodes = merkle_alloc(sizeof(*nodes));
    nodes->length = 0;
    nodes->alloc = 1;
    nodes->list = 0;
  }

  unsigned int index = 2 * merkle->blocks++;

  // returned to and managed by (destroyed) caller
  merkle_node_t *node = merkle_alloc(sizeof(*node));
  memset(node, 0, sizeof(*node));

  // init node
  node->alloc = 1;
  node->parent = ft_parent(index, 0);
  node->index = index;
  node->data = data;
  node->size = size;
  // compute node hash
  node->hash_size = merkle->codec.node(&node->hash, node, &merkle->roots);

  // private reference and managed by (destroyed) by `merkle_t`
  merkle_node_t *root = merkle_alloc(sizeof(*root));
  memset(root, 0, sizeof(*root));
  memcpy(root, node, sizeof(*root));

  // reset `ref`
  root->ref = 0;

  // make copy of hash
  root->hash = merkle_alloc(node->hash_size);
  memset(root->hash, 0, node->hash_size);
  memcpy(root->hash, node->hash, node->hash_size);

  // append to `node` to `nodes`
  push(nodes, node);

  // append `root` from `node` to merkle roots array
  push(&merkle->roots, root);

  // compture hashes for parents
  while (merkle->roots.length > 1) {
    unsigned long int length = merkle->roots.length;
    merkle_node_t *left = merkle->roots.list[length - 2];
    merkle_node_t *right = merkle->roots.list[length - 1];

    if (left->parent != right->parent) {
      break;
    }

    // pop root as we compute a new one (points to `right`)
    root = pop(&merkle->roots);

    // newly computed node
    node = merkle_alloc(sizeof(*node));
    memset(node, 0, sizeof(*node));

    // init node
    node->alloc = 1;
    node->parent = ft_parent(left->parent, 0);
    node->index = left->parent;
    node->size = left->size + right->size;
    node->data = 0;
    // compute parent hash
    node->hash_size = merkle->codec.parent(&node->hash, left, right);

    // append to nodes
    push(nodes, node);

    // destroy root that `right` pointed to
    merkle_node_destroy(root);

    // compute next private reference and managed by (destroyed) by `merkle_t`
    root = merkle_alloc(sizeof(*root));
    memset(root, 0, sizeof(*root));
    memcpy(root, node, sizeof(*root));

    // reset `ref`
    root->ref = 0;

    // make copy of hash
    root->hash = merkle_alloc(node->hash_size);
    memset(root->hash, 0, node->hash_size);
    memcpy(root->hash, node->hash, node->hash_size);

    merkle_node_destroy(pop(&merkle->roots));
    push(&merkle->roots, root);
  }

  return nodes;
}

void
merkle_destroy(merkle_t *merkle) {
  merkle_node_list_destroy(&merkle->roots);

  if (1 == merkle->alloc) {
    merkle_free(merkle);
  }

  merkle = 0;
}

void
merkle_node_destroy(merkle_node_t *node) {
  if (0 != node && 0 == node->ref) {
    if (0 != node->hash) {
      merkle_free(node->hash);
      node->hash = 0;
    }

    if (1 == node->alloc) {
      node->alloc = 0;
      merkle_free(node);
    }

    node = 0;
  }
}

void
merkle_node_list_destroy(merkle_node_list_t *nodes) {
  if (0 != nodes) {
    while (nodes->length > 0) {
      merkle_node_t *node = pop(nodes);
      merkle_node_destroy(node);
    }

    merkle_free(nodes->list);
    if (1 == nodes->alloc) {
      merkle_free(nodes);
    }

    nodes = 0;
  }
}

static unsigned long int
push(merkle_node_list_t *nodes, merkle_node_t *node) {
  if (0 == nodes->list) {
    nodes->list = merkle_alloc(sizeof(merkle_node_t));
    memset(nodes->list, 0, sizeof(merkle_node_t));
  } else if (0 == nodes->list[nodes->length]) {
    merkle_node_t **list = merkle_realloc(
      nodes->list,
      (nodes->length + 1) * sizeof(merkle_node_t));

    if (0 == nodes) {
      return 0;
    }

    nodes->list = list;
  }

  (void) node->ref++;
  nodes->list[nodes->length++] = node;

  return 1;
}

static merkle_node_t *
pop(merkle_node_list_t *nodes) {
  merkle_node_t *node = 0;
  if (0 != nodes && nodes->length > 0) {
    node = nodes->list[--nodes->length];
    nodes->list[nodes->length] = 0;
  }

  if (0 != node) {
    if (node->ref > 0) {
      (void) node->ref--;
    }
  }
  return node;
}
