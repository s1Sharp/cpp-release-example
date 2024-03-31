.PHONY: test build

config:
	cmake -B ./build -DCMAKE_BUILD_TYPE=Release -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_SHARED=OFF -DREDIS_PLUS_PLUS_BUILD_STATIC=ON -DBUILD_SHARED_LIB=OFF -G Ninja

config_shared:
	cmake -B ./build -DCMAKE_BUILD_TYPE=Release -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_SHARED=ON -DREDIS_PLUS_PLUS_BUILD_STATIC=OFF -DBUILD_SHARED_LIB=ON -G Ninja

build_shared: config_shared
	cmake --build ./build --target hiredis --config Release
	cmake --build ./build --target all --config Release

build: config
	cmake --build ./build --target all --config Release

pack:
	cmake --build ./build --target package --verbose
	cd build/_packages && dpkg --info fancy-app-example-static*.deb || true
	cd build/_packages && dpkg --contents fancy-app-example-static*.deb || true
	cd build/_packages && dpkg --info fancy-app-example-shared*.deb || true
	cd build/_packages && dpkg --contents fancy-app-example-shared*.deb || true

test: build
	cmake --build ./build --target fancy-test
	cd build/test && ctest

clean:
	rm -rf build/

cli:
	./build/app/db/client/fancy_client localhost

ser:
	./build/app/db/server/fancy_server
