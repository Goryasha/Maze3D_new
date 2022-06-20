//
#include "camera.h"
#include <gl/gl.h>
#include <windows.h>
#include <cmath>

SCam camera ={0,0,1.7,70,-40};

void Camera_Apply(){
    glRotatef(-camera.xdeg,1,0,0);
    glRotatef(-camera.zdeg,0,0,1);
    glTranslatef(-camera.x,-camera.y,-camera.z);
}

void Camera_Rotation(float xAngle,float zAngle){
    camera.zdeg+=zAngle;
    if(camera.zdeg<0) camera.zdeg+=360;
    if(camera.zdeg>360) camera.zdeg-=360;
    camera.xdeg+=xAngle;
    if(camera.xdeg<0) camera.xdeg=0;
    if(camera.xdeg>180) camera.xdeg=180;
}

void Camera_MoveByMouse(int CenterX, int CenterY){

    float angle = -camera.zdeg / 180 * M_PI ;
    float speed =0;
    float speedz =0;
    if (GetKeyState('W')<0) speed =0.1;
    if (GetKeyState(VK_SPACE)<0) speedz =0.1;
    if (GetKeyState(VK_CONTROL)<0) speedz =-0.1;
    if (GetKeyState('S')<0) speed =-0.1;
    if (GetKeyState('A')<0) {speed =0.1;angle -=M_PI*0.5;};
    if (GetKeyState('D')<0) {speed =0.1;angle +=M_PI*0.5;};
    if (speed!=0) {
        camera.x +=sin(angle)*speed;
        camera.y +=cos(angle)*speed;
    }
    if(speedz!=0){
        camera.z+=speedz;
    }

    POINT cur;
    POINT base ={CenterX,CenterY};
    GetCursorPos(&cur);
    Camera_Rotation((base.y-cur.y)/5.0,(base.x-cur.x)/5.0);
    SetCursorPos(base.x,base.y);
}