.PHONY: test build

config:
	cmake -B ./build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIB=OFF

config_shared:
	cmake -B ./build -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIB=ON

build_shared: config_shared
	cmake --build ./build --target all --config Debug

build: config
	cmake --build ./build --target all --config Debug

pack:
	cmake --build ./build --target package
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
	./build/app/db/client/client localhost

ser:
	./build/app/db/server/server
