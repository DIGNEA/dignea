/**
 * @file PCA.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-10-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __PCA_H__
#define __PCA_H__

#include <vector>

#include "NumCpp.hpp"

namespace PCA {

/**
 * @brief This is the PCA components.T we obtained when using 5000 samples and
 * considering only the 8-dimensional features space. HERE WE DO NOT USE THE
 * INSTANCE DATA TO CREATE THE PCA and we ensure a balanced distribution of the
 * targets
 *
 */
nc::NdArray<float> componentsT = {
    {7.375311811592301370e-02, -3.178655266058625917e-01},
    {4.093087483174921992e-01, 4.955981249916805442e-01},
    {3.346693276473038936e-01, -6.128877541751576175e-01},
    {5.953059210206299579e-01, 1.328377255939875989e-01},
    {-5.162302414308733711e-02, -4.926638878583582404e-01},
    {5.983052657524839946e-01, -1.317157490269994513e-01},
};

/**
 * @brief This is the PCA mean we obtained using only 5000 samples and
 * considering only the 8-dimensional features space. HERE WE DO NOT USE THE
 * INSTANCE DATA TO CREATE THE PCA and we ensure a balanced distribution of the
 * targets
 *
 */
nc::NdArray<float> mean = {
    -1.302661682226850347e-16, 5.921189464667501929e-18,
    -2.107943449421630714e-15, 9.577523959099684045e-16,
    5.329070518200751197e-17,  -3.907985046680551042e-16,
};
/**
 * @brief This is the scaler mean we obtained using only 5000 samples and
 * considering only the 8-dimensional features space. HERE WE DO NOT USE THE
 * INSTANCE DATA TO CREATE THE PCA
 *
 */
nc::NdArray<float> scaler_mean = {
    1.079515125000000080e+04, 2.652374999999999972e+01,
    2.904959681383768952e+02, 5.040460461552937659e+02,
    1.784837500024586854e+00, 9.787104166666666742e+02,
};

/**
 * @brief This is the scaler scale obtained with only 5000 samples and
 * considering only the 8-dimensional features space. HERE WE DO NOT USE THE
 * INSTANCE DATA TO CREATE THE PCA
 *
 */
nc::NdArray<float> scaler_scale = {
    6.728206620826987091e+03, 2.899059219708179924e+01,
    1.302756303387655201e+01, 2.544038827736384079e+01,
    1.753588622119440776e+00, 1.909781553371280793e+01,
};

/**
 * @brief Scales the features using a pre-built StandardScaler
 *
 * @param sample
 * @return nc::NdArray<float>
 */
nc::NdArray<float> scale(const nc::NdArray<float>& sample) {
    auto result = sample - scaler_mean;
    result /= scaler_scale;
    return result;
}

/**
 * @brief Scales the features using a pre-built StandardScaler. This version is
 * for a batch of samples
 *
 * @param samples
 * @param extMean
 * @param extScale
 * @return nc::NdArray<float>
 */
nc::NdArray<float> scale(const nc::NdArray<float>& samples,
                         nc::NdArray<float>& extMean,
                         nc::NdArray<float>& extScale) {
    auto result = samples - extMean;
    result /= extScale;
    return result;
}

/**
 * @brief Transforms the sample using a PCA model previously built
 *
 * @param sample
 * @return nc::NdArray<float>
 */
nc::NdArray<float> transform(const nc::NdArray<float>& sample) {
    auto transformed = sample - mean;
    transformed = nc::dot<float>(transformed, componentsT);
    return transformed;
}

/**
 * @brief Transforms the sample using a PCA model previously built. This version
 * is for a batch of samples
 *
 * @param samples
 * @param extMean
 * @return nc::NdArray<float>
 */
nc::NdArray<float> transform(const nc::NdArray<float>& samples,
                             const nc::NdArray<float>& extMean) {
    auto transformed = samples - extMean;
    transformed = nc::dot<float>(transformed, componentsT);
    return transformed;
}

/**
 * @brief Applies the pipeline Standard Scaler plus PCA to the sample data
 *
 * @param samples
 * @return nc::NdArray<float>
 */
nc::NdArray<float> PCA(const std::vector<float>& sample) {
    nc::NdArray<float> sampleArray =
        nc::fromiter<float>(sample.begin(), sample.end());
    nc::NdArray<float> scaled = scale(sampleArray);
    return transform(scaled);
}

/**
 * @brief Applies the pipeline Standard Scaler plus PCA to the sample data
 *
 * @param samples
 * @return nc::NdArray<float>
 */
nc::NdArray<float> PCA(const nc::NdArray<float>& sample) {
    nc::NdArray<float> scaled = scale(sample);
    return transform(scaled);
}

/**
 * @brief Applies the pipeline Standard Scaler plus PCA to the batch of samples
 *
 * @param samples
 * @return nc::NdArray<float>
 */
nc::NdArray<float> PCABatch(const nc::NdArray<float>& samples) {
    auto toInsert = samples.numRows() - scaler_mean.numRows();
    auto extScalerMean = scaler_mean.copy();
    auto extScalerScale = scaler_scale.copy();
    auto extPCAMean = mean.copy();
    // Broadcasting the matrices to allow algebraic operations
    for (unsigned int i = 0; i < toInsert; i++) {
        extScalerMean = nc::append(extScalerMean, scaler_mean, nc::Axis::ROW);
        extScalerScale =
            nc::append(extScalerScale, scaler_scale, nc::Axis::ROW);
        extPCAMean = nc::append(extPCAMean, mean, nc::Axis::ROW);
    }
    nc::NdArray<float> scaled = scale(samples, extScalerMean, extScalerScale);
    return transform(scaled, extPCAMean);
}

}  // namespace PCA
#endif