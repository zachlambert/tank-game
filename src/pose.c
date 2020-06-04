
#include "pose.h"
#include <math.h>
#include "mathDefines.h"

Pose addPose(Pose parent, Pose child)
{
    Pose result;
    // Rotate child by child angle
    result.x = child.x * cos(parent.angle)
              - child.y * sin(parent.angle);
    result.y = child.x * sin(parent.angle)
              + child.y * cos(parent.angle);
    result.angle = child.angle;
    
    // Add child to parent
    result.x += parent.x;
    result.y += parent.y;
    result.angle += parent.angle;
    if(result.angle<0){
        result.angle += 2*PI;
    }else if(result.angle>2*PI){
        result.angle -= 2*PI;
    }


    return result;
}
