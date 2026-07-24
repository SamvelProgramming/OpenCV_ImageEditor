
# Team2_Project: C++ Image Processor

A lightweight, high-performance Command Line Interface (CLI) tool for image manipulation, built with **C++17** and **OpenCV**. This project is optimized for Ubuntu Linux environments.

---

## Features
* **Grayscale Conversion:** Convert color images to high-quality grayscale.
* **Gaussian Blur:** Apply customizable smoothing and noise reduction.
* **Image Resizing:** Scale images to specific pixel dimensions dynamically.
* **Chained Operations:** Run multiple filters sequentially in a single command execution.

---

## Prerequisites & Installation

### 1. System Dependencies
Ensure your package lists are updated and install the required build tools, CMake, and the OpenCV developer libraries:


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

 **[Team 2 Presentation on Prezi](https://prezi.com/view/DZLCWydNxKAEmezSNen8/?referral_token=lfccoClnB3FN)**
