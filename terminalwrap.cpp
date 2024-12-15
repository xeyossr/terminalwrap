#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <regex>
#include <fstream>
#include <ctime>

// Colors for the output
const std::string CYAN = "\033[0;36m";
const std::string MAGENTA = "\033[0;35m";
const std::string GREEN = "\033[0;32m";
const std::string YELLOW = "\033[1;33m";
const std::string RED = "\033[0;31m";
const std::string RESET = "\033[0m";

using Str = std::string;

// Function to check if a file exists
bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

// Returns the history file based on the shell type (fish, zsh, bash)
std::string getHistoryFile(const std::string& shell) {
    if (shell == "fish") {
        std::string path = getenv("HOME") + std::string("/.local/share/fish/fish_history");
        if (fileExists(path)) return path;
    } else if (shell == "zsh") {
        std::string zdotdir = getenv("ZDOTDIR") ? getenv("ZDOTDIR") : getenv("HOME");
        std::string path = zdotdir + "/.zsh_history";
        if (fileExists(path)) return path;
    } else if (shell == "bash") {
        std::string path = getenv("HOME") + std::string("/.bash_history");
        if (fileExists(path)) return path;
    }
    std::cerr << RED << "The specified terminal history file was not found!" << RESET << "\n";
    return "";
}

// Prints a banner to the terminal
void banner() {
    Str banner = R"( 
 _____                   _             _  __        __               
|_   _|__ _ __ _ __ ___ (_)_ __   __ _| | \ \      / / __ __ _ _ __  
  | |/ _ \ '__| '_ ` _ \| | '_ \ / _` | |  \ \ /\ / / '__/ _` | '_ \ 
  | |  __/ |  | | | | | | | | | | (_| | |   \ V  V /| | | (_| | |_) |
  |_|\___|_|  |_| |_| |_|_|_| |_|\__,_|_|    \_/\_/ |_|  \__,_| .__/ 
                                                              |_|        
)";

    std::cout << CYAN << banner << "\n"; // Output banner in cyan
}

// Returns a string with spaces for formatting command output
std::string spacer(const int element) {
    int elementLength = std::to_string(std::abs(element)).length();
    int howMuchSpacesNeeded = 12 - elementLength;
    std::string spaces;
    for (int i=0; i<howMuchSpacesNeeded; i++) {
        spaces += " ";
    }

    return spaces;
}

