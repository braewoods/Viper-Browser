#include <libsecret/secret.h>
#include "./CredentialStoreSecret.h"

// Schema used to store WebCredentials
static const SecretSchema Schema =
{
    CredentialStore_iid,
    SECRET_SCHEMA_NONE,
    {
        { "host",      SECRET_SCHEMA_ATTRIBUTE_STRING,  },
        { "lastlogin", SECRET_SCHEMA_ATTRIBUTE_INTEGER, },
        { "username",  SECRET_SCHEMA_ATTRIBUTE_STRING,  },
        { "formdata",  SECRET_SCHEMA_ATTRIBUTE_STRING,  },
    },
};
