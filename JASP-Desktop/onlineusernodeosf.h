#ifndef ONLINEUSERNODEOSF_H
#define ONLINEUSERNODEOSF_H

#include "common.h"
#include "onlineusernode.h"

class OnlineUserNodeOSF : public OnlineUserNode {
    Q_OBJECT

public:
    OnlineUserNodeOSF(QNetworkAccessManager* manager, QString id, QObject* parent = 0);

    virtual void initialise() OVERRIDE;

    static bool authenticationSuccessful(QNetworkAccessManager* manager);

private slots:
    void nodeInfoReceived();
};

#endif // ONLINEUSERNODEOSF_H
