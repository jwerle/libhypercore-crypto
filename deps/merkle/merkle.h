#ifndef MERKLE_H
#define MERKLE_H

#include "allocator.h"
#include "platform.h"
#include "version.h"

typedef struct merkle merkle_t;
typedef struct merkle_node merkle_node_t;
typedef struct merkle_codec merkle_codec_t;
typedef struct merkle_options merkle_options_t;
typedef struct merkle_node_list merkle_node_list_t;

typedef unsigned long int (merkle_node_callback_t)(
  unsigned char **hash,
  merkle_node_t *node,
  merkle_node_list_t *roots);

typedef unsigned long int (merkle_parent_callback_t)(
  unsigned char **hash,
  merkle_node_t *left,
  merkle_node_t *right);

#define MERKLE_DEFAULT_OPTIONS ((merkle_options_t) { 0 })

struct merkle_codec {
  merkle_node_callback_t *node;
  merkle_parent_callback_t *parent;
};

struct merkle_options {
  merkle_codec_t codec;
};

struct merkle_node {
  unsigned int alloc:1;
  unsigned int ref;
  unsigned long int index;
  unsigned long int parent;
  unsigned long int size;
  unsigned long int hash_size;
  unsigned char *hash;
  unsigned char *data;
};

struct merkle_node_list {
  unsigned int alloc:1;
  unsigned long int length;
  merkle_node_t **list;
};

struct merkle {
  unsigned int alloc:1;
  merkle_node_list_t roots;
  unsigned long int blocks;
  merkle_codec_t codec;
};

MERKLE_EXPORT int
merkle_init(merkle_t *merkle, merkle_options_t options);

MERKLE_EXPORT void
merkle_destroy(merkle_t *merkle);

MERKLE_EXPORT merkle_node_list_t *
merkle_next(
  merkle_t *merkle,
  unsigned char *data,
  unsigned long int size,
  merkle_node_list_t *nodes);

MERKLE_EXPORT void
merkle_node_destroy(merkle_node_t *node);

MERKLE_EXPORT void
merkle_node_list_destroy(merkle_node_list_t *nodes);

#endif
