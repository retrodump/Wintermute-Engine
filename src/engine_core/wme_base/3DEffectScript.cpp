// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DEffectScript.h"
#include "3DEffect.h"
#include "BStringUtil.h"

#ifdef WME_D3D9


//////////////////////////////////////////////////////////////////////////
C3DEffectScript::C3DEffectScript(CBGame* inGame, C3DEffect* effect) : CBBase(inGame)
{
	m_Effect = effect;
	m_CurrentCommand = -1;
	m_RttChange = false;
}

//////////////////////////////////////////////////////////////////////////
C3DEffectScript::~C3DEffectScript(void)
{
	m_Effect = NULL; // ref only

	ClearCommands();
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectScript::InitFrame()
{
	m_CurrentCommand = -1;
	if (m_RttChange) m_Effect->StoreDefaultBuffers();
}

//////////////////////////////////////////////////////////////////////////
bool C3DEffectScript::Execute()
{
	while (true)
	{
		m_CurrentCommand++;
		if (m_CurrentCommand >= m_Commands.size())
		{
			if (m_RttChange) m_Effect->RestoreDefaultBuffers();
			return false;
		}

		ScriptCommand* cmd = m_Commands[m_CurrentCommand];
		switch (cmd->m_Type)
		{
		case ScriptCommand::CMD_DRAW_GEOMETRY:
			return true;

		case ScriptCommand::CMD_DRAW_QUAD:
			Game->m_Renderer->DrawShaderQuad();
			break;

		case ScriptCommand::CMD_SET_RENDER_COLOR_TARGET:
			if (cmd->TargetName.empty())
				m_Effect->RestoreColorBuffer();
			else
				m_Effect->SetRenderColorTarget(cmd->TargetName);
			break;

		case ScriptCommand::CMD_SET_RENDER_DEPTH_TARGET:
			if (cmd->TargetName.empty())
				m_Effect->RestoreDepthBuffer();
			else
				m_Effect->SetRenderDepthTarget(cmd->TargetName);
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffectScript::ParseString(const std::string& script)
{
	ClearCommands();

	typedef	std::vector<std::string> StringList;

	StringList lines;
	CBStringUtil::Split(script, ";", lines);

	bool hasDrawCommand = false;
	bool restoreRenderColorTarget = false;
	bool restoreRenderDepthTarget = false;

	for (StringList::iterator it = lines.begin(); it != lines.end(); it++)
	{
		StringList commands;
		CBStringUtil::Split(*it, "=", commands);

		if (commands.empty()) continue;

		std::string command = commands[0];
		std::string param = commands.size() > 1 ? commands[1] : "";

		CBStringUtil::Trim(command);
		CBStringUtil::Trim(param);

		// store known commands
		if (CBStringUtil::CompareNoCase(command, "RenderColorTarget0"))
		{
			m_Commands.push_back(new ScriptCommand(ScriptCommand::CMD_SET_RENDER_COLOR_TARGET, param));
			restoreRenderColorTarget = !param.empty();
			m_RttChange = true;
		}
		else if (CBStringUtil::CompareNoCase(command, "RenderDepthStencilTarget"))
		{
			m_Commands.push_back(new ScriptCommand(ScriptCommand::CMD_SET_RENDER_DEPTH_TARGET, param));
			restoreRenderDepthTarget = !param.empty();
			m_RttChange = true;
		}
		else if (CBStringUtil::CompareNoCase(command, "Draw"))
		{
			hasDrawCommand = true;

			if (CBStringUtil::CompareNoCase(param, "Buffer"))
			{
				m_Commands.push_back(new ScriptCommand(ScriptCommand::CMD_DRAW_QUAD));
			}
			else
			{
				m_Commands.push_back(new ScriptCommand(ScriptCommand::CMD_DRAW_GEOMETRY));
			}
		}
	}

	// add at least one draw command
	if (!hasDrawCommand)
		m_Commands.push_back(new ScriptCommand(ScriptCommand::CMD_DRAW_GEOMETRY));

	// restore render targets if necessary
	if (restoreRenderColorTarget)
		m_Commands.push_back(new ScriptCommand(ScriptCommand::CMD_SET_RENDER_COLOR_TARGET, ""));
	if (restoreRenderDepthTarget)
		m_Commands.push_back(new ScriptCommand(ScriptCommand::CMD_SET_RENDER_DEPTH_TARGET, ""));


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void C3DEffectScript::ClearCommands()
{
	CommandVector::iterator it;
	for (it = m_Commands.begin(); it != m_Commands.end(); it++)
	{
		delete (*it);
	}
	m_Commands.clear();

	m_RttChange = false;
}

#endif