#pragma once

#include <vector>
#include <limits>

// ---------------------------------------------------------------------------------------------------------------------
// - This header contains methods used to perform the wavelength calibration of a spectrometer using a ransac approach -
// ---------------------------------------------------------------------------------------------------------------------

namespace novac
{

class CSpectrum;
struct SpectrumDataPoint;

/// <summary>
/// A Correspondence is an essential part in the ransac calibration routine, it
/// represents a connection between a point in the measured spectrum and a point
/// in the theoretical fraunhofer spectrum.
/// </summary>
struct Correspondence
{
    Correspondence()
        : measuredIdx(0), theoreticalIdx(0), error(0.0)
    { }

    Correspondence(size_t measured, size_t theoretical, double error)
        : measuredIdx(measured), theoreticalIdx(theoretical), error(error)
    { }

    /// <summary>
    /// The index of the keypoint in the measured spectrum
    /// </summary>
    size_t measuredIdx = 0;

    /// <summary>
    /// The value of the keypoint in the measured spectrum (pixel, in the case of wavelength calibration)
    /// </summary>
    double measuredValue = 0.0;

    /// <summary>
    /// The index of the keypoint in the theoretical spectrum
    /// </summary>
    size_t theoreticalIdx = 0;

    /// <summary>
    /// The value of the keypoint in the theoretical spectrum (wavelength, in the case of wavelength calibration)
    /// </summary>
    double theoreticalValue = 0.0;

    /// <summary>
    /// An error measure between the keypoints in the two spectra, lower is better
    /// </summary>
    double error = 0.0;
};


// ------------- Keypoint selection and preparation -------------

/// <summary>
/// A collection of the settings necessary to determine which keypoints
/// will make up good correspondences between the measured and fraunhofer spectra.
/// </summary>
struct CorrespondenceSelectionSettings
{
    /// <summary>
    /// The width, in pixels, around each keypoint which will be used to gauge the error in the correspondence.
    /// The default value of 20 is retrieved as 2x the average keypoint distance in tested spectra and should hence cover the entire width of a valley / peak.
    /// </summary>
    size_t pixelRegionSizeForCorrespondenceErrorMeasurement = 20;

    /// <summary>
    /// The relative number of correspondences to select out of the total.
    /// 0.2 corresponds to selecting the 20% correspondences with the lowest error.
    /// </summary>
    double percentageOfCorrespondencesToSelect = 0.2;

    /// <summary>
    /// The first pixel to include in the calibration routine. 
    /// Often do the signal in the spectra decline at short wavelengths and this is a means to disregard points with low intensity.
    /// </summary>
    size_t measuredPixelStart = 650; // TODO: Lower

