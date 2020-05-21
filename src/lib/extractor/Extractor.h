#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class Extractor
{
public:
	Extractor();

	bool sharpen(cv::Mat& out);

	bool extract(const cv::Mat& img, cv::Mat& out);
	bool extract(std::string read_path, cv::Mat& out);
	bool extract(std::string read_path, std::string write_path);

protected:
};
