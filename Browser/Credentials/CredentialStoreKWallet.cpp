#include "CredentialStoreKWallet.h"

#include <kwallet.h>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QDebug>

CredentialStoreKWallet::CredentialStoreKWallet() :
    CredentialStore(),
    m_wallet(nullptr),
    m_credentials()
{
    openWallet();
}

CredentialStoreKWallet::~CredentialStoreKWallet()
{
    if (m_wallet != nullptr)
        delete m_wallet;
}

void CredentialStoreKWallet::addCredentials(const WebCredentials &credentials)
{
    if (credentials.Host.isEmpty())
        return;

    std::vector<WebCredentials> &creds = m_credentials[credentials.Host];
    creds.push_back(credentials);

    saveCredentialsFor(credentials.Host);
}

std::vector<WebCredentials> CredentialStoreKWallet::getCredentialsFor(const QUrl &url)
{
    const QString host = url.host();
    if (!m_credentials.contains(host))
        return std::vector<WebCredentials>();

    return m_credentials.value(host);
}

void CredentialStoreKWallet::removeCredentials(const WebCredentials &credentials)
{
    if (!m_credentials.contains(credentials.Host))
        return;

    std::vector<WebCredentials> &creds = m_credentials[credentials.Host];
    for (auto it = creds.begin(); it != creds.end();)
    {
        if (it->Username.compare(credentials.Username) == 0)
            it = creds.erase(it);
        else
            ++it;
    }

    saveCredentialsFor(credentials.Host);
}

void CredentialStoreKWallet::updateCredentials(const WebCredentials &credentials)
{
    if (!m_credentials.contains(credentials.Host))
        return;

    std::vector<WebCredentials> &creds = m_credentials[credentials.Host];
    for (WebCredentials &item : creds)
    {
        if (item.Username.compare(credentials.Username) == 0)
        {
            item.Password = credentials.Password;
            item.FormData = credentials.FormData;
            item.LastLogin = credentials.LastLogin;
        }
    }

    saveCredentialsFor(credentials.Host);
}

void CredentialStoreKWallet::openWallet()
{
    if (m_wallet != nullptr)
        return;

    m_wallet = KWallet::Wallet::openWallet(KWallet::Wallet::NetworkWallet(), 0);

    if (m_wallet == nullptr || !m_wallet->isOpen())
    {
        qDebug() << "CredentialStoreKWallet: could not open network wallet";
        return;
    }

    const QString folderName = QLatin1String("Viper-Browser");

    if (!m_wallet->hasFolder(folderName) && !m_wallet->createFolder(folderName))
    {
        qDebug() << "CredentialStoreKWallet: could not create folder Viper-Browser";
        return;
    }

    if (!m_wallet->setFolder(folderName))
    {
        qDebug() << "CredentialStoreKWallet: could not set working folder to Viper-Browser";
        return;
    }

    // Load the credentials saved for each website
    QStringList entryList = m_wallet->entryList();
    for (const QString &host : entryList)
    {
        std::vector<WebCredentials> credentials;

        QByteArray data;
        if (m_wallet->readEntry(host, data) != 0)
        {
            qDebug() << "CredentialStoreKWallet: could not read entry for " << host;
            continue;
        }

        QDataStream stream(&data, QIODevice::ReadOnly);

        int numEntries;
        stream >> numEntries;

        for (int i = 0; i < numEntries; ++i)
        {
            WebCredentials item;
            stream >> item;
            credentials.push_back(item);
        }

        m_credentials[host] = credentials;
    }
}

void CredentialStoreKWallet::saveCredentialsFor(const QString &host)
{
    const std::vector<WebCredentials> &creds = m_credentials[host];
    int numEntries = static_cast<int>(creds.size());

    QByteArray walletData;
    QDataStream stream(&walletData, QIODevice::WriteOnly);
    stream << numEntries;

    for (const WebCredentials &item : creds)
        stream << item;

    stream.device()->close();

    if (m_wallet->writeEntry(host, walletData) != 0)
    {
        qDebug() << "CredentialStoreKWallet: could not save credentails for " << host;
    }
}
