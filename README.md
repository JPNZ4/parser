# Spalk Take Home Test

- I have used CMake so hopefully this runs gracefully on any operating system as I only have a windows PC at home.

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher

## Testing

- Windows 11

## Building

```bash
# Create build directory
mkdir build

# Generate build files
cmake -B build

# Build the project
cmake --build build
```

## Usage

Run with a file:
```bash
./build/Debug/Parser.exe test_success.ts
./build/Debug/Parser.exe test_failure.ts
```

Or pipe input through stdin (Windows PowerShell):
```powershell
Get-Content -Raw test_success.ts | ./build/Debug/Parser.exe
Get-Content -Raw test_failure.ts | ./build/Debug/Parser.exe
```

Note: When using stdin on Windows, use PowerShell's `Get-Content -Raw` instead of `cat` to properly handle binary data.