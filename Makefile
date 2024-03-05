.PHONY: test build

conan_conf:
	conan profile detect --force
	conan install . --build=missing -pr conan_debug_profile.txt -c tools.system.package_manager:mode=install
	cmake --preset conan-debug

build_conan:
	cmake --build ./build/Debug --target fancy-test
	cmake --build ./build/Debug --target fancy-test

config:
	cmake -B ./build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIB=OFF

config_shared:
	cmake -B ./build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIB=ON

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

test_conan: build_conan
	cmake --build ./build/Debug --target fancy-test
	cd build/Debug/test && ctest

clean:
	rm -rf build/
