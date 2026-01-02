#!/usr/bin/env bash
# ============================================================================
# Universal C++ Build Script
# Supports: debug, release, clean, run, install, test, sanitize, help
# Auto-detects project structure and creates directories as needed
# ============================================================================

set -euo pipefail

# Color codes for output
readonly RED='\033[0;31m'
readonly GREEN='\033[0;32m'
readonly YELLOW='\033[1;33m'
readonly BLUE='\033[0;34m'
readonly MAGENTA='\033[0;35m'
readonly CYAN='\033[0;36m'
readonly NC='\033[0m' # No Color

# ============================================================================
# Configuration (can be overridden via environment variables)
# ============================================================================

readonly CXX="${CXX:-g++}"
readonly STD="${STD:-c++20}"
readonly ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Auto-detect project name from directory
readonly PROJECT_NAME="${PROJECT_NAME:-$(basename "$ROOT" | tr '[:upper:]' '[:lower:]')}"

# Directory structure
readonly BUILD_DIR="$ROOT/build"
readonly BIN_DIR="$ROOT/bin"
readonly LIB_DIR="$ROOT/lib"
readonly INCLUDE_DIR="$ROOT/include"
readonly SRC_DIR="$ROOT/src"
readonly TEST_DIR="$ROOT/test"
readonly INSTALL_PREFIX="${INSTALL_PREFIX:-/usr/local}"

# Build artifacts
readonly TARGET="$BIN_DIR/$PROJECT_NAME"
readonly COMPILE_DB="$ROOT/compile_commands.json"

# ============================================================================
# Utility Functions
# ============================================================================

print_info() {
    echo -e "${CYAN}[INFO]${NC} $*"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $*"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $*"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $*" >&2
}

print_section() {
    echo -e "\n${MAGENTA}=== $* ===${NC}\n"
}

# ============================================================================
# Directory Setup
# ============================================================================

setup_directories() {
    print_info "Setting up directory structure..."
    mkdir -p "$BUILD_DIR" "$BIN_DIR" "$LIB_DIR"
    print_success "Directories ready"
}

# ============================================================================
# Source File Discovery
# ============================================================================

discover_sources() {
    local dir="$1"
    if [[ ! -d "$dir" ]]; then
        return 1
    fi
    find "$dir" -type f \( -name "*.cpp" -o -name "*.cxx" -o -name "*.cc" -o -name "*.c" \) | sort
}

discover_headers() {
    local dir="$1"
    if [[ ! -d "$dir" ]]; then
        return 1
    fi
    find "$dir" -type f \( -name "*.hpp" -o -name "*.hxx" -o -name "*.hh" -o -name "*.h" \) | sort
}

# ============================================================================
# Compiler Flags
# ============================================================================

get_common_flags() {
    local mode="$1"
    local sanitize="${2:-}"
    
    local flags=(
        "-std=$STD"
        "-Wall" "-Wextra" "-Wpedantic"
        "-Wno-unused-parameter"
    )
    
    # Include directories
    if [[ -d "$INCLUDE_DIR" ]]; then
        flags+=("-I$INCLUDE_DIR")
    fi
    
    # Mode-specific flags
    if [[ "$mode" == "release" ]]; then
        flags+=("-O3" "-DNDEBUG" "-march=native" "-flto")
        flags+=("-ffast-math" "-funroll-loops")
    else
        flags+=("-O0" "-g" "-DDEBUG")
        flags+=("-fno-omit-frame-pointer")
    fi
    
    # Sanitizers
    if [[ -n "$sanitize" ]]; then
        case "$sanitize" in
            address)
                flags+=("-fsanitize=address")
                ;;
            undefined)
                flags+=("-fsanitize=undefined")
                ;;
            memory)
                flags+=("-fsanitize=memory")
                ;;
            thread)
                flags+=("-fsanitize=thread")
                ;;
            all)
                flags+=("-fsanitize=address,undefined")
                ;;
        esac
        # Clang-specific flag (not supported by GCC)
        if [[ "$CXX" == *"clang"* ]]; then
            flags+=("-fno-optimize-sanitizers")
        fi
    fi
    
    printf '%s\n' "${flags[@]}"
}

