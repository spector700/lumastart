.PHONY: build build-debug run clean default install

default: build

build:
	meson build
	ninja -C build

build-debug:
	meson build --buildtype=debug
	ninja -C build

install: build
	ninja -C build install

run: build
	./build/lumastart

debug-run: build-debug
	# ./build/lumastart --log-level debug
	./build/lumastart

test:
	meson test -C build --no-rebuild --verbose --suite lumastart
.PHONY: test

clean:
	rm -rf build
