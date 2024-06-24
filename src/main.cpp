#include <expected>
#include <iostream>
#include <string>
#include <vector>

#include "arguments.cpp"
#include "cipher.cpp"

auto main(int argc, char* argv[]) -> int {
    ArgsResult value = parse_args(argc, argv);
    if (!value.has_value()) {
        return value.error();
    }
    Arguments args = value.value();

    switch (args.action) {
        case Action::Encrypt:
            encrypt(args.files);
            break;
        case Action::Decrypt:
            decrypt(args.files);
            break;
    }

    std::cout << "It runs" << std::endl;
    return 0;
}
