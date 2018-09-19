#pragma once


struct TabletInfo {
public:
    CString oem;
    CString model;
    CString fixtureDate;

    SIZE size;
    UINT maxPressure;
    UINT pbtnNum;
    UINT hbtnNum;
    UINT sbtnNum;
    UINT lpi;
    UINT rate;
    BOOL isMonitor;
    BOOL isPassive;
};