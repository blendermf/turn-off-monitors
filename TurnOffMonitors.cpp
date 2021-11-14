#include <Windows.h>
#include <stdio.h>
#include <physicalmonitorenumerationapi.h>
#include <lowlevelmonitorconfigurationapi.h>

BOOL CALLBACK MonitorEnumProc(
    HMONITOR hMonitor,
    HDC hDevice,
    LPRECT rect,
    LPARAM data
) {
    DWORD numMonitors;
    GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &numMonitors);

    if (numMonitors > 0) {
        LPPHYSICAL_MONITOR physicalMonitorArray = NULL;
        physicalMonitorArray = (LPPHYSICAL_MONITOR)malloc(
            numMonitors * sizeof(PHYSICAL_MONITOR));

        if (physicalMonitorArray != NULL)
        {
            GetPhysicalMonitorsFromHMONITOR(hMonitor, numMonitors, physicalMonitorArray);

            for (unsigned int i = 0; i < numMonitors; i++) {
                DWORD capabilitiesStringLength = 0;
                HANDLE physicalMonitor = physicalMonitorArray[i].hPhysicalMonitor;

                GetCapabilitiesStringLength(physicalMonitor, &capabilitiesStringLength);

                if (capabilitiesStringLength > 0)
                {
                    SetVCPFeature(physicalMonitor, 0xD6, 5);
                }
            }

            DestroyPhysicalMonitors(numMonitors, physicalMonitorArray);

            free(physicalMonitorArray);
        }
    }
    
    return true;
}

int main()
{
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, NULL);
}