// Displays the command usage results sorted by frequency
void displayResults(const std::string& title, const std::map<std::string, int>& data) {
    std::cout << MAGENTA << title << RESET << "\n";
    std::cout << YELLOW << "---------------------------------" << RESET << "\n";
    std::cout << CYAN << "Usage Count | Command" << RESET << "\n";
    std::cout << YELLOW << "---------------------------------" << RESET << "\n";

    // Sorting commands by usage frequency
    std::vector<std::pair<std::string, int>> sortedData(data.begin(), data.end());
    std::sort(sortedData.begin(), sortedData.end(), [](const auto& a, const auto& b) {
        return b.second < a.second;
    });

	// Determine the most used command
    int topCall{sortedData[0].second};
    Str topItem{sortedData[0].first};

    // Display the top 10 commands
    for (size_t i = 0; i < std::min<size_t>(10, sortedData.size()); ++i) {
        if (sortedData[i].second > topCall) {
            topCall = sortedData[i].second;
            topItem = sortedData[i].first;
        }
        std::cout << sortedData[i].second << spacer(sortedData[i].second) << "| " << sortedData[i].first << "\n";
    }

    std::cout << YELLOW << "---------------------------------" << RESET << "\n";

    // Display motivational comments based on the most used command
    if (topItem == "ls")
        std::cout << YELLOW << "Look at you, flexing that directory like a pro!" << RESET << "\n";
    else if (topItem == "cd")
        std::cout << YELLOW << "Traveling through folders like a digital nomad. Respect!" << RESET << "\n";
    else if (topItem == "pwd")
        std::cout << YELLOW << "Lost? Nah, you're just double-checking your coordinates. Smart move!" << RESET << "\n";
    else if (topItem == "echo")
        std::cout << YELLOW << "Talking to yourself again, huh? At least the terminal listens lol." << RESET << "\n";
    else if (topItem == "mkdir")
        std::cout << YELLOW << "Building virtual homes one directory at a time. Architect vibes!" << RESET << "\n";
    else if (topItem == "rm")
        std::cout << YELLOW << "Destroyer of files and master of chaos. Destruction always comes before creation!" << RESET << "\n";
    else if (topItem == "touch")
        std::cout << YELLOW << "Making empty files like a magician pulling rabbits from a hat!" << RESET << "\n";
    else if (topItem == "cp")
        std::cout << YELLOW << "Duplication master! Copy-paste level: terminal wizard." << RESET << "\n";
    else if (topItem == "mv")
        std::cout << YELLOW << "Relocating files like a moving company for your hard drive!" << RESET << "\n";
    else if (topItem == "cat")
        std::cout << YELLOW << "Reading files like it's storytime in the terminal. (or maybe he just loves cats) Adorable!" << RESET << "\n";
    else if (topItem == "nano")
        std::cout << YELLOW << "Editing text? You’re basically a novelist now." << RESET << "\n";
    else if (topItem == "vim")
        std::cout << YELLOW << "Survived vim? You’ve unlocked legendary status!" << RESET << "\n";
    else if (topItem == "man")
        std::cout << YELLOW << "Real pros read manuals. Or at least pretend to." << RESET << "\n";
    else if (topItem == "history")
        std::cout << YELLOW << "Digging into your command past? It’s like stalking yourself!" << RESET << "\n";
    else if (topItem == "grep")
        std::cout << YELLOW << "Searching for patterns like a detective on a case. Sleuth mode activated!" << RESET << "\n";
    else if (topItem == "find")
        std::cout << YELLOW << "Locating stuff like Google, but cooler." << RESET << "\n";
    else if (topItem == "top")
        std::cout << YELLOW << "Watching processes like a hawk. Big sysadmin energy!" << RESET << "\n";
    else if (topItem == "htop")
        std::cout << YELLOW << "Because top wasn't fancy enough for you, right?" << RESET << "\n";
    else if (topItem == "ps")
        std::cout << YELLOW << "Keeping tabs on processes. Are you a process whisperer?" << RESET << "\n";
    else if (topItem == "kill")
        std::cout << YELLOW << "Terminating processes with style. Hitman vibes!" << RESET << "\n";
    else if (topItem == "wget")
        std::cout << YELLOW << "Snatching files from the web like a digital ninja!" << RESET << "\n";
    else if (topItem == "curl")
        std::cout << YELLOW << "Talking to servers like it’s speed dating for data." << RESET << "\n";
    else if (topItem == "tar")
        std::cout << YELLOW << "Compressing and extracting like a zip/unzip god. Respect!" << RESET << "\n";
    else if (topItem == "gzip")
        std::cout << YELLOW << "Making files lighter and faster. Environmentalist goals!" << RESET << "\n";
    else if (topItem == "unzip")
        std::cout << YELLOW << "Unpacking files like it’s Christmas morning." << RESET << "\n";
    else if (topItem == "chmod")
        std::cout << YELLOW << "Permission granted! You’re the boss here." << RESET << "\n";
    else if (topItem == "chown")
        std::cout << YELLOW << "File ownership changes? You're practically a landlord." << RESET << "\n";
    else if (topItem == "sudo")
        std::cout << YELLOW << "Elevated privileges? The system bows to you now!" << RESET << "\n";
    else if (topItem == "apt-get")
        std::cout << YELLOW << "Installing software like a package manager wizard. Bravo!" << RESET << "\n";
    else if (topItem == "yum")
        std::cout << YELLOW << "Installing with `yum`? Let’s hope it’s tasty!" << RESET << "\n";
    else if (topItem == "dnf")
        std::cout << YELLOW << "Fedora users rolling like a pro with `dnf`. Smooth!" << RESET << "\n";
    else if (topItem == "python")
        std::cout << YELLOW << "Coding snakes? Nah, you're just playing with Python magic!" << RESET << "\n";
    else if (topItem == "python3")
        std::cout << YELLOW << "Moving with the times, huh? Python 3 is the future, and so are you!" << RESET << "\n";
    else if (topItem == "node")
        std::cout << YELLOW << "Running JavaScript outside the browser? Rebel energy!" << RESET << "\n";
    else if (topItem == "npm")
        std::cout << YELLOW << "Installing packages like it’s a shopping spree for coders!" << RESET << "\n";
    else if (topItem == "g++")
        std::cout << YELLOW << "Compiling C++? You’re like a blacksmith for code!" << RESET << "\n";
    else if (topItem == "gcc")
        std::cout << YELLOW << "Crafting binaries from source. Hardcore dev vibes!" << RESET << "\n";
    else if (topItem == "make")
        std::cout << YELLOW << "Turning Makefiles into working software. Productivity level: 1000." << RESET << "\n";
    else if (topItem == "ssh")
        std::cout << YELLOW << "Logging into remote servers? Cyber ninja unlocked!" << RESET << "\n";
    else if (topItem == "scp")
        std::cout << YELLOW << "Transferring files like a pro courier in cyberspace!" << RESET << "\n";
    else if (topItem == "rsync")
        std::cout << YELLOW << "Syncing files like the universe depends on it. Respect!" << RESET << "\n";
    else if (topItem == "docker")
        std::cout << YELLOW << "Containers for days. You're the captain of this ship!" << RESET << "\n";
    else if (topItem == "kubectl")
        std::cout << YELLOW << "Kubernetes admin? Bow down to the cluster king/queen!" << RESET << "\n";
    else if (topItem == "df")
        std::cout << YELLOW << "Disk space check? Keeping it tidy like a digital minimalist." << RESET << "\n";
    else if (topItem == "du")
        std::cout << YELLOW << "Investigating disk usage like a file inspector. Nice!" << RESET << "\n";
    else if (topItem == "ping")
        std::cout << YELLOW << "Pinging servers like a friendly neighbor saying hi!" << RESET << "\n";
    else if (topItem == "traceroute")
        std::cout << YELLOW << "Tracing network routes like a true digital pathfinder!" << RESET << "\n";
    else if (topItem == "alias")
        std::cout << YELLOW << "Shortcuts for commands? Work smarter, not harder!" << RESET << "\n";
    else if (topItem == "awk")
        std::cout << YELLOW << "Processing text like a linguistic ninja. Respect!" << RESET << "\n";
    else if (topItem == "sed")
        std::cout << YELLOW << "Editing streams like a pro coder surgeon. Genius!" << RESET << "\n";
    else if (topItem == "pacman")
        std::cout << YELLOW << "Arch btw!!" << RESET << "\n";
    else if (topItem == "terminalwrap") {
        std::cout << YELLOW << "Do you really like terminalwrap? Then can you star the project to make me happy? pliss. :) Btw, if you are also ambitious that you can do better, how about forking the project?" << RESET << "\n";
        std::cout << GREEN << ">>>" << RED << "https://github.com/xeyossr/terminalwrap" << "\n";
    }
    else   
        std::cout << YELLOW << "Your top tool is \'" << topItem << "\'. Keep rocking your terminal!" << RESET << "\n";
    }


