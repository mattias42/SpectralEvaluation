#pragma once

#include <memory>
#include <vector>
#include <string>
#include <utility>

// ---------------------------------------------------------------------------------------------------------------
// ---- This header contains FraunhoferSpectrumGeneration which helps with convolving Fraunhofer references  -----
// ---------------------------------------------------------------------------------------------------------------

namespace novac
{
    class CSpectrum;
    class CCrossSectionData;
    struct WavelengthRange;

    class IFraunhoferSpectrumGenerator
    {
    public:

        /** Returns the wavelength range over which subsequent calls to 'GetFraunhoferSpectrum' with the provided
            pixel-to-wavelength calibration will be valid. This is determined both by the provided range and by the
            range of the included solar atlas. */
        virtual WavelengthRange GetFraunhoferRange(const std::vector<double>& wavelengthCalibration) = 0;

        /** Creates a Fraunhofer reference spectrum using the provided pixel-to-wavelength mapping and measured instrument line shape.
            This will determine the fwhm of the provided instrument line shape and use this value to determine the convolution grid.
            @param pixelToWavelengthMapping The wavelength (in nm air) for each pixel on the detector.
            @param measuredInstrumentLineShape A measurement of the instrument line shape
            @return The high resolution solar spectrum convolved with the measured slf and resample to the provided grid.  */
        virtual std::unique_ptr<CSpectrum> GetFraunhoferSpectrum(
            const std::vector<double>& wavelengthCalibration,
            const novac::CCrossSectionData& measuredInstrumentLineShape) = 0;

        /** Creates a Fraunhofer reference spectrum using the provided pixel-to-wavelength mapping and measured instrument line shape.
            This will determine the fwhm of the provided instrument line shape and use this value to determine the convolution grid.
            @param pixelToWavelengthMapping The wavelength (in nm air) for each pixel on the detector.
            @param measuredInstrumentLineShape A measurement of the instrument line shape
            @return The high resolution solar spectrum convolved with the measured slf and resample to the provided grid.  */
        virtual std::unique_ptr<CSpectrum> GetFraunhoferSpectrum(
            const std::vector<double>& wavelengthCalibration,
            const novac::CCrossSectionData& measuredInstrumentLineShape,
            double fwhmOfInstrumentLineShape,
            bool normalize) = 0;

        /** Creates a Fraunhofer reference spectrum using the provided pixel-to-wavelength mapping and differential instrument line shape.
            @param pixelToWavelengthMapping The wavelength (in nm air) for each pixel on the detector.
            @param measuredInstrumentLineShape A measurement of the instrument line shape.
            @param fwhmOfInstrumentLineShape>The Full Width at Half Maximum of the provided instrument line shape.
            @return The high resolution solar spectrum convolved with the measured slf and resample to the provided grid.  */
        virtual std::unique_ptr<CSpectrum> GetDifferentialFraunhoferSpectrum(
            const std::vector<double>& wavelengthCalibration,
            const novac::CCrossSectionData& measuredInstrumentLineShape,
            double fwhmOfInstrumentLineShape) = 0;
    };

    /** This is a helper class for generating a Fraunhofer spectrum from a high resolved
        solar spectrum, a likewise high resolved ozone spectrum and a given instrument setup.
        Notice that this class will read in the high-resolved solar spectrum when needed (calling GetFraunhoferSpectrum)
        and will keep it in memory to save loading time. If memory is a consern, then make sure that this object gets destructed when no longer needed.  */
    class FraunhoferSpectrumGeneration : public IFraunhoferSpectrumGenerator
    {
    public:
        /** Sets up the generation parameters
            @param highResolutionSolarAtlas The full path to the high resolved solar atlas. This must be in nm air.
            @param highResolutionCrossSections The full path to a set of high resolved molecular cross section together with the total column for them.
                These must have x-axis unit of nm air and y-axis unit of molecules / cm2  */
        FraunhoferSpectrumGeneration(const std::string& highResolutionSolarAtlas, const std::vector<std::pair<std::string, double>>& highResolutionCrossSections)
            : solarAtlasFile(highResolutionSolarAtlas), crossSectionsToInclude(highResolutionCrossSections.size())
        {
            for (size_t ii = 0; ii < highResolutionCrossSections.size(); ++ii)
            {
                crossSectionsToInclude[ii].path = highResolutionCrossSections[ii].first;
                crossSectionsToInclude[ii].totalColumn = highResolutionCrossSections[ii].second;
            }
        }

        /** Set to true to enable debugging output to stdout */
        bool debugOutput = false;

        virtual WavelengthRange GetFraunhoferRange(const std::vector<double>& wavelengthCalibration) override;

        virtual std::unique_ptr<CSpectrum> GetFraunhoferSpectrum(
            const std::vector<double>& pixelToWavelengthMapping,
            const novac::CCrossSectionData& measuredInstrumentLineShape) override;

        virtual std::unique_ptr<CSpectrum> GetFraunhoferSpectrum(
            const std::vector<double>& pixelToWavelengthMapping,
            const novac::CCrossSectionData& measuredInstrumentLineShape,
            double fwhmOfInstrumentLineShape,
            bool normalize) override;

        virtual std::unique_ptr<CSpectrum> GetDifferentialFraunhoferSpectrum(
            const std::vector<double>& wavelengthCalibration,
            const novac::CCrossSectionData& measuredInstrumentLineShape,
            double fwhmOfInstrumentLineShape) override;

    private:
        /** The path and filename of the solar atlas file to use.  */
        const std::string solarAtlasFile;

        struct AbsorbingCrossSection
        {
            AbsorbingCrossSection() = default;

            AbsorbingCrossSection(const std::pair<std::string, double>& value)
                : path(value.first), totalColumn(value.second)
            {
            }

            std::string path;
            double totalColumn = 0.0;
            std::unique_ptr<novac::CCrossSectionData> crossSectionData;
        };

        /** The path and total column of the high resolved absorption cross section files to include.  */
        std::vector<AbsorbingCrossSection> crossSectionsToInclude;

        /** The read in high resolution solar cross section, saved in order to reduce file-io time. */
        std::unique_ptr<novac::CCrossSectionData> solarCrossSection;

        /** This creates a Fraunhofer spectrum using the provided Absorbing cross sections instead of using the member */
        std::unique_ptr<CSpectrum> GetFraunhoferSpectrum(
            const std::vector<double>& pixelToWavelengthMapping,
            const novac::CCrossSectionData& measuredInstrumentLineShape,
            std::vector<AbsorbingCrossSection>& crossSectionsToInclude,
            double fwhmOfInstrumentLineShape,
            bool normalize);

        std::unique_ptr<CSpectrum> GetDifferentialFraunhoferSpectrum(
            const std::vector<double>& pixelToWavelengthMapping,
            const novac::CCrossSectionData& measuredInstrumentLineShape,
            std::vector<AbsorbingCrossSection>& crossSectionsToInclude,
            double fwhmOfInstrumentLineShape);

        void ReadSolarCrossSection();
    };

}

