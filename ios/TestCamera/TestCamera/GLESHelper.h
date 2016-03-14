//
//  GLESHelper.h
//  CameraFilter
//
//  Created by lee on 16/2/24.
//  Copyright © 2016年 miku. All rights reserved.
//

#include <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#import <UIKit/UIKit.h>
#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

typedef struct _SizeI
{
    int x;
    int y;
}SizeI;

@interface GLESHelper : NSObject
+(GLuint)InitProgram:(const char*)pVertexSource fragment:(const char*)pFragmentSource;
+(void)InitTextFromImage:(CVImageBufferRef)imageBufferRef textureCacheRef:(CVOpenGLESTextureCacheRef*)pTextureCacheRef textureRef:(CVOpenGLESTextureRef*)pTextureRef;
@end

bool checkLocation(int location, const char *label);
void checkGlError(const char* op);






