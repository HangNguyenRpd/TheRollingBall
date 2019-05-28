#include "GameScene.h"


void GameScene::setFOV(double ui_FOV)
{
    m_FOV = ui_FOV;
    update();
}

void GameScene::setNearCp(double ui_NearCp)
{
    m_NearCp = ui_NearCp;
    update();
}

void GameScene::setFarCp(double ui_FarCp)
{
    m_FarCp = ui_FarCp;
    update();
}
