#import <Cocoa/Cocoa.h>

int main(int argc, char *argv[])
{
#if DEBUG
    try {
        return NSApplicationMain(argc, (const char **)argv);
    }
    catch (const char* str) {
        printf("Exception %s\n", str);
    }
    catch (Rt::u4 ID) {
        printf("Exception 0x%X\n", ID);
    }
    catch (...) {
        printf("An exception was thrown\n");
    }
#else
    return NSApplicationMain(argc, (const char **)argv);
#endif
}