    /// <summary>
    /// The last pixel to include in the calibration routine. 
    /// Often do the signal in the spectra decline at short wavelengths and this is a means to disregard points with low intensity.
    /// This must be larger than measuredPixelStart.
    /// </summary>
    size_t measuredPixelStop = 2100; // TODO: Raise
};

/// <summary>
/// Measures the similarity between the two spectra at the two indices given by the correspondence.
/// The similarity is measured as a sum of squared differences between the two 
/// spectra in the region around the given pixels.
/// A lower return value corresponds to a higher similarity
/// <param name="corr">The Correspondence to measure the error of. This will be updated with the measured error.</param>
/// <param name="measuredSpectrum">The measured spectrum of the correspondence.</param>
/// <param name="theoreticalSpectrum">The theoretical spectrum of the correspondence.</param>
/// </summary>
void MeasureCorrespondenceError(novac::Correspondence& corr, const CSpectrum& measuredSpectrum, const CSpectrum& theoreticalSpectrum, const CorrespondenceSelectionSettings& settings);

struct RansacWavelengthCalibrationSettings;
/// <summary>
/// This should be run as a preparatory step before the Ransac algorithm can be run.
/// This generates the list of all reasonable correspondences between the measured and 
/// fraunhofer spectra based on keypoints found in the two spectra.
/// </summary>
/// <param name="measuredKeypoints">The keypoints found in the measured spectrum.</param>
/// <param name="measuredSpectrum">The measured spectrum itself.</param>
/// <param name="fraunhoferKeypoints">The keypoints found in the fraunhofer spectrum.</param>
/// <param name="fraunhoferSpectrum">The fraunhofer spectrum itself</param>
/// <param name="settings">The settings for the following Ransac wavelength calibration</param>
/// <param name="percentageOfCorrespondencesToSelect">A hard limit of how</param>
/// <returns></returns>
std::vector<novac::Correspondence> ListPossibleCorrespondences(
    const std::vector<novac::SpectrumDataPoint>& measuredKeypoints,
    const CSpectrum& measuredSpectrum,
    const std::vector<novac::SpectrumDataPoint>& fraunhoferKeypoints,
    const CSpectrum& fraunhoferSpectrum,
    const RansacWavelengthCalibrationSettings& ransacSettings,
    const CorrespondenceSelectionSettings& correspondenceSettings);


// TODO: Move
double PolynomialValueAt(const std::vector<double>& coefficients, double x);

// ------------- Wavelength calibration by Ransac  -------------

struct RansacWavelengthCalibrationSettings
{
    size_t modelPolynomialOrder = 3;
    int numberOfRansacIterations = 500000;
    size_t sampleSize = 4; // the number of correspondences to select in one iteration - should really be (modelPolynomialOrder + 1)
    double inlierLimitInWavelength = 0.2; // how close a keypoint needs to be for it to be considered an inlier. In nm
    int maximumPixelDistanceForPossibleCorrespondence = 150; // maximum pixel error in the initial clb file.
    bool refine = true;
};

struct RansacWavelengthCalibrationResult
{
    RansacWavelengthCalibrationResult(size_t polynomialOrder);

    RansacWavelengthCalibrationResult(const RansacWavelengthCalibrationResult& other);
    RansacWavelengthCalibrationResult& operator=(const RansacWavelengthCalibrationResult& other);

    /** The best estimation of the pixel-to-wavelength mapping polynomial that we have.
        The coefficients make up a polynomial and are stored with the 0th order coefficient first. */
    std::vector<double> bestFittingModelCoefficients;

    /** The order of the 'bestFittingModelCoefficients' */
    size_t modelPolynomialOrder = 3;

    /** The number of inliers which were achieved with this model */
    size_t highestNumberOfInliers = 0U;

    /// <summary>
    /// Lists which of the incoming correspondences is an inlier.
    /// The number of true elements in this vector equals 'highestNumberOfInliers'
    /// </summary>
    std::vector<bool> correspondenceIsInlier;

    /** The smallest error in the model, by using the inliers */
    double smallestError = std::numeric_limits<double>::max();

    /** The total number of possible correlations, the maximum number for 'highestNumberOfInliers' */
    size_t numberOfPossibleCorrelations = 0U;
};

/// <summary>
/// RansacWavelengthCalibrationSetup is the setup of a calibration run
///     and contains all necessary elements to perform the calibration
///     by using a set of correspondences between a measured and an already calibrated spectrum.
/// </summary>
class RansacWavelengthCalibrationSetup
{
public:
    RansacWavelengthCalibrationSetup(RansacWavelengthCalibrationSettings calibrationSettings);

    /// <summary>
    /// This performs the actual calibration of a measured spectrum against a 
    ///   high resolution fraunhofer spectrum.
    /// </summary>
    /// <param name="possibleCorrespondences"></param>
    /// <param name="measuredKeypoints"></param>
    /// <param name="fraunhoferKeypoints"></param>
    /// <returns>The result of the calibration</returns>
    RansacWavelengthCalibrationResult DoWavelengthCalibration(const std::vector<Correspondence>& possibleCorrespondences);

private:

    RansacWavelengthCalibrationSettings settings;

};

}