#ifndef CREDENTIALSTORESECRET_H
#define CREDENTIALSTORESECRET_H

#include "CredentialStore.h"

/**
 * @class CredentialStoreSecret
 * @brief Implementation of the credential store using Secret as a backend storage system
 */
class CredentialStoreSecret : public QObject, public CredentialStore
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CredentialStore_iid FILE "credentialstoresecret.json")
    Q_INTERFACES(CredentialStore)
};

#endif // CREDENTIALSTORESECRET_H
