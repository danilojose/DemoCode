#pragma once
#include <System\StdLibraries.h>
#include <rapidjson\document.h>
#include <sstream>

using namespace rapidjson;
namespace GameSystem
{
// Typedefs and forward declarations
	typedef std::unordered_map<std::string, std::string> IniValuesMap;
	typedef std::unordered_map<std::string, IniValuesMap> IniSectionValuesMap;

	/// <summary>
	/// Class that reads a file and is able to provide a  map that contains the key values from that INI file
	/// </summary>
	class GameOptions
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="GameOptions"/> class.
		/// </summary>
		/// <param name="path">The path.</param>
		explicit GameOptions(const char * path);
		/// <summary>
		/// Finalizes an instance of the <see cref="GameOptions"/> class.
		/// </summary>
		virtual ~GameOptions();
		/// <summary>
		/// Initializes this instance with the data loaded in the constructor
		/// </summary>
		void Init();
		/// <summary>
		/// Gets the values for a given key.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <returns></returns>
		const IniValuesMap &GetValuesForAGivenKey(const std::string &key);

	private:

		const char *m_FilePath;
		std::stringstream m_StrStream;
		IniSectionValuesMap m_KeyValues;
	};

	/// <summary>
	/// Class that reads a JSON file and is able to provide a  map that contains the key values from that INI file
	/// </summary>
	class GameOptionsJSON
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="GameOptions"/> class.
		/// </summary>
		/// <param name="path">The path.</param>
		explicit GameOptionsJSON(const char * path);
		/// <summary>
		/// Finalizes an instance of the <see cref="GameOptions"/> class.
		/// </summary>
		virtual ~GameOptionsJSON();
		/// <summary>
		/// Initializes this instance with the data loaded in the constructor
		/// </summary>
		void Init();

		GenericDocument<UTF8<>, CrtAllocator> m_Document;	// Use Crt allocator to check exception-safety (no memory leak)
	private:

		std::string m_FilePath;
		std::stringstream m_StrStream;
	};

}