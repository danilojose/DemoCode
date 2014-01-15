#include "stdafx.h"
#include "CppUnitTest.h"
#include <System\GameOptions.h>
#include <System\FileSystemReader.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace GameSystem;
using namespace rapidjson;
namespace SpaceTestProject
{		
	TEST_CLASS(JSONParserTests)
	{
	public:
		
		TEST_METHOD(JSONParser)
		{
			FileSystemReader file("D:\\GitHub\\DemoCode\\SpaceInvaders\\Data\\settings\\options.json");

			JSONFileSystemParser json(file.GetContents());

			json.Init();

			JSONNode* options=json.GetNode("options");
			JSONNode* game=options->GetChild("Game");
			const char* font = game->GetString("Font");
			const int32_t width = json.GetNode("options")->GetChild("Game")->GetInteger("Width");

		}

	};
}