
#include <stdio.h>
#include <winpr/crt.h>
#include <winpr/path.h>
#include <winpr/tchar.h>
#include <winpr/winpr.h>

static const TCHAR testPathBackslash[] = _T("C:\\Program Files\\");
static const TCHAR testPathNoBackslash[] = _T("C:\\Program Files");

int TestPathCchAddBackslashEx(int argc, char* argv[])
{
	HRESULT status = 0;
	LPTSTR pszEnd = NULL;
	size_t cchRemaining = 0;
	TCHAR Path[PATHCCH_MAX_CCH] = { 0 };

	WINPR_UNUSED(argc);
	WINPR_UNUSED(argv);

	/**
	 * PathCchAddBackslashEx returns S_OK if the function was successful,
	 * S_FALSE if the path string already ends in a backslash,
	 * or an error code otherwise.
	 */

	_tcsncpy(Path, testPathNoBackslash, ARRAYSIZE(Path));

	/* Add a backslash to a path without a trailing backslash, expect S_OK */

	status = PathCchAddBackslashEx(Path, sizeof(Path) / sizeof(TCHAR), &pszEnd, &cchRemaining);

	if (status != S_OK)
	{
		_tprintf(_T("PathCchAddBackslash status: 0x%08") _T(PRIX32) _T("\n"), status);
		return -1;
	}

	if (_tcsncmp(Path, testPathBackslash, ARRAYSIZE(Path)) != 0)
	{
		_tprintf(_T("Path Mismatch: Actual: %s, Expected: %s\n"), Path, testPathBackslash);
		return -1;
	}

	/* Add a backslash to a path with a trailing backslash, expect S_FALSE */

	_tcsncpy(Path, testPathBackslash, ARRAYSIZE(Path));

	status = PathCchAddBackslashEx(Path, sizeof(Path) / sizeof(TCHAR), &pszEnd, &cchRemaining);

	if (status != S_FALSE)
	{
		_tprintf(_T("PathCchAddBackslash status: 0x%08") _T(PRIX32) _T("\n"), status);
		return -1;
	}

	if (_tcsncmp(Path, testPathBackslash, ARRAYSIZE(Path)) != 0)
	{
		_tprintf(_T("Path Mismatch: Actual: %s, Expected: %s\n"), Path, testPathBackslash);
		return -1;
	}

	/* Use NULL PSTR, expect FAILED(status) */

	status = PathCchAddBackslashEx(NULL, PATHCCH_MAX_CCH, NULL, NULL);

	if (SUCCEEDED(status))
	{
		_tprintf(
		    _T("PathCchAddBackslashEx unexpectedly succeeded with null buffer. Status: 0x%08") _T(
		        PRIX32) _T("\n"),
		    status);
		return -1;
	}

	/* Use insufficient size value, expect FAILED(status)  */

	_tcsncpy(Path, _T("C:\\tmp"), ARRAYSIZE(Path));

	status = PathCchAddBackslashEx(Path, 7, NULL, NULL);

	if (SUCCEEDED(status))
	{
		_tprintf(_T("PathCchAddBackslashEx unexpectedly succeeded with insufficient buffer size. ")
		         _T("Status: 0x%08") _T(PRIX32) _T("\n"),
		         status);
		return -1;
	}

	/* Use minimum required size value, expect S_OK  */

	_tcsncpy(Path, _T("C:\\tmp"), ARRAYSIZE(Path));

	status = PathCchAddBackslashEx(Path, 8, NULL, NULL);

	if (status != S_OK)
	{
		_tprintf(_T("PathCchAddBackslashEx failed with status: 0x%08") _T(PRIX32) _T("\n"), status);
		return -1;
	}

	return 0;
}
