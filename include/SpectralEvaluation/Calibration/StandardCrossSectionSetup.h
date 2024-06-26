#pragma once

#include <string>
#include <vector>

namespace novac
{

    /** This is a helper class which is able to read in a set of standard cross sections
        which may be shipped with the novac software in a separate directory. */
    class StandardCrossSectionSetup
    {
    public:

        StandardCrossSectionSetup(const std::string& directory);

        /** Creates list of the names of the standard references */
        std::vector<std::string> ListReferences() const;

        size_t NumberOfReferences() const { return m_standardReferences.size(); }

        /** Gets the full file name of the standard cross section with the given index.
            @throws std::invalid_argument if index >= NumberOfReferences(). */
        std::string ReferenceFileName(size_t index) const;

        /** Gets the specie name of the standard cross section with the given index.
            @throws std::invalid_argument if index >= NumberOfReferences(). */
        std::string ReferenceSpecieName(size_t index) const;

        /** Returns true if the standard cross section with the given index is in vacuum wavelengths.
            @throws std::invalid_argument if index >= NumberOfReferences(). */
        bool IsReferenceInVacuum(size_t index) const;

        /** Returns true if the standard cross section with the given index is an additional (non-gas) absorber.
        *   Typically true for the Ring-spectrum, but also for some other pseudo-absorbers.
            @throws std::invalid_argument if index >= NumberOfReferences(). */
        bool IsAdditionalAbsorber(size_t index) const;

        /** Returns the full file name of the Fraunhofer reference - if any exists.
            @returns empty-string if no Fraunhofer reference exists. */
        std::string FraunhoferReferenceFileName() const;

    private:

        struct StandardReference
        {
            std::string fileName;
            std::string specieName;
            bool isVacuum = true;
            // isAdditionalAbsorber is true for non-gaseous absorbers, eg. Ring
            bool isAdditionalAbsorber = false;
        };

        struct FraunhoferReference
        {
            std::string fileName;
            bool isVacuum = false;
        };

        std::vector< StandardReference> m_standardReferences;

        FraunhoferReference m_fraunhoferReference;
    };

}