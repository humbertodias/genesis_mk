SGDK_VERSION := 2.11
UID := $(shell id -u)
GID := $(shell id -g)
DOCKER_IMAGE := registry.gitlab.com/doragasu/docker-sgdk:v$(SGDK_VERSION)
UNAME_S := $(shell uname -s)
RETROARCH ?= $(shell which retroarch 2>/dev/null)
CORE := genesis_plus_gx
TAG_NAME := $(shell git describe --tags --exact-match 2>/dev/null || git rev-parse --short HEAD)

ifeq ($(UNAME_S),Darwin)
    RETROARCH := /Applications/RetroArch.app/Contents/MacOS/RetroArch
endif

.PHONY: compile
compile:
	docker run --rm --user ${UID}:${GID} -v "${PWD}":/workdir -w /workdir ${DOCKER_IMAGE}

shell:
	docker run -it --rm -v "${PWD}":/workdir -w /workdir --entrypoint=/bin/bash ${DOCKER_IMAGE}

clean:
	rm -rf out/* build/* src/boot/*

format:
	clang-format -i src/main.c

run:
	"${RETROARCH}" -L "${CORE}" out/rom.bin
	
zip:
	zip -9 -j mk-plus-$(TAG_NAME).zip out/rom.bin

doxygen:
	docker run --rm -v "${PWD}":/workdir -w /workdir nakatt/doxygen

doxygen/serve:
	docker run -d -p 8080:80 --name doxygen-server --rm -v "${PWD}"/docs/html:/usr/share/nginx/html:ro nginx
	echo "Doxygen documentation server running at http://localhost:8080"
