#include "qstylewidget.h"
#include <QPushButton>
#include <QBoxLayout>
#include <QFile>
#include <QDebug>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>
#include <QToolButton>
#include <QPlainTextEdit>

QStyleWidget::QStyleWidget(QWidget *parent)
    : QWidget(parent)
{
    m_path = getenv("TMP");
    m_path += "/metaface.qss";

    initWindow();
    onReadStyle();
}

QStyleWidget::~QStyleWidget()
{

}

const QString &QStyleWidget::stylePath()
{
    return m_path;
}

void QStyleWidget::setStylePath(const QString &path)
{
    m_path = path;
}

void QStyleWidget::initWindow()
{
    this->resize(1200, 800);
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    {
        m_comboBox_class = new QComboBox();
        m_comboBox_class->setEditable(true);
        m_comboBox_class->addItem("QWidget");
        m_comboBox_class->addItem("QDockWidget");
        m_comboBox_class->addItem("QPushButton");
        m_comboBox_class->addItem("QMenu");
        m_comboBox_class->addItem("QListWidget");
        m_comboBox_class->addItem("QTreeWidget");
        m_comboBox_class->addItem("QTableWidget");

        m_comboBox_effect = new QComboBox();
        m_comboBox_effect->setEditable(true);
        m_comboBox_effect->addItem("");
        m_comboBox_effect->addItem("::item");

        m_comboBox_state = new QComboBox();
        m_comboBox_state->setEditable(true);
        m_comboBox_state->addItem("");
        m_comboBox_state->addItem(":active");
        m_comboBox_state->addItem(":checked");
        m_comboBox_state->addItem(":hover");
        m_comboBox_state->addItem(":pressed");
        m_comboBox_state->addItem(":disabled");
        m_comboBox_state->addItem(":first");
        m_comboBox_state->addItem(":has-children");
        m_comboBox_state->addItem(":open");
        m_comboBox_state->addItem(":unchecked");

        QPushButton *button = new QPushButton();
        button->setFixedSize(20, 20);
        button->setText("+");
        connect(button, &QPushButton::clicked, this, &QStyleWidget::onFilterStyle);

        QHBoxLayout* hlayout = new QHBoxLayout();
        hlayout->addWidget(new QLabel(tr("class")));
        hlayout->addWidget(m_comboBox_class);
        hlayout->addWidget(new QLabel(tr("effect")));
        hlayout->addWidget(m_comboBox_effect);
        hlayout->addWidget(new QLabel(tr("state")));
        hlayout->addWidget(m_comboBox_state);
        hlayout->addStretch(0);
        hlayout->addWidget(button);
        vlayout->addLayout(hlayout);
    }

    {
        m_itemEdit_shape.append(new QItemEdit(tr("width")));
        m_itemEdit_shape.append(new QItemEdit(tr("height")));
        m_itemEdit_shape.append(new QItemEdit(tr("border")));
        m_itemEdit_shape.append(new QItemEdit(tr("padding")));
        m_itemEdit_shape.append(new QItemEdit(tr("margin")));
        m_itemEdit_shape.append(new QItemEdit(tr("outline")));

        QPushButton* button  = new QPushButton();
        button->setFixedSize(20, 20);
        button->setText("+");
        connect(button, &QPushButton::clicked, this, &QStyleWidget::onShapeItemChanged);

        QHBoxLayout* hlayout = new QHBoxLayout();
        for(QItemEdit* item : m_itemEdit_shape)
        {
            hlayout->addWidget(item->itemLabel());
            hlayout->addWidget(item);
        }
        hlayout->addStretch(0);
        hlayout->addWidget(button);
        vlayout->addLayout(hlayout);
    }

    {
        m_itemEdit_detail.append(new QItemEdit(tr("font")));
        m_itemEdit_detail.append(new QItemEdit(tr("color")));
        m_itemEdit_detail.append(new QItemEdit(tr("background")));
        m_itemEdit_detail.append(new QItemEdit(tr("opacity")));

        QPushButton* button  = new QPushButton();
        button->setFixedSize(20, 20);
        button->setText("+");
        connect(button, &QPushButton::clicked, this, &QStyleWidget::onDetailItemChanged);

        QHBoxLayout* hlayout = new QHBoxLayout();
        for(QItemEdit* item : m_itemEdit_detail)
        {
            hlayout->addWidget(item->itemLabel());
            hlayout->addWidget(item);
        }
        hlayout->addStretch(0);
        hlayout->addWidget(button);
        vlayout->addLayout(hlayout);
    }

    {
        m_itemEdit_other.append(new QItemEdit(tr("image:")));
        m_itemEdit_other.append(new QItemEdit(tr("background-image:")));

        QPushButton* button  = new QPushButton();
        button->setFixedSize(20, 20);
        button->setText("+");
        connect(button, &QPushButton::clicked, this, &QStyleWidget::onOtherItemChanged);

        QHBoxLayout* hlayout = new QHBoxLayout();
        for(QItemEdit* item : m_itemEdit_other)
        {
            hlayout->addWidget(item->itemLabel());
            hlayout->addWidget(item);
        }
        hlayout->addStretch(0);
        hlayout->addWidget(button);
        vlayout->addLayout(hlayout);
    }

    {
        QPushButton *button_add = new QPushButton();
        button_add->setText(tr("add"));
        button_add->setShortcut(QKeySequence("Ctrl+A"));
        connect(button_add, &QPushButton::clicked, this, &QStyleWidget::onAddItemStyle);

        QPushButton *button_tidy = new QPushButton();
        button_tidy->setText(tr("tidy"));
        button_tidy->setShortcut(QKeySequence("Ctrl+T"));
        connect(button_tidy, &QPushButton::clicked, this, &QStyleWidget::onTidyStyle);

        QPushButton *button_save = new QPushButton();
        button_save->setText(tr("save"));
        button_save->setShortcut(QKeySequence("Ctrl+S"));
        connect(button_save, &QPushButton::clicked, this, &QStyleWidget::onSaveStyle);

        QPushButton *button_write = new QPushButton();
        button_write->setText(tr("write"));
        button_write->setShortcut(QKeySequence("Ctrl+W"));
        connect(button_write, &QPushButton::clicked, this, &QStyleWidget::onWriteStyle);

        QHBoxLayout* hlayout = new QHBoxLayout();
        hlayout->addWidget(button_add);
        hlayout->addWidget(button_tidy);
        hlayout->addWidget(button_save);
        hlayout->addWidget(button_write);
        vlayout->addLayout(hlayout);
    }

    {
        m_widget_tag = new QWidget();
        m_widget_tag->setLayout(new QHBoxLayout());

        m_textEdit_style = new QPlainTextEdit();

        vlayout->addWidget(m_widget_tag);
        vlayout->addWidget(m_textEdit_style);
    }
}

