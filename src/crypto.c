#include <uint64be/uint64be.h>
#include <sodium.h>
#include <string.h>
#include <errno.h>

#include "hypercore/crypto/allocator.h"
#include "hypercore/crypto/crypto.h"

#include "require.h"

#define INIT_STATE() {                    \
  int rc = hypercore_crypto_init_state(); \
  if (0 != rc) { return rc; }             \
}

static unsigned char DATA_TYPES[] = {
  HYPERCORE_CRYPTO_LEAF_BYTE,
  HYPERCORE_CRYPTO_PARENT_BYTE,
  HYPERCORE_CRYPTO_ROOT_BYTE
};

static int
hypercore_crypto_init_state() {
  require(-1 != sodium_init(), 1);
  hypercore_crypto_allocator_set(sodium_malloc);
  hypercore_crypto_deallocator_set(sodium_free);
  return 0;
}

static int
blake2b(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_buffer_t **buffers,
  unsigned long int size
) {
  crypto_generichash_state state = { 0 };
  int allocs = 0;
  int rc = 0;

  if (0 == out->bytes) {
    out->bytes = hypercore_crypto_alloc(crypto_generichash_BYTES);
    require(0 != out->bytes, ENOMEM);
    out->size = crypto_generichash_BYTES;
    (void) allocs++;
  }

  rc = crypto_generichash_init(&state, 0, 0, out->size);

  if (0 != rc) {
    if (0 == --allocs) {
      hypercore_crypto_free(out->bytes);
      out->bytes = 0;
      out->size = 0;
    }

    return rc;
  }

  for (int i = 0; i < size; ++i) {
    rc = crypto_generichash_update(&state, buffers[i]->bytes, buffers[i]->size);
    if (0 != rc) {
      crypto_generichash_final(&state, out->bytes, out->size);

      if (0 == --allocs) {
        hypercore_crypto_free(out->bytes);
        out->bytes = 0;
        out->size = 0;
      }

      return rc;
    }
  }

  rc = crypto_generichash_final(&state, out->bytes, out->size);
  return rc;
}

int
hypercore_crypto_keypair(
  hypercore_crypto_keypair_t *kp,
  const unsigned char *seed
) {
  INIT_STATE();

  int allocs = 0;
  int rc = 0;

  require(0 != kp, EFAULT);

  if (0 == kp->public_key.bytes) {
    kp->public_key.bytes = hypercore_crypto_alloc(crypto_sign_PUBLICKEYBYTES);

    require(0 != kp->public_key.bytes, ENOMEM);
    kp->public_key.size = crypto_sign_PUBLICKEYBYTES;
    (void) allocs++;
  }

  if (0 == kp->secret_key.bytes) {
    kp->secret_key.bytes = hypercore_crypto_alloc(crypto_sign_SECRETKEYBYTES);

    if (0 == kp->secret_key.bytes) {
      if (0 == --allocs) {
        hypercore_crypto_free(kp->public_key.bytes);
        kp->public_key.bytes = 0;
        kp->public_key.size = 0;
      }
    }

    require(0 != kp->secret_key.bytes, ENOMEM);
    kp->secret_key.size = crypto_sign_SECRETKEYBYTES;
    (void) allocs++;
  }

  if (0 != seed) {
    rc = crypto_sign_seed_keypair(
      kp->public_key.bytes,
      kp->secret_key.bytes,
      seed);
  } else {
    rc = crypto_sign_keypair(kp->public_key.bytes, kp->secret_key.bytes);
  }

  if (0 != rc) {
    if (allocs-- > 0)  {
      hypercore_crypto_free(kp->public_key.bytes);
      kp->public_key.bytes = 0;
      kp->public_key.size = 0;
    }

    if (allocs-- > 0)  {
      hypercore_crypto_free(kp->secret_key.bytes);
      kp->secret_key.bytes = 0;
      kp->secret_key.size = 0;
    }
  }

  return rc;
}

void
hypercore_crypto_keypair_destroy(hypercore_crypto_keypair_t *kp) {
  if (0 != kp) {
    if (0 != kp->public_key.bytes) {
      hypercore_crypto_free(kp->public_key.bytes);
      kp->public_key.bytes = 0;
      kp->public_key.size = 0;
    }

    if (0 != kp->secret_key.bytes) {
      hypercore_crypto_free(kp->secret_key.bytes);
      kp->secret_key.bytes = 0;
      kp->secret_key.size = 0;
    }
  }
}

int
hypercore_crypto_sign(
  hypercore_crypto_buffer_t *signature,
  const hypercore_crypto_buffer_t *message,
  const hypercore_crypto_buffer_t *secret_key
) {
  INIT_STATE();

  int rc = 0;

  require(0 != signature, EFAULT);
  require(0 != message, EFAULT);
  require(0 != secret_key, EFAULT);

  if (0 == signature->bytes) {
    signature->bytes = hypercore_crypto_alloc(crypto_sign_BYTES);

    require(0 != signature->bytes, ENOMEM);

    signature->size = crypto_sign_BYTES;
  }

  rc = crypto_sign_detached(
    signature->bytes,
    &signature->size,
    message->bytes,
    message->size,
    secret_key->bytes);

  if (-1 == rc) {
    return rc;
  }

  return 0;
}

