#!/usr/bin/env bash
set -euo pipefail

IMAGE="evidence.vfat"
SECRET_FILE="secret.txt"
RECOVERED="recovered_strings.txt"

echo "File System Forensics Demo"
echo

echo "[1] Creating a 16 MB raw disk image..."
dd if=/dev/zero of="$IMAGE" bs=1M count=16 status=none

echo "[2] Formatting the image as a FAT filesystem..."
mkfs.vfat "$IMAGE" > /dev/null

echo "[3] Creating an evidence file with recognizable content..."
cat > "$SECRET_FILE" <<'SECRET_EOF'
CASE_ID=OSD-FS-001
EVIDENCE=Deleted files may leave recoverable data in raw filesystem space.
SECRET=HAWAII_FILESYSTEM_FORENSICS_DEMO
SECRET_EOF

echo "[4] Copying the evidence file into the FAT filesystem image..."
mcopy -i "$IMAGE" "$SECRET_FILE" ::SECRET.TXT

echo "[5] Listing files inside the filesystem image before deletion..."
mdir -i "$IMAGE" ::

echo
echo "[6] Deleting SECRET.TXT from the filesystem image..."
mdel -i "$IMAGE" ::SECRET.TXT

echo "[7] Listing files inside the filesystem image after deletion..."
mdir -i "$IMAGE" ::

echo
echo "[8] Running strings against the raw filesystem image..."
strings "$IMAGE" > "$RECOVERED"

echo "[9] Searching the raw image for deleted evidence content..."
if grep -n "HAWAII_FILESYSTEM_FORENSICS_DEMO" "$RECOVERED"; then
    echo
    echo "Recovered deleted file content from raw filesystem data."
else
    echo
    echo "Deleted content was not found."
    exit 1
fi

echo
echo "Demo complete."
echo "Created files:"
echo "- $IMAGE"
echo "- $SECRET_FILE"
echo "- $RECOVERED"
