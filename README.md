# MD5 Hash Generator

A C++ implementation of the MD5 hashing algorithm. This project demonstrates the process of computing an MD5 hash for any given input.

<img src="https://github.com/Marti-code/yt-playlist-converter/blob/master/static/weload-desktop.jpg"/>

## Description

This project implements the MD5 hashing algorithm in C++. It processes input strings and generates a 128-bit hash represented as a 32-character hexadecimal string. MD5 used to be commonly used for checksums, data integrity verification, and cryptographic purposes.
The result of MD5 is always a 128-bit hash, providing 2^128 possible unique hashes. This is an enormous number (approximately 3.4 × 10^38), but for modern applications, it is far from sufficient. This is one of the key reasons why MD5 is not collision-resistant. If two different inputs can produce the same hash result, it becomes possible to falsify digital signatures, manipulate data, crack passwords, or even launch DoS attacks.

## How it works

- Fetches YouTube playlist information (title, number of videos, thumbnail).
- Downloads all videos in the playlist as audio or video.
- Zips the downloaded files for easy download.
