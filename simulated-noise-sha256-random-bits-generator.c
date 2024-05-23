/*
Created by Tsubasa Kato with the help of ChatGPT (GPT-4)
(C) Tsubasa Kato - 5/8/2024 - Intended for Experimental Use.
Tested to work on MacOS Sonoma 14 with OpenSSL 3. (5/23/2024 - 21:51PM)
Read Readme for details on how to compile.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define SOURCE_SIZE 64

// Function to collect noise using OS entropy
void collect_noise(uint8_t *noise) {
    if (RAND_bytes(noise, SOURCE_SIZE) != 1) {
        fprintf(stderr, "Failed to collect noise using RAND_bytes\n");
        exit(EXIT_FAILURE);
    }
}

// Function to perform health tests on the collected noise
void health_tests(uint8_t *noise) {
    int counts[256] = {0};
    for (int i = 0; i < SOURCE_SIZE; i++) {
        counts[noise[i]]++;
    }
    int unique_count = 0;
    for (int i = 0; i < 256; i++) {
        if (counts[i] > 0) {
            unique_count++;
        }
    }
    if (unique_count < SOURCE_SIZE / 2) {
        fprintf(stderr, "Repetition Count Test Failed\n");
        exit(EXIT_FAILURE);
    }
}

// Function to estimate entropy using the 'min-entropy' approach
double estimate_entropy(uint8_t *noise) {
    int counts[256] = {0};
    for (int i = 0; i < SOURCE_SIZE; i++) {
        counts[noise[i]]++;
    }
    double probabilities[256] = {0.0};
    for (int i = 0; i < 256; i++) {
        probabilities[i] = (double)counts[i] / SOURCE_SIZE;
    }
    double max_probability = 0.0;
    for (int i = 0; i < 256; i++) {
        if (probabilities[i] > max_probability) {
            max_probability = probabilities[i];
        }
    }
    return -log2(max_probability);
}

// Function to condition the raw noise using SHA-256
void condition(uint8_t *noise, uint8_t *hash_result) {
    EVP_MD_CTX *mdctx;
    if((mdctx = EVP_MD_CTX_new()) == NULL) {
        fprintf(stderr, "EVP_MD_CTX_new failed\n");
        exit(EXIT_FAILURE);
    }

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
        fprintf(stderr, "EVP_DigestInit_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        exit(EXIT_FAILURE);
    }

    if(1 != EVP_DigestUpdate(mdctx, noise, SOURCE_SIZE)) {
        fprintf(stderr, "EVP_DigestUpdate failed\n");
        EVP_MD_CTX_free(mdctx);
        exit(EXIT_FAILURE);
    }

    if(1 != EVP_DigestFinal_ex(mdctx, hash_result, NULL)) {
        fprintf(stderr, "EVP_DigestFinal_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        exit(EXIT_FAILURE);
    }

    EVP_MD_CTX_free(mdctx);
}

// Function to generate random bits following NIST SP 800-90B guidelines
void generate_random_bits(uint8_t *random_bits, int bit_length) {
    uint8_t noise[SOURCE_SIZE];
    collect_noise(noise);
    health_tests(noise);
    double estimated_entropy = estimate_entropy(noise);
    if (estimated_entropy < 2.0) {
        fprintf(stderr, "Insufficient entropy\n");
        exit(EXIT_FAILURE);
    }
    uint8_t conditioned_output[EVP_MAX_MD_SIZE];
    condition(noise, conditioned_output);
    memcpy(random_bits, conditioned_output, bit_length / 8);
}

int main() {
    uint8_t random_bits[32];
    int a = 1;
    while (a <= 10000) {
        generate_random_bits(random_bits, 256);
        printf("Number: %d\n", a);
        for (int i = 0; i < 32; i++) {
            printf("%02x", random_bits[i]);
        }
        printf("\n");
        a++;
    }
    return 0;
}
