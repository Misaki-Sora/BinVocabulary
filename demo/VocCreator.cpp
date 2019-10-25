#include <iostream>
#include <vector>
#include <string>

// DLoopDetector and DBoW2
// #include <DBoW2/DBoW2.h> // defines BriefVocabulary
// #include <DVision/DVision.h> // Brief

// OpenCV
// #include <opencv2/core.hpp>
// #include <opencv2/highgui.hpp>
// #include "demoDetector.h"
#include "VocCreator.h"

VocCreator::VocCreator(std::string imagedir, std::string patternfile, int l, int k):
mImageDir(imagedir),
mPatternFile(patternfile),
mL(l),
mK(k)
{
}

VocBriefExtractor::VocBriefExtractor(const std::string &pattern_file)
{
    // The DVision::BRIEF extractor computes a random pattern by default when
    // the object is created.
    // We load the pattern that we used to build the vocabulary, to make
    // the descriptors compatible with the predefined vocabulary

    // loads the pattern
    cv::FileStorage fs(pattern_file.c_str(), cv::FileStorage::READ);
    if(!fs.isOpened()) throw string("Could not open file ") + pattern_file;

    vector<int> x1, y1, x2, y2;
    fs["x1"] >> x1;
    fs["x2"] >> x2;
    fs["y1"] >> y1;
    fs["y2"] >> y2;

    m_brief.importPairs(x1, y1, x2, y2);
}

void VocBriefExtractor::operator() (const cv::Mat &im, vector<cv::KeyPoint> &keys, std::vector<FBrief::TDescriptor> &descriptors) const
{
    m_brief.compute(im, keys, (vector<BRIEF::bitset> &) (descriptors));
}

void VocCreator::loadFeatures(std::vector<std::vector<FBrief::TDescriptor> >&features)
{
    std::vector<std::string> filenames = DUtils::FileFunctions::Dir(mImageDir.c_str(), ".bmp", true);
    features.clear();
    features.reserve(filenames.size());
    VocBriefExtractor extractor(mPatternFile);
    for(unsigned int i = 0; i < filenames.size(); ++i)
    {
        cv::Mat im = cv::imread(filenames[i].c_str(), 0);
        std::vector<FBrief::TDescriptor> descriptor;
        vector<cv::KeyPoint> keypoints;
        const int fast_th = 20;
        cv::FAST(im, keypoints, fast_th, true);
        extractor(im, keypoints, descriptor);
        features.push_back(descriptor);
    }
}


void VocCreator::VocCreation(const std::vector<std::vector<FBrief::TDescriptor> >  &features, std::string outputfile)
{
    // branching factor and depth levels 
    const WeightingType weight = TF_IDF;
    const ScoringType score = L1_NORM;
    BriefVocabulary voc(mK, mL, weight, score);
    voc.create(features);
    // lets do something with this vocabulary
    // save the vocabulary to disk
    std::cout << endl << "Saving vocabulary..." << std::endl;
    voc.save(outputfile);
    cout << "Vocabulary information: " << endl
         << voc << endl << endl;
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        cerr << endl << "Usage: ./VocCreator  image_data_path output_path" << endl;
        return 1;
    }
    else
    {
       cout << "***********************" << endl;
        cout << "  Welcome to VocCreator !" << endl;
        cout << "***********************" << endl;
        std::string dataset(argv[1]);
        std::string outputfile(argv[2]);
        // std::string dataset("/home/matrix/Downloads/TY_2324_10HZ_NAE_401_9.15_2_Dataset/dow");
        // std::string outputfile("test1.bin");
        std::vector<std::vector<FBrief::TDescriptor> >features;
        std::string patternfile = "/home/matrix/VINS_WS/src/VINS-Mono/support_files/brief_pattern.yml";
        int k = 10;
        int l = 6;
        VocCreator creater(dataset, patternfile, l, k);
        creater.loadFeatures(features);
        creater.VocCreation(features, outputfile);
}
}