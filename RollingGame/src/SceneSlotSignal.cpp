#include "GameScene.h"


void GameScene::timerEvent(QTimerEvent *_event )
{

    if (_event->timerId() == m_fpsTimer && start_Game)
    {
        if (m_time.elapsed()>10)
        {
            if (turn_Right)
            {
                m_Ball.addPosition(-movingSpeed,0.0f,movingSpeed);
            }
            else
            {
                m_Ball.addPosition(movingSpeed,0.0f,-movingSpeed);
            }

            path->movePath(movingSpeed);
            float ball_X = m_Ball.getPosition().m_x;
            float ball_Z = m_Ball.getPosition().m_z;
            inside_Block = path->ballDetect(ball_X, ball_Z, inside_Block, m_score);
            m_time.restart();
        }
        update();

        if (!start_Loop && m_updateBlockTime.elapsed()>5000)
        {
            start_Loop=true;
            m_updateBlockTime.restart();
        }

        if (m_updateBlockTime.elapsed()>800 && start_Loop)
        {
            block_index=path->updateBlock(block_index);
            m_updateBlockTime.restart();
            qDebug()<<"Score: "<<m_score;

        }

    }
    update();
}


void GameScene::keyPressEvent( QKeyEvent *_event)
{
    if (_event->key() == Qt::Key_Left)
    {
        turn_Right=false;
    }
    if (_event->key() == Qt::Key_Right)
    {
        turn_Right=true;
    }

    if (_event->key() == Qt::Key_Space)
    {
        start_Game = true;
        m_updateBlockTime.start();
        m_time.start();
        qDebug()<<"START GAME";
    }
    update();

}
