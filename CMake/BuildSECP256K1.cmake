cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

if (NOT SECP256K1_LIBRARY)

    set(SECP256K1_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/secp256k1)
    set(SECP256K1_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third-party/secp256k1)
    set(SECP256K1_INCLUDE_DIR ${SECP256K1_BINARY_DIR}/include)

    file(MAKE_DIRECTORY ${SECP256K1_BINARY_DIR})

    if (MSVC)
      set(SECP256K1_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/secp256k1)
      set(SECP256K1_LIBRARY ${SECP256K1_SOURCE_DIR}/bin/x64/Release/v142/static/secp256k1.lib)
      set(SECP256K1_INCLUDE_DIR ${SECP256K1_BINARY_DIR}/include)
      add_custom_command(
        WORKING_DIRECTORY ${SECP256K1_SOURCE_DIR}
        COMMAND cd builds/msvc/vs2017
        COMMAND msbuild /m /v:n /p:Configuration=StaticRelease -p:PlatformToolset=v142 -p:Platform=x64
        COMMENT "Build secp256k1 with vs2017"
        DEPENDS ${SECP256K1_SOURCE_DIR}
        OUTPUT ${SECP256K1_LIBRARY}
      )
    elseif (EMSCRIPTEN)
      set(SECP256K1_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/secp256k1)
      set(SECP256K1_LIBRARY ${SECP256K1_BINARY_DIR}/.libs/libsecp256k1.a)
      set(SECP256K1_INCLUDE_DIR ${SECP256K1_BINARY_DIR}/include)
      add_custom_command(
          WORKING_DIRECTORY ${SECP256K1_SOURCE_DIR}
          COMMAND ./autogen.sh
          COMMAND emconfigure ./configure --disable-option-checking --enable-module-recovery --with-pic --disable-shared --enable-static --disable-tests --disable-benchmark
          COMMAND emmake make
          COMMENT "Build secp256k1 with emscripten"
          DEPENDS ${SECP256K1_SOURCE_DIR}
          OUTPUT ${SECP256K1_LIBRARY}
      )
    else()
      set(SECP256K1_LIBRARY ${SECP256K1_BINARY_DIR}/lib/libsecp256k1.a)
      add_custom_command(
          WORKING_DIRECTORY ${SECP256K1_SOURCE_DIR}
          COMMAND ./autogen.sh
          COMMAND ./configure --disable-option-checking --enable-module-recovery --prefix ${SECP256K1_BINARY_DIR} --with-pic --disable-shared --enable-static --disable-tests --disable-benchmark
          COMMAND make
          COMMAND make install
          COMMENT "Build secp256k1"
          DEPENDS ${SECP256K1_SOURCE_DIR}
          OUTPUT ${SECP256K1_LIBRARY}
      )
    endif()

else()
   message(STATUS "Use secp256k1: ${SECP256K1_LIBRARY}")
endif()

add_custom_target(secp256k1 DEPENDS ${SECP256K1_LIBRARY})
