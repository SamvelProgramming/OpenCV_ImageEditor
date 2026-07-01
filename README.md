Team2_project
This project is optimized for **Ubuntu Linux**. You will need a C++ compiler supporting **C++17**, CMake, and the OpenCV developer libraries.

To install all required dependencies at once, open your terminal and run:

```bash 
sudo apt update
sudo apt install build-essential cmake libopencv-dev
```
Follow these steps to compile the project locally without cluttering your source directories:
1)git clone https://github.com/SamvelProgramming/Team2_Project
  cd Team2_Project
  
2)mkdir build && cd build

3)cmake ..
  make



Usage

./image_processor <input_path> <output_path> [options]
