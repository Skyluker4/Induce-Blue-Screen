#include <Windows.h>

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// Load the library in order to adjust privileges 
	const LPVOID RTLAdjustPrivilegeLibAddress = GetProcAddress(LoadLibrary(L"ntdll.dll"), "RtlAdjustPrivilege");

	// Load the module in order to create the BSoD 
	const LPVOID NtRaiseHardErrorModuleAddress = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtRaiseHardError");

	// Function cast for RtlAdjustPrivilege 
	using pdef_RTLAdjustPrivilege = NTSTATUS(NTAPI*)(ULONG privilege, BOOLEAN enable, BOOLEAN currentThread,
	                                                 PBOOLEAN enabled);

	// Create the function to modify program's rights - typecast the typedef earlier to adjustPrivilegeLibAddress 
	const auto RtlAdjustPrivilege = static_cast<pdef_RTLAdjustPrivilege>(RTLAdjustPrivilegeLibAddress);

	// Function cast for NtRaiseHardError 
	using pdef_NTRaiseHardError = NTSTATUS(NTAPI*)(NTSTATUS errorStatus, ULONG numberOfParameters,
	                                               ULONG unicodeStringParameterMask OPTIONAL, PULONG_PTR parameters,
	                                               ULONG responseOption, PULONG response);

	// Create the function to crash the computer - typecast the typedef earlier to errorModuleAddress 
	const auto NtRaiseHardError = static_cast<pdef_NTRaiseHardError>(NtRaiseHardErrorModuleAddress);

	// Adjust privileges to debug mode so the program can have permission to edit kernel memory; see line 10343 of winnt.h for list of privileges 
	BOOLEAN previousValue;
	RtlAdjustPrivilege(19, TRUE, FALSE, &previousValue);

	// Call to crash the computer
	ULONG response;
	NtRaiseHardError(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &response);

	// Should take about 5 seconds to crash. Return an error if computer does not crash.
	return -1;
}
