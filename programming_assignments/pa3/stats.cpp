#include "stats.h"

stats::stats(PNG & im){
	sumRed.resize(im.height());
	sumGreen.resize(im.height());
	sumBlue.resize(im.height());
	sumsqRed.resize(im.height());
	sumsqGreen.resize(im.height());
	sumsqBlue.resize(im.height());
	for (int y = 0; y < (int) im.height(); y++) {
		sumRed[y].resize(im.width());
		sumGreen[y].resize(im.width());
		sumBlue[y].resize(im.width());
		sumsqRed[y].resize(im.width());
		sumsqGreen[y].resize(im.width());
		sumsqBlue[y].resize(im.width());
		for (int x = 0; x < (int) im.width(); x++) {
			RGBAPixel* pixel = im.getPixel(x, y);
			sumRed[y][x] 				= pixel->r;
			sumGreen[y][x] 			= pixel->g;
			sumBlue[y][x] 			= pixel->b;
			sumsqRed[y][x] 			= pixel->r * pixel->r;
			sumsqGreen[y][x] 		= pixel->g * pixel->g;
			sumsqBlue[y][x] 		= pixel->b * pixel->b;
			if (x == 0 && y > 0) {
				sumRed[y][x] 			+= sumRed[y-1][x];
				sumGreen[y][x] 		+= sumGreen[y-1][x];
				sumBlue[y][x] 		+= sumBlue[y-1][x];
				sumsqRed[y][x] 		+= sumsqRed[y-1][x];
				sumsqGreen[y][x] 	+= sumsqGreen[y-1][x];
				sumsqBlue[y][x] 	+= sumsqBlue[y-1][x];
			} else if (x > 0 && y == 0) {
				sumRed[y][x] 			+= sumRed[y][x-1];
				sumGreen[y][x] 		+= sumGreen[y][x-1];
				sumBlue[y][x] 		+= sumBlue[y][x-1];
				sumsqRed[y][x] 		+= sumsqRed[y][x-1];
				sumsqGreen[y][x] 	+= sumsqGreen[y][x-1];
				sumsqBlue[y][x] 	+= sumsqBlue[y][x-1];
			} else if (x > 0 && y > 0) {
				sumRed[y][x] 			+= sumRed[y-1][x] + sumRed[y][x-1] - sumRed[y-1][x-1];
				sumGreen[y][x] 		+= sumGreen[y-1][x] + sumGreen[y][x-1] - sumGreen[y-1][x-1];
				sumBlue[y][x] 		+= sumBlue[y-1][x] + sumBlue[y][x-1] - sumBlue[y-1][x-1];
				sumsqRed[y][x] 		+= sumsqRed[y-1][x] + sumsqRed[y][x-1] - sumsqRed[y-1][x-1];
				sumsqGreen[y][x] 	+= sumsqGreen[y-1][x] + sumsqGreen[y][x-1] - sumsqGreen[y-1][x-1];
				sumsqBlue[y][x] 	+= sumsqBlue[y-1][x] + sumsqBlue[y][x-1] - sumsqBlue[y-1][x-1];
			}
		}
	}
}

