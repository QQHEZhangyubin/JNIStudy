GENCCODE_ASSEMBLY_TYPE=-a gcc
SO=so
SOBJ=so
A=a
LIBPREFIX=lib
LIB_EXT_ORDER=.62.1
COMPILE=/Users/anker/Library/Android/sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/darwin-x86_64/bin/armv7a-linux-androideabi19-clang -ffunction-sections -fdata-sections -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -DU_HAVE_STRTOD_L=0  -DU_ATTRIBUTE_DEPRECATED= -g -ffunction-sections -fdata-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -mthumb -Wformat -Werror=format-security -Oz -DNDEBUG -fPIC -frtti -fexceptions  -std=c11 -Wall -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings   -c
LIBFLAGS=-I/Users/anker/Desktop/3d3/a_boost/Boost-for-Android/libiconv-libicu-android/armeabi-v7a/icu/source/../..//include -DPIC -fPIC
GENLIB=/Users/anker/Library/Android/sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/darwin-x86_64/bin/armv7a-linux-androideabi19-clang -g -ffunction-sections -fdata-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -mthumb -Wformat -Werror=format-security -Oz -DNDEBUG -fPIC -frtti -fexceptions  -std=c11 -Wall -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings   -fPIC -g -ffunction-sections -fdata-sections -Wl,--gc-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -mthumb -Wformat -Werror=format-security -Oz -DNDEBUG -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments -Wl,-z,relro -Wl,-z,now -Wl,--no-rosegment -landroid -llog -latomic -lm -ldl -frtti -fexceptions -L/Users/anker/Desktop/3d3/a_boost/Boost-for-Android/libiconv-libicu-android/armeabi-v7a/lib  -Wl,--gc-sections  -shared -Wl,-Bsymbolic
LDICUDTFLAGS=-nodefaultlibs -nostdlib
LD_SONAME=
RPATH_FLAGS=
BIR_LDFLAGS=-Wl,-Bsymbolic
AR=/Users/anker/Library/Android/sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/darwin-x86_64/bin/llvm-ar
ARFLAGS=r
RANLIB=/Users/anker/Library/Android/sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/darwin-x86_64/bin/llvm-ranlib
INSTALL_CMD=/usr/bin/install -c
