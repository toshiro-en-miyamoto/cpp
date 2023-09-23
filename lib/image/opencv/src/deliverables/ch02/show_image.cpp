#include <cxxopts.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv)
{
  cxxopts::Options options("show_image", "OpenCV first program");
  options.add_options()
    ("filename", "The filename to process", cxxopts::value<std::vector<std::string>>());
  options.parse_positional({"filename"});
  auto opt = options.parse(argc, argv);

  if (opt.count("filename") == 0) {
    std::cout << options.help() << std::endl;
    exit(-1);
  }
  std::string filename = opt["filename"].as<std::string>();
  cv::Mat image = cv::imread(filename.c_str(), cv::IMREAD_COLOR);
}
