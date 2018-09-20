#pragma once


#include "stdafx.h"


class GeneralPage : public QWidget {
public:
    GeneralPage(QWidget* parent = nullptr);
    virtual ~GeneralPage() = default;

    void saveSettings();

private:
    QComboBox*         _languageCbo;
};


class SettingsDialog : public QDialog {
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

