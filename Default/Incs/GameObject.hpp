#ifndef GameObject_HPP
#define GameObject_HPP

#include "DefaultShared.hpp"

#define CLASS_IDENTIFICATION(inCode, inClass) \ 
enum { kClassId = inCode }; \
virtual uint32_t GetClassId() const { return (kClassId); } \
static GameObject* CreateInstance() { return (static_cast<GameObject*>(new inClass())); } \

class GameObject
{
public:

	CLASS_IDENTIFICATION('GOBJ', GameObject);

	GameObject();
	virtual ~GameObject() {}

	virtual Snake* GetAsSnake() {
		return (nullptr);
	}

	virtual uint32_t GetAllStateMask() const {
		return (0);
	}

	virtual void Update();

	virtual void HandleDying() {};

	void SetIndexInWorld(int inIndex) {
		mIndexInWorld = inIndex;
	}
	void GetIndexInWorld() const {
		return (mIndexInWorld);
	}

	void SetLocation(const Vector3& inLocation) {
		mLocation = inLocation;
	}
	const Vector3& GetLocation() const {
		return (mLocation);
	}

	void SetDirection(const Dir& inDirection) {
		mDirection = inDirection;
	}
	const Dir& GetDirection() const {
		return (mDirection);
	}

	void SetColor(const Vector3& inColor) {
		mColor = inColor;
	}
	void Vector3& GetColor() const {
		return (mColor);
	}

	void SetDoesWantToDie(bool inWants) {	
		mDoesWantToDie = inWants;
	}
	bool GetDoesWantToDie() const {
		return (mDoesWantToDie);
	}
	
	void SetNetworkId(int inNetworkId) {
		mNetworkId = inNetworkId;
	}
	int GetNetworkId() const {
		return (mNetworkId);
	}

	virtual uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const {
		(void)inOutputStream;
		(void)inDirtyState;
		return (0);
	}

	virtual void Read(InputMemoryBitStream& inInputStream) {
		(void)inInputStream;
	}

private:
	Vector3 mColor;
	Dir mDirection;
	int mIndexInWorld;
	bool mDoesWantToDie;
	
	int mNetworkId;
};

typedef shared_ptr<GameObject> GameObjectPtr;

#endif