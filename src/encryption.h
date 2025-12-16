#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <cstddef>

void aes_encrypt(const unsigned char* plaintext, size_t plaintext_len,
                 const unsigned char* key, const unsigned char* iv,
                 unsigned char* ciphertext, size_t& ciphertext_len);

void aes_decrypt(const unsigned char* ciphertext, size_t ciphertext_len,
                 const unsigned char* key, const unsigned char* iv,
                 unsigned char* plaintext, size_t& plaintext_len);

#endif // ENCRYPTION_H
