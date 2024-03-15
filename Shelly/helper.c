#include <Windows.h>
#include <stdio.h>

/// <summary>
/// 
/// </summary>
/// <param name="cFileName">A constant ASCII string that represents the file name to read.</param>
/// <param name="ppFileBuffer">A pointer to a PBYTE variable that receives the base address of the read file.</param>
/// <param name="pdwFileSize">A pointer to a DWORD variable that receives the size of the file.</param>
/// <returns>BOOL True or False</returns>
BOOL ReadFileFromDiskA(IN LPCSTR cFileName, OUT PBYTE* ppFileBuffer, OUT PDWORD pdwFileSize) {

	HANDLE	hFile					= INVALID_HANDLE_VALUE;
	DWORD	dwFileSize				= NULL,
			dwNumberOfBytesRead		= NULL;
	PBYTE   pBaseAddress			= NULL;

	if (!cFileName || !pdwFileSize || !ppFileBuffer) {
		goto _END_OF_FUNC;
	}
		

	if ((hFile = CreateFileA(cFileName, GENERIC_READ, 0x00, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		printf("[!] CreateFileA failed with error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	if ((dwFileSize = GetFileSize(hFile, NULL)) == INVALID_FILE_SIZE) {
		printf("[!] GetFileSize failed with error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	if (!(pBaseAddress = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize))) {
		printf("[!] HeapAlloc failed with error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	if (!ReadFile(hFile, pBaseAddress, dwFileSize, &dwNumberOfBytesRead, NULL) || dwFileSize != dwNumberOfBytesRead) {
		printf("[!] ReadFile failed with error: %d \n[i] Read %d Of %d Bytes \n", GetLastError(), dwNumberOfBytesRead, dwFileSize);
		goto _END_OF_FUNC;
	}

	*ppFileBuffer = pBaseAddress;
	*pdwFileSize = dwFileSize;

_END_OF_FUNC:
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
	}

	if (pBaseAddress && !*ppFileBuffer) {
		HeapFree(GetProcessHeap(), 0x00, pBaseAddress);
	}

	return (*ppFileBuffer && *pdwFileSize) ? TRUE : FALSE;
}