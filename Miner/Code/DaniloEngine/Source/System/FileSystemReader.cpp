#include <System\FileSystemReader.h>
#include <System\Assert.h>
#include <fstream>

using namespace rapidjson;
/// <summary>
/// Initializes a new instance of the <see cref="GameOptions"/> class. Using RAII it loads the contents of the file
/// </summary>
/// <param name="path">The path.</param>
FileSystemReader::FileSystemReader(const char * path)
{
	std::ifstream in(path);
	ASSERT_DESCRIPTION(in, "Game JSON Options file not read properly");
	m_StrStream << in.rdbuf();;
	in.close();

	m_FilePath = path;
}

/// <summary>
/// Finalizes an instance of the <see cref="GameOptions"/> class.
/// </summary>
FileSystemReader::~FileSystemReader()
{

}

/// <summary>
/// Initializes this instance with the data loaded in the constructor
/// </summary>
const std::stringstream & FileSystemReader::GetContents()
{
	return m_StrStream;
}


/// <summary>
/// Initializes this instance with the data loaded in the constructor
/// </summary>
JSONFileSystemParser::JSONFileSystemParser(const std::stringstream &contents)
{
	m_Document.Parse<0>(contents.str().c_str());
	ASSERT_DESCRIPTION(!m_Document.HasParseError(), "A problem has happened parsing the input file");
}

/// <summary>
/// Initializes this instance with the data loaded in the constructor
/// </summary>
void JSONFileSystemParser::Init()
{
	for (rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::CrtAllocator>::ConstMemberIterator itr = m_Document.MemberBegin(); itr != m_Document.MemberEnd(); ++itr)
	{
		ASSERT_DESCRIPTION(itr->value.IsObject()||itr->value.IsArray(), "Not supported JSON Content, we are only providing support to JSON where the root is a list of objects");
		m_NodeMap[itr->name.GetString()] = std::unique_ptr<JSONNode>(GCC_NEW JSONNode(itr->value));

		m_NodeMap[itr->name.GetString()]->Init();
	}
	return;
}

/// <summary>
/// Gets Node
/// </summary>
JSONNode* JSONFileSystemParser::GetNode(std::string key)
{
	ASSERT_DESCRIPTION(m_NodeMap.find(key) != m_NodeMap.end(), "Key not found in the JSON Node");
	JSONNode * node = m_NodeMap[key].get();
	return node;

}

/// <summary>
/// Gets the NodeList
/// </summary>
const std::vector<JSONNode*> JSONFileSystemParser::GetNodeList()
{
	std::vector<JSONNode*> arrayValues;
	for (const auto& iter : m_NodeMap)
	{
		arrayValues.push_back(iter.second.get());
	}
	return arrayValues;
}

/// <summary>
/// Initializes this instance with the data loaded in the constructor
/// </summary>
void JSONNode::Init()
{
	if (m_Contents.IsArray())
	{
		for (SizeType i = 0; i < m_Contents.Size(); i++) // rapidjson uses SizeType instead of size_t.
		{
			m_ArrayValues.push_back(std::unique_ptr<JSONNode>(GCC_NEW JSONNode(m_Contents[i])));
		}
		for (const auto& iter : m_ArrayValues)
		{
			iter->Init();
		}
	}
	else
	{
		for (rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::CrtAllocator>::ConstMemberIterator itr = m_Contents.MemberBegin(); itr != m_Contents.MemberEnd(); ++itr)
		{
			if (itr->value.IsObject() || itr->value.IsArray())
			{
				m_NodeMap[itr->name.GetString()] = std::unique_ptr<JSONNode>(GCC_NEW JSONNode(itr->value));
				m_NodeMap[itr->name.GetString()]->Init();
			}
		}
	}
	return;
}

/// <summary>
/// Gets GetArrayValues
/// </summary>
const std::vector<JSONNode*> JSONNode::GetArrayValues() 
{

	std::vector<JSONNode*> arrayValues;
	ASSERT_DESCRIPTION(m_Contents.IsArray(), "Key not found in the JSON Node");
	for (const auto& iter : m_ArrayValues)
	{
		arrayValues.push_back(iter.get());
	}
	return arrayValues;
}

/// <summary>
/// Gets ChildNode
/// </summary>
JSONNode* JSONNode::GetChild(const char* key) 
{
	ASSERT_DESCRIPTION(m_NodeMap.find(key) != m_NodeMap.end(), "Key not found in the JSON Node");
	JSONNode * node=m_NodeMap[key].get();
	return node;

}


/// <summary>
/// Gets String
/// </summary>
const char * JSONNode::GetString(const char *key) 
{
	ASSERT_DESCRIPTION(m_Contents.HasMember(key), "Key not found in the JSON Node");
	ASSERT_DESCRIPTION(m_Contents[key].IsString(), "Key not found in the JSON Node");
	return m_Contents[key].GetString();
}

/// <summary>
/// Gets Integer
/// </summary>
const int32_t JSONNode::GetInteger(const char* key) 
{
	ASSERT_DESCRIPTION(m_Contents.HasMember(key), "Key not found in the JSON Node");
	ASSERT_DESCRIPTION(m_Contents[key].IsInt(), "Key not found in the JSON Node");
	return m_Contents [key].GetInt();

}

/// <summary>
/// Gets UnsignedInteger
/// </summary>
const uint32_t JSONNode::GetUInteger(const char* key) 
{
	ASSERT_DESCRIPTION(m_Contents.HasMember(key), "Key not found in the JSON Node");
	ASSERT_DESCRIPTION(m_Contents[key].IsUint(), "Key not found in the JSON Node");
	return m_Contents[key].GetUint();

}
