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

User::User(const QString &id, const QString &email, const QString &token, const QString &refreshToken, bool isEmailVerified) : id(id),
    email(email),
    token(token),
    refreshToken(refreshToken),
    isEmailVerified(isEmailVerified)
{}
