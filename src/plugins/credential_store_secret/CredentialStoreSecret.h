#ifndef CREDENTIALSTORESECRET_H
#define CREDENTIALSTORESECRET_H

#include "CredentialStore.h"

namespace Secret {
    struct SecretSchema;
}

/**
 * @class CredentialStoreSecret
 * @brief Implementation of the credential store using Secret as a backend storage system
 */
class CredentialStoreSecret : public QObject, public CredentialStore
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CredentialStore_iid FILE "credentialstoresecret.json")
    Q_INTERFACES(CredentialStore)

public:
    /// Constructor. Opens the wallet backend
    explicit CredentialStoreSecret();

    /// Credential store destructor, frees resources used by the wallet
    ~CredentialStoreSecret();

    /// Returns a list of the hosts which have at least one set of credentials in the store
    std::vector<QString> getHostNames() override;

    /// Adds a set of credentials to the store
    void addCredentials(const WebCredentials &credentials) override;

    /// Returns a list of the credentials that have been saved for the given url
    std::vector<WebCredentials> getCredentialsFor(const QUrl &url) override;

    /// Removes the set of credentials from the store
    void removeCredentials(const WebCredentials &credentials) override;

    /// Updates the credentials
    void updateCredentials(const WebCredentials &credentials) override;

private:
    // Defines the schema used to store WebCredentials
    static const SecretSchema Schema;
};

#endif // CREDENTIALSTORESECRET_H
