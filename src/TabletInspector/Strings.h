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
    static QString disconnected();
    static QString clear();
    static QString logs();
    static QString properties();
    static QString tabletInfo();
    static QString data();
    static QString oem();
    static QString model();
    static QString fixtureDate();
    static QString company();
    static QString size();
    static QString maxPressure();
    static QString pBtnNum();
    static QString hBtnNum();
    static QString sBtnNum();
    static QString lpi();
    static QString rate();
    static QString isMonitor();
    static QString isPassive();
    static QString trueStr();
    static QString falseStr();

    static QString msg_tabletConnected();
    static QString msg_tabletDisconnected();
    static QString msg_tabletReadError();
};