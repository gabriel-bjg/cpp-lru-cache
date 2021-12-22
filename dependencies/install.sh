# Update package repository
sudo apt-get update -y

# Install Catch2
git clone https://github.com/catchorg/Catch2/tree/v2.13.7
cd Catch2
cmake -Bbuild -H. -DBUILD_TESTING=OFF
sudo cmake --build build/ --target install

# Install cppcheck
sudo apt-get install -y cppcheck

# Install gcovr
sudo apt-get install -y gcovr
