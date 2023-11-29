#include "authhandler.h"
#include <QDebug>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonArray>

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

void AuthHandler::sendEmailVerification(QString idToken)
{
    this->currentOperation = FirebaseOperation::ConfirmEmail;
    QString confirmationEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + m_apiKey;
    QVariantMap variantPayload;
    variantPayload["requestType"] = "VERIFY_EMAIL";
    variantPayload["idToken"] = idToken;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );

    performPOST( confirmationEndpoint, jsonPayload );

}

void AuthHandler::deleteAccount(QString idToken)
{
    this->currentOperation = FirebaseOperation::DeleteAccount;
    QString deleteAccountEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=" + m_apiKey;
    QVariantMap variantPayload;
    variantPayload["idToken"] = idToken;
    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPOST(deleteAccountEndpoint, jsonPayload);
}

void AuthHandler::getUserData(QString idToken)
{
    this->currentOperation = FirebaseOperation::GetUserData;
    QString getUserDataEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=" + m_apiKey;
    QVariantMap variantPayload;
    variantPayload["idToken"] = idToken;
    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPOST(getUserDataEndpoint, jsonPayload);
}

bool AuthHandler::getEmailVerified()
{
    return this->user->getIsEmailVerified();
}

void AuthHandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
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

    if(currentOperation == FirebaseOperation::DeleteAccount){
        emit accountDeleted();
    }
    else if(currentOperation == FirebaseOperation::GetUserData){
        if (!jsonDocument.isNull() && jsonDocument.isObject()) {
             QJsonObject rootObject = jsonDocument.object();

            if (rootObject.contains("users") && rootObject["users"].isArray()) {
                 const QJsonArray& userArray = QJsonArray::fromVariantList(rootObject["users"].toArray().toVariantList());

                  const QJsonValue& userObject = userArray.at(0);
                  bool emailVerified = userObject["emailVerified"].toBool();
                  this->user->setIsEmailVerified(emailVerified);
                  qDebug() << "emailVerified:" << userObject;
                  emit userDataUpdated();
            }
        }
    }
    else if(currentOperation == FirebaseOperation::ConfirmEmail){
        qDebug() << jsonDocument.object().take("email");
        emit emailVerified();
    }
    else if (jsonDocument.object().contains("error"))
    {
        QJsonObject errorObject = jsonDocument["error"].toObject();

        QString errorMessage = errorObject["message"].toString();
        QMessageBox::critical(this->parent,"Error", errorMessage);
        qDebug() << "Error occured!" << response;
    }
    else if ( jsonDocument.object().contains("kind"))
    {
        QString idToken = jsonDocument.object().value("idToken").toString();
        qDebug() << "User signed in successfully!";
        if(currentOperation == FirebaseOperation::SignIn){
        m_idToken = idToken;
        user->setEmail(jsonDocument.object().take("email").toString());
        user->setId(jsonDocument.object().take("localId").toString());
        user->setToken(jsonDocument.object().take("idToken").toString());
        user->setRefreshToken(jsonDocument.object().take("refreshToken").toString());
        emit userSignedIn();
        }
    }
}
