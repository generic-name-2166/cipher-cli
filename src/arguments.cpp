#include <expected>
#include <iostream>
#include <string>
#include <vector>

#include "CLI/CLI.hpp"
#include "cryptopp/aes.h"
#include "cryptopp/base64.h"
#include "cryptopp/rijndael.h"

enum Action {
    Encrypt,
    Decrypt,
};

struct Arguments {
    Action action;
    std::vector<std::string> files;
    CryptoPP::SecByteBlock key;
};

using ArgsResult = std::expected<Arguments, int>;
using KeyResult = std::expected<CryptoPP::SecByteBlock, int>;

auto ensure_key(std::string key) -> KeyResult {
    CryptoPP::SecByteBlock decoded;

    CryptoPP::Base64Decoder decoder;
    decoder.Put((CryptoPP::byte*)key.data(), key.size());
    decoder.MessageEnd();
    CryptoPP::word64 size = decoder.MaxRetrievable();
    if (size == CryptoPP::AES::DEFAULT_KEYLENGTH) {
        decoded.resize(size);
        decoder.Get((CryptoPP::byte*)&decoded[0], decoded.size());
        return decoded;
    }
    std::cout << "Invalid key size. Needs " << CryptoPP::AES::DEFAULT_KEYLENGTH
              << " bytes, got " << size << '\n';
    return std::unexpected(1);
}

auto parse_args(int argc, char* argv[]) -> ArgsResult {
    CLI::App app("CLI application to encrypt and decrypt files");

    Action action = Action::Encrypt;
    std::map<std::string, Action> map {
        {"encrypt", Action::Encrypt},
        {"decrypt", Action::Decrypt},
        {"enc", Action::Encrypt},
        {"dec", Action::Decrypt}
    };
    app.add_option("-a,--action", action, "What to do")
        ->required()
        ->transform(CLI::CheckedTransformer(map, CLI::ignore_case));

    std::string key;
    app.add_option("-k,--key", key, "Base64 key")->required();

    std::string file;
    app.add_option("-f,--file", file, "Require an existing file")
        ->required()
        ->check(CLI::ExistingFile);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return std::unexpected(app.exit(e));
    }
    KeyResult key_result = ensure_key(key);
    if (!key_result.has_value()) {
        return std::unexpected(1);
    }
    CryptoPP::SecByteBlock decoded_key = key_result.value();

    std::vector<std::string> files = {file};

    Arguments args {action, files, decoded_key};

    return args;
}
