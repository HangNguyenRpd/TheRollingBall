# CGAA Assignment

__Assignment for Hang Nguyen__

ABSTRACT

Rolling Game is based on a simple 2D smartphone game called ZigZag, developed by Ketchapp. The task involves tapping on the screen to signal the ball moving
on tall narrow blocks, while collecting "diamonds" along the way.  The key goal is to keep the ball not falling off the blocks and gather as many diamonds
as possible.

In this project, I approach the game in a different manner. The original smartphone ZigZag has its scene made up of several 2D blocks that look like 3D,
but the Rolling Game would be a fully 3D environment developed in OpenGl pipeline.  Therefore, even though this game operates the basic same tasks that ZigZag
does, it will look for a completely distinctive set of algorithms.

# RESEARCH AND GAME DESIGN

To be able to set up a 3D world for our rolling ball, we look at the implementation of OpenGl pipeline in C++, and the methods used in making the ZigZag game.


    a) Building up Blocks
    The most challenging task in building this game is generating those tall, narrow blocks that are:
        - Random in length, but should be within the screen space
        - Consecutively, perpendicularly put next to each other
        - Never-ending path that keeps going until the ball falls off the map

    b) Make the Rolling Ball move
    The ball turns left/right when receiving signals from the player

    c) Keep the scene within the camera/screen space

# ALGORITHMS

To tackle the design proposed in section 1, the idea is to hold the camera in one angle, keep the ball moving along a constant line, then move the blocks backwards.
In this way, it create the illusion of the ball rolling forwards on those blocks. Although the ball and the blocks are 3D artifacts, these objects only slides on a
2D plane.

For convenience in writing the program, I choose the XZ-plane as our platform, with the Z axis on the right, and X axis on the left. From this view, to move forward
and turn right means moving along the Z axis in positive direction, while it is X axis for turning left.

    a) Path class
    Making use of the object-oriented nature of C++, we view the consecutive blocks that the ball rolls on as an object called path, exhibiting the behaviors
    described in 1a.

    With NCCA Graphics Library (NCCA), we can easily draw a cube and position it in the scene with a few lines of code. Instead of endlessly create and destroy the
    blocks as the game runs, the Path class would contain an array of numerous cubes (in this application, it is 20), each cube has the information on how to "draw"
    that cube on the screen: does it turn left or right, where it is positioned and how long it is.


