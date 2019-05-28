///Based on code examples from Jon Macey and Richard Southern

#include "GameScene.h"
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Image.h>
#include <ngl/Texture.h>
#include <iostream>

GameScene::GameScene(QWidget *parent) : QOpenGLWidget(parent) //constructor
{
    setFocus(); //set focus to this window
    this->resize(parent->size()); //resize the widget to the window
}

GameScene::~GameScene()=default;

void GameScene::resizeGL(int width, int height)
{
    m_projection=ngl::perspective( m_FOV, static_cast<float>(width) /height, m_NearCp, m_FarCp );
    m_width = static_cast<int> ( width * devicePixelRatio() );
    m_height = static_cast<int> ( height * devicePixelRatio());
    glViewport(0,0,width,height);
}

bool GameScene::createFramebufferObject()
{
    // create our FBO
    glGenFramebuffers(1, &m_fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

    // Try to use a texture depth component
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_textureID, 0);


    // disable the colour and read buffers as we only want depth
    glDrawBuffer(GL_NONE);

    // attach our texture to the FBO


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
    else return true;
    // switch back to window-system-provided framebuffer

}



void GameScene::initializeGL()
{
    ngl::NGLInit::instance(); //initialize NGL
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glEnable(GL_DEPTH_TEST); //depth test and multisampling for drawing
    glEnable(GL_MULTISAMPLE);

    ngl::VAOPrimitives::instance()->createLineGrid("grid",20,20,20);

    //load Gamey shaders
    shader = ngl::ShaderLib::instance();
    shader->loadShader("Toony",
                       "shaders/ToonVert.glsl",
                       "shaders/ToonFrag.glsl");
    //load shadow shader
    shader->loadShader("Shadow",
                       "shaders/ShadowVert.glsl",
                       "shaders/ShadowFrag.glsl");

    //default camera settings
    look = ngl::Vec3(0.0f, 0.0f, 0.0f);
    up = ngl::Vec3(0.0f, 1.0f, 0.0f);


    glViewport(0,0,m_width,m_height);
}


void GameScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glEnable(GL_CULL_FACE);

    //vao primitive variable
    auto obj = ngl::VAOPrimitives::instance();

    // update our camera
    camera.right = ngl::Vec3(-cos(camera.horizontalAngle),0.0f,sin(camera.horizontalAngle));
    camera.direction = ngl::Vec3(cos(camera.verticalAngle) * sin(camera.horizontalAngle),
                                 sin(camera.verticalAngle),
                                 cos(camera.verticalAngle) * cos(camera.horizontalAngle));
    up.cross(camera.right, camera.direction);
    eye = look + camera.direction * camera.distance;
    m_view=ngl::lookAt(eye,look,up);
    m_projection=ngl::perspective(m_FOV, //fov
                               float(m_width/m_height), //width/height
                               m_NearCp, //near cp
                               m_FarCp); //far cp

    //draw a grid
    MVP = m_projection * m_view * ngl::Mat4(); //calculate camera view
    shader->use(ngl::nglColourShader);
    shader->setUniform("MVP",MVP);
    shader->setUniform("Colour", ngl::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
    obj->draw("grid");

    //draw a cube
    m_model.translate(1.0f,1.0f,1.0f);
    MVP = m_projection * m_view * m_model;
    shader->use("Toony");
    shader->setUniform("MVP",MVP);
    obj->draw("cube");



    glViewport(0.0f,0.0f,m_width,m_height);

}



/*-------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------*/

