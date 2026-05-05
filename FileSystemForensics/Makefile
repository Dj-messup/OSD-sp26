.RECIPEPREFIX := >

.PHONY: setup demo run clean

setup:
> sudo apt-get update
> sudo apt-get install -y dosfstools mtools binutils

demo:
> rm -f evidence.vfat secret.txt recovered_strings.txt
> echo "Creating 16 MB raw filesystem image..."
> dd if=/dev/zero of=evidence.vfat bs=1M count=16 status=none
> echo "Formatting image as FAT/VFAT..."
> mkfs.vfat evidence.vfat > /dev/null
> echo "Creating evidence file..."
> printf "%s\n" "CASE_ID=OSD-FS-001" "EVIDENCE=Deleted files may leave recoverable data in raw filesystem space." "SECRET=HAWAII_FILESYSTEM_FORENSICS_DEMO" > secret.txt
> echo "Copying evidence file into filesystem image..."
> mcopy -i evidence.vfat secret.txt ::SECRET.TXT
> echo
> echo "Directory listing before deletion:"
> mdir -i evidence.vfat ::
> echo
> echo "Deleting SECRET.TXT from filesystem image..."
> mdel -i evidence.vfat ::SECRET.TXT
> echo
> echo "Directory listing after deletion:"
> mdir -i evidence.vfat ::
> echo
> echo "Searching raw filesystem image for deleted content..."
> strings evidence.vfat > recovered_strings.txt
> grep -n "HAWAII_FILESYSTEM_FORENSICS_DEMO" recovered_strings.txt
> echo
> echo "Recovered deleted file content from raw filesystem data."

run: demo

clean:
> rm -f evidence.vfat secret.txt recovered_strings.txt
