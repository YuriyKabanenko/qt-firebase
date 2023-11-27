#ifndef USER_H
#define USER_H

#include <QObject>
class User
{
public:
    User();
    User(const QString &id, const QString &email, const QString &token, const QString &refreshToken, bool isEmailVerified);

    QString getId() const;
    QString getEmail() const;
    bool getIsEmailVerified() const;



    void setId(const QString &newId);
    void setEmail(const QString &newEmail);
    void setToken(const QString &newToken);
    void setRefreshToken(const QString &newRefreshToken);
    void setIsEmailVerified(bool newIsEmailVerified);

private:
    QString id;
    QString email;
    QString token;
    QString refreshToken;
    bool isEmailVerified;
};

#endif // USER_H
