# simulated-noise-sha256-random-bits-generator
Just creating Random Bits with SHA256 and simulated Noise.

One is Python 3 Version and another is in C Language.

My test environment is using gcc + OpenSSL 3 on MacOS 14.x (Sonoma).
To compile the C program version, type the following:
gcc -o random_bits_generator rsimulated-noise-sha256-random-bits-generator.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -g
