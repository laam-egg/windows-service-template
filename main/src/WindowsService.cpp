#include "stdafx.h"
#include "ServiceInstaller.h"
#include "ServiceBase.h"
#include "WindowsService.h"
#include "WindowsServiceImpl.h"

int wmain(int argc, wchar_t *argv[])
{
    if ((argc > 1) && ((*argv[1] == L'-' || (*argv[1] == L'/'))))
    {
        if (_wcsicmp(L"install", argv[1] + 1) == 0)
        {
            // Install the service when the command is 
            // "-install" or "/install".
			
            InstallOrReplaceService(
                SERVICE_NAME,               // Name of service
                SERVICE_DISPLAY_NAME,       // Name to display
                SERVICE_START_TYPE,         // Service start type
                SERVICE_DEPENDENCIES,       // Dependencies
                SERVICE_ACCOUNT,            // Service running account
                SERVICE_PASSWORD,           // Password of the account
                SERVICE_START_IMMEDIATELY   // Whether to start the service immediately after installation
                );
        }
        else if (_wcsicmp(L"remove", argv[1] + 1) == 0)
        {
            // Uninstall the service when the command is 
            // "-remove" or "/remove".
            UninstallServiceAndWait(SERVICE_NAME);
        }
        else
        {
            wprintf(L"Invalid command line argument: %s\n", argv[1]);
        }
    }
    else
    {
        wprintf(L"Parameters:\n");
        wprintf(L" -install  to install the service.\n");
        wprintf(L" -remove   to remove the service.\n");

        CWindowsServiceImpl service(SERVICE_NAME);
        if (!CServiceBase::Run(service))
        {
            wprintf(L"Service failed to run w/err 0x%08lx\n", GetLastError());
        }
    }

    return 0;
}