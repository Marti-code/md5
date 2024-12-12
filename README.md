# MD5 Hash Generator

A C++ implementation of the MD5 hashing algorithm. This project demonstrates the process of computing an MD5 hash for any given input.

<img src="https://github.com/Marti-code/md5/blob/master/static/1.png"/>

## Description

This project implements the MD5 hashing algorithm in C++. It processes input strings and generates a 128-bit hash represented as a 32-character hexadecimal string. MD5 used to be commonly used for checksums, data integrity verification, and cryptographic purposes.
The result of MD5 is always a 128-bit hash, providing 2^128 possible unique hashes. This is an enormous number (approximately 3.4 × 10^38), but for modern applications, it is far from sufficient. This is one of the key reasons why MD5 is not collision-resistant. If two different inputs can produce the same hash result, it becomes possible to falsify digital signatures, manipulate data, crack passwords, or even launch DoS attacks.

## How it works

### Data conversion

First we change our data into binary.

<img src="https://github.com/Marti-code/md5/blob/master/static/0.png"/>

### Padding

MD5 operates on input data blocks of 512 bits and produces a hash of 128 bits. Since input data is rarely exactly 512 bits in length, padding is applied:

- Append a 1 bit to the input
- Add 0 bits until the input length is 448 bits (64 bits less than a multiple of 512)
- Append the original length of the input, represented in 64 bits using the little-endian format.

<img src="https://github.com/Marti-code/md5/blob/master/static/2-padding.png"/>

And even if the data were exactly 448 or 512 bits, the padding is still applied. For example if the data is 448 bits: append 1, fill with 0 to reach 960 bits, and add the length at the end.

### Data processing

The input is divided into 16 chunks of 32 bits each.

<img src="https://github.com/Marti-code/md5/blob/master/static/3-input.png"/>

The output/buffer is initialized with four values derived from fractional parts of square roots:

- A = 0x67452301 (√2)
- B = 0xefcdab89 (√3)
- C = 0x98badcfe (√5)
- D = 0x10325476 (√7)

### Iteration process

Each of the 64 iterations performs the following:

- Copy B, C, and D into C', D', and A'.

  <img src="https://github.com/Marti-code/md5/blob/master/static/4-rotating.png"/>

- Calculate B' using the formula: B′=((F(B, C, D, i) + A + input[currentIdx] + K[i]) << r[i]) + B

  <img src="https://github.com/Marti-code/md5/blob/master/static/5.png"/>

- Update A, B, C, D for the next iteration.

  <img src="https://github.com/Marti-code/md5/blob/master/static/6-buffer-update.png"/>

- Add the new A', B', C', D' values to the existing ones, modulo 2^32

After 64 iterations, the final A, B, C, D values are combined to produce the hash.

<img src="https://github.com/Marti-code/md5/blob/master/static/7-end-result.png"/>

### Example iteration for i=0

Initialize:

- A = 01100111010001010010001100000001
- B = 11101111110011011010101110001001
- C = 10011000101110101101110011111110
- D = 00010000001100100101010001110110
- input[0] = 01001101011000010111001001110100
- k[0] = 11010111011010101010010001111000
- r[0] = 7

Compute F:

- B & C = 10001000100010001000100010001000
- ~B = 00010000001100100101010001110110
- ~B & D = 00010000001100100101010001110110
- F = (B & C) | (~B & D) = 10011000101110101101110011111110

Perform modular additions and bit rotation:

- F + A = 11111111111111111111111111111111
- F + A + input[0] = 01001101011000010111001001110011
- F + A + input[0] + K[0] = 00100100110011000001011011101011
- (F + A + input[0] + K[0]) << r[0] = 01100110000010110111010110010010
- ((F + A + input[0] + K[0]) << r[0]) + B = 01010101110110010010000100011011

Update B':

- B' = 01010101110110010010000100011011(2) = 55D9211B(16)
