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

    encrypt(args.files);

    std::cout << "It runs" << std::endl;
    return 0;
}
