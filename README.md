# simulated-noise-sha256-random-bits-generator
Just creating Random Bits with SHA256 and simulated Noise.

(C)Tsubasa Kato 2024

Updated on 5/23/2024 22:04PM JST

One is Python 3 Version and another is in C Language.

My test environment is using gcc + OpenSSL 3 on MacOS 14.x (Sonoma).
To compile the C program version, type the following:

```gcc -o random_bits_generator simulated-noise-sha256-random-bits-generator.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -g```
