/********************************************************************************
** Form generated from reading UI file 'Viewer.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWER_H
#define UI_VIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViewerClass
{
public:
    QAction *actionRegister;
    QAction *actionFuse;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QPushButton *volumeAdd;
    QListWidget *volumeList;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QPushButton *imageAdd;
    QListWidget *imageList;
    QVBoxLayout *imageLayout;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *image_origin_mode;
    QPushButton *image_bin_mode;
    QPushButton *image_fuse_mode;
    QVBoxLayout *vtkLayout;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *ViewerClass)
    {
        if (ViewerClass->objectName().isEmpty())
            ViewerClass->setObjectName(QStringLiteral("ViewerClass"));
        ViewerClass->resize(800, 634);
        actionRegister = new QAction(ViewerClass);
        actionRegister->setObjectName(QStringLiteral("actionRegister"));
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/register.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRegister->setIcon(icon);
        actionFuse = new QAction(ViewerClass);
        actionFuse->setObjectName(QStringLiteral("actionFuse"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/fuse.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFuse->setIcon(icon1);
        centralWidget = new QWidget(ViewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        volumeAdd = new QPushButton(centralWidget);
        volumeAdd->setObjectName(QStringLiteral("volumeAdd"));

        horizontalLayout_2->addWidget(volumeAdd);


        verticalLayout_5->addLayout(horizontalLayout_2);

        volumeList = new QListWidget(centralWidget);
        volumeList->setObjectName(QStringLiteral("volumeList"));

        verticalLayout_5->addWidget(volumeList);


        verticalLayout->addLayout(verticalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        imageAdd = new QPushButton(centralWidget);
        imageAdd->setObjectName(QStringLiteral("imageAdd"));

        horizontalLayout_3->addWidget(imageAdd);


        verticalLayout_3->addLayout(horizontalLayout_3);

        imageList = new QListWidget(centralWidget);
        imageList->setObjectName(QStringLiteral("imageList"));

        verticalLayout_3->addWidget(imageList);


        verticalLayout->addLayout(verticalLayout_3);

        imageLayout = new QVBoxLayout();
        imageLayout->setSpacing(6);
        imageLayout->setObjectName(QStringLiteral("imageLayout"));

        verticalLayout->addLayout(imageLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        image_origin_mode = new QPushButton(centralWidget);
        image_origin_mode->setObjectName(QStringLiteral("image_origin_mode"));
        image_origin_mode->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_4->addWidget(image_origin_mode);

        image_bin_mode = new QPushButton(centralWidget);
        image_bin_mode->setObjectName(QStringLiteral("image_bin_mode"));
        image_bin_mode->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_4->addWidget(image_bin_mode);

        image_fuse_mode = new QPushButton(centralWidget);
        image_fuse_mode->setObjectName(QStringLiteral("image_fuse_mode"));
        image_fuse_mode->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_4->addWidget(image_fuse_mode);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);

        horizontalLayout->addLayout(verticalLayout);

        vtkLayout = new QVBoxLayout();
        vtkLayout->setSpacing(6);
        vtkLayout->setObjectName(QStringLiteral("vtkLayout"));

        horizontalLayout->addLayout(vtkLayout);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);
        ViewerClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(ViewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        mainToolBar->addAction(actionRegister);
        mainToolBar->addAction(actionFuse);

        retranslateUi(ViewerClass);

        QMetaObject::connectSlotsByName(ViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ViewerClass)
    {
        ViewerClass->setWindowTitle(QApplication::translate("ViewerClass", "Viewer", nullptr));
        actionRegister->setText(QApplication::translate("ViewerClass", "Register", nullptr));
#ifndef QT_NO_TOOLTIP
        actionRegister->setToolTip(QApplication::translate("ViewerClass", "\351\205\215\345\207\206", nullptr));
#endif // QT_NO_TOOLTIP
        actionFuse->setText(QApplication::translate("ViewerClass", "Fuse", nullptr));
#ifndef QT_NO_TOOLTIP
        actionFuse->setToolTip(QApplication::translate("ViewerClass", "\350\236\215\345\220\210", nullptr));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("ViewerClass", "\344\275\223\346\225\260\346\215\256", nullptr));
        volumeAdd->setText(QApplication::translate("ViewerClass", "+", nullptr));
        label_2->setText(QApplication::translate("ViewerClass", "\344\272\214\347\273\264\345\233\276\345\203\217", nullptr));
        imageAdd->setText(QApplication::translate("ViewerClass", "+", nullptr));
        image_origin_mode->setText(QApplication::translate("ViewerClass", "\345\216\237\345\247\213", nullptr));
        image_bin_mode->setText(QApplication::translate("ViewerClass", "\344\272\214\345\200\274", nullptr));
        image_fuse_mode->setText(QApplication::translate("ViewerClass", "\350\236\215\345\220\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ViewerClass: public Ui_ViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWER_H
