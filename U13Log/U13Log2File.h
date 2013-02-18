//
//  U13Log2File.h
//  U13Log
//
//  Created by Brane on 13-02-18.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

@class MFMailComposeViewController;

typedef NSString *(^U13Log2FileNamer)(int nth);
typedef void(^U13Log2FileHeaderWriter)();

/** Manage a rotating log file that ALL NSLog calls (not just U13Log calls) get written to.
 
 */
@interface U13Log2File : NSObject

+ (void)setup;
+ (void)setup:(int)max_logs
       folder:(NSString *)folder
    fileNamer:(U13Log2FileNamer)fileNamer
 headerWriter:(U13Log2FileHeaderWriter)headerWriter;

+ (NSString *)nameForFileNth:(int)nth;

+ (void)writeHeader;

+ (BOOL)attachLogsToMail:(MFMailComposeViewController *)mail;
    
@end
