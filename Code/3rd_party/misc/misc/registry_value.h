#ifndef REGISTRY_VALUE_H
#define REGISTRY_VALUE_H

// Slightly modified because of some compile errors
// - Steffen A. Langnes

#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include <tchar.h>

namespace registry {

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_stringstream<TCHAR> tstringstream;

class registry_value
{
	public:
		registry_value(const tstring & name, HKEY base_) : key(0), base(base_)
		{
			make_path(name);
		}
		virtual ~registry_value()
		{}

		bool exists()
		{
			bool returnval = false;

			if(open(false) == true)
			{
				DWORD type, size;
				returnval = query_value(&type, &size, 0);				
				close();
			}

			return returnval;
		}

		void remove_value(void)
		{
			if(open(true) == true)
			{
				RegDeleteValue(key, valuename.c_str());				
				close();
			}
		}

		void remove_key(void)
		{
			RegDeleteKey(base, keyname.c_str());
		}

	protected:

		bool open(bool write_access)
		{
			assert(key == 0);

			if(write_access == true)
			{
				DWORD disposition;
				if(RegCreateKeyEx(base, keyname.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &key, &disposition) == ERROR_SUCCESS)
					return true;
			}

			if(RegOpenKeyEx(base, keyname.c_str(), 0, KEY_READ, &key) == ERROR_SUCCESS)
				return true;

			return false;
		}

		void close(void)
		{
			assert(key != 0);
			RegCloseKey(key);
			key = 0;
		}

		bool query_value(DWORD * type, DWORD * size, void * buffer)
		{
			assert(key != 0);
			return RegQueryValueEx(key, valuename.c_str(), 0, type, reinterpret_cast<BYTE*>(buffer), size) == ERROR_SUCCESS;
		}

		bool set_value(DWORD type, DWORD size, const void * buffer)
		{
			assert(key != 0);
			return RegSetValueEx(key, valuename.c_str(), 0, type, reinterpret_cast<const BYTE *>(buffer), size) == ERROR_SUCCESS;
		}

	private:

		void make_path(const tstring & name)
		{
			tstring::size_type pos = name.find_last_of(_T('\\'));
			assert(pos != tstring::npos);
            keyname = name.substr(0, pos);
			valuename = name.substr(pos + 1);
		}

		HKEY key;
		HKEY base;

		tstring valuename;
		tstring keyname;
};


template<class T>
class registry_string : public registry_value
{
	public:
		registry_string(const tstring & name, HKEY base) : registry_value(name, base)
		{
		}

		operator T()
		{
			tstringstream ss;
			T returnval = T();

			if(open(false) == true)
			{
				DWORD type, size;
				if(query_value(&type, &size, 0) == true)
				{
					assert(type == REG_SZ);
					char * data = new char[size];

					if(query_value(0, &size, data) == true)
					{
						ss.str(data);
						ss >> returnval;
					}

					delete [] data;
				}

				close();
			}

			return returnval;
		}

		const registry_string & operator=(const T & value)
		{
			tstringstream ss;

			if(open(true) == true)
			{
				ss << std::setprecision(20) << value;
				tstring data = ss.str();

				set_value(REG_SZ, data.length() + 1, data.c_str());
				close();
			}

            return *this;
		}
};


template<>
class registry_string<tstring> : public registry_value
{
	public:
		registry_string(const tstring & name, HKEY base) : registry_value(name, base)
		{
		}

		operator tstring()
		{
			tstring returnval;

			if(open(false) == true)
			{
				DWORD type, size;
				if(query_value(&type, &size, 0) == true)
				{
					assert(type == REG_SZ);
					TCHAR * data = new TCHAR[size];

					if(query_value(0, &size, data) == true)
						returnval = data;

					delete [] data;
				}

				close();
			}

			return returnval;
		}

		const registry_string & operator=(const tstring & value)
		{
			if(open(true) == true)
			{
				set_value(REG_SZ, value.length() + 1, value.c_str());
				close();
			}

            return *this;
		}
};


template<class T>
class registry_int : public registry_value
{
	public:
		registry_int(const tstring & name, HKEY base) : registry_value(name, base)
		{
		}

		operator T()
		{
			DWORD returnval = DWORD();

			if(open(false) == true)
			{
				DWORD type, size = sizeof(DWORD);
				if(query_value(&type, &size, &returnval) == true)
				{
					assert(type == REG_DWORD);
				}

				close();
			}

			return T(returnval);
		}

		const registry_int & operator=(const T & value)
		{
			if(open(true) == true)
			{
				DWORD data(value);
				set_value(REG_DWORD, sizeof(DWORD), &data);
				close();
			}

            return *this;
		}
};


template<class T>
class registry_binary : public registry_value
{
	public:
		registry_binary(const tstring & name, HKEY base) : registry_value(name, base)
		{
		}

		operator T()
		{
			T returnval = T();

			if(open(false) == true)
			{
				DWORD type, size = sizeof(T);
				if(query_value(&type, &size, &returnval) == true)
				{
					assert(type == REG_BINARY);
				}

				close();
			}

			return returnval;
		}

		const registry_binary & operator=(const T & value)
		{
			if(open(true) == true)
			{
				set_value(REG_BINARY, sizeof(T), &value);
				close();
			}

            return *this;
		}
};

} // registry

#endif
