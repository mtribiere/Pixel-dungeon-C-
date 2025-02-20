#pragma once

#include "program.h"
#include "shader.h"

#include <map>

class Script:public Program{
public:
	void compile(const std::string& vertSrc, const std::string& fragSrc)
	{
		Shader temp = Shader::createCompiled(Shader::VERTEX, vertSrc);
		attach(temp);

		temp  = Shader::createCompiled(Shader::FRAGMENT, fragSrc);
		attach(temp);		

		link();
	}

	//static Script* get(const std::string& c);
	template<class T>
	static Script* get()
	{
		static const std::string name = typeid(T).name();
		if (name.compare(_curScriptClass) != 0)
		{
			Script* script = NULL;
			std::map<std::string, Script*>::iterator itr = _all.find(name);
			if (itr == _all.end())
			{
				script = new T();
				_all.insert(std::make_pair(name, script));
			}
			else
			{
				script = itr->second;
			}

			{
				if (_curScript != NULL)
				{
					_curScript->unuse();
				}
		
				_curScript = script;
				_curScriptClass = name;
				_curScript->use();			
			}
		}
		
		return _curScript;
	}

	static void reset();
	virtual void unuse() {}
protected:
	static std::map<std::string, Script*> _all;
	static Script* _curScript;
	static std::string _curScriptClass;
};