# MD5 Hash Generator

A C++ implementation of the MD5 hashing algorithm. This project demonstrates the process of computing an MD5 hash for any given input.

<img src="https://github.com/Marti-code/md5/blob/master/static/1.png"/>

## Description

This project implements the MD5 hashing algorithm in C++. It processes input strings and generates a 128-bit hash represented as a 32-character hexadecimal string. MD5 used to be commonly used for checksums, data integrity verification, and cryptographic purposes.
The result of MD5 is always a 128-bit hash, providing 2^128 possible unique hashes. This is an enormous number (approximately 3.4 × 10^38), but for modern applications, it is far from sufficient. This is one of the key reasons why MD5 is not collision-resistant. If two different inputs can produce the same hash result, it becomes possible to falsify digital signatures, manipulate data, crack passwords, or even launch DoS attacks.

## How it works

### Padding

MD5 operates on input data blocks of 512 bits and produces a hash of 128 bits. Since input data is rarely exactly 512 bits in length, padding is applied:

- Append a 1 bit to the input
- Add 0 bits until the input length is 448 bits (64 bits less than a multiple of 512)
- Append the original length of the input, represented in 64 bits using the little-endian format.
