#include "unittest.h"

#include "Undistort.h"

#include "Extractor.h"
#include "SimpleCameraCalibration.h"
#include "image_hash/average_hash.h"
#include <iostream>
#include <string>
#include <vector>

TEST_CASE( "UndistortTest/testUndistort", "[unit]" )
{
	cv::Mat img = cv::imread(TestCimbar::getSample("6bit/4_30_f0_627.jpg"));
	cv::Mat out;

	Undistort<SimpleCameraCalibration> und;
	assertTrue( und.undistort(img, out) );

	assertEquals( 0x622c50583c3c0c72, image_hash::average_hash(out) );
}

TEST_CASE( "UndistortTest/testUndistortAndExtract", "[unit]" )
{
	cv::Mat img = cv::imread(TestCimbar::getSample("6bit/4_30_f0_627.jpg"));
	cv::Mat out;

	Undistort<SimpleCameraCalibration> und;
	assertTrue( und.undistort(img, out) );

	Extractor ex;
	assertTrue( ex.extract(out, out) );

	assertEquals( 0x8326faca7727600, image_hash::average_hash(out) );
}
