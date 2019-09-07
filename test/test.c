#include <assert.h>
#include <sodium.h>
#include <stdlib.h>
#include <string.h>

#include <merkle/merkle.h>
#include <ok/ok.h>

#include "hypercore/crypto/crypto.h"

#define bytes(b) (unsigned char *) (b)

void
printb(unsigned char *bytes, unsigned long int size) {
  for (int i = 0; i < size; ++i) {
    printf("%02x", bytes[i]);
    if (i + 1 < size) { printf(" "); }
  }

  printf("\n");
}

int
main(void) {
#ifdef OK_EXPECTED
  ok_expect(OK_EXPECTED);
#else
  ok_expect(0);
#endif

  hypercore_crypto_keypair_t keypair = { 0 };
  int rc = 0;

  hypercore_crypto_keypair(&keypair, 0);

  //printb(keypair.public_key.bytes, keypair.public_key.size);
  //printb(keypair.secret_key.bytes, keypair.secret_key.size);

  if (0 != keypair.public_key.bytes) {
    ok("keypair.public_key.bytes");
  }

  if (crypto_sign_PUBLICKEYBYTES == keypair.public_key.size) {
    ok("keypair.public_key.size");
  }

  if (0 != keypair.secret_key.bytes) {
    ok("keypair.secret_key.bytes");
  }

  if (crypto_sign_SECRETKEYBYTES == keypair.secret_key.size) {
    ok("keypair.secret_key.size");
  }

  hypercore_crypto_buffer_t signature = { 0 };
  rc = hypercore_crypto_sign(
    &signature,
    &(hypercore_crypto_buffer_t) { 5, bytes("hello") },
    &keypair.secret_key);

  if (0 != signature.bytes) {
    ok("signature.bytes");
  }

  if (crypto_sign_BYTES == signature.size) {
    ok("signature.size");
  }

  if (0 == rc) {
    ok("hypercore_crypto_sign");
  }

  rc = hypercore_crypto_verify(
    &signature,
    &(hypercore_crypto_buffer_t) { 5, bytes("hello") },
    &keypair.public_key);

  if (0 == rc) {
    ok("hypercore_crypto_verify");
  }

  hypercore_crypto_free(signature.bytes);

  hypercore_crypto_buffer_t message = { 5, bytes("hello") };
  hypercore_crypto_buffer_t data = { 0 };
  rc = hypercore_crypto_data(
    &data,
    &message);

  if (0 == rc) {
    ok("hypercore_crypto_data");
  }

  hypercore_crypto_free(data.bytes);
  //printb(data.bytes, data.size);

  hypercore_crypto_buffer_t leaf = { 0 };
  hypercore_crypto_node_t node = {
    .data = &(hypercore_crypto_buffer_t) { 5, bytes("hello") }
  };

  hypercore_crypto_leaf(&leaf, &node);

  if (0 == memcmp(leaf.bytes, data.bytes, leaf.size)) {
    ok("hypercore_crypto_leaf");
  }

  hypercore_crypto_free(leaf.bytes);

  //printb(leaf.bytes, leaf.size);
  hypercore_crypto_buffer_t parent = { 0 };
  hypercore_crypto_parent(
    &parent,
    &(hypercore_crypto_node_t){
      .index = 0,
      .size = 4,
      .hash = &(hypercore_crypto_buffer_t) { 4, bytes("\x0e\x0c\x0e\x0f")  }
    },
    &(hypercore_crypto_node_t){
      .index = 2,
      .size = 4,
      .hash = &(hypercore_crypto_buffer_t) { 4, bytes("\x0e\x0e\x0e\x0e") }
    });

  unsigned char expected_parent_hash[] = {
    0xf6, 0xd0, 0xc8, 0x88, 0xca, 0x9e, 0x09, 0xc0,
    0x9d, 0xbc, 0x41, 0x10, 0x8d, 0x5b, 0x24, 0xc9,
    0x06, 0x04, 0x1c, 0x73, 0x74, 0xb2, 0xce, 0x89,
    0x4b, 0xd5, 0x5b, 0xa6, 0x77, 0xe8, 0xc5, 0x15
  };

  if (0 == memcmp(expected_parent_hash, parent.bytes, parent.size)) {
    ok("hypercore_crypto_parent");
  }

  hypercore_crypto_free(parent.bytes);

  //printb(parent.bytes, parent.size);
  // parent({index: 0, size: 4, hash: Buffer.from('\x0e\x0c\x0e\x0f')}, {index:2, size: 4, hash: Buffer.from([0xe, 0xe, 0xe, 0xe])})                                       
  // f6 d0 c8 88 ca 9e 09 c0 9d bc 41 10 8d 5b 24 c9 06 04 1c 73 74 b2 ce 89 4b d5 5b a6 77 e8 c5 15

  hypercore_crypto_buffer_t out = { 0 };
  merkle_t merkle = { 0 };
  const hypercore_crypto_node_t *roots[2];
  merkle_init(&merkle, MERKLE_DEFAULT_OPTIONS);
  merkle_node_list_destroy(merkle_next(&merkle, (unsigned char *) "hello", 5, 0));
  merkle_node_list_destroy(merkle_next(&merkle, (unsigned char *) "hello", 5, 0));
  merkle_node_list_destroy(merkle_next(&merkle, (unsigned char *) "hello", 5, 0));

  roots[0] = &(const hypercore_crypto_node_t) {
    .data = 0,
    .index = merkle.roots.list[0]->index,
    .size = merkle.roots.list[0]->size,
    .hash = &(hypercore_crypto_buffer_t) {
      merkle.roots.list[0]->hash_size,
      merkle.roots.list[0]->hash,
    },
  };

  roots[1] = &(const hypercore_crypto_node_t) {
    .index = merkle.roots.list[1]->index,
    .size = merkle.roots.list[1]->size,
    .hash = &(hypercore_crypto_buffer_t) {
      merkle.roots.list[1]->hash_size,
      merkle.roots.list[1]->hash,
    },
    .data = &(hypercore_crypto_buffer_t) {
      merkle.roots.list[1]->hash_size,
      merkle.roots.list[1]->data,
    }
  };

  hypercore_crypto_tree(&out, roots, 2);
  unsigned char expected_tree[] = {
    0xb8, 0xbd, 0x59, 0xab, 0x95, 0x6d, 0x5f, 0x35,
    0x62, 0xea, 0x10, 0xff, 0xa0, 0x2a, 0x95, 0x6a,
    0x58, 0x4b, 0xad, 0xd1, 0x36, 0xb7, 0xb8, 0xc7,
    0x0c, 0x04, 0xe2, 0x02, 0xee, 0x0f, 0xac, 0x8e

  };

  if (0 == memcmp(expected_tree, out.bytes, out.size)) {
    ok("hypercore_crypto_tree");
  }

  //printb(out.bytes, out.size);
  hypercore_crypto_free(out.bytes);

  merkle_destroy(&merkle);

  hypercore_crypto_buffer_t randombytes = {
    .size = 32,
    .bytes = (unsigned char [32]) { 0 }
  };

  if (32 == hypercore_crypto_randombytes(&randombytes)) {
    ok("hypercore_crypto_randombytes");
  }
  //printb(randombytes.bytes, randombytes.size);

  unsigned char key[] = {
    0x70, 0xe0, 0x2b, 0x49, 0x2d, 0xb6, 0x83, 0x81,
    0x7f, 0x46, 0x63, 0xef, 0x78, 0xe6, 0x63, 0xb7,
    0xab, 0x74, 0xee, 0x19, 0x35, 0xd1, 0x2e, 0x18,
    0x76, 0x51, 0x6a, 0x30, 0xa7, 0xf4, 0xee, 0xf2
  };

  unsigned char expected[] = {
    0x3e, 0xad, 0xc8, 0x72, 0xf5, 0x69, 0x10, 0x6b,
    0x67, 0x69, 0xb9, 0xea, 0xc4, 0x0b, 0x55, 0x45,
    0x59, 0xe5, 0x25, 0xe0, 0x17, 0xdc, 0x6b, 0x7d,
    0x9a, 0x61, 0x8a, 0x33, 0x3c, 0x6a, 0x39, 0xc0
  };

  hypercore_crypto_buffer_t discoverykey = { 0 };
  hypercore_crypto_discoverykey(&discoverykey,
    &(hypercore_crypto_buffer_t){ 32, key });

  //printb(discoverykey.bytes, discoverykey.size);
  if (0 == memcmp(expected, discoverykey.bytes, discoverykey.size)) {
    ok(" hypercore_crypto_discoverykey");
  }

  hypercore_crypto_free(discoverykey.bytes);
  hypercore_crypto_keypair_destroy(&keypair);

  ok_done();
  return ok_count() == ok_expected() ? 0 : 1;
}
