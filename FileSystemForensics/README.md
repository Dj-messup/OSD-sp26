# File System Forensics Write-Up

This assignment demonstrates something interesting with file systems using a terminal-based file system forensics example.

I created a raw FAT filesystem image, copied a file into it, deleted the file, and then searched the raw image to show that the deleted file content could still be found.

## Why I Chose This

I chose file system forensics because it connects directly to how filesystems store and delete data.

A file can be deleted from the directory listing, but that does not always mean the actual data is erased from the storage area right away. The space may just be marked as available, meaning the old content can stay there until something else overwrites it.

## Connection to Class

This connects to the class filesystem demos because the class examples use raw filesystem images and VFAT formatting.

This write-up uses the same general idea:

- create a raw image
- format it as a FAT/VFAT filesystem
- put files inside it
- inspect what is left behind after deletion

## Files

FileSystemForensics/
- Makefile
- README.md
- .gitignore

The generated files are not meant to be committed:

- evidence.vfat
- secret.txt
- recovered_strings.txt

## Tools Used

This demo uses:

- dd
- mkfs.vfat
- mcopy
- mdir
- mdel
- strings
- grep

The FAT tools come from dosfstools and mtools.

## Setup

Run this if the tools are not installed:

    make setup

## How to Run the Demo

Run:

    make run

or:

    make demo

## How to Clean

Run:

    make clean

## What the Demo Does

The Makefile runs the terminal commands needed for the demo.

First, it creates a 16 MB raw image:

    dd if=/dev/zero of=evidence.vfat bs=1M count=16 status=none

Then it formats the image as a FAT filesystem:

    mkfs.vfat evidence.vfat

Then it creates an evidence file named secret.txt with this content:

    SECRET=HAWAII_FILESYSTEM_FORENSICS_DEMO

Then it copies that file into the filesystem image as SECRET.TXT.

After that, it lists the filesystem image and shows that SECRET.TXT exists.

Then it deletes SECRET.TXT from the filesystem image.

After deletion, the directory listing shows:

    No files

Finally, it runs strings on the raw filesystem image and searches for the deleted evidence string.

## Expected Result

The important result is that the file is gone from the normal directory listing, but the raw filesystem image still contains the deleted file content.

Example result:

    SECRET   TXT       134

Then after deletion:

    No files

Then the deleted content is still found:

    SECRET=HAWAII_FILESYSTEM_FORENSICS_DEMO

## Main Point

This shows that deleting a file and destroying the data are not always the same thing.

The file no longer appears in the filesystem directory listing, but the content can still be recovered from raw filesystem data.

That is why file system forensics matters. Deleted files may still leave evidence behind until the storage space is overwritten.
