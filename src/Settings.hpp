#pragma once

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




/* Verwendung */
class Settings : public Singleton <Settings>
{
	friend class Singleton <Settings>;
	public:
		~Settings () { }
		int getIntSetting (std::string name) { }
		float getFloatSetting (std::string name) { if (name=="LevelSpeed") return 20.; }
	protected:
		Settings () { }
};
