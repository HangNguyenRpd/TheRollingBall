#ifndef GAMESCENE_H
#define GAMESCENE_H


#include <ngl/Mat4.h>
#include <ngl/Vec3.h>
#include <ngl/Text.h>
#include <QOpenGLWidget>
#include <memory>
#include <ngl/Obj.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTime>
#include "Path.h"


struct WinParams
{
  bool rotate = false;
  bool translate = false;
  int origX = 0;
  int origY = 0;
  int origXPos = 0;
  int origYPos = 0;

};

struct Camera
{
    float horizontalAngle = 0.0f;
    float verticalAngle = 0.0f;
    ngl::Vec3 direction = ngl::Vec3(0.0f, 0.0f, -1.0f);
    float distance = 20.0f;
    ngl::Vec3 right = ngl::Vec3(-1.0f, 0.0f, 0.0f);
};

struct ModelTransform
{
    ngl::Vec3 Translate = ngl::Vec3(0.0f, 0.0f, 0.0f);
    ngl::Vec3 Rotate = ngl::Vec3(0.0f, 0.0f, 0.0f);
    ngl::Vec3 Scale = ngl::Vec3(1.0f, 1.0f, 1.0f);
};


class GameScene : public QOpenGLWidget //NGL openGL drawing context
{
    Q_OBJECT
    public:
        GameScene(QWidget *parent);
        ~GameScene() override;
        void initializeGL() override;        //set up GL context
        void paintGL() override;             //draw the scene
        void resizeGL(int width, int height) override;
        bool createFramebufferObject();

        void mouseMoveEvent(QMouseEvent *_event) override;
        void mousePressEvent(QMouseEvent *_event) override;
        void mouseReleaseEvent(QMouseEvent *_event) override;
        void wheelEvent( QWheelEvent* _event ) override;
        void keyPressEvent(QKeyEvent *_event) override;

        void timerEvent(QTimerEvent *event) override;

    public slots:
        void setFOV(double m_FOV);
        void setNearCp(double m_NearCp);
        void setFarCp(double m_FarCp);

    protected:
        WinParams m_win;
        Camera camera;


    private:
        float m_width = 1024;
        float m_height = 720;
        ngl::Vec3 eye, look, up;
        ngl::Mat4 m_view;
        ngl::Mat4 m_projection;
        ngl::Transformation m_Cube;
        ngl::Mat4 MVP;
        ngl::Transformation m_Ball;
        float m_FOV = 45.0f, m_NearCp = 0.01f, m_FarCp = 300.0f;
        ngl::ShaderLib* shader;
        Path *path;

        //from light view
        ngl::Mat4 m_lightCameraView;
        ngl::Mat4 m_lightCameraProject;
        ngl::Vec3 m_lightPosition;

        //FBO
        GLuint m_textureID;
        GLuint m_fboID;

        //text
        std::unique_ptr<ngl::Text> m_text;

        //timer
        QTime m_time;
        QTime m_updateBlockTime;
        int block_index=0;
        int m_fpsTimer;
        bool start_Game = false;
        bool start_Loop = false;
        bool turn_Right = false;
        float movingSpeed = 0.04f;
        float backSpeed =0.0f;
        int inside_Block = 0;
        int m_score = 0;

};

#endif // GAMESCENE_H
