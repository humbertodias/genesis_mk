SGDK_VERSION=2.11
UID=$(shell id -u)
GID=$(shell id -g)
DOCKER_IMAGE=registry.gitlab.com/doragasu/docker-sgdk:v${SGDK_VERSION}
UNAME_S := $(shell uname -s)
RETROARCH ?= $(shell which retroarch 2>/dev/null)
RETROARCH_CORE_NAME = blastem

ifeq ($(UNAME_S),Darwin)
    RETROARCH ?= /Applications/RetroArch.app/Contents/MacOS/RetroArch
    RETROARCH_CORE ?= $(shell find "$${HOME}/Library/Application Support/RetroArch/cores" -type f -name "${RETROARCH_CORE_NAME}_libretro.dylib" 2>/dev/null | head -n1)
else
    RETROARCH_CORE ?= $(shell find "$${HOME}/.config/retroarch/cores" -type f -name "${RETROARCH_CORE_NAME}_libretro.so" 2>/dev/null | head -n1)
endif

.PHONY: compile
compile:
	docker run -it --rm --user ${UID}:${GID} -v "${PWD}":/workdir -w /workdir ${DOCKER_IMAGE}

shell:
	docker run -it --rm -v "${PWD}":/workdir -w /workdir --entrypoint=/bin/bash ${DOCKER_IMAGE}

clean:
	rm -rf out/* build/* src/boot/*

format:
	clang-format -i src/main.c

run:
	"${RETROARCH}" -L "${RETROARCH_CORE}" out/rom.bin