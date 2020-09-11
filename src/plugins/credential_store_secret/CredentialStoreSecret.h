#ifndef CREDENTIALSTORESECRET_H
#define CREDENTIALSTORESECRET_H

#include "CredentialStore.h"

/**
 * @class CredentialStoreSecret
 * @brief Implementation of the credential store using Secret as a backend storage system
 */
class CredentialStoreSecret : public QObject, public CredentialStore
{
};

#endif // CREDENTIALSTORESECRET_H
