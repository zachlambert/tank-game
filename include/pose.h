#ifndef POSE_H
#define POSE_H

typedef struct {
    double x;
    double y;
    double angle;
} Pose;

Pose addPose(Pose parent, Pose child);

#endif
