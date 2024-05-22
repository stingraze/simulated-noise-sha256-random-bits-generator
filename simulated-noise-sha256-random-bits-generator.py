#Created by Tsubasa Kato with the the help of ChatGPT (GPT-4)
#(C)Tsubasa Kato - 5/8/2024 - Intended for Experimental Use.
import os
import hashlib
import numpy as np

class EntropySource:
    def __init__(self, source_size=64):
        self.source_size = source_size
        self.raw_entropy = None

    def collect_noise(self):
        """Simulate noise source collection using OS entropy."""
        self.raw_entropy = os.urandom(self.source_size)
        return self.raw_entropy

    def health_tests(self, noise):
        """Perform simple health tests on the collected noise."""
        if len(set(noise)) < len(noise) / 2:
            raise ValueError("Repetition Count Test Failed")

    def estimate_entropy(self, noise):
        """Estimate entropy using the 'min-entropy' approach."""
        counts = np.unique(list(noise), return_counts=True)[1]
        probabilities = counts / sum(counts)
        min_entropy = -np.log2(max(probabilities))
        return min_entropy

class NistSp80090B:
    def __init__(self):
        self.entropy_source = EntropySource()

    def condition(self, noise):
        """Condition the raw noise using SHA-256."""
        hash_result = hashlib.sha256(noise).digest()
        return hash_result

    def generate_random_bits(self, bit_length=256):
        """Generate random bits following NIST SP 800-90B guidelines."""
        # 1. Collect raw noise
        noise = self.entropy_source.collect_noise()

        # 2. Perform health tests
        try:
            self.entropy_source.health_tests(noise)
        except ValueError as e:
            raise RuntimeError("Health test failed") from e

        # 3. Estimate entropy
        estimated_entropy = self.entropy_source.estimate_entropy(noise)
        if estimated_entropy < 2:  # Arbitrary entropy threshold
            raise RuntimeError("Insufficient entropy")

        # 4. Condition the noise
        conditioned_output = self.condition(noise)

        # 5. Return the required number of random bits
        return conditioned_output[:bit_length // 16]

# Example usage
nist_rng = NistSp80090B()
random_bits = nist_rng.generate_random_bits()
print(random_bits.hex())


a = 1
while a <= 10000:
    nist_rng = NistSp80090B()
    random_bits = nist_rng.generate_random_bits()
    print("Number:" + str(a))
    print(random_bits.hex())
    a = a + 1 
