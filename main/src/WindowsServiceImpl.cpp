#pragma region Includes
#include "WindowsService.h"
#include "WindowsServiceImpl.h"
#include "ThreadPool.h"
#include "Logging.h"
#pragma endregion

#define ARRSIZE(a) (sizeof(a)/sizeof(a[0]))

CWindowsServiceImpl::CWindowsServiceImpl(PWSTR pszServiceName, 
                               BOOL fCanStop, 
                               BOOL fCanShutdown, 
                               BOOL fCanPauseContinue) : CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
	m_fStopping = FALSE;

    // Create a manual-reset event that is not signaled at first to indicate 
    // the stopped signal of the service.
    m_hStoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_hStoppedEvent == NULL)
    {
        throw GetLastError();
    }

    // Event to notify service stop to the worker thread
    m_hStoppingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_hStoppingEvent == NULL)
    {
        throw GetLastError();
    }
}


CWindowsServiceImpl::~CWindowsServiceImpl(void)
{
    if (m_hStoppingEvent)
    {
        CloseHandle(m_hStoppingEvent);
        m_hStoppingEvent = NULL;
    }

	if (m_hStoppedEvent)
    {
        CloseHandle(m_hStoppedEvent);
        m_hStoppedEvent = NULL;
    }
}


void CWindowsServiceImpl::OnStart(DWORD dwArgc, LPWSTR *lpszArgv)
{
    // Log a service start message to the Application log.
    WriteEventLogEntry(SERVICE_DISPLAY_NAME L" is in OnStart", 
        EVENTLOG_INFORMATION_TYPE
    );
    
    DebugLog(L"Service is starting...");

    // Queue the main service function for execution in a worker thread.
    CThreadPool::QueueUserWorkItem(&CWindowsServiceImpl::ServiceWorkerThread, this);
}


void CWindowsServiceImpl::ServiceWorkerThread(void)
{
    DebugLog(L"Service worker thread is running.");

    // You might add another event here (let's call it `E`)
    // to perform work whenever that event fires.
    // Add it to the end of this array.
    HANDLE events[] = { m_hStoppingEvent };
    while (!m_fStopping)
    {
        DebugLog(L"Waiting for either stopping event or pending task...");
        DWORD w = WaitForMultipleObjects(ARRSIZE(events), events, FALSE, INFINITE);
        DebugLog(L"Woke up from wait with result: ", w);
        if (w == WAIT_OBJECT_0) {
            // Cancel any pending task/event processing and exit
            break;
        } else {
            // Perform main service function here
            // whenever `E` is signaled.
        }
    }

cleanup:
    DebugLog(L"Service worker thread is stopping.");

    // Perform any necessary cleanup here
    // ...

    // Signal the stopped event.
    SetEvent(m_hStoppedEvent);
}


void CWindowsServiceImpl::OnStop()
{
    // Log a service stop message to the Application log.
    WriteEventLogEntry(SERVICE_DISPLAY_NAME L" is in OnStop",
        EVENTLOG_INFORMATION_TYPE
    );

    DebugLog(L"Service is stopping...");

    // Indicate that the service is stopping and wait for the finish of the 
    // main service function (ServiceWorkerThread).
    m_fStopping = TRUE;
    SetEvent(m_hStoppingEvent);
    if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
    {
        throw GetLastError();
    }

    DebugLog(L"Service has stopped.");
}
