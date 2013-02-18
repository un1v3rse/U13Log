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

/** Manage a rotating log file.
 
 ALL NSLog calls (not just U13Log calls) get written to the file.
 
 The file rotates once per day or once per app cold startup.
 
 The log can be sent via an email, or you can devise other means.
 
 You should call setup at or near the start of your app in order to effect this.  For example:
 
    int main(int argc, char *argv[])
    {
        @autoreleasepool {
            [U13Log2File setup];
        }
        @autoreleasepool {
            return UIApplicationMain(argc, argv, nil, NSStringFromClass([U13AppDelegate class]));
        }
    }

 
 */
@interface U13Log2File : NSObject

/** Set up the file logger using the default settings
 
 See setup:folder:fileNamer:headerWriter: for defaults.
 
 */
+ (void)setup;

/** Set up the file logger with custom settings
 
 @param max_logs The maximum number of log files to generate.  Default is 4.  Passing <0 will apply default.
 @param folder The folder to store the files in.  Default is Logs subfolder of the Documents folder.  Passing nil applies the default.
 @param fileNamer Custom file naming block.  Default simply calls nameForFileNth:.  Passing nil applies the default.
 @param fileNamer Custom header writing block.  Default simply calls writeHeader.  Passing nil applies the default.
 
 */
+ (void)setup:(int)max_logs
       folder:(NSString *)folder
    fileNamer:(U13Log2FileNamer)fileNamer
 headerWriter:(U13Log2FileHeaderWriter)headerWriter;


/** Get the name of the nth log file.
 
 Called internally to get the name of a file, exposed externally in case U13LogFileNamer block requires it.
 
 */
+ (NSString *)nameForFileNth:(int)nth;


/** Write the header for a log file.
 
 Called internally to write the header of each new log file, exposed externally in case U13Log2FileHeaderWriter block requires it.
 
 */
+ (void)writeHeader;


/** Attach the current set of logs to an email.
 
 */
+ (BOOL)attachLogsToMail:(MFMailComposeViewController *)mail;
    
@end
