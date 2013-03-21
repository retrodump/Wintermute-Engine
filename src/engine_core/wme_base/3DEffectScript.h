// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "BBase.h"
#include <vector>

#ifdef WME_D3D9


class C3DEffect;

class C3DEffectScript : public CBBase
{
public:
	C3DEffectScript(CBGame* inGame, C3DEffect* effect);
	~C3DEffectScript();

	void InitFrame();
	bool Execute();

	HRESULT ParseString(const std::string& script);


private:
	void ClearCommands();
	bool m_RttChange;

	C3DEffect* m_Effect;

	class ScriptCommand
	{
	public:
		enum CommandType
		{
			CMD_SET_RENDER_COLOR_TARGET,
			CMD_SET_RENDER_DEPTH_TARGET,
			CMD_DRAW_GEOMETRY,
			CMD_DRAW_QUAD,
		};


		ScriptCommand(CommandType type)
		{
			m_Type = type;
			TargetName = "";
		}

		ScriptCommand(CommandType type, const std::string& targetName)
		{
			m_Type = type;
			TargetName = targetName;
		}


		CommandType m_Type;
		std::string TargetName;
	};


	typedef std::vector<ScriptCommand*> CommandVector;
	CommandVector m_Commands;
	int m_CurrentCommand;

};

#endif