# simulated-noise-sha256-random-bits-generator
Just creating Random Bits with SHA256 and simulated Noise.

My test environment is using gcc on MacOS 14 (Sonoma).
To compile the C program version, type the following:
gcc -o random_number_generator rsimulated-noise-sha256-random-bits-generator.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -g
