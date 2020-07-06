//
//  main.cpp
//  OpenGLTest
//
//  Created by tlab on 2020/6/29.
//  Copyright © 2020 tlab. All rights reserved.
//

#include "GLShaderManager.h"

#include "GLTools.h"
#include <glut/glut.h>

//简单的批次容器，是GLTOOLs 的一个简单的容器类
GLBatch triangleBatch;

GLShaderManager shaderManager;

GLfloat blockSize = 0.1f;

GLfloat vVerts[] = {
    -blockSize, -blockSize, 0.0f,
    blockSize, -blockSize, 0.0f,
    blockSize, blockSize, 0.0f,
    -blockSize, blockSize, 0.0f
};

//记录移动图形时，在x轴上平移的距离
GLfloat xPos = 0.0f;
//记录移动图形时，在y轴上平移的距离
GLfloat yPos = 0.0f;

//窗口大小改变时接受新的宽度和高度，其中0,0代表窗口中视口的左下角坐标，w，h代表像素
void ChangeSize(int w, int h) {
    glViewport(0,0, w, h);
}

//为程序作一次性的设置
void SetupRC() {
    //设置背影颜色
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //初始化着色管理器
    shaderManager.InitializeStockShaders();

    //批次处理，图元链接方式设置为 GL_TRIANGLE_FAN
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}


void SpecialKeys(int key, int x, int y) {
    GLfloat stepSize = 0.025f;
    
    if (key == GLUT_KEY_UP) {
        
        yPos += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        yPos -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        xPos -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        xPos += stepSize;
    }
    
    //碰撞检测
    if (xPos < (-1.0f + blockSize)) {
        
        xPos = -1.0f + blockSize;
    }
    
    if (xPos > (1.0f - blockSize)) {
        xPos = 1.0f - blockSize;
    }
    
    if (yPos < (-1.0f + blockSize)) {
        yPos = -1.0f + blockSize;
    }
    
    if (yPos > (1.0f - blockSize)) {
        yPos = 1.0f - blockSize;
    }
    
    glutPostRedisplay();
}

//开始渲染

void RenderScene() {
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    //设置一组浮点数来表示红色
    GLfloat vRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    
    //定义矩阵
    M3DMatrix44f mTransformMatrix;
    
    //平移矩阵
    m3dTranslationMatrix44(mTransformMatrix, xPos, yPos, 0.0f);
    
    //当单元着色器不够用时，使用平面着色器
    //参数1：存储着色器类型
    //参数2：使用什么矩阵变换
    //参数3：颜色
    shaderManager.UseStockShader(GLT_SHADER_FLAT, mTransformMatrix, vRed);
    
    //提交着色器
    triangleBatch.Draw();
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    //初始化GLUT库
    glutInit(&argc, argv);
    
    /*初始化双缓冲窗口，其中标志GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别指：双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区*/
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    //GLUT窗口大小，标题窗口
    glutInitWindowSize(400, 400);
    glutCreateWindow("Square");
    
    //注册回调函数
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
    //特殊键位响应函数(当用户使⽤特殊键位则会调⽤用该函数)
    glutSpecialFunc(SpecialKeys);
    
    //驱动程序的初始化中没有出现任何问题。
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        fprintf(stderr, "glew error:%s\n", glewGetErrorString(err));
        return 1;
    }
    
    //调用SetupRC
    SetupRC();
    glutMainLoop();
    
    return 0;
}

