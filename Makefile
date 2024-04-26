
init_env:
	sudo apt-get update ; sudo apt-get install autoconf -y ; \
	yes | cp -rf libxcrypt/ vcpkg/ports

build_debug:
	mkdir -p build ; cd build ;\
	cmake -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
	-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
	-DCMAKE_BUILD_TYPE=Debug ../ ; \
	make -j10

clean:
	rm -fr build
