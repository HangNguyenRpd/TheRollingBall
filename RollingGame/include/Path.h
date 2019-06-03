#ifndef PATH_H
#define PATH_H


#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <QDebug>
#include <cstdlib>
#include <iostream>
#include <vector>


struct cube
{
    unsigned int length = 2;
    float positionX = 0.0f;
    float positionZ = 0.0f;
    float endBlock_X = 0.0f;
    float endBlock_Z = 0.0f;
    bool turn_Right = false;

};

class Path
{
    public:
        Path();
        ~Path();
        void movePath(float backSpeed);
        void drawPath();
        void placeBlock(int i);
        void calBlocks();
        int updateBlock(int i);
        int num_of_Blocks = 20;

        int ballDetect(float ball_X, float ball_Z, int old_inside_Block, int &score);



    private:

        ngl::Transformation position_Block;
        std::vector<cube> block;
};

#endif // PATH_H
