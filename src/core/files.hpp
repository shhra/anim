#ifndef FILES_H_
#define FILES_H_

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

class Files {
public:
  Files() { root = std::filesystem::current_path(); }

  void load(std::string &path, std::string ext) {
    auto root = std::filesystem::path(path);
    // Iterate through this directory and load the animations.
    for (auto const &file : std::filesystem::directory_iterator(root)) {
      if (file.is_regular_file() &&
          std::filesystem::path(file).extension().string() == "." + ext) {
        std::cout << file << std::endl;
      }
    }
  }

  const std::vector<std::filesystem::path>& loaded() const {
    return files;
  }

private:
  std::filesystem::path root;
  std::vector<std::filesystem::path> files;
};

#endif // FILES_H_
