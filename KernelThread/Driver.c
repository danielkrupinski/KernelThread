#include <ntddk.h>
#include <wdf.h>

VOID threadRoutine(PCSTR string)
{
    UNICODE_STRING uniName = RTL_CONSTANT_STRING(L"\\SystemRoot\\KernelThread.txt");
    OBJECT_ATTRIBUTES objAttr;

    InitializeObjectAttributes(&objAttr, &uniName,
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
        NULL, NULL);

    HANDLE file;
    IO_STATUS_BLOCK ioStatusBlock;

    if (NT_SUCCESS(ZwCreateFile(&file,
        GENERIC_WRITE,
        &objAttr, &ioStatusBlock, NULL,
        FILE_ATTRIBUTE_NORMAL,
        0,
        FILE_OVERWRITE_IF,
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL, 0))) {
        ZwWriteFile(file, NULL, NULL, NULL, &ioStatusBlock, (PVOID)string, (ULONG)strlen(string), NULL, NULL);
        ZwClose(file);
    }
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);

    HANDLE thread;
    NTSTATUS status = PsCreateSystemThread(&thread, 0, NULL, NULL, NULL, threadRoutine, "System thread says hello!");
    if (NT_SUCCESS(status))
        ZwClose(thread);

    return status;
}
