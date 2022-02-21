#ifndef _PATHFINDER_H
#define _PATHFINDER_H

#include "physics.h"
#include <vector>

using namespace physx;

namespace PathFinder {
	// struct Vector3 {
	// 	float x; float y; float z;
	// };
  struct Voxel {
    Vec position;
    bool _isStart = false;
    bool _isDest = false;
    bool _isReached = false;
    float _priority = 0;
    float _costSoFar = 0;
    Voxel *_prev = NULL;
    Voxel *_next = NULL;
    Voxel *_leftVoxel = NULL;
    Voxel *_rightVoxel = NULL;
    Voxel *_btmVoxel = NULL;
    Voxel *_topVoxel = NULL;
    bool _canLeft = false;
    bool _canRight = false;
    bool _canBtm = false;
    bool _canTop = false;
    bool _isPath = false;
    bool _isFrontier = false;
  };
  void init(std::vector<PxRigidActor *> _actors, float hx, float hy, float hz, float *_position, float *_quaternion, float *_meshPosition, float *_meshQuaternion, unsigned int _maxIterDetect, unsigned int _numIgnorePhysicsIds, unsigned int *_ignorePhysicsIds);
  std::vector<Voxel *> getPath(Vec start, Vec dest);
}

#endif