void QStyleWidget::onTidyStyle()
{
    for(QObject* child : m_widget_tag->children())
    {
        if(child->isWidgetType())
        {
            child->deleteLater();
        }
    }
    QSet<QString > names;
    for(QString name : m_data.keys())
    {
        int index = name.indexOf(":");
        if(name != -1)
        {
            name = name.mid(0, index);
        }
        names.insert(name);
    }
    for(const QString& name : names)
    {
        QToolButton* button = new QToolButton();
        button->setCheckable(true);
        button->setText(name);
        connect(button, &QPushButton::clicked, this, &QStyleWidget::onClickItemStyle);
        m_widget_tag->layout()->addWidget(button);
    }
    m_textEdit_style->clear();
}

void QStyleWidget::onSaveStyle()
{
    QString data = m_textEdit_style->toPlainText();
    const QStringList &list = data.simplified().split("}");
    for(const QString &line : list)
    {
        const QStringList &temp = line.split("{");
        if(temp.size() == 2)
        {
            QVariantMap itemData;
            const QStringList& dlist = temp.last().simplified().split(";");
            for(const QString &data : dlist)
            {
                const QStringList &tmp = data.split(":");
                if(tmp.size() == 2)
                {
                    itemData.insert(tmp.first().simplified(), tmp.last().simplified());
                }
            }
            const QString& name = temp.first().simplified();
            m_data.insert(name, itemData);
        }
    }
}

void QStyleWidget::onReadStyle()
{
    qDebug() << "read style :" << m_path;
    QFile file(m_path);
    if (file.open(QIODevice::ReadOnly))
    {
        const QString& data = file.readAll();
        file.close();

        const QStringList &list = data.simplified().split("}");
        for(const QString &line : list)
        {
            const QStringList &temp = line.split("{");
            if(temp.size() == 2)
            {
                QVariantMap itemData;
                const QStringList& dlist = temp.last().simplified().split(";");
                for(const QString &data : dlist)
                {
                    const QStringList &tmp = data.split(":");
                    if(tmp.size() == 2)
                    {
                        itemData.insert(tmp.first().simplified(), tmp.last().simplified());
                    }
                }
                const QString& name = temp.first().simplified();
                m_data.insert(name, itemData);
            }
        }
        onTidyStyle();
    }
}

void QStyleWidget::onWriteStyle()
{
    qDebug() << "write style :" << m_path;
    QFile file(m_path);
    if (file.open(QIODevice::WriteOnly))
    {
        QString data;
        for(const QString& name : m_data.keys())
        {
            QString itemBody;
            const QVariantMap &values = m_data[name].toMap();
            for(const QString& key : values.keys())
            {
                const QString &text = QString("  %1:%2;\n").arg(key).arg(values[key].toString());
                itemBody.append(text);
            }
            const QString &itemText = QString("%1 {\n%2}\n").arg(name).arg(itemBody);
            data.append(itemText);
        }
        file.write(data.toLatin1());
        file.flush();
        file.close();
    }
}

