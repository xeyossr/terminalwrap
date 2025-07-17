package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"
)

func printError(msg string, args ...interface{}) {
	if len(args) > 0 {
		msg = fmt.Sprintf(msg, args...)
	}
	// Print the error message in red color
	fmt.Fprintf(os.Stderr, "\033[31mError:\033[0m %s\n", msg)

	os.Exit(1)
}

func detectShell() (string, error) {
	shellEnv := filepath.Base(os.Getenv("SHELL"))
	if shellEnv == "" {
		return "", fmt.Errorf("SHELL environment variable is not set")
	}
	return shellEnv, nil
}

func findHistoryFile(shell string) (string, error) {
	var err error

	// If shell is not provided, try to detect it
	// using the SHELL environment variable.
	if shell == "" {
		shell, err = detectShell()
		if err != nil {
			return "", fmt.Errorf("failed to detect shell: %v", err)
		}
	}

	// Get the user's home directory
	home, err := os.UserHomeDir()
	if err != nil {
		return "", fmt.Errorf("failed to get home directory: %v", err)
	}

	// Determine the history file based on the shell type
	switch shell {
	case "bash":
		return filepath.Join(home, ".bash_history"), nil
	case "zsh":
		return filepath.Join(home, ".zsh_history"), nil
	case "fish":
		return filepath.Join(home, ".local/share/fish/fish_history"), nil
	default:
		return "", fmt.Errorf("unsupported shell: %s", shell)
	}
}

func parseBashHistory(path string) (map[string]int, error) {
	hfile, err := os.Open(path)
	if err != nil {
		return nil, fmt.Errorf("failed to open bash history file: %v", err)
	}
	defer hfile.Close()

	scanner := bufio.NewScanner(hfile)
	history := make(map[string]int)
	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			continue
		}
		history[line]++
	}

	return history, scanner.Err()
}

func parseZshHistory(path string) (map[string]int, error) {
	hfile, err := os.Open(path)
	if err != nil {
		return nil, fmt.Errorf("failed to open zsh history file: %v", err)
	}
	defer hfile.Close()

	scanner := bufio.NewScanner(hfile)
	history := make(map[string]int)
	re := regexp.MustCompile(`^: [0-9]+:[0-9]+;(.*)$`)
	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			continue
		}
		matches := re.FindStringSubmatch(line)
		if matches != nil {
			history[matches[1]]++
		} else {
			continue
		}
	}

	return history, scanner.Err()
}

func parseFishHistory(path string) (map[string]int, error) {
	hfile, err := os.Open(path)
	if err != nil {
		return nil, fmt.Errorf("failed to open fish history file: %v", err)
	}
	defer hfile.Close()

	scanner := bufio.NewScanner(hfile)
	history := make(map[string]int)
	re := regexp.MustCompile(`^\s*- cmd: (.*)$`)
	for scanner.Scan() {
		line := scanner.Text()
		if !strings.Contains(line, "- cmd:") {
			continue
		}

		matches := re.FindStringSubmatch(line)
		if matches != nil {
			history[matches[1]]++
		} else {
			continue
		}
	}

	return history, scanner.Err()
}

type ToolCount struct {
	Tool  string
	Count int
}

func topNTools(parsedHistory map[string]int, n int) ([]ToolCount, error) {
	if len(parsedHistory) == 0 {
		return nil, fmt.Errorf("no history entries found")
	}

	toolCount := make(map[string]int)
	for cmd, count := range parsedHistory {
		tool := strings.Split(cmd, " ")[0]
		if count > 0 {
			toolCount[tool] += count
		}
	}

	var sorted []ToolCount

	for t, c := range toolCount {
		sorted = append(sorted, ToolCount{t, c})
	}

	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i].Count > sorted[j].Count
	})

	if n > len(sorted) {
		n = len(sorted)
	}

	return sorted[:n], nil
}

func wrapper(shell string, n *int) ([]ToolCount, error) {
	var (
		history    map[string]int
		topHistory []ToolCount
		err        error
	)

	historyFile, err := findHistoryFile(shell)
	if err != nil {
		return nil, fmt.Errorf("error finding history file: %v", err)
	}

	switch shell {
	case "bash":
		history, err = parseBashHistory(historyFile)
	case "zsh":
		history, err = parseZshHistory(historyFile)
	case "fish":
		history, err = parseFishHistory(historyFile)
	default:
		return nil, fmt.Errorf("unsupported shell: %s", shell)
	}

	if err != nil {
		return nil, fmt.Errorf("error parsing history file: %v", err)
	}

	topHistory, err = topNTools(history, *n)
	if err != nil {
		return nil, fmt.Errorf("error getting top tools: %v", err)
	}

	return topHistory, nil
}

func getEmoji(rank int) string {
	switch rank {
	case 1:
		return "🚀"
	case 2:
		return "🔥"
	case 3:
		return "💡"
	case 4:
		return "🛠️"
	case 5:
		return "🧰"
	default:
		return "✨"
	}
}

func printOutput(topHistory []ToolCount) {
	if len(topHistory) == 0 {
		fmt.Println("No commands found in history.")
		return
	}

	// Define ANSI color codes
	bold := "\033[1m"
	reset := "\033[0m"
	yellow := "\033[33m"
	cyan := "\033[36m"
	green := "\033[32m"
	blue := "\033[34m"
	red := "\033[31m"
	magenta := "\033[35m"

	title := fmt.Sprintf("Most Used Tools (Top %d)", len(topHistory))
	fmt.Printf("%s%s%s\n", bold, title, reset)
	fmt.Printf("%s%s%s\n", bold, strings.Repeat("=", len(title)), reset)

	maxToolLen := 0
	for _, tool := range topHistory {
		if len(tool.Tool) > maxToolLen {
			maxToolLen = len(tool.Tool)
		}
	}

	for i, tool := range topHistory {
		rank := i + 1
		emoji := getEmoji(rank)

		var color string

		// Assign colors based on rank
		switch rank {
		case 1:
			color = yellow
		case 2:
			color = green
		case 3:
			color = magenta
		case 4:
			color = blue
		case 5:
			color = red
		default:
			color = cyan
		}

		fmt.Printf("%3d. %s%-*s%s %4d %s\n",
			rank,
			color, maxToolLen, tool.Tool, reset,
			tool.Count,
			emoji,
		)
	}
}

func main() {
	// Set up logging
	log.SetOutput(os.Stdout)
	log.SetPrefix("terminalwrap: ")
	log.SetFlags(0)

	// CLI flags
	isBash := flag.Bool("bash", false, "Parse bash history")
	isZsh := flag.Bool("zsh", false, "Parse zsh history")
	isFish := flag.Bool("fish", false, "Parse fish history")
	topN := flag.Int("top", 10, "Number of top tools to display")
	flag.Parse()

	// Validate that only one shell type is specified
	shell := ""
	shellFlags := 0
	if *isBash {
		shell = "bash"
		shellFlags++
	}
	if *isZsh {
		shell = "zsh"
		shellFlags++
	}
	if *isFish {
		shell = "fish"
		shellFlags++
	}
	if shellFlags > 1 {
		printError("Only one of --bash, --zsh, or --fish can be specified at a time.")
	}
	if shell == "" {
		var err error
		// If no shell is specified, try to detect it
		shell, err = detectShell()
		if err != nil {
			printError("detecting shell: %v\n", err)
		}
	}

	wrapped, err := wrapper(shell, topN)
	if err != nil {
		printError("%v\n", err)
	}

	printOutput(wrapped)
}
