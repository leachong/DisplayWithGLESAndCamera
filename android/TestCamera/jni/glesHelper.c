

#include <stdlib.h>
#include <android/log.h>
#include "glesHelper.h"

#define LOG_FILE "[GLESEX]"

BOOL checkGlError(const char* funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
    	LOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return TRUE;
    }
    return FALSE;
}
GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                LOGE("Could not compile %s shader:\n%s\n",shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
GLuint createProgram(const char* vtxSrc, const char* fragSrc)
{
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        //ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                LOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

static const GLfloat noRotationTextureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
};

static const GLfloat rotateLeftTextureCoordinates[] = {
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
};

static const GLfloat rotateRightTextureCoordinates[] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

static const GLfloat verticalFlipTextureCoordinates[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f,  0.0f,
    1.0f,  0.0f,
};

static const GLfloat horizontalFlipTextureCoordinates[] = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f,  1.0f,
    0.0f,  1.0f,
};

static const GLfloat rotateRightVerticalFlipTextureCoordinates[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
};

static const GLfloat rotateRightHorizontalFlipTextureCoordinates[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
};

static const GLfloat rotate180TextureCoordinates[] = {
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};


const float* getRotation(GPUImageRotationMode rotationMode)
{
	const float* fret = noRotationTextureCoordinates;
    switch(rotationMode)
    {
        case kGPUImageRotateLeft:
        	LOGD(LOG_FILE "select kGPUImageRotateLeft");
        	fret = rotateLeftTextureCoordinates;
        	break;
        case kGPUImageRotateRight:
        	LOGD(LOG_FILE "select kGPUImageRotateRight");
        	fret = rotateRightTextureCoordinates;
        	break;
        case kGPUImageFlipVertical:
        	LOGD(LOG_FILE "select kGPUImageFlipVertical");
        	fret = verticalFlipTextureCoordinates;
        	break;
        case kGPUImageFlipHorizonal:
        	LOGD(LOG_FILE "select kGPUImageFlipHorizonal");
        	fret = horizontalFlipTextureCoordinates;
        	break;
        case kGPUImageRotateRightFlipVertical:
        	LOGD(LOG_FILE "select kGPUImageRotateRightFlipVertical");
        	fret = rotateRightVerticalFlipTextureCoordinates;
        	break;
        case kGPUImageRotateRightFlipHorizontal:
        	LOGD(LOG_FILE "select kGPUImageRotateRightFlipHorizontal");
        	fret = rotateRightHorizontalFlipTextureCoordinates;
        	break;
        case kGPUImageRotate180:
        	LOGD(LOG_FILE "select kGPUImageRotate180");
        	fret = rotate180TextureCoordinates;
        	break;
        default:
        	LOGD(LOG_FILE "select kGPUImageNoRotation");
        	break;
    }
    return fret;
}

GLuint createTexture(uint8_t* data, int width, int height)
{
    int oldTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D,&oldTexture);

	GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
    if(width>0 && height>0)
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, oldTexture);
    return texture;
}
