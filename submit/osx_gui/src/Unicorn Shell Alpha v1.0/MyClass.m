//
//  MyClass.m
//  Unicorn Shell Alpha v1.0
//
//  Created by Christopher McMillan on 5/19/14.
//  Copyright (c) 2014 FIve Make A Shell. All rights reserved.
//

#import "MyClass.h"

@implementation MyClass
-(IBAction)displayString:(id)sender{
    NSString *string = [textField stringValue];
    NSLog (@"%@", string);
}
@end
