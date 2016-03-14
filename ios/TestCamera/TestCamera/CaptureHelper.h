//
//  CaptureHelper.h
//  CameraFilter
//
//  Created by lee on 16/2/25.
//  Copyright © 2016年 miku. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>

@interface CaptureHelper : NSObject
+(void)ChangeDeviceSetting:(AVCaptureDevice*)device fpsden:(int)fpsden;
+(AVCaptureDevice*) GetCaptureDevice:(bool)isPositionBack;
+(AVCaptureVideoDataOutput*)GetCaptureOutput;
@end
