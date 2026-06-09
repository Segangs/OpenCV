#pragma once
#include <opencv2/opencv.hpp>

namespace Color{
    inline const cv::Scalar Black{0, 0, 0};
    inline const cv::Scalar White{255, 255, 255};

    inline const cv::Scalar Red{0, 0, 255};
    inline const cv::Scalar Green{0, 255, 0};
    inline const cv::Scalar Blue{255, 0, 0};

    inline const cv::Scalar Yellow{0, 255, 255};
    inline const cv::Scalar Cyan{255, 255, 0};
    inline const cv::Scalar Magenta{255, 0, 255};

    inline const cv::Scalar Gray{128, 128, 128};
    inline const cv::Scalar Orange{0, 165, 255};
    inline const cv::Scalar Purple{128, 0, 128};
}