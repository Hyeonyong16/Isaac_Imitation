#include "pch.h"
#include "CBase.h"

UINT CBase::g_NextID = 0;

CBase::CBase()
	: m_ID(g_NextID++)
{
}

CBase::CBase(const CBase& _Origin)
	: m_Name(_Origin.m_Name), m_ID(g_NextID++)
{
}

CBase::~CBase()
{
}
