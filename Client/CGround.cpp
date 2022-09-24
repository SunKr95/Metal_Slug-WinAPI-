#include "pch.h"
#include "CGround.h"

#include "CCollider.h"
#include "CRigidBody.h"

#include "CPlayer.h"
#include "CPlayer1.h"


CGround::CGround()
{
	AddComponent(new CCollider);
	GetCollider()->SetScale(Vec2(600.f, 50.f));
}

CGround::~CGround()
{
}

void CGround::tick()
{
	CObject::tick();
}

void CGround::CollisionBeginOverlap(CCollider* _pCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pCollider->GetOwner());
	if (nullptr == pPlayer || !IsValid(pPlayer))
		return;

	pPlayer->GetRigidBody()->SetGround(true);

	float fLen =  fabs(pPlayer->GetCollider()->GetFinalPos().y - GetCollider()->GetFinalPos().y);
	float fScale = (pPlayer->GetCollider()->GetScale().y / 2.f) + (GetCollider()->GetScale().y / 2.f);

	if (fLen < fScale)
	{
		Vec2 vPlayerPos = pPlayer->GetPos();
		vPlayerPos.y -= (fScale - fLen) - 1.f;
		pPlayer->SetPos(vPlayerPos);
	}
}

void CGround::CollisionOverlap(CCollider* _pCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pCollider->GetOwner());
	if (nullptr == pPlayer || !IsValid(pPlayer))
		return;

	//pPlayer->GetRigidBody()->SetGround(true);
}

void CGround::CollisionEndOverlap(CCollider* _pCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pCollider->GetOwner());
	if (nullptr == pPlayer || !IsValid(pPlayer))
		return;

	pPlayer->GetRigidBody()->SetGround(false);
}
