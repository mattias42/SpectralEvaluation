#pragma once

#include <vector>

class CSpectrum;

// Simple structure used to represent a point in a spectrum
struct SpectrumDataPoint
{
    // The spectrometer pixel where this point is found
    double pixel = 0.0;

    // The wavelength (in nm) where this point is found, set to zero if not known
    double wavelength = 0.0;

    // The intensity value of the spectrum at this point
    double intensity = 0.0;

    // A type enumerator, may be used to classify points
    int type = 0;
};

/**
 * @brief Locates all significant peaks in the provided spectrum and returns the result in the provided vector
 * @param spectrum The spectrum in which peaks should be found,
 *  if this has a wavelength calibration then the resulting points will have a wavelength filled in.
 * @param minimumIntensity Only peaks with a maximum intensity above this value will be returned.
 * @param result Will on return be filled with the found peaks.
*/
void FindPeaks(const CSpectrum& spectrum, double minimumIntensity, std::vector<SpectrumDataPoint>& result);

/**
 * @brief Locates all significant valleys in the provided spectrum and returns the result in the provided vector
 * @param spectrum The spectrum in which valleys should be found,
 *  if this has a wavelength calibration then the resulting points will have a wavelength filled in.
 * @param minimumIntensity Only valleys with a maximum intensity above this value will be returned.
 * @param result Will on return be filled with the found valleys.
*/
void FindValleys(const CSpectrum& spectrum, double minimumIntensity, std::vector<SpectrumDataPoint>& result);

/**
 * @brief Calculates a first order derivative on the provided data array wrt index value using finite difference.
 * @param data The data array (spectrum) to calculate the derivative of.
 * @param dataLength The length of data.
 * @param result Will on successful result be filled with the calculated first order derivative.
 *       This will be reallocated to dataLength long and the first and last elements will be set to zero
 * @returns true if all is ok
*/
bool Derivative(const double* data, size_t dataLength, std::vector<double>& result);