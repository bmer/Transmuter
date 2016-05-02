#pragma once

class IContentCommand
	{
	public:
		virtual int OnCommand(CString sCommand) { return -1; }
	};
