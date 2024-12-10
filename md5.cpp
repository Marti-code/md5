#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace std;

// K = abs(sin(i+1)*2^32)
static const uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,

    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,

    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,

    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// Shift amounts for each round
static const uint32_t r[64] = {
    // Round 1
    7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
    // Round 2
    5,9,14,20, 5,9,14,20, 5,9,14,20, 5,9,14,20,
    // Round 3
    4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
    // Round 4
    6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
};


/**
 * @brief The core MD5 auxiliary function (F, G, H, or I) based on the current round.
 * @param B Current state value B.
 * @param C Current state value C.
 * @param D Current state value D.
 * @param i Current round index (0-63).
 * @return Computed result of the function for the given inputs.
 */
inline uint32_t F(uint32_t B, uint32_t C, uint32_t D, uint32_t i) {
    if (i <= 15) {
        // Round 1
        return (B & C) | ((~B) & D);
    } else if (i <= 31) {
        // Round 2
        return (B & D) | (C & (~D));
    } else if (i <= 47) {
        // Round 3
        return B ^ C ^ D;
    } else {
        // Round 4
        return C ^ (B | (~D));
    }
}

/**
 * @brief Performs a circular left bit rotation.
 * @param x Value to rotate.
 * @param n Number of bits to rotate by.
 * @return Rotated value.
 */
inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return (x << n) | (x >> (32 - n));
}

/**
 * @brief Pads the input message to make its length a multiple of 512 bits.
 * @param input Pointer to the original message.
 * @param original_length Length of the original message in bytes.
 * @return Padded message as a vector of bytes.
 */
vector<uint8_t> padMessage(const uint8_t *input, size_t original_length) {
    size_t bit_len = original_length * 8; // Convert to bits
    size_t mod = original_length % 64;
    size_t padding = (mod < 56) ? (56 - mod) : (64 + 56 - mod);

    size_t padded_len = original_length + padding + 8; // Total length after padding
    vector<uint8_t> output(padded_len, 0); // Allocate and initialize with 0

    // Copy original message into the beginning of the output vector
    memcpy(output.data(), input, original_length);

    // Append '1', marks the end of original message
    output[original_length] = 0x80;

    // Append original length in bits as a 64-bit little-endian integer 
    // The least significant byte of the length is stored first
    // This is how it would look like in binary: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 01100000
    // This is how it looks like in little-endian: 01100000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    for (size_t i = 0; i < 8; i++) {
        output[padded_len - 8 + i] = static_cast<uint8_t>((bit_len >> (i * 8)) & 0xFF);
    }

    return output;
}

/**
 * @brief Processes a 512-bit block of the padded message.
 * @param state Current hash state (A, B, C, D).
 * @param block Pointer to the 512-bit block.
 */
void transform(uint32_t state[4], const uint8_t block[64]) {
    uint32_t A = state[0];
    uint32_t B = state[1];
    uint32_t C = state[2];
    uint32_t D = state[3];

    // Decode 64 bytes into 16 32-bit words
    uint32_t input[16];
    for (int i = 0; i < 16; i++) {
        input[i] = static_cast<uint32_t>(block[i * 4]) |
                   (static_cast<uint32_t>(block[i * 4 + 1]) << 8) |
                   (static_cast<uint32_t>(block[i * 4 + 2]) << 16) |
                   (static_cast<uint32_t>(block[i * 4 + 3]) << 24);
    }

    for (uint32_t i = 0; i < 64; i++) {
        uint32_t currentIdx;
        if (i <= 15) {
            // Round 1
            currentIdx = i % 16;
        } else if (i <= 31) {
            // Round 2
            currentIdx = (5 * i + 1) % 16;
        } else if (i <= 47) {
            // Round 3
            currentIdx = (3 * i + 5) % 16;
        } else {
            // Round 4
            currentIdx = (7 * i) % 16;
        }

        uint32_t temp = B + rotate_left((F(B, C, D, i) + A + input[currentIdx] + K[i]), r[i]);

        // Rotate the variables
        A = D;
        D = C;
        C = B;
        B = temp;
    }

    // Add this chunk's hash to the result so far
    state[0] += A;
    state[1] += B;
    state[2] += C;
    state[3] += D;
}

/**
 * @brief Computes the MD5 hash of a given message.
 * @param input Pointer to the original message.
 * @param original_length Length of the original message in bytes.
 * @return The 128-bit hash as a vector of bytes.
 */
vector<uint8_t> md5(const uint8_t *input, size_t original_length) {
    // Initial state (A, B, C, D)
    uint32_t state[4] = {
        0x67452301, // A
        0xefcdab89, // B
        0x98badcfe, // C
        0x10325476  // D
    };

    // Pad the input message
    vector<uint8_t> padded_message = padMessage(input, original_length);

    // Process each 512-bit chunk
    for (size_t i = 0; i < padded_message.size(); i += 64) {
        transform(state, &padded_message[i]);
    }

    // Combine the states into the almost final hash (just need to convert it into hex string)
    // Once again the little-endian byte order
    vector<uint8_t> digest(16);
    for (size_t i = 0; i < 4; i++) {
        digest[i * 4 + 0] = static_cast<uint8_t>(state[i] & 0xFF); // Least significant byte
        digest[i * 4 + 1] = static_cast<uint8_t>((state[i] >> 8) & 0xFF);
        digest[i * 4 + 2] = static_cast<uint8_t>((state[i] >> 16) & 0xFF);
        digest[i * 4 + 3] = static_cast<uint8_t>((state[i] >> 24) & 0xFF); // Most significant byte
    }

    return digest;
}

/**
 * @brief Converts the digested message to a hexadecimal string.
 * @param digest The MD5 hash as a vector of bytes.
 * @return The hexadecimal representation of the hash.
 */
string toHexString(const vector<uint8_t> &digest) {
    ostringstream hex_stream;
    hex_stream << hex << setfill('0');
    
    for (uint8_t byte : digest) {
        hex_stream << setw(2) << static_cast<int>(byte);
    }
    
    return hex_stream.str();
}

int main() {
    while (true) {
        string message;
        cout << "Enter a message to be hashed (Ctrl-C to exit): ";
        getline(cin, message);

        // Compute and print the MD5 hash
        vector<uint8_t> result = md5(reinterpret_cast<const uint8_t *>(message.c_str()), message.size());
        cout << "MD5(\"" << message << "\") = " << toHexString(result) << endl;

        // Ask if the user wants to continue

        string input;
        while (true) {
            cout << "Do you want to hash another message? (y/n): ";
            getline(cin, input);

            if (input == "y" || input == "Y") break;
            if (input == "n" || input == "N") return 0;
            cout << "Invalid input. Please enter 'y' for 'yes' or 'n' for 'no'.\n";
        }
    }
    return 0;
}