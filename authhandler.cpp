#include "authhandler.h"
#include <QDebug>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QMessageBox>

AuthHandler::AuthHandler(QObject *parent, User* user, QWidget* widgetParent)
    : QObject(parent)
    , m_apiKey( QString() )
{
    m_networkAccessManager = new QNetworkAccessManager( this );
    this->user = user;
    this->parent = widgetParent;
}

AuthHandler::~AuthHandler()
{
    m_networkAccessManager->deleteLater();
}

void AuthHandler::setAPIKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}

void AuthHandler::signUserUp(const QString &emailAddress, const QString &password)
{
    this->currentOperation = FirebaseOperation::SignUp;
    QString signUpEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPOST( signUpEndpoint, jsonPayload );
}

User AuthHandler::signUserIn(const QString &emailAddress, const QString &password)
{
    this->currentOperation = FirebaseOperation::SignIn;
    QString signInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );

    performPOST( signInEndpoint, jsonPayload );

    return User();
}

void AuthHandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    //qDebug() << response;
    m_networkReply->deleteLater();

    parseResponse( response );
}

void AuthHandler::performPOST(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newRequest( (QUrl( url )) );
    newRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
    m_networkReply = m_networkAccessManager->post( newRequest, payload.toJson());
    connect( m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead );
}

void AuthHandler::parseResponse(const QByteArray &response )
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson( response );
    if ( jsonDocument.object().contains("error") )
    {
        QJsonObject errorObject = jsonDocument["error"].toObject();

        QString errorMessage = errorObject["message"].toString();
        QMessageBox::critical(this->parent,"Error", errorMessage);
        qDebug() << "Error occured!" << response;
    }
    else if ( jsonDocument.object().contains("kind"))
    {
        QString idToken = jsonDocument.object().value("idToken").toString();
        //qDebug() << "Obtained user ID Token: " << idToken;
        qDebug() << "User signed in successfully!";
        if(currentOperation == FirebaseOperation::SignIn){
        m_idToken = idToken;
        user->setEmail(jsonDocument.object().take("email").toString());
        user->setId(jsonDocument.object().take("localId").toString());
        user->setIsEmailVerified(jsonDocument.object().take("registered").toBool());
        user->setToken(jsonDocument.object().take("idToken").toString());
        user->setRefreshToken(jsonDocument.object().take("refreshToken").toString());
        emit userSignedIn();
        }
    }
    else
        qDebug() << "The response was: " << response;
}
