cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(KadNode VERSION 1.0.0 LANGUAGES C)

find_package(MbedTLS 2 REQUIRED)
find_package(NATPMP REQUIRED)

option(BUILD_SHARED_LIBS     "Build shared instead of static libraries."              OFF)
option(DNS		"dns"		OFF)
option(LPD		"lpd"		OFF)
option(TLS		"tls"		OFF)
option(BOB		"bob"		OFF)

file(GLOB_RECURSE headers include/kadnode/*.h)

set(sources
	src/announces.c
	src/conf.c
	src/dht.c
	#src/ext-bob.c
	#src/ext-cmd.c
	#src/ext-dns.c
	#src/ext-fwd.c
	#src/ext-libnss.c
	#src/ext-lpd.c
	#src/ext-nss.c
	#src/ext-tls-client.c
	#src/ext-tls-server.c
	src/kad.c
	src/log.c
	src/natpmp.c
	src/net.c
	src/peerfile.c
	src/searches.c
	#src/unix.c
	src/upnp.c
	src/utils.c
	#src/windows.c
)

if(WIN32)
	list(APPEND sources src/windows.c)
else(WIN32)
	list(APPEND sources src/unix.c)
endif(WIN32)

if(DNS)
	list(APPEND sources src/ext-dns.c)
	add_definitions(-DDNS)
endif(DNS)

if(LPD)
	list(APPEND sources src/ext-lpd.c)
	add_definitions(-DLPD)
endif(LPD)

if(TLS)
	list(APPEND sources src/ext-tls-client.c src/ext-tls-server.c)
	add_definitions(-DTLS)
endif(TLS)

if(BOB)
	list(APPEND sources src/ext-bob.c)
	add_definitions(-DBOB)
endif(BOB)

file(COPY ${headers} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/kadnode/)

add_library( libkadnode src/libkadnode.c ${sources} ${headers} )

add_library(KadNode::libkadnode ALIAS libkadnode)

target_link_libraries(libkadnode
	PRIVATE
	PUBLIC
	${MBEDTLS_LIBRARY} ${MBEDX509_LIBRARY} ${MBEDCRYPTO_LIBRARY} ${NATPMP_LIBRARY}
	INTERFACE
)

target_include_directories (libkadnode PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include/)

target_compile_definitions(libkadnode
PRIVATE
PUBLIC
$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:LIBKADNODE_STATIC_DEFINE>
INTERFACE
)