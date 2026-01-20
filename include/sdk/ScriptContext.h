#pragma once
#include "Types.h"

#include "../core/Offsets.h"

namespace fb
{
	class ScriptContext {
	public:
		void executeString(const char* str) {
			using tScExecuteStr = void(*)(ScriptContext*, const char*, int, char*);
			auto scExecuteString = reinterpret_cast<tScExecuteStr>(offsets::gw3::fn_ScriptContext_ExecuteString);

			scExecuteString(this, str, static_cast<int>(strlen(str)), nullptr);
		}
	};
}