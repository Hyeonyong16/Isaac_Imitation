#pragma once

class CDbgRender
{
	SINGLE(CDbgRender);
private:
	list<tDbgRenderInfo>	m_DbgInfo;
	bool					m_Show;

public:
	void AddDbgInfo(const tDbgRenderInfo& _info) 
	{ 
		if (false == m_Show)
			return;

		m_DbgInfo.push_back(_info); 
	}

	void ShowDebugRender(bool _Show) { m_Show = _Show; }

public:
	void Tick();
	void Render();

};

