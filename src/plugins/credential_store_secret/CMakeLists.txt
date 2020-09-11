set(credential_store_secret_src
    CredentialStoreSecret.cpp
)

add_library(CredentialStoreSecret SHARED ${credential_store_secret_src})

target_link_libraries(CredentialStoreSecret
  viper-core
  Qt5::Core
)
install(TARGETS CredentialStoreSecret DESTINATION "${VIPER_PLUGIN_DIR}")
