//
//  GLESHelper.m
//  CameraFilter
//
//  Created by lee on 16/2/24.
//  Copyright © 2016年 ;. All rights reserved.
//

#import "GLESHelper.h"

bool checkLocation(int location, const char *label) {
    if (location < 0) {
        NSLog(@"Unknown location:%s.", label);
        return false;
    }
    return true;
}
void checkGlError(const char* op) {
    for (GLenum error = glGetError(); error; error = glGetError()) {
        NSLog(@"after %@() glError (0x%x)\n", [NSString stringWithUTF8String:op], error);
    }
    //GL_NO_ERROR
}

@implementation GLESHelper

+(GLuint)LoadShader:(GLenum)shaderType code:(const char*)pSource
{
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    NSLog(@"loadShader Could not compile shader %d:\n%@\n",shaderType, [NSString stringWithUTF8String:buf]);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}
+(GLuint)InitProgram:(const char*)pVertexSource fragment:(const char*)pFragmentSource
{
    GLuint program;
    
    GLuint vertexShader = [GLESHelper LoadShader:GL_VERTEX_SHADER code:pVertexSource];
    if (!vertexShader) {
        return 0;
    }
    
    GLuint pixelShader = [GLESHelper LoadShader:GL_FRAGMENT_SHADER code:pFragmentSource];
    if (!pixelShader) {
        return 0;
    }
    
    program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("createProgram glAttachShader vertexShader");
        glAttachShader(program, pixelShader);
        checkGlError("createProgram glAttachShader pixelShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    NSLog(@"createProgram Could not link program:\n%@\n", [NSString stringWithUTF8String:buf]);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

+(void)InitTextFromImage:(CVImageBufferRef)imageBufferRef textureCacheRef:(CVOpenGLESTextureCacheRef*)pTextureCacheRef textureRef:(CVOpenGLESTextureRef*)pTextureRef
{
    if (!*pTextureCacheRef) {
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, [EAGLContext currentContext], NULL, pTextureCacheRef);
        if (err)
        {
            NSLog(@"CVOpenGLESTextureCacheCreate %d, %p", err,[EAGLContext currentContext]);
        }
    }
    size_t width = CVPixelBufferGetWidth(imageBufferRef);
    size_t height = CVPixelBufferGetHeight(imageBufferRef);
    CVOpenGLESTextureCacheCreateTextureFromImage (kCFAllocatorDefault, *pTextureCacheRef
                                                  , imageBufferRef,
                                                  NULL, // texture attributes
                                                  GL_TEXTURE_2D,
                                                  GL_RGBA, // opengl format
                                                  (int)width,
                                                  (int)height,
                                                  GL_BGRA, // native iOS format
                                                  GL_UNSIGNED_BYTE,
                                                  0,
                                                  pTextureRef);
    checkGlError("captureOutput CVOpenGLESTextureCacheCreateTextureFromImage");
    glBindTexture(GL_TEXTURE_2D, CVOpenGLESTextureGetName(*pTextureRef));
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
}

@end
