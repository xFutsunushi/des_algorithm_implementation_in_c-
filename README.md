
# DES Algorithm Implementation in C++

## Description

This repository contains an implementation of the DES (Data Encryption Standard) algorithm in C++. DES is one of the oldest symmetric encryption algorithms that operates on 64-bit data blocks and uses a 56-bit key for encryption and decryption. DES is an example of a Feistel cipher, where the data block is divided into two parts, and in each round, operations are performed on these parts using the round key, as well as XOR, permutation, and S-box operations.

The repository includes implementations of core DES functions, such as:
- Expanding the right half of the data block.
- XOR operations with the round key.
- S-box and permutation operations.
- 16 rounds of the Feistel algorithm.
- Generating round keys from the master key.

### Current Status

The project is in development. Currently, the focus is on:
- Implementing the core DES algorithm, including:
  - Generating round keys.
  - Performing XOR operations on the appropriate parts of the data.
  - Implementing permutation functions and S-boxes.
  - Executing the 16 rounds of the Feistel cipher.
- Testing individual components, especially XOR operations, to ensure they produce the expected results.
- Verifying the correctness of the algorithm on various data blocks.

### Example Operation

Here is an example C++ code showing how to perform an XOR operation on the expanded right block and the round key in the DES algorithm:

```cpp
// Example XOR function in C++
std::string xorOperation(const std::string& r, const std::string& roundKey) {
    std::string result;
    for (size_t i = 0; i < r.length(); ++i) {
        result += (r[i] == roundKey[i] ? '0' : '1');  // XOR
    }
    return result;
}
```

### Project Structure

- `src/` - Main source files for the DES algorithm implementation.
  - `des.cpp` - Core DES algorithm implementation.
  - `keygen.cpp` - Round key generation.
  - `xor.cpp` - XOR operation implementation.
  - `permutation.cpp` - Permutation functions (IP, FP, P-Box).
- `test/` - Unit and functional tests.
  - `test_xor.cpp` - Testing the correctness of XOR operations.
  - `test_des.cpp` - Testing the full DES algorithm.
- `roundkeys.bin` - File containing the generated round keys in binary format.
- `README.md` - This file.

### How to Run the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/YourRepository.git
   cd YourRepository
   ```

2. Build the project using the `g++` compiler:
   ```bash
   g++ -o des des.cpp keygen.cpp xor.cpp permutation.cpp
   ```

3. Run the program:
   ```bash
   ./des
   ```

4. The program will perform DES operations on selected data blocks and generate round keys, saving them to the `roundkeys.bin` file.

### Planned Features

- **Expansion:** Ability to run tests on different input data.
- **Decryption:** Implementing the decryption function using the round keys.
- **Optimization:** Improving the algorithm's performance and adding support for larger data blocks.

### License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).

### Contact

If you have any questions or suggestions, feel free to contact me at [email@example.com].

---

This repository is part of my learning process and experiments with cryptographic algorithms, including DES. The goal is to gain deeper knowledge about implementing encryption algorithms and testing their correctness.
