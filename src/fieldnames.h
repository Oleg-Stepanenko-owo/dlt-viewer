#ifndef FIELDNAMES_H
#define FIELDNAMES_H

#include "settingsdialog.h"
#include <QObject>
#include <QString>

class FieldNames : public QObject
{
    Q_OBJECT
public:
    explicit FieldNames(QObject *parent = 0);

    /* Order of columns */
    enum Fields
    {
        Index,
        Time,
        TimeStamp,
        Counter,
        EcuId,
        AppId,
        ContextId,
        Type,
        Subtype,
        Mode,
        ArgCount,
        Payload
    };

    /* Get name for one column/field.
     * Settings needed for App and Context Description change.
     * Pass null for settings to get ID only always.
     * Use case: Exports */
    static QString getName(Fields cn, SettingsDialog *settings = NULL);
signals:
    
public slots:
    
};

#endif // FIELDNAMES_H