// Preprocess the shell history file to extract commands
std::vector<std::string> preprocessHistory(const std::string& filePath) {
    std::vector<std::string> commands;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open history file.\n";
        return commands;
    }

    std::string line;
    if (filePath.find("fish_history") != std::string::npos) {
      // Fish history file: extract commands using regex
        std::regex cmdRegex("- cmd: (.+)");
        while (std::getline(file, line)) {
            std::smatch match;
            if (std::regex_search(line, match, cmdRegex)) {
                commands.push_back(match[1]);
            }
        }
    } else if (filePath.find("zsh_history") != std::string::npos) {
	  // Zsh history file: extract commands after semicolon
        while (std::getline(file, line)) {
            size_t semicolonPos = line.find(';');
            if (semicolonPos != std::string::npos) {
                std::string command = line.substr(semicolonPos + 1); // Get Commamd
                commands.push_back(command);
            }
        }
    } else {
        while (std::getline(file, line)) {
            commands.push_back(line); // For other shell history files, just read lines
        }
    }
    file.close();
    return commands;
}


// Count the frequency of commands in the history
std::map<std::string, int> countCommands(const std::vector<std::string>& commands) {
    std::map<std::string, int> commandCount;
    for (const auto& cmd : commands) {
        std::istringstream iss(cmd);
        std::string tool;
        iss >> tool; // Get the command tool (e.g., 'ls', 'cd', etc.)
        if (!tool.empty()) {
            commandCount[tool]++; // Increment the count for this tool
        }
    }
    return commandCount;
}

