#include "Mainwindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_scene = new GameScene(this);
    m_ui->GameSceneLayout->addWidget(m_scene);
    m_scene->setFocus();

    //connect all slots and signals
    connect(m_ui->CameraFOVInput, SIGNAL(valueChanged(double)), m_scene, SLOT(setFOV(double)));
    connect(m_ui->NearCp, SIGNAL(valueChanged(double)), m_scene, SLOT(setNearCp(double)));
    connect(m_ui->FarCp, SIGNAL(valueChanged(double)), m_scene, SLOT(setFarCp(double)));
}

MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_scene;
}
