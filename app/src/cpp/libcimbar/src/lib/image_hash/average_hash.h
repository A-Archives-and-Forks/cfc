/* This code is subject to the terms of the Mozilla Public License, v.2.0. http://mozilla.org/MPL/2.0/. */
#pragma once

#include "ahash_result.h"
#include "bit_extractor.h"
#include "bit_file/bitmatrix.h"
#include "cimb_translator/Cell.h"

#include "intx/int128.hpp"
#include <opencv2/opencv.hpp>

#include <array>
#include <bitset>
#include <cstdint>

namespace image_hash
{
	inline uint64_t average_hash(const cv::Mat& img, uchar threshold=0)
	{
		cv::Mat gray = img;
		if (img.channels() != 1)
			cv::cvtColor(gray, gray, cv::COLOR_RGB2GRAY);
		if (gray.cols != 8 or gray.rows != 8)
			cv::resize(gray, gray, cv::Size(8, 8));

		if (threshold == 0)
			threshold = Cell(gray).mean_grayscale();

		uint64_t res = 0;
		int bitpos = 63; // 8*8 - 1
		for (int i = 0; i < gray.rows; ++i)
		{
			const uchar* p = gray.ptr<uchar>(i);
			for (int j = 0; j < gray.cols; ++j, --bitpos)
				res |= (uint64_t)(p[j] > threshold) << bitpos;
		}
		return res;
	}

	inline ahash_result special_case_fuzzy_ahash(const cv::Mat& gray, unsigned mode)
	{
		// if the Mat is already 10x10 and threshold'd to (0, 0xFF), we can do some things...
		intx::uint128 res(0);
		int bitpos = 95; // 10*10 - 5
		for (int i = 0; i < gray.rows; ++i)
		{
			const uchar* p = gray.ptr<uchar>(i);
			for (int j = 0; j < gray.cols; j+=5, bitpos-=5)
			{
				const uint64_t* hax = reinterpret_cast<const uint64_t*>(p+j);
				uint64_t mval = (*hax) & 0x101010101ULL;
				const uint8_t* cv = reinterpret_cast<const uint8_t*>(&mval);
				uint8_t val = cv[0] << 4 | cv[1] << 3 | cv[2] << 2 | cv[3] << 1 | cv[4];
				res |= intx::uint128(val) << bitpos;
			}
		}
		return ahash_result(res, mode);
	}

	// need something like a bitset_extractor(), with an api like:
	// bits.extract(0, 8,  9, 17,  18, 26,  27, 35,  36, 44,  45, 53,  54, 62,  63, 71)
	//  ... probably using template magic. For our purposes, we need >= 8 pairs of bit indices, which will sum to a total of 64 bits
	// or maybe a better api is:
	// bits.extract(0, 9, 18, 27, 36, 45, 54, 63)
	//  ... with each index corresponding to an 8 bit read?
	//  ... this way, we could do compile time validation that the return value makes sense.
	//  ... e.g. if we have 8 params, that means it's a 64 bit number being returned.
	inline ahash_result fuzzy_ahash(const cv::Mat& img, uchar threshold=0, unsigned mode=ahash_result::ALL)
	{
		// return 9 uint64_ts, each representing an 8x8 section of the 10x10 img
		cv::Mat gray = img;
		if (img.channels() != 1)
			cv::cvtColor(gray, gray, cv::COLOR_RGB2GRAY);
		if (gray.cols != 10 or gray.rows != 10)
			cv::resize(gray, gray, cv::Size(10, 10));

		if (threshold == 0)
			threshold = Cell(gray).mean_grayscale();
		else if (threshold == 0xFF)
			return special_case_fuzzy_ahash(gray, mode);

		intx::uint128 res(0);
		int bitpos = 99; // 10*10 - 1
		for (int i = 0; i < gray.rows; ++i)
		{
			const uchar* p = gray.ptr<uchar>(i);
			for (int j = 0; j < gray.cols; ++j, --bitpos)
				res |= intx::uint128(p[j] > threshold) << bitpos;
		}
		return ahash_result(res, mode);
	}

	inline ahash_result fuzzy_ahash(const bitmatrix& img, unsigned mode=ahash_result::ALL)
	{
		intx::uint128 res(0);
		int bitpos = 90; // 10*10 - 10
		for (int i = 0; i < 10; ++i, bitpos-=10)
		{
			unsigned r = img.get(0, i, 10);
			res |= intx::uint128(r) << bitpos;
		}
		return ahash_result(res, mode);
	}
}