// Checks and returns the correct history file based on the shell type
std::string check_and_return_history_file(const std::string& shell_type) {
    std::string history_file = "";

    if (shell_type == "fish") {
        if (std::filesystem::exists(std::filesystem::path(getenv("HOME")) / ".local" / "share" / "fish" / "fish_history")) {
            history_file = std::string(getenv("HOME")) + "/.local/share/fish/fish_history";
        } else {
            std::cerr << RED << "The specified terminal history file was not found! Why don't you run `cliwrap -h` to find a compatible one?" << RESET << "\n";
            return "";
        }
    }
    else if (shell_type == "zsh") {
        if (std::filesystem::exists(std::filesystem::path(getenv("ZDOTDIR")) / ".zsh_history")) {
            history_file = std::string(getenv("ZDOTDIR")) + "/.zsh_history";
        } else if (std::filesystem::exists(std::filesystem::path(getenv("HOME")) / ".zsh_history")) {
            history_file = std::string(getenv("HOME")) + "/.zsh_history";
        } else {
            std::cerr << RED << "The specified terminal history file was not found! Why don't you run `cliwrap -h` to find a compatible one?" << RESET << "\n";
            return "";
        }
    }

    else if (shell_type == "bash") {
        if (std::filesystem::exists(std::filesystem::path(getenv("HOME")) / ".bash_history")) {
            history_file = std::string(getenv("HOME")) + "/.bash_history";
        } else {
            std::cerr << RED << "The specified terminal history file was not found! Why don't you run `cliwrap -h` to find a compatible one?" << RESET << "\n";
            return "";
        }
    } else {
        std::cerr << RED << "Your shell is not added to the script. Considering creating an issue on the GitHub repository." << RESET << "\n";
        return "";
    }

    return history_file;
}

// Help message, args, etc.
void showHelpMessage() {
    std::cout << "Usage: terminalwrap [OPTION]\n\n"
              << "Options:\n"
              << "  --help, -h       Show this help message.\n"
              << "  --bash           Force using BASH\n"
              << "  --zsh            Force using ZSH\n"
              << "  --fish           Force using Fish\n"
              << "  (default)        Detect the shell from the environment.\n"
              << "\n";
}


int main(int argc, char* argv[]) {
    std::string shell;
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            showHelpMessage();
            return 0;
        } else if (arg == "--zsh") {
            shell = "zsh";
        } else if (arg == "--bash") {
            shell = "bash";
        } else if (arg == "--fish") {
            shell = "fish";
        } else {
            std::cerr << "Unknown option: " << arg << "\n"
                      << "Use --help or -h to see the available options.\n";
            return 1;
        }
    } else {
        shell = std::filesystem::path(getenv("SHELL")).filename();
    }


    std::string historyFile = getHistoryFile(shell);
    if (historyFile.empty()) {
        return 1;
    }

    banner();
    std::cout << GREEN << "Parsing history file: " << historyFile << RESET << "\n\n";

    auto commands = preprocessHistory(historyFile);
    auto commandCounts = countCommands(commands);

    displayResults("Top Tools", commandCounts);

    std::cout << GREEN << "\nTotal Commands Executed: " << commands.size() << RESET << "\n";


    return 0;
}
