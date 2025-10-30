SGDK_VERSION=2.11
UID=$(shell id -u)
GID=$(shell id -g)
DOCKER_IMAGE=registry.gitlab.com/doragasu/docker-sgdk:v${SGDK_VERSION}

.PHONY: compile
compile:
	docker run -it --rm --user ${UID}:${GID} -v "${PWD}":/workdir -w /workdir ${DOCKER_IMAGE}

shell:
	docker run -it --rm -v "${PWD}":/workdir -w /workdir --entrypoint=/bin/bash ${DOCKER_IMAGE}

clean:
	rm -rf out/* build/* src/boot/*

format:
	clang-format -i src/main.c

RETROARCH ?= $(shell which retroarch 2>/dev/null || [ -x /Applications/RetroArch.app/Contents/MacOS/RetroArch ] && echo /Applications/RetroArch.app/Contents/MacOS/RetroArch)
RETROARCH_CORE ?= $(shell find "$${HOME}/Library/Application Support/RetroArch/cores" -type f -name "genesis_plus_gx_libretro.*" 2>/dev/null | head -n1)
run:
	"${RETROARCH}" -L "${RETROARCH_CORE}" out/rom.bin