//
//  U13Log2File.m
//  U13Log
//
//  Created by Brane on 13-02-18.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

#import "U13Log2File.h"

#import "U13DebugBreak.h"
#import <MessageUI/MFMailComposeViewController.h>

static NSTimeInterval NEXT_ROTATION = 0;

static NSString *LOG_FOLDER = nil;
static int MAX_LOGS = 4;

static U13Log2FileNamer FILE_NAMER = nil;
static U13Log2FileHeaderWriter HEADER_WRITER = nil;

@implementation U13Log2File

+ (NSString *)fileName:(int)nth {
	return FILE_NAMER ? FILE_NAMER(nth) : [self nameForFileNth:nth];
}

+ (NSString *)filePath:(int)nth {
	return [NSString stringWithFormat:@"%@/%@", LOG_FOLDER, [self fileName:nth]];
}

+ (void)rotate {
	
	NSFileManager *fm = [[NSFileManager alloc] init];
	
	NSString *prev_path = nil;
	for (int ii = MAX_LOGS; ii > 0; --ii) {
		NSString *path = [self filePath:ii];
		if ([fm fileExistsAtPath:path]) {
			NSError *error = nil;
			if (prev_path) {
				[fm moveItemAtPath:path toPath:prev_path error:&error];
			} else {
				[fm removeItemAtPath:path error:&error];
			}
		}
		prev_path = path;
	}
    
    // Don't actually redirect the log file if we're debugging
    if (!AmIBeingDebugged()) {
        freopen([prev_path fileSystemRepresentation], "a", stderr);
    }
    
    if (HEADER_WRITER)
        HEADER_WRITER();
    else
        [self writeHeader];
    
    [U13Log setWillLogHandler:^{
		if (NEXT_ROTATION < [NSDate timeIntervalSinceReferenceDate])
			[self rotate];
    }];
    
	NEXT_ROTATION = [NSDate timeIntervalSinceReferenceDate] + (60 * 60 * 24);
}

+ (void)setup {
    [self setup:0 folder:nil fileNamer:nil headerWriter:nil];
}

+ (void)setup:(int)max_logs
       folder:(NSString *)folder
    fileNamer:(U13Log2FileNamer)fileNamer
 headerWriter:(U13Log2FileHeaderWriter)headerWriter
{
    
    MAX_LOGS = max_logs;
    if (MAX_LOGS <= 0)
        MAX_LOGS = 4;
    
    LOG_FOLDER = folder;
    if (!LOG_FOLDER.length) {
        NSArray *documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDir = [documentPaths objectAtIndex:0];
        LOG_FOLDER = [documentsDir stringByAppendingPathComponent:@"/Logs"];
    }
    
    FILE_NAMER = [fileNamer copy];
    HEADER_WRITER = [headerWriter copy];
    
    BOOL isDir = YES;
	NSFileManager *fm = [[NSFileManager alloc] init];
	if(![fm fileExistsAtPath:LOG_FOLDER isDirectory:&isDir]) {
		NSError *error;
		if(![fm createDirectoryAtPath:LOG_FOLDER withIntermediateDirectories:NO attributes:nil error:&error])
			NSLog(@"Error: Create folder failed: %@", error );
	}
    
	[self rotate];
}

+ (NSString *)nameForFileNth:(int)nth {
    return [NSString stringWithFormat:@"log_%d.txt", nth];
}

+ (void)writeHeader {
    // write device info at top of log
    UIDevice *device = [UIDevice currentDevice];
    NSLog(@"Model: %@", device.model );
    NSLog(@"System Name: %@", device.systemName );
    NSLog(@"System Version: %@", device.systemVersion );
}

+ (BOOL)attachLogsToMail:(MFMailComposeViewController *)mail {
	
	BOOL result = NO;
	
	// iterate the logs, attaching them to the mail
	
	NSFileManager *fm = [[NSFileManager alloc] init];

	for (int ii = 0; ii < MAX_LOGS; ++ii) {
		NSString *path = [self filePath:ii + 1];
		if ([fm fileExistsAtPath:path]) {
			NSData *data = [NSData dataWithContentsOfFile:path];
			
			if (data) {
				[mail addAttachmentData:data mimeType:@"text/plain" fileName:[self fileName:ii + 1]];
				result = YES;
			}
		}
	}
	return result;
}


@end
