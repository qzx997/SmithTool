#!/bin/bash
#
# SmithTool Quick Start Script
# 快速启动Smith圆图工具
#

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
EXECUTABLE="${BUILD_DIR}/SmithTool"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}   SmithTool - Interactive Smith Chart${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Build directory not found. Creating...${NC}"
    mkdir -p "$BUILD_DIR"
fi

# Check if executable exists and is up to date
NEEDS_BUILD=false
if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${YELLOW}Executable not found. Building...${NC}"
    NEEDS_BUILD=true
else
    # Check if any source file is newer than executable
    NEWEST_SRC=$(find "${SCRIPT_DIR}/src" -name "*.cpp" -o -name "*.h" 2>/dev/null | xargs ls -t 2>/dev/null | head -1)
    if [ -n "$NEWEST_SRC" ] && [ "$NEWEST_SRC" -nt "$EXECUTABLE" ]; then
        echo -e "${YELLOW}Source files changed. Rebuilding...${NC}"
        NEEDS_BUILD=true
    fi
fi

# Build if needed
if [ "$NEEDS_BUILD" = true ]; then
    echo -e "${YELLOW}Running CMake...${NC}"
    cd "$BUILD_DIR"
    cmake .. -DCMAKE_BUILD_TYPE=Release
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}CMake configuration failed!${NC}"
        exit 1
    fi
    
    echo -e "${YELLOW}Compiling...${NC}"
    make -j$(nproc)
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}Build failed!${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}Build successful!${NC}"
    cd "$SCRIPT_DIR"
fi

# Run the application
if [ -f "$EXECUTABLE" ]; then
    echo -e "${GREEN}Starting SmithTool...${NC}"
    echo ""
    exec "$EXECUTABLE" "$@"
else
    echo -e "${RED}Error: Executable not found at ${EXECUTABLE}${NC}"
    exit 1
fi