void QStyleWidget::onFilterStyle()
{
    QDialog dialog;
    dialog.exec();
}

void QStyleWidget::onAddItemStyle()
{
    QString itemHeader;
    const QString& classd = m_comboBox_class->currentText();
    const QString& state = m_comboBox_state->currentText();
    const QString& effect = m_comboBox_effect->currentText();
    if(effect.length())
    {
        itemHeader = QString("%1#%3%2").arg(classd).arg(state).arg(effect);
    }
    else{
        itemHeader = QString("%1%2").arg(classd).arg(state);
    }

    QVariantMap itemData;
    QVector<QItemEdit*> totalItems;
    totalItems << m_itemEdit_shape << m_itemEdit_detail << m_itemEdit_other;
    for(QItemEdit* item : totalItems)
    {
        if(item->isVisible() && item->currentText().length())
        {
            itemData.insert(item->itemName().simplified(), item->currentText().simplified());
        }
    }

    const QString& name = itemHeader.simplified();
    m_data.insert(name, itemData);
    onTidyStyle();
}

void QStyleWidget::onTakeItemStyle()
{
    QString itemHeader;
    const QString& name = m_comboBox_class->currentText();
    const QString& state = m_comboBox_state->currentText();
    const QString& effect = m_comboBox_effect->currentText();
    if(effect.length())
    {
        itemHeader = QString("%1#%3%2").arg(name).arg(state).arg(effect);
    }
    else{
        itemHeader = QString("%1%2").arg(name).arg(state);
    }
    m_data.remove(itemHeader);
    onTidyStyle();
}

void QStyleWidget::onClickItemStyle()
{
    QStringList names;
    for(QObject* child : m_widget_tag->children())
    {
        if(child->isWidgetType())
        {
            QAbstractButton* button = qobject_cast<QAbstractButton*>(child);
            if(button->isChecked())
            {
                names.append(button->text());
            }
        }
    }

    QString style;
    for(const QString& name : m_data.keys())
    {
        for(int i = 0; i < names.size(); i++)
        {
            if(name == names[i] || name.startsWith(names[i]+':'))
            {
                QString itemBody;
                const QVariantMap &values = m_data[name].toMap();
                for(const QString& key : values.keys())
                {
                    const QString &text = QString("  %1:%2;\n").arg(key).arg(values[key].toString());
                    itemBody.append(text);
                }
                const QString &itemText = QString("%1 {\n%2}\n").arg(name).arg(itemBody);
                style.append(itemText);
                break;
            }
        }
    }
    m_textEdit_style->setPlainText(style);
}

void QStyleWidget::onShapeItemChanged()
{
    onItemEditChanged(m_itemEdit_shape);
}

void QStyleWidget::onDetailItemChanged()
{
    onItemEditChanged(m_itemEdit_detail);
}

void QStyleWidget::onOtherItemChanged()
{
    onItemEditChanged(m_itemEdit_other);
}

void QStyleWidget::onItemEditChanged(QVector<QItemEdit*> itemEdit)
{
    QDialog dialog;
    QVBoxLayout* vlayout = new QVBoxLayout(&dialog);
    for(QItemEdit* item : itemEdit)
    {
        const QString& name = item->itemName();
        QCheckBox* box = new QCheckBox(name);
        box->setCheckable(true);
        box->setChecked(item->isVisible());
        box->setLayoutDirection(Qt::RightToLeft);
        vlayout->addWidget(box);
    }
    dialog.exec();
    for(QObject* child : dialog.children())
    {
        if(!child->isWidgetType())
        {
            continue;
        }
        QCheckBox* box = qobject_cast<QCheckBox*>(child);
        for(QItemEdit* item : itemEdit)
        {
            if(item->itemName() == box->text())
            {
                item->setVisible(box->isChecked());
                item->itemLabel()->setVisible(box->isChecked());
                itemEdit.removeOne(item);
                break;
            }
        }
    }
}

QItemEdit::QItemEdit(const QString &name, QWidget *parent) : QComboBox(parent)
{
    m_label = new QLabel(name);
    this->setEditable(true);
    this->setFixedWidth(100);
}

QItemEdit::~QItemEdit()
{

}

QLabel *QItemEdit::itemLabel() const
{
    return m_label;
}

QString QItemEdit::itemName() const
{
    return m_label->text();
}

void QItemEdit::setItemName(const QString &name)
{
    m_label->setText(name);
}
