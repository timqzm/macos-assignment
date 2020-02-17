//
//  main.cpp
//  fsmonitor
//
//  Created by Timofey Kuzmin on 17.2.2020.
//  Copyright © 2020 Timofey Kuzmin. All rights reserved.
//

#include <CoreServices/CoreServices.h>
#include <stdio.h>
#include <string.h>

void eventCallback(FSEventStreamRef stream,
                   void* callbackInfo,
                   size_t numEvents,
                   void* paths,
                   const FSEventStreamEventFlags eventFlags[],
                   const FSEventStreamEventId eventIds[]) {
    char **pathsList = (char **)paths;

    for(int i = 0; i < numEvents; i++) {
        uint32 flags = eventFlags[i];
        if(flags & kFSEventStreamEventFlagItemRemoved) {
            printf("- %s\n", pathsList[i]);
        } else if(flags & kFSEventStreamEventFlagItemCreated) {
            printf("+ %s\n", pathsList[i]);
        }
    }
}

int main(int argc, const char * argv[])
{
    CFMutableArrayRef paths = CFArrayCreateMutable(nullptr,
                                                   0,
                                                   &kCFTypeArrayCallBacks);
    for (int i = 1; i < argc; i++) {
        CFStringRef path = CFStringCreateWithCString(kCFAllocatorDefault,
                                                     argv[i],
                                                     kCFStringEncodingUTF8);
        CFArrayAppendValue(paths, path);
    }

    CFRunLoopRef loop = CFRunLoopGetCurrent();
    FSEventStreamRef stream = FSEventStreamCreate(nullptr,
                                                  (FSEventStreamCallback)eventCallback,
                                                  nullptr,
                                                  paths,
                                                  kFSEventStreamEventIdSinceNow,
                                                  0,
                                                  kFSEventStreamCreateFlagFileEvents | kFSEventStreamCreateFlagNoDefer);

    FSEventStreamScheduleWithRunLoop(stream, loop, kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);

    CFRunLoopRun();

    FSEventStreamStop(stream);
    FSEventStreamInvalidate(stream);
    FSEventStreamRelease(stream);

    return 0;
}
