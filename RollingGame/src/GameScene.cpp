///Based on code examples from Jon Macey and Richard Southern

#include "GameScene.h"
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/Image.h>
#include <ngl/Texture.h>
#include <math.h>
#include <QFont>


GameScene::GameScene(QWidget *parent) : QOpenGLWidget(parent) //constructor
{
    setFocus(); //set focus to this window
    this->resize(parent->size()); //resize the widget to the window
    path = new Path();
}

GameScene::~GameScene()
{
    delete path;
}

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
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST); //depth test and multisampling for drawing
    glEnable(GL_MULTISAMPLE);

    ngl::VAOPrimitives::instance()->createLineGrid("grid",30,30,30);
    ngl::VAOPrimitives::instance()->createSphere("sphere",0.2,10);

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
    //eye = ngl::Vec3(-20.0f, 20.0f, -20.0f);


    //start timer
    m_time.start();
    m_updateBlockTime.start();
    m_fpsTimer =startTimer(0);
    m_Cube.setPosition(0.5f,0.0f,0.5f);
    m_Cube.setScale(1.0f,2.0f,1.0f);
    m_Ball.setPosition(0.0f,1.2f,0.0f);
    m_text.reset( new ngl::Text(QFont("Arial",14)));
    //m_text->setTransform(20,20);
    m_text->setScreenSize(m_width,m_height);


    path->calBlocks();


    //update viewport
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
    //eye = ngl::Vec3(-11.0f, 11.0f, -11.0f);
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
    //obj->draw("grid");



    //a rolling sphere
    shader->use("Toony");
    shader->setUniform("P",m_projection);
    shader->setUniform("M",m_Ball.getMatrix());
    shader->setUniform("V", m_view);
    shader->setUniform("ColourMap", ngl::Vec4(0.3f,0.2f,0.1f,1.0f));
    obj->draw("sphere");


    //draw path
    shader->use("Toony");
    shader->setUniform("P",m_projection);
    shader->setUniform("V", m_view);
    shader->setUniform("ColourMap", ngl::Vec4(0.1f,0.4f,0.6f,1.0f));
    path->drawPath();

    //draw text score
    QString text="Score: ";
    m_text->setColour(0,0,0);
    m_text->renderText(10,20,text);

    //when ball falls out of the map -> lose
    if (inside_Block==-1)
    {
        text="LOSE";
        m_text->renderText(10,20,text);
        m_Ball.addPosition(0,-0.2f,0);
        start_Game=false;
        if (end_Game)
        {
            qDebug()<<"END GAME";
            qDebug()<<"TOTAL SCORE:  "<<m_score;
            end_Game=false;
        }


    }




    glViewport(0.0f,0.0f,m_width,m_height);

}



/*-------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------*/

