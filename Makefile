#!/usr/bin/make

OUT_DIR=target/
BUILD_TYPE?=Debug
VERSION=0.0.1

.PHONY: compile clean package all
.DEFAULT_GOAL := compile


${OUT_DIR}:
	mkdir -p $@

compile: | ${OUT_DIR}
	cd ${OUT_DIR} && cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} .. && make -j

package-deb: compile
	fpm --name klamath \
	    --version ${VERSION} \
	    --maintainer "Adam Kewley <contact@adamkewley.com>" \
	    --url "https://adamkewley.com" \
	    --description "Utilities for working with Fallout 1/2 assets" \
	    --license "Apache-2.0" \
	    --architecture "x86_64" \
	    --depends "libsdl2-2.0-0,zlib1g,libstdc++6" \
	    --input-type dir \
	    --output-type deb \
            --force \
	    target/klamath/klamath=/usr/local/bin/klamath

package: package-deb

clean:
	rm -rf ${OUT_DIR}
