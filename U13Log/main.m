//
//  main.m
//  U13Log
//
//  Created by Chris Wright on 13-02-18.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "U13AppDelegate.h"
#import "U13Log2File.h"

int main(int argc, char *argv[])
{
    // this is what you add for file-based logging
    @autoreleasepool {
        [U13Log2File setup];
    }
    
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([U13AppDelegate class]));
    }
}
