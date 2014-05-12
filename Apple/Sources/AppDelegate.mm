#import "AppDelegate.h"
#import "OpenGLView/OpenGLView.h"

@implementation AppDelegate

- (void)awakeFromNib
{
    [window center];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
	return YES;
}

@end

