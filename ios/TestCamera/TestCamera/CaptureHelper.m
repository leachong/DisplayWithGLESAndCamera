//
//  CaptureHelper.m
//  CameraFilter
//
//  Created by lee on 16/2/25.
//  Copyright © 2016年 miku. All rights reserved.
//

#import "CaptureHelper.h"

@implementation CaptureHelper

+(void)ChangeDeviceSetting:(AVCaptureDevice*)device fpsden:(int)fpsden
{
    
    AVCaptureDeviceFormat* newFormat = device.activeFormat;
     NSArray* supportedFormats = device.formats;
    for (AVCaptureDeviceFormat* format in supportedFormats) {
        //NSLog(@"%@",format);
         if (format.highResolutionStillImageDimensions.width == 640
             && true && CMFormatDescriptionGetMediaSubType(format.formatDescription) == '420v') {
             newFormat = format;
        }
    }
    
    NSError *error;
    CMTime frameDuration = CMTimeMake(1, fpsden);
    NSArray *supportedFrameRateRanges = [newFormat videoSupportedFrameRateRanges];
    BOOL frameRateSupported = NO;
    for (AVFrameRateRange *range in supportedFrameRateRanges) {
        if (CMTIME_COMPARE_INLINE(frameDuration, >=, range.minFrameDuration) &&
            CMTIME_COMPARE_INLINE(frameDuration, <=, range.maxFrameDuration)) {
            frameRateSupported = YES;
        }
    }
    
    if ([device lockForConfiguration:&error]) {
        device.activeFormat = newFormat;
        if (frameRateSupported) {
            [device setActiveVideoMaxFrameDuration:frameDuration];
            [device setActiveVideoMinFrameDuration:frameDuration];
        }
        [device unlockForConfiguration];
    }
}
+(AVCaptureDevice*) GetCaptureDevice:(bool)isPositionBack
{
    AVCaptureDevicePosition pos = isPositionBack?AVCaptureDevicePositionFront:AVCaptureDevicePositionBack;
    NSArray* devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice* device in devices ){
        if (device.position == pos) {
            return device;
        }
    }
    return nil;
}
+(AVCaptureVideoDataOutput*)GetCaptureOutput
{
    AVCaptureVideoDataOutput *captureOutput = [[AVCaptureVideoDataOutput alloc]
                                               init];
    captureOutput.alwaysDiscardsLateVideoFrames = YES;
    
    NSString* key = (NSString*)kCVPixelBufferPixelFormatTypeKey;
    NSNumber* value = [NSNumber
                       numberWithUnsignedInt:kCVPixelFormatType_32BGRA];
    NSDictionary* videoSettings = [NSDictionary
                                   dictionaryWithObject:value forKey:key];
    [captureOutput setVideoSettings:videoSettings];
    
    return captureOutput;
}

@end
