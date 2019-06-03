#include "Path.h"


Path::Path()
{
    block.resize(num_of_Blocks);
}

Path::~Path()=default;

void Path::drawPath()
{
    ngl::VAOPrimitives* cube = ngl::VAOPrimitives::instance();
    ngl::ShaderLib* shader = ngl::ShaderLib::instance();
    int i=0;
    /*
    for (i=0; i<num_of_Blocks; ++i)
    {
        if ((block[i].endBlock_X < -12) && (block[i].endBlock_Z < -12)) //block goes out of screen
        {
            updateBlock(i);
        }
    } */

    for (i=0; i<num_of_Blocks; ++i )
    {
        if (block[i].turn_Right)
        {
            position_Block.setScale(1.0f, 2.0f, block[i].length);
        }
        else
        {
            position_Block.setScale(block[i].length, 2.0f, 1.0f);
        }
        position_Block.setPosition(block[i].positionX, 0.0f, block[i].positionZ);
        shader->setUniform("M",position_Block.getMatrix());
        cube->draw("cube");
    }
}

void Path::movePath(float backSpeed)
{
    int i=0;

    for (i=0; i<num_of_Blocks; ++i )
    {

        block[i].positionX-=backSpeed;
        block[i].positionZ-=backSpeed;

        block[i].endBlock_X-=backSpeed;
        block[i].endBlock_Z-=backSpeed;
    }
}

void Path::placeBlock(int i)
{
    block[i].turn_Right = !block[i-1].turn_Right;
    if (!block[i].turn_Right)
    {
        do //to prevent the blocks go outside screen
        {
            block[i].length = ((rand() % 8) + 1);
            //calculate the end position of the block
            block[i].endBlock_Z = block[i-1].positionZ + block[i-1].length*0.5f;
            block[i].endBlock_X = block[i-1].positionX;
        }
        while ((block[i].endBlock_Z-block[i].endBlock_X)*0.5f > 7);

        //new block centre
        block[i].positionZ = block[i-1].positionZ + block[i-1].length*0.5f - 0.5f;
        block[i].positionX = block[i-1].positionX + block[i].length*0.5f + 0.5f;

    }
    else
    {
        do //to prevent the blocks go outside screen
        {
            block[i].length = ((rand() % 8) + 1);
            //calculate the end position of the block
            block[i].endBlock_X = block[i-1].positionX + block[i-1].length*0.5f;
            block[i].endBlock_Z = block[i-1].positionZ;
        }
        while ((block[i].endBlock_Z-block[i].endBlock_X)*0.5f < -7);

        //new block centre
        block[i].positionX = block[i-1].positionX + block[i-1].length*0.5f - 0.5f;
        block[i].positionZ = block[i-1].positionZ + block[i].length*0.5f + 0.5f;
    }

}

void Path::calBlocks()
{
     //create extra memory space, just in case
    int i=0;

    //set the first block[0]
    block[0].length = 4;
    block[0].positionX = 1.0f;
    block[0].positionZ = 0.0f;
    block[0].turn_Right = false;

    for (i=1; i<num_of_Blocks; ++i )
    {
        placeBlock(i);
    }


}



int Path::updateBlock(int index) //update 1 block after m_updateBlockTime
{
    int last_Block_Index = 0;
    int i =0;

    for (i=index; i<num_of_Blocks; ++i)
    {
        if ((block[i].endBlock_X < -13) && (block[i].endBlock_Z < -13)) //block goes out of screen
        {
            if (i==0)
            {
                placeBlock(num_of_Blocks); //if the first block, reallocate it based on the last block
                block[0]=block[num_of_Blocks]; //copy the new block to the first one
            }
            else placeBlock(i);
            last_Block_Index=i+1;
        }
        else
        {
            last_Block_Index = i;
            break;
        }
    }

    if (last_Block_Index==num_of_Blocks) return 0;

    return last_Block_Index;
}


int Path::ballDetect(float ball_X, float ball_Z, int old_inside_Block, int &score)
{
    int inside_Block = -1; //-1 means the ball falls out of the map
    int i=0;
    float block_X=0, block_Z=0;

    for (i=0; i<num_of_Blocks; ++i)
    {
        block_X = block[i].positionX;
        block_Z = block[i].positionZ;

        if (block[i].turn_Right)
        {
            if ((ball_X>=block_X-0.5) && ((ball_X<=block_X+0.5)) && (ball_Z>=block_Z-block[i].length*0.5) && (ball_Z<=block_Z+block[i].length*0.5))
            {
                inside_Block=i;
                break;
            }
            else inside_Block=-1;
        }
        else
        {
            if ((ball_Z>=block_Z-0.5) && ((ball_Z<=block_Z+0.5)) && (ball_X>=block_X-block[i].length*0.5) && (ball_X<=block_X+block[i].length*0.5))
            {
                inside_Block=i;
                break;
            }
            else inside_Block=-1;
        }
    }

    if (inside_Block!=old_inside_Block)
    {
        score+=1;
    }

    return inside_Block;

}
