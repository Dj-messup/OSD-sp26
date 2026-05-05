.RECIPEPREFIX := >

IMAGE = fs-forensics-demo

.PHONY: build run run-docker run-local test clean

build:
> docker build -t $(IMAGE) .

run:
> if command -v docker >/dev/null 2>&1; then \
>     echo "Docker found. Running demo inside Docker."; \
>     docker build -t $(IMAGE) .; \
>     docker run --rm -v "$(PWD):/work" -w /work $(IMAGE) bash fs_forensics.sh; \
> else \
>     echo "Docker not found. Running demo locally."; \
>     bash fs_forensics.sh; \
> fi

run-docker: build
> docker run --rm -v "$(PWD):/work" -w /work $(IMAGE) bash fs_forensics.sh

run-local:
> bash fs_forensics.sh

test: run

clean:
> rm -f evidence.vfat secret.txt recovered_strings.txt
