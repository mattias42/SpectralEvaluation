#include "catch.hpp"
#include <SpectralEvaluation/Evaluation/FitWindow.h>
#include <SpectralEvaluation/Evaluation/CrossSectionData.h>

using namespace novac;

TEST_CASE("FitWindow - Default constructor", "[CFitWindow]")
{
    CFitWindow sut;

    SECTION("No references are defined")
    {
        REQUIRE(sut.ref[0].m_data == nullptr);
        REQUIRE(sut.nRef == 0);
    }
}

CCrossSectionData* CreateCrossSection(int startValue)
{
    CCrossSectionData* obj = new CCrossSectionData();

    for (int k = 0; k < 100; ++k)
    {
        obj->m_waveLength.push_back(startValue + k);
        obj->m_crossSection.push_back(k * startValue);
    }

    return obj;
}

TEST_CASE("FitWindow - Copy constructor", "[CFitWindow]")
{
    CFitWindow original;

    SECTION("Original has no references, then no references are copied.")
    {
        REQUIRE(original.nRef == 0);
        REQUIRE(original.ref[0].m_data == nullptr);

        CReferenceFile ref1;
        ref1.m_specieName = "SO2";
        ref1.m_path = "C:/Novac/So2.txt";
        ref1.m_data.reset(CreateCrossSection(1));
        
        CReferenceFile ref2;
        ref2.m_specieName = "O3";
        ref2.m_path = "C:/Novac/O3.txt";
        ref2.m_data.reset(CreateCrossSection(2));

        original.ref[0] = ref1;
        original.ref[1] = ref2;
        original.nRef = 2;

        CFitWindow copy{original};

        REQUIRE(copy.nRef == 2);
        REQUIRE(copy.ref[0].m_data != nullptr);
        REQUIRE(copy.ref[1].m_data != nullptr);
    }

}