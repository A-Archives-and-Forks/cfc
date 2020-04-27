#include "unittest.h"

#include "average_hash.h"

#include "cimb_translator/Common.h"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;

TEST_CASE( "averageHashTest/testLight", "[unit]" )
{
	string dir = TestCimbar::getProjectDir();
	cv::Mat tile = cimbar::getTile(4, 0, false, 0, dir);
	assertEquals(0xfefcf8f0e0c08000, image_hash::average_hash(tile));

	tile = cimbar::getTile(4, 0, false);
	assertEquals(0xfefcf8f0e0c08000, image_hash::average_hash(tile));
}

TEST_CASE( "averageHashTest/testDark", "[unit]" )
{
	string dir = TestCimbar::getProjectDir();
	cv::Mat tile = cimbar::getTile(4, 0, true, 0, dir);
	assertEquals(0x103070f1f3f7fff, image_hash::average_hash(tile));

	tile = cimbar::getTile(4, 0, true);
	assertEquals(0x103070f1f3f7fff, image_hash::average_hash(tile));
}

TEST_CASE( "averageHashTest/testResize", "[unit]" )
{
	string dir = TestCimbar::getProjectDir();
	cv::Mat tile = cimbar::getTile(4, 0, true, 0, dir);

	cv::Mat big;
	cv::resize(tile, big, cv::Size(32, 32));
	assertEquals(0x103070f1f3f7fff, image_hash::average_hash(big));
}
