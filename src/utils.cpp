#include "utils.h"
#include <openssl/rand.h>
#include <stdexcept>

void generate_random_iv(unsigned char* iv, size_t iv_len) {
    if (1 != RAND_bytes(iv, iv_len))
        throw std::runtime_error("Failed to generate random IV");
}
