# File System Forensics Demo

This project demonstrates something interesting with file systems by creating a raw FAT filesystem image, deleting a file from it, and then recovering leftover file content from the raw filesystem data.

## Purpose

The purpose of this demo is to show that deleting a file does not always erase the file contents from storage.

A filesystem can remove a file from the directory listing or mark its space as available, but the old data may still remain in the raw disk image until it is overwritten.

## Connection to Class Demo

This project is based on the file-system examples from the class repository.

The class file-system demo shows how to create a raw file with dd, format it as a VFAT filesystem with mkfs.vfat, and work with that filesystem image.

This project extends that idea by using a FAT filesystem image for a small forensic recovery demonstration.

## What This Demonstrates

This demo shows:

1. Creating a raw disk image
2. Formatting the image as a FAT filesystem
3. Copying a file into the filesystem image
4. Listing the file before deletion
5. Deleting the file from the filesystem image
6. Confirming the file no longer appears in the directory listing
7. Searching the raw filesystem image for deleted content

## Files

FileSystemForensics/
- Dockerfile
- Makefile
- README.md
- fs_forensics.sh

## How to Run

Run:

make run

The Makefile tries to use Docker if Docker is available.

If Docker is not available, the Makefile runs the script locally. The local version requires dosfstools, mtools, and binutils.

## How to Run Locally

Run:

make run-local

## How to Run With Docker

Run:

make run-docker

## How to Clean

Run:

make clean

## Expected Behavior

The script creates a raw FAT filesystem image named evidence.vfat.

It then creates a file named secret.txt and copies it into the filesystem image as SECRET.TXT.

Before deletion, the directory listing shows SECRET.TXT.

After deletion, the directory listing shows no files.

The script then runs strings against the raw filesystem image and searches for the deleted evidence string.

## Evidence String

The deleted evidence string is:

SECRET=HAWAII_FILESYSTEM_FORENSICS_DEMO

If this string is found after SECRET.TXT was deleted, the demo shows that deleted file content can still remain inside raw filesystem data.

## Example Output

File System Forensics Demo

[1] Creating a 16 MB raw disk image...
[2] Formatting the image as a FAT filesystem...
[3] Creating an evidence file with recognizable content...
[4] Copying the evidence file into the FAT filesystem image...
[5] Listing files inside the filesystem image before deletion...

SECRET   TXT       134

[6] Deleting SECRET.TXT from the filesystem image...
[7] Listing files inside the filesystem image after deletion...

No files

[8] Running strings against the raw filesystem image...
[9] Searching the raw image for deleted evidence content...

8:SECRET=HAWAII_FILESYSTEM_FORENSICS_DEMO

Recovered deleted file content from raw filesystem data.

## Main Concept

This is a file-system forensics demonstration.

The important idea is that file deletion and data destruction are not the same thing.

A deleted file may disappear from the normal directory listing, but forensic tools can sometimes recover leftover content from the raw filesystem image.
