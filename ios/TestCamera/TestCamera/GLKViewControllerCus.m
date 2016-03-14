//
//  GLKViewControllerCus.m
//  TestCamera
//
//  Created by lee on 16/3/11.
//  Copyright © 2016年 examples. All rights reserved.
//

#import "GLESHelper.h"
#import "CaptureHelper.h"
#import "GLKViewControllerCus.h"

// 全屏(纹理)
static const GLfloat FULL_RECTANGLE_TEX_COORDS[] = {
    1.0f, 0.0f,     // 1 right top
    1.0f, 1.0f,     // 3 right bottom
    0.0f, 0.0f,     // 0 left top
    0.0f, 1.0f,     // 2 left bottom
};
// 全屏(fbo)
static const GLfloat FULL_RECTANGLE_COORDS[] = {
    -1.0f, -1.0f,   // 0 left top
    1.0f, -1.0f,    // 1 right top
    -1.0f, 1.0f,    // 2 left bottom
    1.0f, 1.0f,     // 3 right bottom
};
static const char VERTEX_SHADER[] =
"attribute vec4 aPosition;\n"
"attribute vec4 aTextureCoord;\n"
"varying vec2 vTextureCoord;\n"
"void main() {\n"
"    gl_Position = aPosition;\n"
"    vTextureCoord = (aTextureCoord).xy;\n"
"}\n";
static const char FRAGMENT_SHADER_COPY[] =
"precision mediump float;\n"
"varying vec2 vTextureCoord;\n"
"uniform sampler2D sTexture;\n"
"void main() {\n"
"       gl_FragColor = texture2D(sTexture, vTextureCoord);\n"
"}\n";

@interface GLKViewControllerCus ()

@end

@implementation GLKViewControllerCus
{
    /* gl */
    EAGLContext* _glContext;
    GLKView* _view;
    CVOpenGLESTextureCacheRef _textureCacheRef;
    CVOpenGLESTextureRef _textureRef;
    GLuint _program;
    GLuint  _glslTextureLoc;
    GLuint _glslVertexArrarysID;
    
    /* capture */
    AVCaptureSession* _captureSession;
    AVCaptureDeviceInput* _captureInput;
    AVCaptureVideoDataOutput* _captureOutput;
    CVImageBufferRef _cvImageRef;
    
}
-(id)init
{
    self = [super init];
    if (self) {
        [self initAll];
    }
    return self;
}
- (void)initAll {
    // gl
    _glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:_glContext];
    
    [self createProgram];
    
    GLKView *view = [[GLKView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.view = _view = view;
    _view.backgroundColor = [UIColor colorWithRed:1 green:1 blue:1 alpha:1];
    _view.context = _glContext;
    _view.delegate = self;
    glClearColor(1.0, 0.0, 0.0, 1.0);
    
//    self.preferredFramesPerSecond = 20;
    
    // capture
    [self startCapture];
}

-(void)createProgram {
    _program = [GLESHelper InitProgram:(const char*)VERTEX_SHADER fragment:(const char*)FRAGMENT_SHADER_COPY];
    if (!_program) {
        return;
    }
    
    bool status;
    
    GLuint maPositionLoc = glGetAttribLocation(_program, "aPosition");
    status = checkLocation(maPositionLoc, "maPositionLoc");
    
    GLuint maTextureCoordLoc = glGetAttribLocation(_program, "aTextureCoord");
    status = checkLocation(maTextureCoordLoc, "maTextureCoordLoc");
    
    
    _glslTextureLoc = glGetUniformLocation(_program, "sTexture");
    status = checkLocation(_glslTextureLoc, "_glslTextureLoc");
    
    // to screen
    glGenVertexArraysOES(1, &_glslVertexArrarysID);
    glBindVertexArrayOES(_glslVertexArrarysID);
    
    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), FULL_RECTANGLE_COORDS, GL_STATIC_DRAW);
    glEnableVertexAttribArray(maPositionLoc);
    glVertexAttribPointer(maPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);//vertexStride, vertexBuffer
    
    GLuint vertexBufferID_texture;
    glGenBuffers(1, &vertexBufferID_texture);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_texture);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), FULL_RECTANGLE_TEX_COORDS, GL_STATIC_DRAW);
    glEnableVertexAttribArray(maTextureCoordLoc);
    glVertexAttribPointer(maTextureCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);//texStride, texBuffer
    checkGlError("CreatePrograme vertexArrarysID to screen");

}

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    if(!_textureRef) {
        return;
    }
    if(!_program) {
        return;
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(_program);
    checkGlError("Filter glUseProgram");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, CVOpenGLESTextureGetName(_textureRef));
    glUniform1i(_glslTextureLoc, 0);
    checkGlError("Filter,_glslTextureLoc");
    
    glBindVertexArrayOES(_glslVertexArrarysID);
    checkGlError("Filter,_glslVertexArrarysID");
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CFRelease(_textureRef);
    _textureRef = 0;
}

-(void)startCapture {
    
    AVCaptureDevice* device = [CaptureHelper GetCaptureDevice:true];
    _captureInput = [AVCaptureDeviceInput deviceInputWithDevice:device  error:nil];
    
    dispatch_queue_t queue;
    queue = dispatch_queue_create("cameraQueue", NULL);
    _captureOutput = [CaptureHelper GetCaptureOutput];
    [_captureOutput setSampleBufferDelegate:self queue:queue];
    
    _captureSession = [[AVCaptureSession alloc] init];
    [_captureSession addInput:_captureInput];
    [_captureSession addOutput:_captureOutput];
    
    [_captureSession startRunning];
    [CaptureHelper ChangeDeviceSetting:device fpsden:20]; // 只能在addinput之后设置,否则addinput时会被重置
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection {
    
    _cvImageRef = CMSampleBufferGetImageBuffer(sampleBuffer);
    [self performSelectorOnMainThread:@selector(updateFrame) withObject:nil waitUntilDone:YES];
}
-(void)updateFrame {
    
    if (!_textureRef) {
        [GLESHelper InitTextFromImage:_cvImageRef textureCacheRef:&_textureCacheRef textureRef:&_textureRef];
    }
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskPortrait;
}
@end
