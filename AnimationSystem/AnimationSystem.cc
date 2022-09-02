#include "AnimationSystem.h"

namespace AnimationSystem
{
  // std::map<std::string, Interpolant *> interpolants;
  // std::vector<Interpolant> _interpolants;
  std::vector<AnimationMixer *> _animationMixers;
  std::vector<AnimationMapping> _animationMappings;
  // std::vector<Animation *> _animations;
  std::map<std::string, Animation *> animationo;
  // float finishedFlag = 0;
  // std::vector<float> _finishedFlags;
  // float _finishedFlags[100];
  // float finishedAnimationIndex; // todo: use pointer instead of index.
  // float **animationValues = (float **)malloc(53 * sizeof(float)); // ok too
  // Interpolant _interpolant;

  float *localQuaternion = (float *)malloc(5 * sizeof(float));

  Animation testAnimation = Animation();
  Animation *testAnimation2 = &testAnimation;
  Animation **testAnimation3 = &testAnimation2;

  float identityQuaternion[4] = {0, 0, 0, 1};

  Avatar avatar; // todo: support multi avatars

  // functions:

  // Utils ------

  float min(float a, float b)
  {
    if (a > b)
    {
      return b;
    }
    else
    {
      return a;
    }
  }
  float max(float a, float b)
  {
    if (a > b)
    {
      return a;
    }
    else
    {
      return b;
    }
  }

  // Main ------

  // float setWeight(unsigned int animationIndex, float weight)
  // {
  //   // // no effect if _animations not store pointer.
  //   // Animation animation = _animations[animationIndex];
  //   // Animation animation = rootNode.children[animationIndex];
  //   // animation.weight = weight;

  //   rootNode->children[animationIndex]->weight = weight; // todo: test: animationIndex is 0 | 1 here, not real animationIndex.
  //   return weight;
  // }

  void setWeight(AnimationNode *node, float weight)
  {
    node->weight = weight;
  }

  void setFactor(AnimationNode *node, float factor)
  {
    node->factor = factor;
  }

  void setArg(AnimationNode *node, float arg)
  {
    node->arg = arg;
  }

  float getWeight(AnimationSystem::AnimationNode *node)
  {
    return node->weight;
  }

  float getFactor(AnimationSystem::AnimationNode *node)
  {
    return node->factor;
  }

  unsigned int getChildren(AnimationNode *node, AnimationNode **scratchStack) {
    unsigned int count = node->children.size();
    for (unsigned int i = 0; i < count; i++) {
      scratchStack[i] = node->children[i];
    }
    return count;
  }

  void interpolateFlat(float *dst, unsigned int dstOffset, float *src0, unsigned int srcOffset0, float *src1, unsigned int srcOffset1, float t, bool isPosition)
  {
    if (isPosition)
    {
      lerpFlat(dst, dstOffset, src0, srcOffset0, src1, srcOffset1, t);
    }
    else
    {
      slerpFlat(dst, dstOffset, src0, srcOffset0, src1, srcOffset1, t);
    }
  }

