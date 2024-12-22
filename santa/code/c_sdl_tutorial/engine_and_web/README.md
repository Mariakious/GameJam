# Simple SDL Engine

> WARNING: Audio is temporarily disabled so that the app can be played
> in WSL. To enable it back, just delete the `-D NO_AUDIO` option
> inside the Makefile.

## Building for the Web

The engine supports building for the web as well. Just execute the
`build_web.sh` shell script included in the file tree. Emscripten is
an alternative compiler which, contrary to gcc, targets Javascript and
WebAssembly. The instructions below will guide you into setting things
up. We're essentially installing `emsdk`.

```bash
# Create a new directory that will hold the installed version
# It should not be part of your game, but rather part of your system
cd ~/applications/

git clone https://github.com/emscripten-core/emsdk.git
./emsdk install latest
./emsdk activate latest

# This command will insert an alias into your PATH, so it can be
# accessed from any directory on your terminal, without having to list
# the whole path. Note however that this is temporary!
# Don't open up a new terminal, because the alias will be gone!
# To fix this, copy and paste the line below inside your ~/.bashrc
source ./emsdk_env.sh
```

Move into the root of the game and type `./build_web.sh`. A new window
should now pop up inside your browser with the game playing. The
output HTML file is **configurable**. The Emscripten logo and the
console are there for debugging purposes, you can get rid of them.
