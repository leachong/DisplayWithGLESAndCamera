/*
 * TestRenderer.h
 *
 *  Created on: 2016-3-11
 *      Author: lee
 */

#ifndef TESTRENDER_H_
#define TESTRENDER_H_

void Renderer_Init();
void Renderer_Draw();
void Renderer_Release();
void Renderer_SetTexture(int);
void Renderer_SetInputSize(int w,int h);
void Renderer_SetOutputSize(int w,int h);
void Renderer_SetRotationMode(int);




#endif /* TESTRENDER_H_ */
