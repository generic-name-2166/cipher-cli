#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "cryptopp/aes.h"
#include "cryptopp/base64.h"
#include "cryptopp/modes.h"
#include "cryptopp/rijndael.h"

auto read_file(std::string filename) -> std::string {
    std::filesystem::path path = std::filesystem::path(filename);
    std::ifstream file(path, std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Couldn't open the specified file: " << filename << '\n';
        return "";
    }

    std::string contents = "";

    std::vector<char> buffer(1024, 0);  // reads only 1024 bytes at a time
    while (!file.eof()) {
        file.read(buffer.data(), buffer.size());
        std::streamsize size = file.gcount();
        contents += {buffer.begin(), buffer.begin() + size};
    }

    file.close();

    return contents;
}

auto write_file(std::string filename, std::string data) -> void {
    std::filesystem::path path = std::filesystem::path(filename);
    std::ofstream file(path, std::ios::binary | std::ios::out);

    file.write(data.c_str(), data.size());

    file.close();
}

auto encrypt_file(std::string filename, CryptoPP::SecByteBlock key) -> void {
    // code for CryptoPP from https://stackoverflow.com/a/15388182

    std::string plain = read_file(filename);

    std::string cipher;

    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

    CryptoPP::AES::Encryption aesEncryption(
        key,
        CryptoPP::AES::DEFAULT_KEYLENGTH
    );
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(
        aesEncryption,
        iv
    );

    CryptoPP::StreamTransformationFilter stfEncryptor(
        cbcEncryption,
        new CryptoPP::StringSink(cipher)
    );
    stfEncryptor.Put(
        reinterpret_cast<const unsigned char*>(plain.c_str()),
        plain.length()
    );
    stfEncryptor.MessageEnd();

    std::cout << "Cipher Text (" << cipher.size() << " bytes)" << std::endl;

    for (int i = 0; i < cipher.size(); i++) {
        std::cout << "0x" << std::hex
                  << (0xFF & static_cast<CryptoPP::byte>(cipher[i])) << ' ';
    }
    std::cout << '\n';

    write_file(filename, cipher);
}

auto encrypt(std::vector<std::string> files, CryptoPP::SecByteBlock key)
    -> void {
    for (const std::string file : files) {
        encrypt_file(file, key);
    }
}

auto decrypt_file(std::string filename, CryptoPP::SecByteBlock key) -> void {
    // code for CryptoPP from https://stackoverflow.com/a/15388182

    std::string cipher = read_file(filename);

    std::string decrypted;

    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

    CryptoPP::AES::Decryption aesDecryption(
        key,
        CryptoPP::AES::DEFAULT_KEYLENGTH
    );
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(
        aesDecryption,
        iv
    );

    CryptoPP::StreamTransformationFilter stfDecryptor(
        cbcDecryption,
        new CryptoPP::StringSink(decrypted)
    );
    stfDecryptor.Put(
        reinterpret_cast<const unsigned char*>(cipher.c_str()),
        cipher.size()
    );
    stfDecryptor.MessageEnd();

    std::cout << "Decrypted Text (" << cipher.size() << " bytes) \n"
              << decrypted << '\n';

    write_file(filename, decrypted);
}

auto decrypt(std::vector<std::string> files, CryptoPP::SecByteBlock key)
    -> void {
    for (const std::string file : files) {
        decrypt_file(file, key);
    }
}
