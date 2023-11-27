#include "user.h"

User::User()
{

}

QString User::getId() const
{
    return id;
}

QString User::getEmail() const
{
    return email;
}

bool User::getIsEmailVerified() const
{
    return isEmailVerified;
}

void User::setId(const QString &newId)
{
    id = newId;
}

void User::setEmail(const QString &newEmail)
{
    email = newEmail;
}

void User::setToken(const QString &newToken)
{
    token = newToken;
}

void User::setRefreshToken(const QString &newRefreshToken)
{
    refreshToken = newRefreshToken;
}

void User::setIsEmailVerified(bool newIsEmailVerified)
{
    isEmailVerified = newIsEmailVerified;
}

User::User(const QString &id, const QString &email, const QString &token, const QString &refreshToken, bool isEmailVerified) : id(id),
    email(email),
    token(token),
    refreshToken(refreshToken),
    isEmailVerified(isEmailVerified)
{}
