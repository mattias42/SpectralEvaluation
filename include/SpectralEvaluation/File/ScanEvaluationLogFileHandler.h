#pragma once

#include <string>
#include <SpectralEvaluation/Evaluation/BasicScanEvaluationResult.h>

namespace novac
{
    /** CScanEvaluationLogFileHandler is a class to read and write evaluation information of one or multiple scans
    *   from/to the text file format used by the NovacProgram and the NovacPPP.
    * TODO: Make sure that this class is used by both the NovacProgram and the NovacPPP.
    * TODO: The file formats used by the NovacProgram and the NovacPPP does differ somewhat and this is not handled by this reader.
    * TODO: The NovacProgram and NovacPPP uses different subclasses of BasicScanEvaluationResult for handling the results, this needs to be unified. */
    class CScanEvaluationLogFileHandler
    {
    public:
        CScanEvaluationLogFileHandler();
        ~CScanEvaluationLogFileHandler();

        /** The evaluation log */
        std::string m_evaluationLog;

        // ------------------- PUBLIC METHODS -------------------------

        /** Parses the contens of the provided evaluation log file and fills in the contents 
            into the member variable 'm_scan'. Updates m_scan and m_evaluationLog.
            @return true if the file could be parsed successfully, otherwise false. */
        bool ReadEvaluationLog(const std::string& evaluationLogFile);

        /** Writes the contents of the array 'm_scan' to a new evaluation-log file */
        bool WriteEvaluationLog(const std::string fileName);

        /** Returns true if the scan number 'scanNo' in the most recently read
                evaluation log file is a wind speed measurement. */
        bool IsWindSpeedMeasurement(int scanNo);

        /** Appends the evaluation result of one spectrum to the given string.
                @param info - the information about the spectrum
                @param result - the evaluation result, can be NULL
                @param string - will on return be filled with the output line to be written to the evaluation-log.
                @return SUCCESS - always */
        // static bool FormatEvaluationResult(const novac::CSpectrumInfo* info, const novac::BasicScanEvaluationResult* result, double maxIntensity, int nSpecies, std::string& string);

        // ------------------- PUBLIC DATA -------------------------

        /** Information from the evaluated scans */
        std::vector<BasicScanEvaluationResult> m_scan;

        /** Information of the wind field used to calculate the flux of each scan */
        // std::vector<CWindField> m_windField;

        /** The species that were found in this evaluation log */
        std::vector<std::string> m_specie;

        /** The additional spectrum information of one spectrum. */
        novac::CSpectrumInfo m_specInfo;

    private:

        // The maximum number of references that can be fitted to a single spectrum
#define MAX_N_REFERENCES_IN_EVALLOG 10

        // Mapping the column in the file to a property
        typedef struct LogColumns {
            int column[MAX_N_REFERENCES_IN_EVALLOG];
            int columnError[MAX_N_REFERENCES_IN_EVALLOG];
            int shift[MAX_N_REFERENCES_IN_EVALLOG];
            int shiftError[MAX_N_REFERENCES_IN_EVALLOG];
            int squeeze[MAX_N_REFERENCES_IN_EVALLOG];
            int squeezeError[MAX_N_REFERENCES_IN_EVALLOG];
            int starttime = 2;
            int stoptime = 3;
            int delta = 4;
            int expTime = 5;
            int nSpec = 6;
            int intensity = -1;
            int fitIntensity = -1;
            int peakSaturation = -1;
            int fitSaturation = -1;
            int offset = -1;
            int chiSquare = -1;
            int position = -1;
            int position2 = -1; // azimuth does not exist in the typical eval-log files 
            int nSpecies = 0;
            int name = -1;// the name does not exist in the original version
        }LogColumns;

        /** Data structure to remember what column corresponds to which value in the evaluation log */
        LogColumns m_tableColumnMapping;

        /** The result from the evaluation of one spectrum. */
        novac::CEvaluationResult m_evResult;

        /** Reads the header line for the scan information and retrieves which
            column represents which value. */
        void ParseScanHeader(const char szLine[8192]);

        /** Reads and parses the XML-shaped 'scanInfo' header before the scan */
        void ParseScanInformation(novac::CSpectrumInfo& scanInfo, double& flux, FILE* f);

        /** Reads and parses the XML-shaped 'fluxInfo' header before the scan */
        // void ParseFluxInformation(CWindField& windField, double& flux, FILE* f);

        /** Resets the information about which column data is stored in */
        void ResetColumns();

        /** Resets the old scan information */
        void ResetScanInformation();

        /** Makes a quick scan through the evaluation-log
            to count the number of scans in it */
        long CountScansInFile();

        /** Makes a quick scan through the evaluation-log
            to get the start-times of each scan. */
        void GetScanStartTimes(std::vector<novac::CDateTime>& startTimes);

        /** Sorts the scans in order of collection */
        void SortScans();

        /** Returns true if the scans are already ordered */
        bool IsSorted();

        /** Sorts the CDateTime-objects in the given array. */
        static void SortScanStartTimes(std::vector<CDateTime>& timestamps, std::vector<unsigned int>& sortOrder);
    };
}