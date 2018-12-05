/********************************************************************************
** Form generated from reading UI file 'ImgWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMGWIDGET_H
#define UI_IMGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImgWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;

    void setupUi(QWidget *ImgWidget)
    {
        if (ImgWidget->objectName().isEmpty())
            ImgWidget->setObjectName(QStringLiteral("ImgWidget"));
        ImgWidget->resize(400, 300);
        horizontalLayout = new QHBoxLayout(ImgWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(ImgWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);


        retranslateUi(ImgWidget);

        QMetaObject::connectSlotsByName(ImgWidget);
    } // setupUi

    void retranslateUi(QWidget *ImgWidget)
    {
        ImgWidget->setWindowTitle(QApplication::translate("ImgWidget", "ImgWidget", nullptr));
        label->setText(QApplication::translate("ImgWidget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImgWidget: public Ui_ImgWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMGWIDGET_H
