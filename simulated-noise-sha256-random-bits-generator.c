/*
Created by Tsubasa Kato with the help of ChatGPT (GPT-4)
(C) Tsubasa Kato - 5/23/2024 - Intended for Experimental Use.
This is a C version converted from the Python 3 version.
Not tested yet as of 5/23/2024 21:21PM JST
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <openssl/sha.h>

// Define the source size
#define SOURCE_SIZE 64

// Function to collect noise using OS entropy
void collect_noise(uint8_t *noise) {
    FILE *fp = fopen("/dev/urandom", "rb");
    if (fp == NULL) {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }
    fread(noise, 1, SOURCE_SIZE, fp);
    fclose(fp);
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
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, noise, SOURCE_SIZE);
    SHA256_Final(hash_result, &sha256);
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
    uint8_t conditioned_output[SHA256_DIGEST_LENGTH];
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
