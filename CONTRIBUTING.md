# Contributing to TerminalWrap

Thank you for considering contributing to TerminalWrap!

## How to Contribute

- **Bug Reports & Feature Requests:**  
  Please open an issue describing your bug or feature idea.

- **Pull Requests:**  
  - Fork the repository and create your branch from `main`.
  - Make your changes.
  - Ensure your code is clean and tested.
  - Submit a pull request with a clear description.

## Adding Support for a New Shell

If you'd like to add support for a new shell:
1. Add a new parser function for the shell's history format in Go.
2. Update the shell detection and selection logic if needed.
3. Add tests or usage examples if possible.
4. Document your changes in the README if