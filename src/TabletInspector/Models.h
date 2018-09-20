#pragma once


#include "stdafx.h"


struct TabletInfo {
    QString         oem;
    QString         model;
    QString         fixtureDate;
    QString         company;

    QSize           size;
    quint16         maxPressure;
    quint16         pbtnNum;
    quint16         hbtnNum;
    quint16         sbtnNum;
    quint16         lpi;
    quint16         rate;
    bool            isMonitor;
    bool            isPassive;
};