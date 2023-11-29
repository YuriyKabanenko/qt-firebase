#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include "user.h"

enum FirebaseOperation {
    SignIn,
    SignUp,
    DeleteAccount,
    ConfirmEmail,
    GetUserData
};

class AuthHandler : public QObject
{
    Q_OBJECT
public:
    explicit AuthHandler(QObject *parent = nullptr, User* user = nullptr, QWidget* widgetParent = nullptr);
    ~AuthHandler();
    void setAPIKey( const QString & apiKey );
    void signUserUp( const QString & emailAddress, const QString & password );
    User signUserIn( const QString & emailAddress, const QString & password );
    void sendEmailVerification(QString idToken);
    void deleteAccount(QString idToken);
    void getUserData(QString idToken);

    bool getEmailVerified();
public slots:
    void networkReplyReadyRead();
signals:
    void userSignedIn();
    void accountDeleted();
    void emailVerified();
    void userDataUpdated();
private:
    void performPOST( const QString & url, const QJsonDocument & payload );
    void parseResponse( const QByteArray & reponse);
    QString m_apiKey;
    QNetworkAccessManager * m_networkAccessManager;
    QNetworkReply * m_networkReply;
    QString m_idToken;
    User* user;
    FirebaseOperation currentOperation = FirebaseOperation::SignUp;
    QWidget* parent;
};

#endif // AUTHHANDLER_H
