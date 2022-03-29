#include "BasicTools.h"

void genNameUsingIndex(const Ogre::String & prefix, int index, Ogre::String &out_name)
{
	out_name= prefix + Ogre::StringConverter::toString(static_cast<int>(index));
}

void resolveCollision(
    SceneNode *nodeA, SceneNode *nodeB,
    float rA, float rB, float wA, float wB)
{
    Vector3 posA = nodeA->getPosition();
    Vector3 posB = nodeB->getPosition();
    float R = rA + rB;
	float W = wA + wB;
	Vector3 movingVec(posB[0]-posA[0],0,posB[2]-posA[2]);
	if(movingVec.isZeroLength())	movingVec = Vector3::UNIT_X;
    if(movingVec.length() < R){
		float distanceA = wB / W;
		float distanceB = wA / W;
		movingVec.normalise();
		Vector3 destA = distanceA * -movingVec;
		Vector3 destB = distanceB * movingVec;
		nodeA->setPosition(posA + destA);
		nodeB->setPosition(posB + destB);
	}
}