  void initAvatar(AnimationMixer *mixer) {
    // avatar.mixer = createAnimationMixer();
    avatar.mixer = mixer;

    // create motion

    avatar.motiono["idle"] = avatar.mixer->createMotion(animationo["idle.fbx"]);

    avatar.motiono["walkForward"] = avatar.mixer->createMotion(animationo["walking.fbx"]);
    avatar.motiono["walkBackward"] = avatar.mixer->createMotion(animationo["walking backwards.fbx"]);
    avatar.motiono["walkLeft"] = avatar.mixer->createMotion(animationo["left strafe walking.fbx"]);
    avatar.motiono["walkRight"] = avatar.mixer->createMotion(animationo["right strafe walking.fbx"]);
    avatar.motiono["walkLeftMirror"] = avatar.mixer->createMotion(animationo["right strafe walking reverse.fbx"]);
    avatar.motiono["walkRightMirror"] = avatar.mixer->createMotion(animationo["left strafe walking reverse.fbx"]);

    avatar.motiono["runForward"] = avatar.mixer->createMotion(animationo["Fast Run.fbx"]);
    avatar.motiono["runBackward"] = avatar.mixer->createMotion(animationo["running backwards.fbx"]);
    avatar.motiono["runLeft"] = avatar.mixer->createMotion(animationo["left strafe.fbx"]);
    avatar.motiono["runRight"] = avatar.mixer->createMotion(animationo["right strafe.fbx"]);
    avatar.motiono["runLeftMirror"] = avatar.mixer->createMotion(animationo["right strafe reverse.fbx"]);
    avatar.motiono["runRightMirror"] = avatar.mixer->createMotion(animationo["left strafe reverse.fbx"]);

    avatar.motiono["crouchForward"] = avatar.mixer->createMotion(animationo["Sneaking Forward.fbx"]);
    avatar.motiono["crouchBackward"] = avatar.mixer->createMotion(animationo["Sneaking Forward reverse.fbx"]);
    avatar.motiono["crouchLeft"] = avatar.mixer->createMotion(animationo["Crouched Sneaking Left.fbx"]);
    avatar.motiono["crouchRight"] = avatar.mixer->createMotion(animationo["Crouched Sneaking Right.fbx"]);
    avatar.motiono["crouchLeftMirror"] = avatar.mixer->createMotion(animationo["Crouched Sneaking Right reverse.fbx"]);
    avatar.motiono["crouchRightMirror"] = avatar.mixer->createMotion(animationo["Crouched Sneaking Left reverse.fbx"]);

    avatar.motiono["bowForward"] = avatar.mixer->createMotion(animationo["Standing Aim Walk Forward.fbx"]);
    avatar.motiono["bowBackward"] = avatar.mixer->createMotion(animationo["Standing Aim Walk Forward reverse.fbx"]);
    avatar.motiono["bowLeft"] = avatar.mixer->createMotion(animationo["Standing Aim Walk Left.fbx"]);
    avatar.motiono["bowRight"] = avatar.mixer->createMotion(animationo["Standing Aim Walk Right.fbx"]);
    avatar.motiono["bowLeftMirror"] = avatar.mixer->createMotion(animationo["Standing Aim Walk Right reverse.fbx"]);
    avatar.motiono["bowRightMirror"] = avatar.mixer->createMotion(animationo["Standing Aim Walk Left reverse.fbx"]);

    avatar.motiono["crouchIdle"] = avatar.mixer->createMotion(animationo["Crouch Idle.fbx"]);
    avatar.motiono["fly"] = avatar.mixer->createMotion(animationo["treading water.fbx"]);
    avatar.motiono["flyIdle"] = avatar.mixer->createMotion(animationo["fly_idle.fbx"]);
    avatar.motiono["flyDodgeForward"] = avatar.mixer->createMotion(animationo["fly_dodge_forward.fbx"]);
    avatar.motiono["flyDodgeBackward"] = avatar.mixer->createMotion(animationo["fly_dodge_backward.fbx"]);
    avatar.motiono["flyDodgeLeft"] = avatar.mixer->createMotion(animationo["fly_dodge_left.fbx"]);
    avatar.motiono["flyDodgeRight"] = avatar.mixer->createMotion(animationo["fly_dodge_right.fbx"]);
    avatar.motiono["flyDash"] = avatar.mixer->createMotion(animationo["fly_dash_forward.fbx"]);
    avatar.motiono["narutoRun"] = avatar.mixer->createMotion(animationo["naruto run.fbx"]);

    // create node
    avatar.nodeo["_8DirectionsWalkNodeList"] = avatar.mixer->createNode(NodeType::LIST);
    addChild(avatar.nodeo["_8DirectionsWalkNodeList"], avatar.motiono["walkForward"]);
    addChild(avatar.nodeo["_8DirectionsWalkNodeList"], avatar.motiono["walkBackward"]);
    addChild(avatar.nodeo["_8DirectionsWalkNodeList"], avatar.motiono["walkLeft"]);
    addChild(avatar.nodeo["_8DirectionsWalkNodeList"], avatar.motiono["walkRight"]);
    addChild(avatar.nodeo["_8DirectionsWalkNodeList"], avatar.motiono["walkLeftMirror"]);
    addChild(avatar.nodeo["_8DirectionsWalkNodeList"], avatar.motiono["walkRightMirror"]);

    avatar.nodeo["_8DirectionsRunNodeList"] = avatar.mixer->createNode(NodeType::LIST);
    addChild(avatar.nodeo["_8DirectionsRunNodeList"], avatar.motiono["runForward"]);
    addChild(avatar.nodeo["_8DirectionsRunNodeList"], avatar.motiono["runBackward"]);
    addChild(avatar.nodeo["_8DirectionsRunNodeList"], avatar.motiono["runLeft"]);
    addChild(avatar.nodeo["_8DirectionsRunNodeList"], avatar.motiono["runRight"]);
    addChild(avatar.nodeo["_8DirectionsRunNodeList"], avatar.motiono["runLeftMirror"]);
    addChild(avatar.nodeo["_8DirectionsRunNodeList"], avatar.motiono["runRightMirror"]);

    avatar.nodeo["_8DirectionsCrouchNodeList"] = avatar.mixer->createNode(NodeType::LIST);
    addChild(avatar.nodeo["_8DirectionsCrouchNodeList"], avatar.motiono["crouchForward"]);
    addChild(avatar.nodeo["_8DirectionsCrouchNodeList"], avatar.motiono["crouchBackward"]);
    addChild(avatar.nodeo["_8DirectionsCrouchNodeList"], avatar.motiono["crouchLeft"]);
    addChild(avatar.nodeo["_8DirectionsCrouchNodeList"], avatar.motiono["crouchRight"]);
    addChild(avatar.nodeo["_8DirectionsCrouchNodeList"], avatar.motiono["crouchLeftMirror"]);
    addChild(avatar.nodeo["_8DirectionsCrouchNodeList"], avatar.motiono["crouchRightMirror"]);

    avatar.nodeo["_8DirectionsBowNodeList"] = avatar.mixer->createNode(NodeType::LIST);
    addChild(avatar.nodeo["_8DirectionsBowNodeList"], avatar.motiono["bowForward"]);
    addChild(avatar.nodeo["_8DirectionsBowNodeList"], avatar.motiono["bowBackward"]);
    addChild(avatar.nodeo["_8DirectionsBowNodeList"], avatar.motiono["bowLeft"]);
    addChild(avatar.nodeo["_8DirectionsBowNodeList"], avatar.motiono["bowRight"]);
    addChild(avatar.nodeo["_8DirectionsBowNodeList"], avatar.motiono["bowLeftMirror"]);
    addChild(avatar.nodeo["_8DirectionsBowNodeList"], avatar.motiono["bowRightMirror"]);

    avatar.nodeo["_8DirectionsWalkRunNodeTwo"] = avatar.mixer->createNode(NodeType::TWO);
    addChild(avatar.nodeo["_8DirectionsWalkRunNodeTwo"], avatar.nodeo["_8DirectionsWalkNodeList"]);
    addChild(avatar.nodeo["_8DirectionsWalkRunNodeTwo"], avatar.nodeo["_8DirectionsRunNodeList"]);

    avatar.nodeo["idle8DWalkRunNodeTwo"] = avatar.mixer->createNode(NodeType::TWO);
    addChild(avatar.nodeo["idle8DWalkRunNodeTwo"], avatar.motiono["idle"]);
    addChild(avatar.nodeo["idle8DWalkRunNodeTwo"], avatar.nodeo["_8DirectionsWalkRunNodeTwo"]);

    avatar.nodeo["idle8DCrouchNodeTwo"] = avatar.mixer->createNode(NodeType::TWO);
    addChild(avatar.nodeo["idle8DCrouchNodeTwo"], avatar.motiono["crouchIdle"]);
    addChild(avatar.nodeo["idle8DCrouchNodeTwo"], avatar.nodeo["_8DirectionsCrouchNodeList"]);

    avatar.nodeo["flyForwardNodeTwo"] = avatar.mixer->createNode(NodeType::TWO);
    addChild(avatar.nodeo["flyForwardNodeTwo"], avatar.motiono["flyDodgeForward"]);
    addChild(avatar.nodeo["flyForwardNodeTwo"], avatar.motiono["flyDash"]);

    avatar.nodeo["_8DirectionsFlyNodeList"] = avatar.mixer->createNode(NodeType::LIST);
    addChild(avatar.nodeo["_8DirectionsFlyNodeList"], avatar.nodeo["flyForwardNodeTwo"]);
    addChild(avatar.nodeo["_8DirectionsFlyNodeList"], avatar.motiono["flyDodgeBackward"]);
    addChild(avatar.nodeo["_8DirectionsFlyNodeList"], avatar.motiono["flyDodgeLeft"]);
    addChild(avatar.nodeo["_8DirectionsFlyNodeList"], avatar.motiono["flyDodgeRight"]);

    avatar.nodeo["idle8DFlyNodeTwo"] = avatar.mixer->createNode(NodeType::TWO);
    addChild(avatar.nodeo["idle8DFlyNodeTwo"], avatar.motiono["flyIdle"]);
    addChild(avatar.nodeo["idle8DFlyNodeTwo"], avatar.nodeo["_8DirectionsFlyNodeList"]);
  }
  void updateAvatar(float *scratchStack)
  {
    // std::cout << "idleWalkFactor: " << scratchStack[index] << std::endl;

    unsigned int index = 0;
    setWeight(avatar.motiono["walkForward"], scratchStack[index++]); // forwardFactor,
    setWeight(avatar.motiono["walkBackward"], scratchStack[index++]); // backwardFactor,
    setWeight(avatar.motiono["walkLeft"], scratchStack[index++]); // mirrorLeftFactorReverse,
    setWeight(avatar.motiono["walkLeftMirror"], scratchStack[index++]); // mirrorLeftFactor,
    setWeight(avatar.motiono["walkRight"], scratchStack[index++]); // mirrorRightFactorReverse,
    setWeight(avatar.motiono["walkRightMirror"], scratchStack[index++]); // mirrorRightFactor,

    setWeight(avatar.motiono["runForward"], scratchStack[index++]); // forwardFactor,
    setWeight(avatar.motiono["runBackward"], scratchStack[index++]); // backwardFactor,
    setWeight(avatar.motiono["runLeft"], scratchStack[index++]); // mirrorLeftFactorReverse,
    setWeight(avatar.motiono["runLeftMirror"], scratchStack[index++]); // mirrorLeftFactor,
    setWeight(avatar.motiono["runRight"], scratchStack[index++]); // mirrorRightFactorReverse,
    setWeight(avatar.motiono["runRightMirror"], scratchStack[index++]); // mirrorRightFactor,

    setWeight(avatar.motiono["crouchForward"], scratchStack[index++]); // forwardFactor,
    setWeight(avatar.motiono["crouchBackward"], scratchStack[index++]); // backwardFactor,
    setWeight(avatar.motiono["crouchLeft"], scratchStack[index++]); // mirrorLeftFactorReverse,
    setWeight(avatar.motiono["crouchLeftMirror"], scratchStack[index++]); // mirrorLeftFactor,
    setWeight(avatar.motiono["crouchRight"], scratchStack[index++]); // mirrorRightFactorReverse,
    setWeight(avatar.motiono["crouchRightMirror"], scratchStack[index++]); // mirrorRightFactor,

    setWeight(avatar.motiono["bowForward"], scratchStack[index++]); // forwardFactor,
    setWeight(avatar.motiono["bowBackward"], scratchStack[index++]); // backwardFactor,
    setWeight(avatar.motiono["bowLeft"], scratchStack[index++]); // mirrorLeftFactorReverse,
    setWeight(avatar.motiono["bowLeftMirror"], scratchStack[index++]); // mirrorLeftFactor,
    setWeight(avatar.motiono["bowRight"], scratchStack[index++]); // mirrorRightFactorReverse,
    setWeight(avatar.motiono["bowRightMirror"], scratchStack[index++]); // mirrorRightFactor,

    // setFactor(avatar.nodeo["_8DirectionsWalkNodeList"], scratchStack[index++]); // avatar.walkRunFactor,
    // setFactor(avatar.nodeo["idle8DWalkRunNodeTwo"], scratchStack[index++]); // avatar.idleWalkFactor,
    // setFactor(avatar.nodeo["idle8DCrouchNodeTwo"], scratchStack[index++]); // avatar.idleWalkFactor,
    // setFactor(avatar.nodeo["default"], scratchStack[index++]); // avatar.crouchFactor,
    // setFactor(avatar.nodeo["idle8DBow"], scratchStack[index++]); // avatar.idleWalkFactor,

    // setWeight(avatar.nodeo["flyForwardNodeTwo"], scratchStack[index++]); // forwardFactor,
    // setWeight(avatar.motiono["flyDodgeBackward"], scratchStack[index++]); // backwardFactor,
    // setWeight(avatar.motiono["flyDodgeLeft"], scratchStack[index++]); // leftFactor,
    // setWeight(avatar.motiono["flyDodgeRight"], scratchStack[index++]); // rightFactor,

    // setFactor(avatar.nodeo["idle8DFly"], scratchStack[index++]); // avatar.walkRunFactor,
    // setFactor(avatar.nodeo["flyForwardNodeTwo"], scratchStack[index++]); // avatar.flyDashFactor,

    // setArg(avatar.nodeo["hold"], scratchStack[index++]); // holdFactor,
    // setArg(avatar.nodeo["emote"], scratchStack[index++]); // avatar.idleWalkFactor,
  }
  AnimationMixer *createAnimationMixer()
  {
    AnimationMixer *animationMixer = new AnimationMixer();
    // animationMixer->index = _animationMixers.size();
    _animationMixers.push_back(animationMixer);
    // _finishedFlags[animationMixer->index] = 0;
    // return &animationMixer; // todo: warning: address of stack memory associated with local variable 'animationMixer' returned [-Wreturn-stack-address]
    return animationMixer;

    // // init old
    // AnimationNode *walkFlyNode = new AnimationNode();
    // walkFlyNode->children.push_back(_motions[96]); // 96 walk
    // walkFlyNode->children.push_back(_motions[92]); // 92 fly

    // AnimationNode *crouchNode = new AnimationNode();
    // crouchNode->children.push_back(walkFlyNode);
    // crouchNode->children.push_back(_motions[9]); // 9 Crouch Idle.fbx

    // rootNode = crouchNode;

    // // init
    // AnimationNode *walkMotion = createMotion(_animations[96]);
    // AnimationNode *flyMotion = createMotion(_animations[92]);
    // AnimationNode *crouchMotion = createMotion(_animations[9]);

    // AnimationNode *walkFlyNode = createNode();
    // addChild(walkFlyNode, walkMotion);
    // addChild(walkFlyNode, flyMotion);

    // AnimationNode *crouchNode = createNode();
    // addChild(crouchNode, walkFlyNode);
    // addChild(crouchNode, crouchMotion);

    // setRootNode(crouchNode);
  }
  void addChild(AnimationNode *parent, AnimationNode *child)
  {
    parent->children.push_back(child);

    if (parent->type == NodeType::SOLITARY)
    {
      if (parent->children.size() == 1)
      {
        parent->activeNode = child;
        child->weight = 1;
      }
      else
      {
        child->weight = 0;
      }
    }
  }
  void setRootNode(AnimationMixer *mixer, AnimationNode *node)
  {
    mixer->rootNode = node;
  }
  void createAnimationMapping(bool isPosition, unsigned int index, bool isFirstBone, bool isLastBone, bool isTop, bool isArm)
  {
    AnimationMapping animationMapping;
    animationMapping.isPosition = isPosition;
    animationMapping.index = index;
    animationMapping.isFirstBone = isFirstBone;
    animationMapping.isLastBone = isLastBone;
    animationMapping.isTop = isTop;
    animationMapping.isArm = isArm;
    _animationMappings.push_back(animationMapping);
    // std::cout << "_animationMappings size: " << _animationMappings.size() << std::endl;
  }
  Animation *createAnimation(char *scratchStack, unsigned int nameByteLength, float duration)
  {
    Animation *animation = new Animation();
    animation->duration = duration;
    // std::cout << "nameByteLength: " << nameByteLength << std::endl;
    // for (unsigned int i = 0; i < nameByteLength; i++)
    // {
    //   std::cout << scratchStack[i] << std::endl;
    // }
    // https://www.geeksforgeeks.org/convert-character-array-to-string-in-c/
    std::string name = "";
    for (unsigned int i = 0; i < nameByteLength; i++)
    {
      name += scratchStack[i];
    }
    // std::cout << name << std::endl;

    animationo[name] = animation;

    return animation;
  }
  Animation *getAnimation(char *scratchStack, unsigned int nameByteLength)
  {
    std::string name = "";
    for (unsigned int i = 0; i < nameByteLength; i++)
    {
      name += scratchStack[i];
    }

    return animationo[name];
  }
  AnimationNode *AnimationMixer::createNode(NodeType type, unsigned int index)
  {
    AnimationNode *node = new AnimationNode();
    node->mixer = this;
    node->type = type;
    node->index = index;
    // std::cout << "NodeType: " << type << " " << node->type << std::endl;
    return node;
  }
  AnimationNode *AnimationMixer::createMotion(Animation *animation)
  {
    AnimationNode *motion = new AnimationNode();
    motion->mixer = this;
    motion->animation = animation;
    this->motions.push_back(motion);

    return motion;
  }
  AnimationNode *AnimationMixer::getMotion(char *scratchStack, unsigned int nameByteLength)
  {
    std::string name = "";
    for (unsigned int i = 0; i < nameByteLength; i++)
    {
      name += scratchStack[i];
    }

    return avatar.motiono[name];
  }
  AnimationNode *AnimationMixer::getNode(char *scratchStack, unsigned int nameByteLength)
  {
    std::string name = "";
    for (unsigned int i = 0; i < nameByteLength; i++)
    {
      name += scratchStack[i];
    }

    return avatar.nodeo[name];
  }
  void createInterpolant(char *scratchStack, unsigned int animationNameByteLength, unsigned int numParameterPositions, float *parameterPositions, unsigned int numSampleValues, float *sampleValues, unsigned int valueSize)
  {

    // std::cout << "createInterpolant: " << numParameterPositions << " " << numSampleValues << " " << valueSize << std::endl;

    Interpolant interpolant;
    interpolant.numParameterPositions = numParameterPositions;
    interpolant.parameterPositions = parameterPositions;
    interpolant.resultBuffer = (float *)malloc((1 + valueSize) * sizeof(float)); // 1 len for valueSize self ( 3 or 4 ). 3 len or 4 len for values.
    interpolant.numSampleValues = numSampleValues;
    interpolant.sampleValues = sampleValues;
    interpolant.valueSize = valueSize; // only support 3 (vector) or 4 (quaternion)

    std::string name = "";
    for (unsigned int i = 0; i < animationNameByteLength; i++)
    {
      name += scratchStack[i];
    }
    animationo[name]->interpolants.push_back(interpolant);

    // std::cout
    // << "interpolant "
    // << interpolant.numParameterPositions
    // << interpolant.numSampleValues
    // << interpolant.valueSize
    // << _interpolants.size()
    // << std::endl;

    // std::cout << "interpolants size: " << _interpolants.size() << std::endl;
  }
  float *evaluateInterpolant(Animation *animation, unsigned int interpolantIndex, float t)
  {
    // std::cout << "evaluateInterpolant: " << interpolantIndex << " " << t << std::endl;

    // return _sampleValues[(int)t] + _parameterPositions[(int)t] + _valueSize;

    Interpolant interpolant = animation->interpolants[interpolantIndex];

    if (interpolant.numParameterPositions == 1)
    {
      interpolant.resultBuffer[0] = interpolant.valueSize;
      interpolant.resultBuffer[1] = interpolant.sampleValues[0];
      interpolant.resultBuffer[2] = interpolant.sampleValues[1];
      interpolant.resultBuffer[3] = interpolant.sampleValues[2];
      if (interpolant.valueSize == 4)
      {
        interpolant.resultBuffer[4] = interpolant.sampleValues[3];
      }
    }
    else
    {
      int index = 0;
      // std::cout << "numParameterPositions: " << interpolant.numParameterPositions << std::endl;
      for (; index < interpolant.numParameterPositions; index++)
      {
        // std::cout << "index: " << index << " position: " << interpolant.parameterPositions[index] << std::endl;
        if (interpolant.parameterPositions[index] > t)
        {
          break;
        }
      }
      // index -= 1; // evaluate floor
      // if (interpolantIndex == 1) std::cout << "index: " << index << std::endl;
      // std::cout << "index: " << index << std::endl;

      if (index == 0)
      { // Handle situation that, parameterPositions[0] > 0, and t == 0 or t < parameterPositions[0].
        interpolant.resultBuffer[0] = interpolant.valueSize;
        interpolant.resultBuffer[1] = interpolant.sampleValues[0];
        interpolant.resultBuffer[2] = interpolant.sampleValues[1];
        interpolant.resultBuffer[3] = interpolant.sampleValues[2];
        if (interpolant.valueSize == 4)
        {
          interpolant.resultBuffer[4] = interpolant.sampleValues[3];
        }
      }
      else if (index > interpolant.numParameterPositions - 1)
      { // Handle situation that, t > max parameterPosition.
        unsigned int maxIndex = interpolant.numParameterPositions - 1;
        interpolant.resultBuffer[0] = interpolant.valueSize;
        interpolant.resultBuffer[1] = interpolant.sampleValues[maxIndex * interpolant.valueSize + 0];
        interpolant.resultBuffer[2] = interpolant.sampleValues[maxIndex * interpolant.valueSize + 1];
        interpolant.resultBuffer[3] = interpolant.sampleValues[maxIndex * interpolant.valueSize + 2];
        if (interpolant.valueSize == 4)
        {
          interpolant.resultBuffer[4] = interpolant.sampleValues[maxIndex * interpolant.valueSize + 3];
        }
      }
      else
      {
        unsigned int index0 = index - 1;
        unsigned int index1 = index;

        // if (interpolantIndex == 33) { // mixamorigRightHandThumb1.quaternion
        //   std::cout << "index: " << index << std::endl; // always 1
        // }

        // float *outputBuffer = (float *)malloc((
        //   4
        // ) * sizeof(float));

        // outputBuffer[0] = _parameterPositions[index];
        // outputBuffer[1] = _sampleValues[index];
        // outputBuffer[2] = _valueSize;

        // float x0 = interpolant.sampleValues[index0 * interpolant.valueSize + 0];
        // float y0 = interpolant.sampleValues[index0 * interpolant.valueSize + 1];
        // float z0 = interpolant.sampleValues[index0 * interpolant.valueSize + 2];
        // float w0 = interpolant.sampleValues[index0 * interpolant.valueSize + 3];

        // float x1 = interpolant.sampleValues[index1 * interpolant.valueSize + 0];
        // float y1 = interpolant.sampleValues[index1 * interpolant.valueSize + 1];
        // float z1 = interpolant.sampleValues[index1 * interpolant.valueSize + 2];
        // float w1 = interpolant.sampleValues[index1 * interpolant.valueSize + 3];

        float time0 = interpolant.parameterPositions[index0];
        float time1 = interpolant.parameterPositions[index1];
        float f = (t - time0) / (time1 - time0);

        if (interpolant.valueSize == 3)
        {
          interpolant.resultBuffer[0] = 3;
          lerpFlat(
              interpolant.resultBuffer, 1,
              interpolant.sampleValues, index0 * interpolant.valueSize,
              interpolant.sampleValues, index1 * interpolant.valueSize,
              f);
        }
        else
        {
          interpolant.resultBuffer[0] = 4;
          slerpFlat(
              interpolant.resultBuffer, 1,
              interpolant.sampleValues, index0 * interpolant.valueSize,
              interpolant.sampleValues, index1 * interpolant.valueSize,
              f);
        }

        // interpolant.resultBuffer[0] = interpolant.sampleValues[index * interpolant.valueSize + 0];
        // interpolant.resultBuffer[1] = interpolant.sampleValues[index * interpolant.valueSize + 1];
        // interpolant.resultBuffer[2] = interpolant.sampleValues[index * interpolant.valueSize + 2];
        // interpolant.resultBuffer[3] = interpolant.sampleValues[index * interpolant.valueSize + 3];

        // outputBuffer[3] = (float)index;
      }
    }

    return interpolant.resultBuffer;
  }
  // float **getAnimationValues(unsigned int animationIndex, float t)
  // {
  //   for (int i = 0; i < 53; i++)
  //   {
  //     AnimationMapping spec = _animationMappings[i];
  //     animationValues[i] = evaluateInterpolant(animationIndex, i, t);
  //   }
  //   return animationValues;
  // }
  // void crossFade(AnimationNode *parentNode, float duration, AnimationNode *targetNode)
  // {
  //   parentNode->crossFade(duration, targetNode);
  // }
  void lerpFlat(float *dst, unsigned int dstOffset, float *src0, unsigned int srcOffset0, float *src1, unsigned int srcOffset1, float t)
  {
    float x0 = src0[srcOffset0 + 0];
    float y0 = src0[srcOffset0 + 1];
    float z0 = src0[srcOffset0 + 2];

    float x1 = src1[srcOffset1 + 0];
    float y1 = src1[srcOffset1 + 1];
    float z1 = src1[srcOffset1 + 2];

    dst[dstOffset + 0] = x0 + (x1 - x0) * t;
    dst[dstOffset + 1] = y0 + (y1 - y0) * t;
    dst[dstOffset + 2] = z0 + (z1 - z0) * t;
  };
  void slerpFlat(float *dst, unsigned int dstOffset, float *src0, unsigned int srcOffset0, float *src1, unsigned int srcOffset1, float t)
  {

    // fuzz-free, array-based Quaternion SLERP operation

    float x0 = src0[srcOffset0 + 0],
          y0 = src0[srcOffset0 + 1],
          z0 = src0[srcOffset0 + 2],
          w0 = src0[srcOffset0 + 3];

    float x1 = src1[srcOffset1 + 0],
          y1 = src1[srcOffset1 + 1],
          z1 = src1[srcOffset1 + 2],
          w1 = src1[srcOffset1 + 3];

    if (t == 0)
    {

      dst[dstOffset + 0] = x0;
      dst[dstOffset + 1] = y0;
      dst[dstOffset + 2] = z0;
      dst[dstOffset + 3] = w0;
      return;
    }

    if (t == 1)
    {

      dst[dstOffset + 0] = x1;
      dst[dstOffset + 1] = y1;
      dst[dstOffset + 2] = z1;
      dst[dstOffset + 3] = w1;
      return;
    }

    if (w0 != w1 || x0 != x1 || y0 != y1 || z0 != z1)
    {

      float s = 1 - t;
      float cos = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1,
            dir = (cos >= 0 ? 1 : -1),
            sqrSin = 1 - cos * cos;

      // Skip the Slerp for tiny steps to avoid numeric problems:
      float EPSILON = 2.220446049250313e-16;
      if (sqrSin > EPSILON)
      {

        float sinVal = sqrt(sqrSin),
              len = atan2(sinVal, cos * dir);

        s = sin(s * len) / sinVal;
        t = sin(t * len) / sinVal;
      }

      float tDir = t * dir;

      x0 = x0 * s + x1 * tDir;
      y0 = y0 * s + y1 * tDir;
      z0 = z0 * s + z1 * tDir;
      w0 = w0 * s + w1 * tDir;

      // Normalize in case we just did a lerp:
      if (s == 1 - t)
      {

        float f = 1 / sqrt(x0 * x0 + y0 * y0 + z0 * z0 + w0 * w0);

        x0 *= f;
        y0 *= f;
        z0 *= f;
        w0 *= f;
      }
    }

    dst[dstOffset] = x0;
    dst[dstOffset + 1] = y0;
    dst[dstOffset + 2] = z0;
    dst[dstOffset + 3] = w0;
  }

