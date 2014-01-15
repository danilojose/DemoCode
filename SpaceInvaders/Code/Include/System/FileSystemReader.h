#pragma once
#include <System\StdLibraries.h>
#include <rapidjson\document.h>
#include <sstream>

using namespace rapidjson;

/// <summary>
/// Class that reads a JSON file and is able to provide a  map that contains the key values from that INI file
/// </summary>
class FileSystemReader
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="GameOptions"/> class.
	/// </summary>
	/// <param name="path">The path.</param>
	explicit FileSystemReader(const char * path);
	/// <summary>
	/// Finalizes an instance of the <see cref="GameOptions"/> class.
	/// </summary>
	virtual ~FileSystemReader();
	/// <summary>
	/// returns the contents
	/// </summary>
	const std::stringstream &  GetContents();

protected:

	std::string m_FilePath;
	std::stringstream m_StrStream;
};

/// <summary>
/// Class that reads a JSON file and is able to provide a  map that contains the key values from that INI file
/// </summary>
class JSONNode
{
	friend class JSONFileSystemParser;
protected:

	JSONNode(const rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::CrtAllocator>& contents) :m_Contents(contents){}
public:
	
	/// <summary>
	/// Finalizes an instance of the <see cref="GameOptions"/> class.
	/// </summary>
	virtual ~JSONNode(){}

	/// <summary>
	/// Inits
	/// </summary>
	void Init();

	/// <summary>
	/// Gets GetArrayValues
	/// </summary>
	const std::vector<JSONNode*> GetArrayValues();

	/// <summary>
	/// Gets ChildNode
	/// </summary>
	JSONNode* GetChild(const char* key);

	/// <summary>
	/// Gets String
	/// </summary>
	const char * GetString(const char* key);

	/// <summary>
	/// Gets Integer
	/// </summary>
	const int32_t GetInteger(const char* key);

	/// <summary>
	/// Gets UnsignedInteger
	/// </summary>
	const uint32_t GetUInteger(const char* key);

private:
	const rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::CrtAllocator>& m_Contents;	// Use Crt allocator to check exception-safety (no memory leak)

	std::vector<std::unique_ptr<JSONNode>> m_ArrayValues;	

	std::map<std::string, std::unique_ptr<JSONNode>> m_NodeMap;	// Use Crt allocator to check exception-safety (no memory leak)

	JSONNode();

};
/// <summary>
/// Class that reads a JSON file and is able to provide a  map that contains the key values from that INI file
/// </summary>
class JSONFileSystemParser
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="GameOptions"/> class.
	/// </summary>
	/// <param name="contents">The contents to be parsed.</param>
	explicit JSONFileSystemParser(const std::stringstream &contents);
	/// <summary>
	/// Finalizes an instance of the <see cref="GameOptions"/> class.
	/// </summary>
	virtual ~JSONFileSystemParser(){}

	void Init();
	/// <summary>
	/// Gets Document
	/// </summary>
	JSONNode* GetNode(std::string key);

	/// <summary>
	/// Gets the NodeList
	/// </summary>
	const std::vector<JSONNode*> GetNodeList();


private:
	rapidjson::GenericDocument<UTF8<>, CrtAllocator> m_Document;	// Use Crt allocator to check exception-safety (no memory leak)

	std::map<std::string,std::unique_ptr<JSONNode>> m_NodeMap;	// Use Crt allocator to check exception-safety (no memory leak)

};

