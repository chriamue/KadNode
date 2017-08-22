cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(KadNode VERSION 1.0.0 LANGUAGES C)

find_package(MbedTLS REQUIRED)
find_package(NATPMP REQUIRED)

file(GLOB_RECURSE headers src/*.h)

set(sources
	src/announces.c
	src/conf.c
	src/dht.c
	src/ext-bob.c
	#src/ext-cmd.c
	#src/ext-dns.c
	#src/ext-fwd.c
	#src/ext-libnss.c
	#src/ext-lpd.c
	#src/ext-nss.c
	#src/ext-tls-client.c
	#src/ext-tls-server.c
	src/kad.c src/log.c
	src/natpmp.c
	src/net.c
	src/peerfile.c
	src/searches.c
	src/unix.c
	src/upnp.c
	src/utils.c
	#src/windows.c
)

file(COPY ${headers} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/KadNode/)

add_library( libkadnode src/libkadnode.c ${sources} ${headers} )

add_library(KadNode::libkadnode ALIAS libkadnode)

target_link_libraries(libkadnode
	PRIVATE
	PUBLIC
	${MBEDTLS_LIBRARY} ${MBEDX509_LIBRARY} ${MBEDCRYPTO_LIBRARY} ${NATPMP_LIBRARY}
	INTERFACE
)

target_include_directories (libkadnode PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include/)
