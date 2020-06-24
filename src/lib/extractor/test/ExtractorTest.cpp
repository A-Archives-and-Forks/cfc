#include "unittest.h"

#include "Extractor.h"
#include "image_hash/average_hash.h"
#include "util/MakeTempDirectory.h"
#include <iostream>
#include <string>
#include <vector>

TEST_CASE( "ExtractorTest/testExtract", "[unit]" )
{
	MakeTempDirectory tempdir;

	std::string imgPath = tempdir.path() / "ex.jpg";
	Extractor ext;
	ext.extract(TestCimbar::getSample("4color1.jpg"), imgPath);

	cv::Mat out = cv::imread(imgPath);
	assertEquals( 0x434caab59f7e56c7, image_hash::average_hash(out) );
}

TEST_CASE( "ExtractorTest/testExtractMid", "[unit]" )
{
	MakeTempDirectory tempdir;

	std::string imgPath = tempdir.path() / "ex.jpg";
	Extractor ext;
	ext.extract(TestCimbar::getSample("4c-cam-40-f1.jpg"), imgPath);

	cv::Mat out = cv::imread(imgPath);
	assertEquals( 0xe0d3b5b3f6b07075, image_hash::average_hash(out) );
}

TEST_CASE( "ExtractorTest/testExtractUpscale", "[unit]" )
{
	MakeTempDirectory tempdir;

	std::string imgPath = tempdir.path() / "exup.jpg";
	Extractor ext;
	ext.extract(TestCimbar::getSample("4color-cam-140.jpg"), imgPath);

	cv::Mat out = cv::imread(imgPath);
	assertEquals( 0x44c7f08cfd8f4c, image_hash::average_hash(out) );
}

