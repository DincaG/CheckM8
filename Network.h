#pragma once

#include <curl/curl.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <nlohmann/json.hpp>
#include <sstream>
namespace JSON = ::nlohmann;

// Creates a game instance
class GameInstance
{
	std::string urlHash;
	std::string password;
	std::string passwordHash;
	std::string endPoint;
	std::string plainText;
	JSON::json sessionObj;
	int dbVersion{ 0 };

	std::string getWritePermissionProof(std::string content);

public:
	GameInstance(std::string urlId, std::string pswd);

	// Loads the contents of the game session
	// Returns false if the instance hasn't been created, true otherwise
	bool load();

	// Updates the game session
	// It creates the instance if it's not already created
	// Returns true if successful, false otherwise
	bool save(std::string textToSave);

	// Returns the contents of the session
	std::string view();
};