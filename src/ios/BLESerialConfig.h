//
//  BLESerialConfig
//  Holds callback, delimiter and buffer
//

#import <Foundation/Foundation.h>

@interface BLESerialConfig : NSObject

@property NSString *callback;
@property NSString *delimiter;
@property NSMutableData *buffer;

@end