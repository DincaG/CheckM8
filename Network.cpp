#include "Network.h"

size_t Receiver(char* chunk, size_t size, size_t nmemb, std::string* buffer)
{
	buffer->append(chunk, size * nmemb);
	return size * nmemb;
}

// Message digests (input) using sha512 hashing
std::string Sha512(const std::string& input)
{
	unsigned char hash[SHA512_DIGEST_LENGTH];
	SHA512((unsigned char*)input.c_str(), input.length(), hash);
	std::stringstream ss;
	for (int i{ 0 }; i < SHA512_DIGEST_LENGTH; i++)
	{
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	return ss.str();
}

// Encrypt AES (data) using (password)
// Specify if the encryption should be base64 encoded with (b64) flag
std::string AesEncrypt(const std::string& data, const std::string& password, bool b64)
{
	unsigned char salt[9];
	RAND_bytes(salt, 8);
	salt[8] = 0;

	unsigned char key[32];
	unsigned char iv[32];
	EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), salt, (unsigned char*)password.c_str(), password.length(), 1, key, iv);

	constexpr int AES_BLOCK_SIZE{ 16 };
	unsigned char* encrypted{ new unsigned char[data.length() + AES_BLOCK_SIZE + 2] };
	unsigned int encryptedSize{ 0 };
	int updateSize{ 0 };

	EVP_CIPHER_CTX* ctx{ EVP_CIPHER_CTX_new() };
	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
	EVP_EncryptUpdate(ctx, encrypted, &updateSize, (unsigned char*)data.c_str(), data.length());

	encryptedSize += updateSize;
	EVP_EncryptFinal_ex(ctx, encrypted + updateSize, &updateSize);
	encryptedSize += updateSize;

	EVP_CIPHER_CTX_free(ctx);
	std::string encText{ "Salted__" + std::string((char*)salt) + std::string((char*)encrypted, encryptedSize) };
	delete[] encrypted;

	if (b64)
	{
		BIO* bioB64{ BIO_new(BIO_f_base64()) };
		BIO* bioMem{ BIO_new(BIO_s_mem()) };
		BIO_set_flags(bioB64, BIO_FLAGS_BASE64_NO_NL);

		BIO_push(bioB64, bioMem);
		BIO_write(bioB64, encText.c_str(), encText.length());
		BIO_flush(bioB64);

		char* encoded{ nullptr };
		encText.assign(encoded, BIO_get_mem_data(bioMem, &encoded));
		BIO_free_all(bioB64);
	}

	return encText;
}

// Decrypt AES (encrypted) using (password)
// Specify if (encrypted) is base64 encoded with (b64) flag
std::string AesDecrypt(std::string encrypted, std::string password, bool b64)
{
	if (b64)
	{
		BIO* bioB64{ BIO_new(BIO_f_base64()) };
		BIO* source{ BIO_new_mem_buf(encrypted.c_str(), -1) };
		BIO_set_flags(bioB64, BIO_FLAGS_BASE64_NO_NL);
		BIO_push(bioB64, source);

		char* decoded{ new char[encrypted.length()] };
		encrypted.assign(decoded, BIO_read(bioB64, decoded, encrypted.length()));
		BIO_free_all(bioB64);
		delete[] decoded;
	}

	std::string salt{ encrypted.substr(8, 8) };
	encrypted.erase(encrypted.begin(), encrypted.begin() + 16);

	unsigned char key[32];
	unsigned char iv[32];
	EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), (unsigned char*)salt.c_str(), (unsigned char*)password.c_str(), password.length(), 1, key, iv);

	unsigned char* decrypted{ new unsigned char[encrypted.length()] };
	unsigned int decryptedSize{ 0 };
	int updateSize{ 0 };

	EVP_CIPHER_CTX* ctx{ EVP_CIPHER_CTX_new() };
	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_set_key_length(ctx, EVP_MAX_KEY_LENGTH);
	EVP_DecryptUpdate(ctx, decrypted, &updateSize, (unsigned char*)encrypted.c_str(), encrypted.length());

	decryptedSize += updateSize;
	EVP_DecryptFinal_ex(ctx, decrypted + updateSize, &updateSize);
	decryptedSize += updateSize;

	EVP_CIPHER_CTX_free(ctx);
	std::string plainText{ (char*)decrypted, decryptedSize };
	delete[] decrypted;

	return plainText;
}


GameInstance::GameInstance(std::string urlId, std::string pswd)
{
	urlHash = Sha512('/' + urlId);
	password = pswd;
	passwordHash = Sha512(pswd);
	endPoint = "https://www.protectedtext.com/" + urlId;
}

bool GameInstance::load()
{
	std::string buffer;

	int attempt{ 2 };
	while (buffer.empty())
	{
		Sleep(1000 * attempt); attempt *= 2;
		CURL* curl{ curl_easy_init() };
		curl_easy_setopt(curl, CURLOPT_URL, (endPoint + "?action=getJSON").c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Receiver);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	sessionObj = JSON::json::parse(buffer);
	dbVersion = sessionObj["currentDBVersion"];
	if (sessionObj["isNew"]) return false;

	std::string encText{ sessionObj["eContent"].get<std::string>() };
	plainText = AesDecrypt(encText, password, true);
	plainText = plainText.substr(0, plainText.length() - 128);
	return true;
}

bool GameInstance::save(std::string textToSave)
{
	std::string encrypted{ AesEncrypt(textToSave + urlHash, password, true) };

	CURL* curl{ curl_easy_init() };
	std::string encryptedUrlEncoded{ curl_easy_escape(curl, encrypted.c_str(), encrypted.length()) };

	std::string postData;
	postData += "initHashContent=" + getWritePermissionProof(plainText) + "&";
	postData += "currentHashContent=" + getWritePermissionProof(textToSave) + "&";
	postData += "encryptedContent=" + encryptedUrlEncoded + "&";
	postData += "action=save";

	std::string buffer;
	static int attempt{ 1 };

	curl_easy_setopt(curl, CURLOPT_URL, endPoint.c_str());
	curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postData.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Receiver);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	if (buffer.empty() || JSON::json::parse(buffer)["status"] == "failed")
	{
		return save(textToSave);
	}
	else attempt = 1;

	if (JSON::json::parse(buffer)["status"] == "success")
	{
		Sleep(1000 * attempt); attempt *= 2;
		plainText = textToSave;
		return true;
	}
	else return false;
}

std::string GameInstance::view()
{
	return plainText;
}

std::string GameInstance::getWritePermissionProof(std::string content)
{
	return (dbVersion == 1) ? Sha512(content) : Sha512(content + passwordHash) + std::to_string(dbVersion);
}