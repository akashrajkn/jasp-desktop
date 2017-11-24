#ifndef ONLINEUSERNODE_H
#define ONLINEUSERNODE_H

#include "common.h"
#include "onlinenode.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>

class OnlineUserNode : public OnlineNode {
    Q_OBJECT

public:
    OnlineUserNode(QNetworkAccessManager* manager, QString id, QObject* parent = 0);

    virtual QString getFullname() const;

    virtual QString getActionPath() const OVERRIDE;

    virtual bool beginAction() OVERRIDE;

protected:
    QString _fullname;
};

#endif // ONLINEUSERNODE_H
