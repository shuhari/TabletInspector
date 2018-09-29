#pragma once


class Strings : public QObject {
    Q_OBJECT
public:
    static QString appTitle();

    static QString menuFile();
    static QString menuView();
    static QString menuTool();
    static QString menuHelp();

    static QString actionFileExit();

    static QString actionViewToolBar();
    static QString actionViewStatusBar();

    static QString actionToolSettings();

    static QString actionHelpAbout();

    static QString general();
    static QString language();
    static QString settings();
};