long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
	if (channel == 'r') {
		if (ul.first == 0 && ul.second == 0) {
			return sumRed[lr.second][lr.first];
		} else if (ul.first == 0 && ul.second > 0) {
			return sumRed[lr.second][lr.first] - sumRed[ul.second-1][lr.first];
		} else if (ul.first > 0 && ul.second == 0) {
			return sumRed[lr.second][lr.first] - sumRed[lr.second][ul.first-1];
		} else {
			return sumRed[lr.second][lr.first] - sumRed[lr.second][ul.first-1] - sumRed[ul.second-1][lr.first] + sumRed[ul.second-1][ul.first-1];
		}
	} else if (channel == 'g') {
		if (ul.first == 0 && ul.second == 0) {
			return sumGreen[lr.second][lr.first];
		} else if (ul.first == 0 && ul.second > 0) {
			return sumGreen[lr.second][lr.first] - sumGreen[ul.second-1][lr.first];
		} else if (ul.first > 0 && ul.second == 0) {
			return sumGreen[lr.second][lr.first] - sumGreen[lr.second][ul.first-1];
		} else {
			return sumGreen[lr.second][lr.first] - sumGreen[lr.second][ul.first-1] - sumGreen[ul.second-1][lr.first] + sumGreen[ul.second-1][ul.first-1];
		}
	} else {
		if (ul.first == 0 && ul.second == 0) {
			return sumBlue[lr.second][lr.first];
		} else if (ul.first == 0 && ul.second > 0) {
			return sumBlue[lr.second][lr.first] - sumBlue[ul.second-1][lr.first];
		} else if (ul.first > 0 && ul.second == 0) {
			return sumBlue[lr.second][lr.first] - sumBlue[lr.second][ul.first-1];
		} else {
			return sumBlue[lr.second][lr.first] - sumBlue[lr.second][ul.first-1] - sumBlue[ul.second-1][lr.first] + sumBlue[ul.second-1][ul.first-1];
		}
	}
}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
	if (channel == 'r') {
		if (ul.first == 0 && ul.second == 0) {
			return sumsqRed[lr.second][lr.first];
		} else if (ul.first == 0 && ul.second > 0) {
			return sumsqRed[lr.second][lr.first] - sumsqRed[ul.second-1][lr.first];
		} else if (ul.first > 0 && ul.second == 0) {
			return sumsqRed[lr.second][lr.first] - sumsqRed[lr.second][ul.first-1];
		} else {
			return sumsqRed[lr.second][lr.first] - sumsqRed[lr.second][ul.first-1] - sumsqRed[ul.second-1][lr.first] + sumsqRed[ul.second-1][ul.first-1];
		}
	} else if (channel == 'g') {
		if (ul.first == 0 && ul.second == 0) {
			return sumsqGreen[lr.second][lr.first];
		} else if (ul.first == 0 && ul.second > 0) {
			return sumsqGreen[lr.second][lr.first] - sumsqGreen[ul.second-1][lr.first];
		} else if (ul.first > 0 && ul.second == 0) {
			return sumsqGreen[lr.second][lr.first] - sumsqGreen[lr.second][ul.first-1];
		} else {
			return sumsqGreen[lr.second][lr.first] - sumsqGreen[lr.second][ul.first-1] - sumsqGreen[ul.second-1][lr.first] + sumsqGreen[ul.second-1][ul.first-1];
		}
	} else {
		if (ul.first == 0 && ul.second == 0) {
			return sumsqBlue[lr.second][lr.first];
		} else if (ul.first == 0 && ul.second > 0) {
			return sumsqBlue[lr.second][lr.first] - sumsqBlue[ul.second-1][lr.first];
		} else if (ul.first > 0 && ul.second == 0) {
			return sumsqBlue[lr.second][lr.first] - sumsqBlue[lr.second][ul.first-1];
		} else {
			return sumsqBlue[lr.second][lr.first] - sumsqBlue[lr.second][ul.first-1] - sumsqBlue[ul.second-1][lr.first] + sumsqBlue[ul.second-1][ul.first-1];
		}
	}
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
	int width = lr.first - ul.first + 1;
	int height = lr.second - ul.second + 1;
	return width * height;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
long stats::getScore(pair<int,int> ul, pair<int,int> lr){
	long t1 = getSumSq('r', ul, lr) + getSumSq('g', ul, lr) + getSumSq('b', ul, lr);
	long t2 = (getSum('r', ul, lr)*getSum('r', ul, lr) + getSum('g', ul, lr)*getSum('g', ul, lr) + getSum('b', ul, lr)*getSum('b', ul, lr)) / rectArea(ul, lr);
	return t1 - t2;
}

RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	int n = rectArea(ul, lr);
	int redAverage = getSum('r', ul, lr) / n;
	int greenAverage = getSum('g', ul, lr) / n;
	int blueAverage = getSum('b', ul, lr) / n;
	RGBAPixel* pixel = new RGBAPixel(redAverage, greenAverage, blueAverage);
	return *pixel;
}
