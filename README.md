# SearchRPI

This repository contains the core components of SearchRPI, responsible for handling the user-facing functionality of the search engine. It integrates all the essential systems required to deliver relevant and ranked search results to users.

## Getting Started

1. **Clone the repository:**
    ```bash
    git clone https://github.com/SearchRPI/SearchRPI.git
    cd SearchRPI
    ```

2. **Install Dependencies**
    ```bash
    sudo apt-get update
    sudo apt-get install -y cmake g++ make pkg-config liblmdb-dev
    ```

3. **Build the project:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make -j$(nproc)
    ```

4. **Run tests:**
    ```bash
    ./all_tests
    ```
