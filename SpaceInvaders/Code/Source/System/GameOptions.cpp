#include <assert.h>
#include <System\GameOptions.h>
#include <System\Assert.h>
#include <fstream>
#include <regex>

const std::regex section_pattern("\\[(.*)\\]");
const std::regex value_pattern("(\\S+)\\s*=\\s*(\\S+)");
using namespace GameSystem;

/// <summary>
/// Initializes a new instance of the <see cref="GameOptions"/> class. Using RAII it loads the contents of the file
/// </summary>
/// <param name="path">The path.</param>
GameOptions::GameOptions(const char * path) 
{
	std::ifstream in(path);
	ASSERT_DESCRIPTION(in, "Game Options file not read properly");
	m_StrStream << in.rdbuf();;
	in.close();
}

/// <summary>
/// Finalizes an instance of the <see cref="GameOptions"/> class.
/// </summary>
GameOptions::~GameOptions()
{

}

/// <summary>
/// Initializes this instance with the data loaded in the constructor
/// </summary>
void GameOptions::Init()
{
	std::string currentSection;
	m_StrStream.clear();
	m_StrStream.seekg(0, std::ios::beg);
	std::string line;
	while (std::getline(m_StrStream, line))
	{
		std::smatch sectionMatch;
		if (regex_match(line, sectionMatch, section_pattern))
		{
			//TODO Reenable the assert... something happens when trying to include it
			//ASSERT_DESCRIPTION(result.find(sectionMatch[1]) == result.end(), "Section Key already exists inside the INI File please, please double check contents");
			currentSection = sectionMatch[1];
			IniValuesMap temp;
			m_KeyValues[currentSection] = temp;

		}
		else
		{
			std::smatch valueMatch;
			if (regex_match(line, valueMatch, value_pattern))
			{
				//TODO Reenable the assert... something happens when trying to include it
				//ASSERT_DESCRIPTION(result[currentSection].find(valueMatch[1]) == result[currentSection].end(), "Section Key already exists inside the INI File please, please double check contents");
				m_KeyValues[currentSection][valueMatch[1]] = valueMatch[2];
			}
		}
	}
	return;

}

/// <summary>
/// Gets the values for a given key.
/// </summary>
/// <param name="key">The key.</param>
/// <returns></returns>
const IniValuesMap &GameOptions::GetValuesForAGivenKey(const std::string &key)
{
	auto result=m_KeyValues.find(key);

	//TODO como la funcion recibe cadenas que pueden estar rotas... Es adecuado crear un tipo IniValuesMap con Invalid para que el cliente cheque si era v�lido o no.
	//TODO Fix ASSERTS are not found
	//ASSERT_DESCRIPTION(result == m_KeyValues.end(), "Error Section Key Not Found");
	return result->second;
}


/// <summary>
/// Initializes a new instance of the <see cref="GameOptions"/> class. Using RAII it loads the contents of the file
/// </summary>
/// <param name="path">The path.</param>
GameOptionsJSON::GameOptionsJSON(const char * path)
{
	std::ifstream in(path);
	ASSERT_DESCRIPTION(in, "Game JSON Options file not read properly");
	m_StrStream << in.rdbuf();;
	in.close();

	m_FilePath=path;
}

/// <summary>
/// Finalizes an instance of the <see cref="GameOptions"/> class.
/// </summary>
GameOptionsJSON::~GameOptionsJSON()
{

}

/// <summary>
/// Initializes this instance with the data loaded in the constructor
/// </summary>
void GameOptionsJSON::Init()
{
	ASSERT_DESCRIPTION(!m_Document.Parse<0>(m_StrStream.str().c_str()).HasParseError(), m_FilePath + "A problem has happened parsing the input file");
	return;

}
