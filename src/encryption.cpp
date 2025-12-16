#include "encryption.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>

void aes_encrypt(const unsigned char* plaintext, size_t plaintext_len,
                 const unsigned char* key, const unsigned char* iv,
                 unsigned char* ciphertext, size_t& ciphertext_len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv))
        throw std::runtime_error("Failed to initialize AES encryption");

    int len;
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        throw std::runtime_error("Failed to encrypt data");

    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        throw std::runtime_error("Failed to finalize AES encryption");

    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
}

void aes_decrypt(const unsigned char* ciphertext, size_t ciphertext_len,
                 const unsigned char* key, const unsigned char* iv,
                 unsigned char* plaintext, size_t& plaintext_len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv))
        throw std::runtime_error("Failed to initialize AES decryption");

    int len;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        throw std::runtime_error("Failed to decrypt data");

    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        throw std::runtime_error("Failed to finalize AES decryption");

    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
}
