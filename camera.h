//
#ifndef MAZE3D_NEW_CAMERA_H
#define MAZE3D_NEW_CAMERA_H

struct SCam{
    float x,y,z;
    float xdeg,zdeg;
};

void Camera_Apply();
void Camera_Rotation(float xAngle,float zAngle);
void Camera_MoveByMouse(int CenterX, int CenterY);

#endif //MAZE3D_NEW_CAMERA_H
