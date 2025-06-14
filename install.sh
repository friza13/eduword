#!/bin/bash

# Exit on error
set -e

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Build directory not found. Running build script first..."
    ./build.sh
fi

# Enter build directory
cd build

# Install the application
sudo make install

echo "Installation completed successfully!"
echo "You can now run EduWord from your applications menu or by typing 'eduword' in the terminal." 