  float **AnimationMixer::update(float timeS)
  {
    AnimationMixer::timeS = timeS;

    // reset
    // reset animation finished event.
    // finishedFlag = 0; // reset animation finished event.
    this->finishedFlag = 0;
    // *(animationValues[53]) = (float)0;
    // _finishedFlags[this->index] = 0;

    // return getAnimationValues(_animation.index, timeS); // Move `getAnimationValues()` to class AnimationMixer.

    for (int i = 0; i < 53; i++)
    {
      AnimationMapping spec = _animationMappings[i];

      // float * aaa = rootNode.update(spec);
      animationValues[i] = rootNode->update(spec);
    }

    // animationValues[53] = &finishedFlag;
    // animationValues[53] = &_finishedFlags[this->index];
    // animationValues[53] = &this->finishedFlag;
    // animationValues[54] = &finishedAnimationIndex;

    return animationValues;
  }

  float *AnimationNode::update(AnimationMapping &spec) // todo: &spec
  {
    // this->results[spec.index] = NULL; // todo: don't need ?
    for (unsigned int i = 0; i < this->children.size(); i++)
    {
      AnimationNode *childNode = this->children[i];
      childNode->results[spec.index] = NULL;
    }

    if (this->animation) // isMotion ------
    {
      float *value;
      float evaluateTimeS;
      // std::cout << "loop: " << this->loop << std::endl;
      if (this->loop == LoopType::LoopOnce)
      {
        evaluateTimeS = (AnimationMixer::timeS - this->startTime) * this->speed + this->timeBias;
        value = evaluateInterpolant(this->animation, spec.index, evaluateTimeS);
        // std::cout << "evaluateTimeS: " << evaluateTimeS << std::endl;
        // if (isLastBone && this->weight > 0 && !this->isFinished && evaluateTimeS >= this->animation.duration)
        // {
        //   // console.log('finished', this->name);
        //   this->mixer.dispatchEvent({
        //     type : 'finished',
        //     motion : this,
        //   });

        //   this->isFinished = true;
        // }
        if (/* spec.isLastBone &&  */ !this->isFinished && evaluateTimeS >= this->animation->duration) // Don't need and will cause bug if check `isLastBone`.
        {
          // std::cout << "finished: animation: index: " << this->animation->index << " pointer: " << this->animation << std::endl;
          // finishedFlag = 1;
          // _finishedFlags[this->mixer->index] = 1;
          this->mixer->finishedFlag = 1;
          // *(this->mixer->animationValues[53]) = (float)1; // todo: Must explicitly convert `1` (int) to float, otherwise will cause wrong value ?
          // finishedAnimationIndex = (float)this->animation->index; // Must explicitly convert index (unsigned int) to float, otherwise will cause wrong value.
          // animationValues[54] = this->mixer->motions
          for (int i = 0; i < this->mixer->motions.size(); i++)
          {
            AnimationNode *motion = this->mixer->motions[i];
            if (motion->animation == animation)
            {
              // std::cout << "finished: motion: pointer: " << motion << " pointer float: " << (float *)motion << std::endl;
              this->mixer->animationValues[54] = (float *)motion; // finished motion pointer
              break;
            }
          }

          this->isFinished = true;
        }
      }
      else
      {
        evaluateTimeS = fmod((AnimationMixer::timeS - this->startTime) * this->speed + this->timeBias, this->animation->duration);
        value = evaluateInterpolant(this->animation, spec.index, evaluateTimeS);
      }
      this->results[spec.index] = value;
      return value;
    }
    else // isNode ------
    {

      if (this->type == NodeType::TWO)
      {
        if (this->isCrossFade)
        {
          this->factor = (AnimationMixer::timeS - this->crossFadeStartTime) / this->crossFadeDuration;
          this->factor = min(max(this->factor, 0), 1);
          if (this->crossFadeTargetFactor == 0)
          {
            this->factor = 1 - this->factor;
          }
          if (this->factor == this->crossFadeTargetFactor)
            this->isCrossFade = false;
        }
        this->children[0]->weight = 1 - this->factor;
        this->children[1]->weight = this->factor;
      }
      else if (this->type == NodeType::SOLITARY)
      {
        if (this->isCrossFade)
        {
          float factor = (AnimationMixer::timeS - this->crossFadeStartTime) / this->crossFadeDuration;
          factor = min(max(factor, 0), 1);
          float factorReverse = 1 - factor;

          for (int i = 0; i < this->children.size(); i++)
          {
            AnimationNode *childNode = this->children[i];
            if (childNode == this->activeNode)
            {
              // childNode->weight = factor;
              childNode->weight = max(childNode->weight, factor);
            }
            else
            {                                                            // ensure solitary
              childNode->weight = min(childNode->weight, factorReverse); // todo: will cause jumpping values if last crossFade() hasn't finished.
              // childNode->weight = childNode->weightStart * factorReverse;
            }
          }
          // if (spec.isFirstBone)
          // {
          //   std::cout << "factor: " << factor << std::endl;
          // }

          if (factor == 1)
          {
            this->isCrossFade = false;
          }
        }
      }
      else if (this->type == NodeType::OVERWRITE)
      {
        if (spec.isTop)
        {
          if (this->isCrossFade)
          {
            this->factor = (AnimationMixer::timeS - this->crossFadeStartTime) / this->crossFadeDuration;
            this->factor = min(max(this->factor, 0), 1);
            if (this->crossFadeTargetFactor == 0)
            {
              this->factor = 1 - this->factor;
            }
            if (this->factor == this->crossFadeTargetFactor)
              this->isCrossFade = false;
          }
          this->children[0]->weight = 1 - this->factor;
          this->children[1]->weight = this->factor;
        }
        else
        {
          this->children[0]->weight = 1;
          this->children[1]->weight = 0;
        }
      }
      else if (this->type == NodeType::FUNC)
      {
        
        if (this->isCrossFade)
        {
          this->factor = (AnimationMixer::timeS - this->crossFadeStartTime) / this->crossFadeDuration;
          this->factor = min(max(this->factor, 0), 1);
          if (this->crossFadeTargetFactor == 0)
          {
            this->factor = 1 - this->factor;
          }
          if (this->factor == this->crossFadeTargetFactor)
            this->isCrossFade = false;
        }
        
        float *value0 = children[0]->update(spec);
        if (this->factor > 0)
        {
          float *value1 = children[1]->update(spec);

          if (this->index == 0) // hold animation
          {
            if (spec.isTop)
            {
              if (spec.index == BoneName::Left_arm || spec.index == BoneName::Right_arm)
              {
                // result = value1;
              }
              else
              {
                localQuaternion[1] = value0[1];
                localQuaternion[2] = value0[2];
                localQuaternion[3] = value0[3];
                localQuaternion[4] = value0[4];
                if (spec.isArm)
                {
                  interpolateFlat(localQuaternion, 1, localQuaternion, 1, identityQuaternion, 0, this->arg, spec.isPosition);
                }

                Quat quat0(localQuaternion[1], localQuaternion[2], localQuaternion[3], localQuaternion[4]);
                Quat quat1(value1[1], value1[2], value1[3], value1[4]);
                quat0.premultiply(quat1);
                value1[1] = quat0.x;
                value1[2] = quat0.y;
                value1[3] = quat0.z;
                value1[4] = quat0.w;
              }
            }
            else
            {
              value1[1] = value0[1];
              value1[2] = value0[2];
              value1[3] = value0[3];
              value1[4] = value0[4];
            }
          }
          else if (this->index == 1) // emote animation
          {
            if (spec.index == BoneName::Spine || spec.index == BoneName::Chest || spec.index == BoneName::UpperChest || spec.index == BoneName::Neck || spec.index == BoneName::Head) {
              if (!spec.isPosition) {
                Quat quat0(value0[1], value0[2], value0[3], value0[4]);
                Quat quat1(value1[1], value1[2], value1[3], value1[4]);
                quat0.premultiply(quat1);
                value1[1] = quat0.x;
                value1[2] = quat0.y;
                value1[3] = quat0.z;
                value1[4] = quat0.w;
              } else {
                interpolateFlat(value1, 1, value0, 1, value1, 1, this->factor, spec.isPosition);
              }
            } else {
              float f = this->factor;
              if (!spec.isTop) {
                f *= (1 - this->arg); // arg: idleWalkFactor
              }

              interpolateFlat(value1, 1, value0, 1, value1, 1, f, spec.isPosition);
            }
          }
        }

        this->children[0]->weight = 1 - this->factor;
        this->children[1]->weight = this->factor;
      }

      // doBlendList ---
      float *result;
      unsigned int nodeIndex = 0;
      float currentWeight = 0;
      for (int i = 0; i < this->children.size(); i++)
      {
        AnimationNode *childNode = this->children[i]; // todo: If not using pointer, cpp will copy node data when assign here? Yes.
        if (childNode->weight > 0)
        {
          float *value;
          if (childNode->results[spec.index])
          {
            value = childNode->results[spec.index];
            childNode->results[spec.index] = NULL;
          }
          else
          {
            value = childNode->update(spec);
          }
          if (nodeIndex == 0)
          {
            result = value;

            nodeIndex++;
            currentWeight = childNode->weight;
          }
          else
          {
            float t = childNode->weight / (currentWeight + childNode->weight);
            interpolateFlat(result, 1, result, 1, value, 1, t, spec.isPosition);

            nodeIndex++;
            currentWeight += childNode->weight;
          }
        }
      }
      this->results[spec.index] = result;
      return result;
    }
  }
  void AnimationNode::crossFadeTwo(float duration, float factor)
  {
    this->isCrossFade = true;
    this->crossFadeStartTime = AnimationMixer::timeS;
    this->crossFadeDuration = duration;
    this->crossFadeTargetFactor = factor;
  }
  void AnimationNode::crossFadeSolitary(float duration, AnimationNode *targetNode)
  {
    this->isCrossFade = true;
    this->crossFadeStartTime = AnimationMixer::timeS;
    this->crossFadeDuration = duration;
    this->activeNode = targetNode;
  }
  void AnimationNode::play()
  {
    this->weight = abs(this->weight);
    this->startTime = AnimationMixer::timeS;
    this->isFinished = false;
  }
  void AnimationNode::stop()
  {
    this->weight = -abs(this->weight);
  }
  void AnimationNode::setTimeBias(float timeBias)
  {
    this->timeBias = timeBias;
  }
  void AnimationNode::setSpeed(float speed)
  {
    this->speed = speed;
  }
  void AnimationNode::setLoop(LoopType loopType)
  {
    this->loop = loopType;
  }
}