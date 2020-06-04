
#include "pose.h"
#include <math.h>

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

    return result;
}
