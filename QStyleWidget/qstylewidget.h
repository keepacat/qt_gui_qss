#ifndef QSTYLEWIDGET_H
#define QSTYLEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QVariant>
#include <QComboBox>

class QLabel;
class QItemEdit;
class QTextEdit;
class QPlainTextEdit;

class QStyleWidget : public QWidget
{
    Q_OBJECT

public:
    QStyleWidget(QWidget *parent = nullptr);
    ~QStyleWidget();

public:
    const QString &stylePath();
    void setStylePath(const QString &path);

protected:
    void initWindow();

protected slots:
    void onTidyStyle();
    void onSaveStyle();
    void onReadStyle();
    void onWriteStyle();
    void onFilterStyle();

    void onAddItemStyle();
    void onTakeItemStyle();
    void onClickItemStyle();
    void onShapeItemChanged();
    void onDetailItemChanged();
    void onOtherItemChanged();

private:
    void onItemEditChanged(QVector<QItemEdit*> itemEdit);

private:
    QString m_path;
    QVariantMap m_data;

private:
    QPlainTextEdit *m_textEdit_style;
    QComboBox *m_comboBox_class;
    QComboBox *m_comboBox_effect;
    QComboBox *m_comboBox_state;

    QWidget* m_widget_tag;
    QVector<QItemEdit*> m_itemEdit_shape;
    QVector<QItemEdit*> m_itemEdit_detail;
    QVector<QItemEdit*> m_itemEdit_other;
};

class QItemEdit : public QComboBox
{
    Q_OBJECT

public:
    QItemEdit(const QString &name, QWidget *parent = nullptr);
    ~QItemEdit();

public:
    QLabel *itemLabel() const;

    QString itemName() const;
    void setItemName(const QString &name);

private:
    QLabel*m_label;
};

#endif // QSTYLEWIDGET_H
