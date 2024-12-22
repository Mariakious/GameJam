rm web_build/index.js
rm web_build/index.wasm
rm web_build/index.data

# First, collect all source files
source_files=$(find src/ -name "*.c")

echo "Compiling source files, this might take a while..."

# Build the target javascript and webassembly files
# Emscripten will handle the res/ folder appropriately
emcc ${source_files} --preload-file res -o web_build/index.js \
    -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' \
    -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2

# Run using a local web server
#emrun web_build/index.html --port 8080