int
hypercore_crypto_verify(
  hypercore_crypto_buffer_t *signature,
  const hypercore_crypto_buffer_t *message,
  const hypercore_crypto_buffer_t *public_key
) {
  INIT_STATE();

  require(0 != signature, EFAULT);
  require(0 != message, EFAULT);
  require(0 != public_key, EFAULT);

  require(0 != signature->bytes, EFAULT);
  require(0 != message->bytes, EFAULT);
  require(0 != public_key->bytes, EFAULT);

  require(crypto_sign_BYTES == signature->size, EINVAL);
  require(crypto_sign_PUBLICKEYBYTES == public_key->size, EINVAL);
  require(message->size > 0, EINVAL);

  return crypto_sign_verify_detached(
      signature->bytes,
      message->bytes,
      message->size,
      public_key->bytes);
}

int
hypercore_crypto_data(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_buffer_t *data
) {
  INIT_STATE();

  require(0 != out, EFAULT);
  require(0 != data, EFAULT);

  // leaf=0
  const hypercore_crypto_buffer_t header = { DATA_TYPES + 0, 1 };
  const hypercore_crypto_buffer_t length = { (unsigned char [8]) { 0 }, 8 };
  const hypercore_crypto_buffer_t *buffers[] = {
    &header,
    &length,
    data
  };

  if (uint64be_encode(length.bytes, data->size) <= 0) {
    return -1;
  }

  if (0 == out->bytes) {
    out->bytes = hypercore_crypto_alloc(hypercore_crypto_data_BYTES);

    require(0 != out->bytes, ENOMEM);

    out->size = hypercore_crypto_data_BYTES;
  }

  return blake2b(out, buffers, 3);
}

int
hypercore_crypto_leaf(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_node_t *leaf
) {
  return hypercore_crypto_data(out, leaf->data);
}

int
hypercore_crypto_parent(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_node_t *left,
  const hypercore_crypto_node_t *right
) {
  INIT_STATE();

  require(0 != out, EFAULT);
  require(0 != left, EFAULT);
  require(0 != right, EFAULT);

  if (left->index > right->index) {
    const hypercore_crypto_node_t *tmp = left;
    left = right;
    right = tmp;
  }

  // parent=1
  const hypercore_crypto_buffer_t header = { DATA_TYPES + 1, 1 };
  const hypercore_crypto_buffer_t length = { (unsigned char [8]) { 0 }, 8 };
  const hypercore_crypto_buffer_t *buffers[] = {
    &header,
    &length,
    left->hash,
    right->hash
  };

  if (uint64be_encode(length.bytes, left->size  + right->size) <= 0) {
    return -1;
  }

  if (0 == out->bytes) {
    out->bytes = hypercore_crypto_alloc(hypercore_crypto_data_BYTES);

    require(0 != out->bytes, ENOMEM);

    out->size = hypercore_crypto_data_BYTES;
  }

  return blake2b(out, buffers, 4);
}

int
hypercore_crypto_tree(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_node_t **roots,
  unsigned long long count
) {
  INIT_STATE();

  require(0 != out, EFAULT);
  require(0 != roots, EFAULT);

  unsigned long long size = 3 * count + 1;

  // root=2
  const hypercore_crypto_buffer_t header = { DATA_TYPES + 2, 1 };
  const hypercore_crypto_buffer_t *buffers[size];
  hypercore_crypto_buffer_t indices[size];
  hypercore_crypto_buffer_t lengths[size];

  memset(buffers, 0, size);

  unsigned int j = 0;
  buffers[j++] = &header;

  for (int i = 0; i < count; ++i) {
    buffers[j++] = roots[i]->hash;
    buffers[j++] = &indices[i];
    buffers[j++] = &lengths[i];

    indices[i].bytes = hypercore_crypto_alloc(8);
    indices[i].size = 8;

    lengths[i].bytes = hypercore_crypto_alloc(8);
    lengths[i].size = 8;

    if (
      uint64be_encode(indices[i].bytes, roots[i]->index) <= 0 ||
      uint64be_encode(lengths[i].bytes, roots[i]->size) <= 0
    ) {
      return -1;
    }
  }

  if (0 == out->bytes) {
    out->bytes = hypercore_crypto_alloc(hypercore_crypto_data_BYTES);

    require(0 != out->bytes, ENOMEM);

    out->size = hypercore_crypto_data_BYTES;
  }

  return blake2b(out, buffers, size);
}

int
hypercore_crypto_randombytes(hypercore_crypto_buffer_t *out) {
  require(0 != out, EFAULT);

  if (0 == out->bytes) {
    if (0 == out->size) {
      out->size = hypercore_crypto_randombytes_BYTES;
    }

    out->bytes = hypercore_crypto_alloc(out->size);
    memset(out->bytes, 0, out->size);
  }

  randombytes_buf(out->bytes, out->size);

  return out->size;
}

int
hypercore_crypto_discoverykey(
  hypercore_crypto_buffer_t *out,
  hypercore_crypto_buffer_t *tree
) {
  INIT_STATE();

  require(0 != out, EFAULT);

  out->size = crypto_generichash_BYTES;

  if (0 == out->bytes) {
    out->bytes = hypercore_crypto_alloc(out->size);
    memset(out->bytes, 0, out->size);
  }

  crypto_generichash(
    out->bytes,
    out->size,
    HYPERCORE_CRYPTO_KEY_BYTES,
    sizeof(HYPERCORE_CRYPTO_KEY_BYTES),
    tree->bytes,
    tree->size);

  return out->size;
}
