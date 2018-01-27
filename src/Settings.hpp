#pragma once

// from https://de.wikibooks.org/wiki/C%2B%2B-Programmierung:_Entwurfsmuster:_Singleton
template <typename C>
class Singleton
{
public:
	static C* instance ()
	{
		if (!_instance)
		_instance = new C ();
		return _instance;
	}
	virtual
	~Singleton ()
	{
		_instance = 0;
	}
private:
	static C* _instance;
protected:
	Singleton () { }
};
template <typename C> C* Singleton <C>::_instance = 0;



#include <sol.hpp>
#include <iostream>


/* Verwendung */
class Settings : public Singleton <Settings>
{
	friend class Singleton <Settings>;
	public:
		~Settings () { }
		template <typename T> T getProperty(std::string propName){return lua.get<T>(propName);}
	protected:
		Settings () { 
				lua.script_file("assets/scripts/config.lua");
				std::cout << lua.get<std::string>("name") << std::endl;
			}
	
	private:
		sol::state lua;
};
