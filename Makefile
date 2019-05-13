#!/usr/bin/make

OUT_DIR=target/
BUILD_TYPE?=Debug
VERSION?=0.0.3

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
	    --depends "libsdl2-2.0-0,libstdc++6" \
	    --input-type dir \
	    --output-type deb \
            --force \
	    ${OUT_DIR}klamath/klamath=/usr/local/bin/klamath

package: package-deb

deploy:
	cp klamath_${VERSION}_amd64.deb ~/Dropbox/Projects/prototypes/klamath/releases

install:
	dpkg -i klamath_${VERSION}_amd64.deb

clean:
	rm -rf ${OUT_DIR}
