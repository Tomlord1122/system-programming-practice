#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

class FileDownload
{
public:
    virtual void check()
    {
        printf("virtual check\n");
    }
    virtual void wget()
    {
        printf("virtual wget\n");
    }
};

class CustomFileDownload : public FileDownload
{
public:
    virtual void check()
    {
        printf("custom check\n");
    }
    virtual void wget()
    {
        printf("custom wget\n");
    }
};

typedef void (*VFunc)();

int main(int argc, char *argv[])
{
    // Initialize a class object
    FileDownload *U = new CustomFileDownload();
    U->check();
    U->wget();

    // Overwrite the V-table pointer with a writable memory region
    // Allocate memory for a fake vtable
    VFunc *fakeVTable = (VFunc *)malloc(2 * sizeof(VFunc));
    fakeVTable[0] = []()
    { printf("fake check\n"); };
    fakeVTable[1] = []()
    { printf("fake wget\n"); };

    // Overwrite the vtable pointer of the object
    *((VFunc **)U) = fakeVTable;

    // Call the functions again to see the effect of the overwritten vtable
    U->check();
    U->wget();

    // Clean up
    delete U;
    free(fakeVTable);

    return 0;
}
