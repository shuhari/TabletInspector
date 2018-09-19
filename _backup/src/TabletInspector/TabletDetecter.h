#pragma once


#include <vector>
#include <algorithm>

#include "DeviceApi.h"
#include "Public.h"


template<typename T>
class TabletDetecter {
public:
    inline TabletDetecter() {
    }

    inline ~TabletDetecter() {
    }

    BEGIN_MSG_MAP_EX(TabletDetecter)
        MESSAGE_HANDLER_EX(WM_DEVICECHANGE, onDeviceChange)
    END_MSG_MAP()

protected:
    inline bool isConnectced() {
        return !_currentDevicePath.IsEmpty();
    }

    virtual void onTabletConnected(PCWSTR szDevicePath) = 0;
    virtual void onTabletDisconnected(PCWSTR szDevicePath) = 0;

    inline void detectTablets() {
        std::vector<CString> devicePaths;
        enumTablets(devicePaths);

        if (isConnectced()) {
            auto it = find(devicePaths.begin(), devicePaths.end(), _currentDevicePath);
            if (it == devicePaths.end()) {
                CString prevDevice = _currentDevicePath;
                _currentDevicePath = L"";
                onTabletDisconnected(prevDevice);
            }
        }
        else {    
            // !isConnected()
            if (devicePaths.size() == 1) {
                _currentDevicePath = devicePaths[0];
                onTabletConnected(_currentDevicePath);
            }
        }

    }

private:
    CString     _currentDevicePath;

    inline LRESULT     onDeviceChange(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        detectTablets();
        return 0;
    }

    inline void enumTablets(std::vector<CString>& devicePaths) {

        using namespace win32;

        const GUID* pIntfId = &GUID_DEVINTERFACE_TABLET_WINUSB;
        DeviceInfoSet dis;
        if (!dis.open(pIntfId, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE))
            return;

        DWORD dwDevIndex = 0;
        SP_DEVINFO_DATA devInfo;
        while (true) {
            BOOL bRet = dis.enumInfo(dwDevIndex, devInfo);
            if (!bRet)
                break;
            SP_DEVICE_INTERFACE_DATA intfData;
            DWORD dwIntfIndex = 0;
            while (true) {
                if (dis.enumInterfaces(&devInfo, pIntfId, dwIntfIndex, intfData)) {
                    auto pDetail = dis.getInterfaceDetail(&intfData, &devInfo);
                    if (pDetail) {
                        devicePaths.push_back(pDetail->DevicePath);
                        dis.releaseInfoDetail(pDetail);
                    }
                    dwIntfIndex++;
                }
                else
                    break;
            }
            dwDevIndex++;
            continue;
        }
    }
};