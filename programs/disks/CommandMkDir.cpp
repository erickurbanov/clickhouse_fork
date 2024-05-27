#include "ICommand.h"

#include <Interpreters/Context.h>
#include <Common/TerminalSize.h>

namespace DB
{

namespace ErrorCodes
{
extern const int BAD_ARGUMENTS;
}

class CommandMkDir final : public ICommand
{
public:
    CommandMkDir()
    {
        command_name = "mkdir";
        description = "Create a directory";
        options_description.add_options()("recursive", "recursively create directories")(
            "path", po::value<String>(), "the path of listing (mandatory, positional)");
        positional_options_description.add("path", 1);
    }

    void executeImpl(const CommandLineOptions & options, DisksClient & client) override
    {
        bool recursive = options.count("recursive");
        auto disk = client.getCurrentDiskWithPath();

        String path = disk.getRelativeFromRoot(getValueFromCommandLineOptionsThrow<String>(options, "path"));

        if (recursive)
            disk.getDisk()->createDirectories(path);
        else
            disk.getDisk()->createDirectory(path);
    }
};

CommandPtr makeCommandMkDir()
{
    return std::make_unique<DB::CommandMkDir>();
}

}
