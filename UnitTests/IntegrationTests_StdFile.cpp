#include <SpectralEvaluation/Spectra/Spectrum.h>
#include <SpectralEvaluation/File/STDFile.h>
#include "catch.hpp"
#include "TestData.h"
#include <string.h>

namespace novac
{
    // -------- Reading all the properties from an extended Std file --------
    TEST_CASE("Wavelength calibrated std file", "[StdFile][InstrumentCalibration][IntegrationTest]")
    {
        CSpectrum spectrum;
        CSTDFile::ReadSpectrum(spectrum, TestData::GetWavelengthCalibratedStdFileName());

        SECTION("Correct length of spectrum read")
        {
            REQUIRE(2043 == spectrum.m_length);
            REQUIRE(std::abs(spectrum.m_data[0] - 7.03545789517114E-19) < 1e-19);
            REQUIRE(std::abs(spectrum.m_data[1610] - 1.85618452015966E-22) < 1e-22);
            REQUIRE(std::abs(spectrum.m_data[2042] - 0.0) < 1e-19);
        }

        SECTION("Correct pixel to wavelength mapping of spectrum read")
        {
            REQUIRE(2043 == spectrum.m_wavelength.size());
            REQUIRE(std::abs(spectrum.m_wavelength[0] - 278.385) < 1e-3);
            REQUIRE(std::abs(spectrum.m_wavelength[2042] - 422.283) < 1e-13);
        }

        SECTION("Correct exposure time read")
        {
            REQUIRE(123 == spectrum.m_info.m_exposureTime);
        }

        SECTION("Correct number of scans read")
        {
            REQUIRE(15 == spectrum.m_info.m_numSpec);
        }

        SECTION("Correct position read")
        {
            REQUIRE(Approx(37.764732) == spectrum.m_info.m_gps.m_latitude);
            REQUIRE(Approx(15.014968) == spectrum.m_info.m_gps.m_longitude);
        }

        SECTION("Correct angles read")
        {
            REQUIRE(Approx(35.13) == spectrum.m_info.m_scanAngle);
            REQUIRE(Approx(19.54) == spectrum.m_info.m_scanAngle2);
        }

        SECTION("Correct date and time read")
        {
            REQUIRE(2021 == spectrum.m_info.m_startTime.year);
            REQUIRE(4 == spectrum.m_info.m_startTime.month);
            REQUIRE(18 == spectrum.m_info.m_startTime.day);
            REQUIRE(7 == spectrum.m_info.m_startTime.hour);
            REQUIRE(51 == spectrum.m_info.m_startTime.minute);
            REQUIRE(26 == spectrum.m_info.m_startTime.second);

            REQUIRE(2021 == spectrum.m_info.m_stopTime.year);
            REQUIRE(4 == spectrum.m_info.m_stopTime.month);
            REQUIRE(18 == spectrum.m_info.m_stopTime.day);
            REQUIRE(7 == spectrum.m_info.m_stopTime.hour);
            REQUIRE(51 == spectrum.m_info.m_stopTime.minute);
            REQUIRE(28 == spectrum.m_info.m_stopTime.second);
        }
    }

    // -------- Reading all the properties from an extended Std file --------
    TEST_CASE("Mercury spectrum in std file", "[StdFile][InstrumentCalibration][IntegrationTest]")
    {
        CSpectrum spectrum;
        const bool returnValue = CSTDFile::ReadSpectrum(spectrum, TestData::GetMercurySpectrumWithoutWavelengthCalibration());

        SECTION("ReadSpectrum returns success")
        {
            REQUIRE(returnValue == true);
        }

        SECTION("Correct length of spectrum read")
        {
            REQUIRE(2048 == spectrum.m_length);
            REQUIRE(std::abs(spectrum.m_data[0] - 1142.532508929) < 1e-7);
            REQUIRE(std::abs(spectrum.m_data[2047] - 1336.960810714) < 1e-7);
        }

        SECTION("Expected instrument properties")
        {
            REQUIRE("USB2+F01084" == spectrum.m_info.m_device);
            REQUIRE("USB2000+" == spectrum.m_info.m_specModelName);
        }

        SECTION("Expected spectrum measurement properties")
        {
            REQUIRE(100 == spectrum.m_info.m_numSpec);
            REQUIRE(3 == spectrum.m_info.m_exposureTime);
        }

        SECTION("Spectrum contains no wavelength calibration")
        {
            REQUIRE(spectrum.m_wavelength.size() == 0);
        }
    }

}