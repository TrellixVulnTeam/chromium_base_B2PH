project (crcrypto)
ADD_DEFINITIONS(-DNOMINMAX)
if (WIN32)  
ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN -DUNICODE -D_UNICODE)
endif(WIN32)

set (CMAKE_CXX_STANDARD 14)

if(MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
endif()

INCLUDE_DIRECTORIES (. .. ../third_party/boringssl/src/include)


FILE (GLOB source_files  "aead.cc"
    "aead.h"
    "crypto_export.h"
    "ec_private_key.cc"
    "ec_private_key.h"
    "ec_signature_creator.cc"
    "ec_signature_creator.h"
    "ec_signature_creator_impl.cc"
    "ec_signature_creator_impl.h"
    "encryptor.cc"
    "encryptor.h"
    "hkdf.cc"
    "hkdf.h"
    "hmac.cc"
    "hmac.h"
    "nss_crypto_module_delegate.h"
    "openssl_util.cc"
    "openssl_util.h"
    "p224.cc"
    "p224.h"
    "p224_spake.cc"
    "p224_spake.h"
    "random.cc"
    "random.h"
    "rsa_private_key.cc"
    "rsa_private_key.h"
    "scoped_capi_types.h"
    "scoped_nss_types.h"
    "secure_hash.cc"
    "secure_hash.h"
    "secure_util.cc"
    "secure_util.h"
    "sha2.cc"
    "sha2.h"
    "signature_creator.cc"
    "signature_creator.h"
    "signature_verifier.cc"
    "signature_verifier.h"
    "symmetric_key.cc"
    "symmetric_key.h")


if (WIN32)  
  list(APPEND source_files 
    "capi_util.cc"
    "capi_util.h")

elseif (IOS)


elseif (OSX)


elseif (ANDROID)

endif(WIN32)
 
ADD_LIBRARY (crcrypto ${source_files})
