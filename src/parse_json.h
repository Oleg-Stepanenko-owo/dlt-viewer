#ifndef PARSE_JSON_H
#define PARSE_JSON_H

#include <QDialog>
#include <memory>
#include "ui_parse_json.h"
#include "qjsoncontainer.h"

namespace Ui {
class Parse_JSON;
}

class Parse_JSON : public QDialog
{
    Q_OBJECT

public:
    explicit Parse_JSON(QWidget *parent = 0);
    ~Parse_JSON();

    QWidget* getParseWidget() { return ui->tab_parse; }

    void loadJson( const QString& val ) { if( parseJson ) parseJson->loadJson(val); }
private:
    Ui::Parse_JSON *ui;
    std::unique_ptr<QJsonContainer> parseJson;
};

#endif // PARSE_JSON_H
