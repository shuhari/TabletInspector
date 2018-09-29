#pragma once


#include "stdafx.h"


class GeneralPage : public QWidget {
    Q_OBJECT
public:
    GeneralPage(QWidget* parent = nullptr);
    virtual ~GeneralPage() = default;

    void saveSettings();

private:
    QComboBox*         _languageCbo;
};


class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget* parent = nullptr);
    virtual ~SettingsDialog() = default;

private:
    QListWidget*         _categoryList;
    QStackedLayout*      _pageBox;
    GeneralPage*         _generalPage;

    void                 addCategory(const QString& title, QWidget* widget);

private slots:
    void            onOk();
    void            onCategoryCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
};

