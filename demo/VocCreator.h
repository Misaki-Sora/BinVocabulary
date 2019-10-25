#ifndef VOC_CREATOR_H
#define VOC_CREATOR_H
#include <iostream>
#include <vector>
#include <string>

// DLoopDetector and DBoW2
#include "DLoopDetector.h" // defines BriefLoopDetector
#include <DVision/DVision.h> // Brief
#include <DBoW2/DBoW2.h>
#include <FBrief.h>
#include <DUtils/DUtils.h>
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

#include "demoDetector.h"

using namespace std;
using namespace DVision;

class VocCreator
{
public:
    VocCreator(std::string imagedir, std::string patternfile, int l, int k);
    void loadFeatures(std::vector<std::vector<FBrief::TDescriptor> > &features);
    void VocCreation(const std::vector<std::vector<FBrief::TDescriptor> > &features, std::string outputfile);
    int mK;
    int mL;
    std::string mImageDir;
    std::string mPatternFile;
};


class VocBriefExtractor
{
public:
    virtual void operator()(const cv::Mat &im, std::vector<cv::KeyPoint> &keys, std::vector<FBrief::TDescriptor> &descriptors) const;
    VocBriefExtractor(const std::string &pattern_file);

public:    
    DVision::BRIEF m_brief;
};
#endif