#include <expected>
#include <string>
#include <vector>

#include "CLI/CLI.hpp"

enum Action {
    Encrypt,
    Decrypt,
};

struct Arguments {
    Action action;
    std::vector<std::string> files;
};

// using ArgsResult = std::variant<Arguments, int>;
using ArgsResult = std::expected<Arguments, int>;

auto parse_args(int argc, char* argv[]) -> ArgsResult {
    CLI::App app {"CLI application to encrypt and decrypt files"};

    Action action {Action::Encrypt};
    std::map<std::string, Action> map {
        {"encrypt", Action::Encrypt},
        {"decrypt", Action::Decrypt},
        {"enc", Action::Encrypt},
        {"dec", Action::Decrypt}
    };
    app.add_option("-a,--action", action, "What to do")
        ->required()
        ->transform(CLI::CheckedTransformer(map, CLI::ignore_case));

    std::string file;
    app.add_option("-f,--file", file, "Require an existing file")
        ->required()
        ->check(CLI::ExistingFile);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return std::unexpected(app.exit(e));
    }

    std::vector<std::string> files = {file};

    Arguments args {action, files};

    return args;
}
