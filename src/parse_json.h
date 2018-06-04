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

    void loadJson( const QString& val );
    void keyPressEvent(QKeyEvent *evt);
    QVariant getValue( const QString& key ){
        if( nullptr != parseJson ) return parseJson->getValue( key ); }

private slots:

    void on_leSearch_textChanged(const QString &arg1);
    void on_leSearch_returnPressed();

    void on_pbSearchNext_clicked();

    void on_pbSearchPrev_clicked();

    void on_tbCaseSens_clicked();


    void on_tbExpandAll_clicked();

    void on_tbRegExp_clicked(bool checked);

private:
    Ui::Parse_JSON *ui;
    std::unique_ptr<QJsonContainer> parseJson;
};

#endif // PARSE_JSON_H
