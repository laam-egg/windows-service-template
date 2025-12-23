#pragma once

// Start the service right after installation or not?
#define SERVICE_START_IMMEDIATELY  TRUE

// For more information about the following settings,
// refer to the documentation of Windows API function
// `CreateService()`.

// Internal name of the service
#define SERVICE_NAME             L"sample-windows-service"

// Displayed name of the service
#define SERVICE_DISPLAY_NAME     L"Sample Windows Service 1.0"

// Service start options.
#define SERVICE_START_TYPE       SERVICE_AUTO_START

// List of service dependencies - "dep1\0dep2\0\0"
#define SERVICE_DEPENDENCIES     L""

// The name of the account under which the service should run
#define SERVICE_ACCOUNT          NULL // LocalSystem

// The password to the service account name
#define SERVICE_PASSWORD         NULL
