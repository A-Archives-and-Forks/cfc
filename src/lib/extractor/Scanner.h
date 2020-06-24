#pragma once

#include "Anchor.h"
#include "Point.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

class Corners;
class Midpoints;

class Scanner
{
public: // public interface
	Scanner(const cv::Mat& img, bool dark=true, int skip=17);
	int anchor_size() const;

	std::vector<Anchor> scan();
	std::vector<point<int>> scan_edges(const Corners& corners, Midpoints& mps) const;


public: // other interesting methods
	static cv::Mat preprocess_image(const cv::Mat& img);
	std::vector<Anchor> deduplicate_candidates(const std::vector<Anchor>& candidates) const;
	void filter_candidates(std::vector<Anchor>& candidates) const;

	std::vector<Anchor> t1_scan_rows() const;
	std::vector<Anchor> t2_scan_columns(const std::vector<Anchor>& candidates) const;
	std::vector<Anchor> t3_scan_diagonal(const std::vector<Anchor>& candidates) const;
	std::vector<Anchor> t4_confirm_scan(const std::vector<Anchor>& candidates) const;
	bool sort_top_to_bottom(std::vector<Anchor>& points);

protected: // internal member functions
	bool test_pixel(int x, int y) const;

	bool scan_horizontal(std::vector<Anchor>& points, int y, int xstart=-1, int xend=-1) const;
	bool scan_vertical(std::vector<Anchor>& points, int x, int xmax=-1, int ystart=-1, int yend=-1) const;
	void scan_diagonal(std::vector<Anchor>& points, int xstart, int xend, int ystart, int yend) const;

	// edge detection
	bool chase_edge(const point<double>& start, const point<double>& unit) const;
	point<int> find_edge(const point<int>& u, const point<int>& v, point<double> mid) const;

protected:
	cv::Mat _img;
	bool _dark;
	int _skip;
	int _mergeCutoff;
	int _anchorSize;
};

