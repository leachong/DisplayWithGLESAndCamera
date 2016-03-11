/*
 * TestRenderer.c
 *
 *  Created on: 2016-3-11
 *      Author: lee
 */

#include <android/log.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdint.h>
#include "glesHelper.h"
#include "TestRenderer.h"

static float _framebuffer_vertex_buffer[] =
{
	    -1.0f, -1.0f,   // 0 left top
	    1.0f, -1.0f,    // 1 right top
	    -1.0f, 1.0f,    // 2 left bottom
	    1.0f, 1.0f,     // 3 right bottom
};

static const char VERTEX_SHADER[] =
    "#version 100\n"
    "attribute vec4 position;\n"
    "attribute vec4 inputTextureCoordinate;\n"
    "varying vec2 textureCoordinate;\n"
    "void main() {\n"
    "    gl_Position = position;\n"
    "    textureCoordinate = inputTextureCoordinate.xy;\n"
    "}\n";

static const char FRAGMENT_SHADER[] =
    "#version 100\n"
	"#extension GL_OES_EGL_image_external : require\n"
	"uniform samplerExternalOES inputImageTexture;\n"
    "varying highp vec2 textureCoordinate;\n"
    "void main() {\n"
    "    gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n"
    "}\n";

static GLuint _eglProgram = 0;
static GLint _positionAttrib;
static GLint _textureCoordinateAttrib;
static GLint _inputImageTextureUniform;

static GLuint _framebufferVB = 0;
static GLuint _textureVB = 0;

static GLuint _texture = 0;
static int _textureWidth = 0;
static int _textureHeight = 0;
static int _outputWidth = 0;
static int _outputHeight = 0;

static GPUImageRotationMode _rotationMode = kGPUImageNoRotation;
static BOOL _rotationChanged = TRUE;

static void beforeOnDraw()
{
    int oldBuf = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&oldBuf);

    if(!_framebufferVB)
    {
		glGenBuffers(1, &_framebufferVB);
		glBindBuffer(GL_ARRAY_BUFFER, _framebufferVB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_framebuffer_vertex_buffer), &_framebuffer_vertex_buffer[0], GL_STATIC_DRAW);
    }
	if(!_textureVB)
	    glGenBuffers(1, &_textureVB);
	if(_rotationChanged)
	{
		const float* vb = getRotation(_rotationMode);
	    glBindBuffer(GL_ARRAY_BUFFER, _textureVB);
	    glBufferData(GL_ARRAY_BUFFER, 2*4*sizeof(float), vb, GL_STATIC_DRAW);

	    glBindBuffer(GL_ARRAY_BUFFER, oldBuf);
		_rotationChanged = FALSE;
	}
}

void Renderer_Init()
{
	_eglProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!_eglProgram)
    {
        return;
    }

    _positionAttrib = glGetAttribLocation(_eglProgram, "position");
    _textureCoordinateAttrib = glGetAttribLocation(_eglProgram, "inputTextureCoordinate");
    _inputImageTextureUniform = glGetUniformLocation(_eglProgram, "inputImageTexture");
}
void Renderer_Release()
{
    if(_eglProgram)
    	glDeleteProgram(_eglProgram);
    _eglProgram = 0;
}
void Renderer_Draw()
{
	beforeOnDraw();

    glUseProgram(_eglProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, _texture);
    glUniform1i(_inputImageTextureUniform, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _textureVB);
    glEnableVertexAttribArray(_textureCoordinateAttrib);
    glVertexAttribPointer(_textureCoordinateAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, _framebufferVB);
    glEnableVertexAttribArray(_positionAttrib);
    glVertexAttribPointer(_positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer_SetTexture(int tid)
{
	_texture = tid;
}
void Renderer_SetInputSize(int w,int h)
{
	if(_textureWidth != w || _textureHeight != h)
	{
		_textureWidth = w;
		_textureHeight = h;
	}
}

void Renderer_SetOutputSize(int w,int h)
{
	if(_outputWidth != w || _outputWidth != h)
	{
		_outputWidth = w;
		_outputHeight = h;
	}
}
void Renderer_SetRotationMode(int m)
{
	if(_rotationMode != m)
	{
		_rotationMode = m;
		_rotationChanged = TRUE;
	}
}
