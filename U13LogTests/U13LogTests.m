//
//  U13LogTests.m
//  U13LogTests
//
//  Created by Brane on 13-02-18.
//  Copyright (c) 2013 Universe 13. All rights reserved.
//

#import "U13LogTests.h"

@implementation U13LogTests

- (void)setUp
{
    [super setUp];
    
    // clear out last log entry
    LOG_E(@"");
}

- (void)tearDown
{
    
    [super tearDown];
}

- (void)testFormatting {
    // if this ever works, I would very much like to ditch the LOG_*F variants, but for now I'll stick with not crashing thank you very much
    [NSString stringWithFormat:@"%@"];
}

- (void)testLogLevels
{
    
    // only errors should generate a log entry
    [U13Log setLevel:U13LOG_LEVEL_ERROR];
    
    for (int level = U13LOG_LEVEL_VERBOSE; level < U13LOG_LEVEL_COUNT; ++level) {
        NSString *test = [U13Log nameForLevel:level];
        [U13Log log:level fn:__PRETTY_FUNCTION__ line:__LINE__ msg:test];
        NSString *found = [U13Log lastLoggedMessage];
        if ([found hasSuffix:test])
            STAssertTrue(level == U13LOG_LEVEL_ERROR, @"Should not have a log entry for anything but error, had one for %@", test);
        else 
            STAssertTrue(level != U13LOG_LEVEL_ERROR, @"Error should have had log entry but did not");
    }
    
    
    // all logs should generate log entries now
    [U13Log setLevel:U13LOG_LEVEL_VERBOSE];
    
    for (int level = U13LOG_LEVEL_VERBOSE; level < U13LOG_LEVEL_COUNT; ++level) {
        NSString *test = [U13Log nameForLevel:level];
        [U13Log log:level fn:__PRETTY_FUNCTION__ line:__LINE__ msg:test];
        NSString *found = [U13Log lastLoggedMessage];
        STAssertTrue([found isEqualToString:test], @"Expected %@ but got %@", test, found);
    }
    
}

@end