get_linker_flags() {
    local mode="$1"
    local sanitize="${2:-}"
    
    local flags=()
    
    if [[ "$mode" == "release" ]]; then
        flags+=("-flto")
    fi
    
    # Sanitizer linker flags
    if [[ -n "$sanitize" ]]; then
        case "$sanitize" in
            address|undefined|memory|thread|all)
                flags+=("-fsanitize=${sanitize}")
                ;;
        esac
    fi
    
    # Only print if there are flags
    if [[ ${#flags[@]} -gt 0 ]]; then
        printf '%s\n' "${flags[@]}"
    fi
}

# ============================================================================
# Compilation
# ============================================================================

compile_object() {
    local source="$1"
    local mode="$2"
    local sanitize="${3:-}"
    
    local rel_path="${source#$SRC_DIR/}"
    local obj_path="$BUILD_DIR/${rel_path%.*}.o"
    local dep_path="$BUILD_DIR/${rel_path%.*}.d"
    
    mkdir -p "$(dirname "$obj_path")"
    
    local flags
    mapfile -t flags < <(get_common_flags "$mode" "$sanitize")
    
    # Check if recompilation is needed
    if [[ -f "$obj_path" && -f "$dep_path" ]]; then
        local source_time=$(stat -c %Y "$source" 2>/dev/null || echo 0)
        local obj_time=$(stat -c %Y "$obj_path" 2>/dev/null || echo 0)
        
        if [[ $source_time -le $obj_time ]]; then
            # Check dependencies
            if command -v g++ &>/dev/null; then
                local deps_changed=0
                while IFS= read -r dep; do
                    if [[ -f "$dep" ]]; then
                        local dep_time=$(stat -c %Y "$dep" 2>/dev/null || echo 0)
                        if [[ $dep_time -gt $obj_time ]]; then
                            deps_changed=1
                            break
                        fi
                    fi
                done < <(grep -oP '(?<=: )[^\\]+' "$dep_path" 2>/dev/null || true)
                
                if [[ $deps_changed -eq 0 ]]; then
                    return 0  # Skip compilation
                fi
            fi
        fi
    fi
    
    print_info "Compiling: $source"
    
    # Generate dependency file and compile
    "$CXX" \
        -MMD -MP \
        -c "$source" \
        "${flags[@]}" \
        -o "$obj_path" \
        -MF "$dep_path"
}

build_project() {
    local mode="${1:-debug}"
    local sanitize="${2:-}"
    local verbose="${3:-false}"
    
    print_section "Building Project ($mode mode)"
    
    setup_directories
    
    # Discover sources
    local sources
    mapfile -t sources < <(discover_sources "$SRC_DIR")
    
    if [[ ${#sources[@]} -eq 0 ]]; then
        print_error "No source files found in $SRC_DIR"
        exit 1
    fi
    
    print_info "Compiler: $CXX"
    print_info "C++ Standard: $STD"
    print_info "Mode: $mode"
    [[ -n "$sanitize" ]] && print_info "Sanitizer: $sanitize"
    print_info "Sources found: ${#sources[@]}"
    
    if [[ "$verbose" == "true" ]]; then
        printf '  %s\n' "${sources[@]}"
    fi
    
    # Compile all sources
    local compiled=0
    for source in "${sources[@]}"; do
        if compile_object "$source" "$mode" "$sanitize"; then
            ((compiled++)) || true
        fi
    done
    
    print_info "Compiled: $compiled/${#sources[@]} files"
    
    # Link
    print_info "Linking: $TARGET"
    
    local obj_files
    mapfile -t obj_files < <(find "$BUILD_DIR" -name "*.o" -type f | sort)
    
    if [[ ${#obj_files[@]} -eq 0 ]]; then
        print_error "No object files found"
        exit 1
    fi
    
    local flags
    mapfile -t flags < <(get_common_flags "$mode" "$sanitize")
    
    local linker_flags
    mapfile -t linker_flags < <(get_linker_flags "$mode" "$sanitize")
    
    # Build command array
    local cmd=("$CXX" "${flags[@]}" "${obj_files[@]}")
    
    # Only add linker flags if they exist
    if [[ ${#linker_flags[@]} -gt 0 ]]; then
        cmd+=("${linker_flags[@]}")
    fi
    
    cmd+=("-o" "$TARGET")
    
    "${cmd[@]}"
    
    # Generate compile_commands.json for LSP support
    generate_compile_commands "$mode" "$sanitize"
    
    print_success "Build complete: $TARGET"
    
    # Show binary info
    if [[ -f "$TARGET" ]]; then
        local size=$(du -h "$TARGET" | cut -f1)
        print_info "Binary size: $size"
    fi
}

generate_compile_commands() {
    local mode="$1"
    local sanitize="${2:-}"
    
    print_info "Generating compile_commands.json..."
    
    local flags
    mapfile -t flags < <(get_common_flags "$mode" "$sanitize")
    
    local sources
    mapfile -t sources < <(discover_sources "$SRC_DIR")
    
    local entries=()
    for source in "${sources[@]}"; do
        local rel_path="${source#$SRC_DIR/}"
        local obj_path="$BUILD_DIR/${rel_path%.*}.o"
        
        local args=(
            "$CXX"
            "-c"
            "${flags[@]}"
            "-o" "$obj_path"
            "$source"
        )
        
        # Create JSON entry
        local entry=$(cat <<EOF
    {
      "directory": "$ROOT",
      "command": "$(printf '%s ' "${args[@]}" | sed 's/ $//')",
      "file": "$source",
      "output": "$obj_path"
    }
EOF
)
        entries+=("$entry")
    done
    
    # Write compile_commands.json
    {
        echo "["
        local first=true
        for entry in "${entries[@]}"; do
            [[ "$first" == false ]] && echo ","
            first=false
            echo "$entry"
        done
        echo "]"
    } > "$COMPILE_DB"
    
    print_success "compile_commands.json generated"
}

# ============================================================================
# Actions
# ============================================================================

action_clean() {
    print_section "Cleaning Build Artifacts"
    
    if [[ -d "$BUILD_DIR" ]]; then
        rm -rf "$BUILD_DIR"/*
        print_success "Cleaned: $BUILD_DIR"
    fi
    
    if [[ -f "$TARGET" ]]; then
        rm -f "$TARGET"
        print_success "Removed: $TARGET"
    fi
    
    if [[ -f "$COMPILE_DB" ]]; then
        rm -f "$COMPILE_DB"
        print_success "Removed: $COMPILE_DB"
    fi
    
    print_success "Clean complete"
}

action_debug() {
    build_project "debug" "" "${VERBOSE:-false}"
}

action_release() {
    build_project "release" "" "${VERBOSE:-false}"
}

action_sanitize() {
    local sanitizer="${1:-address}"
    build_project "debug" "$sanitizer" "${VERBOSE:-false}"
}

action_run() {
    local mode="${1:-debug}"
    local args=("${@:2}")
    
    if [[ ! -f "$TARGET" ]]; then
        print_warning "Binary not found. Building in $mode mode..."
        build_project "$mode" "" "${VERBOSE:-false}"
    fi
    
    print_section "Running: $TARGET"
    
    if [[ ${#args[@]} -gt 0 ]]; then
        print_info "Arguments: ${args[*]}"
    fi
    
    "$TARGET" "${args[@]}"
}

action_test() {
    if [[ ! -d "$TEST_DIR" ]]; then
        print_warning "Test directory not found: $TEST_DIR"
        return 0
    fi
    
    print_section "Running Tests"
    
    # Build test executable if test sources exist
    local test_sources
    mapfile -t test_sources < <(discover_sources "$TEST_DIR")
    
    if [[ ${#test_sources[@]} -eq 0 ]]; then
        print_warning "No test sources found"
        return 0
    fi
    
    print_info "Test sources found: ${#test_sources[@]}"
    
    # For now, just build the main project
    # In a full implementation, you'd build a separate test executable
    print_info "Building project for testing..."
    build_project "debug" "" "${VERBOSE:-false}"
    
    print_success "Tests would run here (implement test framework integration)"
}

action_install() {
    if [[ ! -f "$TARGET" ]]; then
        print_error "Binary not found. Build the project first."
        exit 1
    fi
    
    print_section "Installing: $PROJECT_NAME"
    
    local bin_dest="$INSTALL_PREFIX/bin"
    mkdir -p "$bin_dest"
    
    cp "$TARGET" "$bin_dest/$PROJECT_NAME"
    chmod +x "$bin_dest/$PROJECT_NAME"
    
    print_success "Installed to: $bin_dest/$PROJECT_NAME"
    
    # Install headers if they exist
    if [[ -d "$INCLUDE_DIR" ]]; then
        local include_dest="$INSTALL_PREFIX/include/$PROJECT_NAME"
        mkdir -p "$include_dest"
        cp -r "$INCLUDE_DIR"/* "$include_dest/" 2>/dev/null || true
        print_success "Headers installed to: $include_dest"
    fi
}

action_uninstall() {
    print_section "Uninstalling: $PROJECT_NAME"
    
    local bin_path="$INSTALL_PREFIX/bin/$PROJECT_NAME"
    if [[ -f "$bin_path" ]]; then
        rm -f "$bin_path"
        print_success "Removed: $bin_path"
    else
        print_warning "Not installed: $bin_path"
    fi
    
    local include_path="$INSTALL_PREFIX/include/$PROJECT_NAME"
    if [[ -d "$include_path" ]]; then
        rm -rf "$include_path"
        print_success "Removed: $include_path"
    fi
}

action_help() {
    cat <<EOF
${CYAN}Universal C++ Build Script${NC}

${GREEN}Usage:${NC}
    ./build.sh [COMMAND] [OPTIONS]

${GREEN}Commands:${NC}
    ${YELLOW}debug${NC}          Build in debug mode (default)
    ${YELLOW}release${NC}        Build in release mode with optimizations
    ${YELLOW}clean${NC}          Remove all build artifacts
    ${YELLOW}run${NC} [args]     Build (if needed) and run the executable
    ${YELLOW}test${NC}           Build and run tests (if test/ directory exists)
    ${YELLOW}sanitize${NC} [type] Build with sanitizers (address|undefined|memory|thread|all)
    ${YELLOW}install${NC}        Install binary and headers to system
    ${YELLOW}uninstall${NC}      Remove installed files
    ${YELLOW}help${NC}           Show this help message

${GREEN}Options:${NC}
    ${YELLOW}VERBOSE=1${NC}      Enable verbose output
    ${YELLOW}CXX=clang++${NC}    Use different compiler
    ${YELLOW}STD=c++23${NC}      Use different C++ standard
    ${YELLOW}INSTALL_PREFIX=/usr${NC}  Custom install prefix

${GREEN}Examples:${NC}
    ./build.sh debug
    ./build.sh release
    ./build.sh clean
    ./build.sh run arg1 arg2
    ./build.sh sanitize address
    VERBOSE=1 ./build.sh debug
    CXX=clang++ ./build.sh release

${GREEN}Project Structure:${NC}
    ${BLUE}Project Name:${NC} Auto-detected from directory: $PROJECT_NAME
    ${BLUE}Source:${NC} $SRC_DIR
    ${BLUE}Headers:${NC} $INCLUDE_DIR
    ${BLUE}Build:${NC} $BUILD_DIR
    ${BLUE}Binary:${NC} $TARGET

EOF
}

# ============================================================================
# Main
# ============================================================================

main() {
    local command="${1:-debug}"
    
    case "$command" in
        debug)
            action_debug
            ;;
        release)
            action_release
            ;;
        clean)
            action_clean
            ;;
        run)
            action_run "${2:-debug}" "${@:3}"
            ;;
        test)
            action_test
            ;;
        sanitize)
            action_sanitize "${2:-address}"
            ;;
        install)
            action_install
            ;;
        uninstall)
            action_uninstall
            ;;
        help|--help|-h)
            action_help
            ;;
        *)
            print_error "Unknown command: $command"
            echo ""
            action_help
            exit 1
            ;;
    esac
}

main "$@"
